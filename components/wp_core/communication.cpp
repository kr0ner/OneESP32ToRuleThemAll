#include "communication.h"
#include "esphome.h"

std::array<CanNode, 8U> CanNode::all_nodes{{{ESPCLIENT_ID, "ESPClient"},
                                            {0x123, "Heizmodul"},
                                            {0x123, "Kessel"},
                                            {0x123, "HK1"},
                                            {0x123, "HK2"},
                                            {0x123, "FET"},
                                            {0x123, "MFG"},
                                            {0x123, "Manager"}}};

CanNode* CanNode::ESPClient = &CanNode::all_nodes[0];
CanNode* CanNode::Heizmodul = &CanNode::all_nodes[1];
CanNode* CanNode::Kessel = &CanNode::all_nodes[2];
CanNode* CanNode::HK1 = &CanNode::all_nodes[3];
CanNode* CanNode::HK2 = &CanNode::all_nodes[4];
CanNode* CanNode::FET = &CanNode::all_nodes[5];
CanNode* CanNode::MFG = &CanNode::all_nodes[6];
CanNode* CanNode::Manager = &CanNode::all_nodes[7];

std::list<ConditionalRequest>& getConditionalRequests() {
    static std::list<ConditionalRequest> requests;
    return requests;
}

std::list<ConditionalTransmission>& getConditionalTransmissions() {
    static std::list<ConditionalTransmission> transmissions;
    return transmissions;
}

/**
 * @brief Splits the given integer into single bytes.
 *
 * @param The integer to be split.
 * @return A tuple of bytes.
 */
template <typename T, std::size_t Remaining = sizeof(T)>
constexpr auto asBytes(T value) {
    if constexpr (Remaining == 1) {
        return std::make_tuple(static_cast<std::uint8_t>(value & 0xFF));
    } else {
        std::uint8_t byte = (value >> ((Remaining - 1) * 8U)) & 0xFF;
        return std::tuple_cat(std::make_tuple(byte), asBytes<T, Remaining - 1U>(value));
    }
}

enum class MessageType : std::uint8_t { Write, Read, Response };

/**
 * @brief Decode a read/write/response ID into its underlying CAN ID and message type.
 *
 * The message type is encoded like this:
 * - Write    : base write ID
 * - Read     : write ID | 0x100
 * - Response : write ID | 0x200
 *
 * This function reconstructs the original CAN ID and MessageType from a given read/write/response ID.
 *
 * @param externalId The 16-bit read/write/response ID.
 * @return A pair consisting of the decoded CAN ID and its MessageType.
 */
std::pair<CANId, MessageType> decodeCanIdAndMessageType(const std::uint16_t externalId) {
    MessageType messageType = MessageType::Write;
    std::uint16_t writeId = externalId;

    if (externalId & 0x200) {
        messageType = MessageType::Response;
        writeId &= ~0x200;
    } else if (externalId & 0x100) {
        messageType = MessageType::Read;
        writeId &= ~0x100;
    }

    std::uint16_t canId = ((writeId >> 5) & 0x7c0) | (writeId & 0x3f);

    return {canId, messageType};
}

const CanNode* getCanNodeByCanId(const CANId id) {
    auto it = std::find_if(CanNode::all_nodes.begin(), CanNode::all_nodes.end(),
                           [id](const CanNode& m) { return m.canId == id; });

    if (it != CanNode::all_nodes.end()) {
        return &(*it);
    }
    return nullptr;
}

const CanNode* getCanNodeByName(const std::string& name) {
    auto it = std::find_if(CanNode::all_nodes.begin(), CanNode::all_nodes.end(),
                           [name](const CanNode& m) { return m.name == name; });

    if (it != CanNode::all_nodes.end()) {
        return &(*it);
    }
    return nullptr;
}

/**
 * @brief Checks if the message is a request, sent by another CAN participant.
 */
bool isRequest(const std::vector<std::uint8_t>& msg) {
    const auto id{msg[1U] | (msg[0U] << 8U)};
    const auto [canId, messageType] = decodeCanIdAndMessageType(id);
    return messageType == MessageType::Read;
}

/**
 * @brief Checks if the message is a response to a previously sent request. Responses contain the actual sensor value.
 */
bool isResponse(const std::vector<std::uint8_t>& msg) {
    const auto id{msg[1U] | (msg[0U] << 8U)};
    const auto [canId, messageType] = decodeCanIdAndMessageType(id);
    return messageType == MessageType::Response;
}

void queueRequest(const CanNode* node, const Property& property) {
    if (node == nullptr) {
        ESP_LOGE("QUEUE", "Node is not initialized!");
        return;
    }
    ESP_LOGI("QUEUE", "Requesting data from %.*s for %.*s", static_cast<int>(node->name.length()), node->name.data(),
             static_cast<int>(property.name.length()), property.name.data());
    getConditionalRequests().emplace_back(std::make_pair(node, property));
}

void queueConditionalRequest(const CanNode* node, const Property& property, std::function<bool()> condition) {
    if (node == nullptr) {
        ESP_LOGE("QUEUE", "Node is not initialized!");
        return;
    }
    ESP_LOGI("QUEUE", "Adding conditional request for data from %.*s for %.*s", static_cast<int>(node->name.length()),
             node->name.data(), static_cast<int>(property.name.length()), property.name.data());
    getConditionalRequests().emplace_back(std::make_pair(node, property), std::move(condition));
}

void scheduleRequest(const CanNode* node, const Property& property, std::chrono::seconds seconds) {
    if (node == nullptr) {
        ESP_LOGE("QUEUE", "Node is not initialized!");
        return;
    }
    ESP_LOGI("QUEUE", "Scheduling request for data from %.*s for %.*s in %lld seconds",
             static_cast<int>(node->name.length()), node->name.data(), static_cast<int>(property.name.length()),
             property.name.data(), seconds.count());
    getConditionalRequests().emplace_back(
        std::make_pair(node, property),
        [t = std::chrono::steady_clock::now() + seconds]() { return t < std::chrono::steady_clock::now(); });
}

void queueTransmission(const CanNode* node, const Property& property, const std::uint16_t value) {
    if (node == nullptr) {
        ESP_LOGE("QUEUE", "Node is not initialized!");
        return;
    }
    ESP_LOGI("QUEUE", "Sending %d for %.*s to %.*s ", value, static_cast<int>(property.name.length()),
             property.name.data(), static_cast<int>(node->name.length()), node->name.data());
    getConditionalTransmissions().emplace_back(std::make_pair(node, property), value);
}

std::pair<Property, SimpleVariant> processCanMessage(const std::vector<std::uint8_t>& msg) {
    Property property{Property::kINDEX_NOT_FOUND};
    std::uint8_t byte1{0U};
    std::uint8_t byte2{0U};

    // Return if the message is too small
    if (msg.size() < 7U) {
        return {Property::kINDEX_NOT_FOUND, {}};
    }

    if (msg[2U] == 0xfa) {
        byte1 = msg[5U];
        byte2 = msg[6U];
        property = static_cast<Property>(msg[4U] | (msg[3U] << 8U));
    } else {
        byte1 = msg[3U];
        byte2 = msg[4U];
        property = static_cast<Property>(msg[2U]);
    }

    const auto value{static_cast<std::uint16_t>((byte1 << 8U) | byte2)};
    const auto canId{static_cast<std::uint16_t>(((msg[0U] & 0xfc) << 3) | (msg[1U] & 0x3f))};
    if (isRequest(msg)) {
        ESP_LOGD("Communication", "Message is a request. Dropping it!");
        ESP_LOGD("Communication",
                 "Message received: Read/Write ID 0x%02x 0x%02x(0x%03x) for property %.*s (0x%04x) with raw value: %d",
                 msg[0U], msg[1U], canId, static_cast<int>(property.name.length()), property.name.data(), property.id,
                 value);
        return {Property::kINDEX_NOT_FOUND, value};
    }
    ESP_LOGI("Communication",
             "Message received: Read/Write ID 0x%02x 0x%02x(0x%03x) for property %.*s (0x%04x) with raw value: %d",
             msg[0U], msg[1U], canId, static_cast<int>(property.name.length()), property.name.data(), property.id,
             value);
    return {property, GetValueByType(value, property.type)};
}

void requestData(esphome::canbus::Canbus* can_bus, const CanNode* node, const Property& property) {
    if (can_bus == nullptr || node == nullptr) {
        return;
    }
    const auto use_extended_id{false};
    const auto [IdByte1, IdByte2] = asBytes(node->getReadId());
    const auto [IndexByte1, IndexByte2] = asBytes(property.id);
    std::vector<std::uint8_t> data{IdByte1, IdByte2, 0xfa, IndexByte1, IndexByte2, 0x00, 0x00};

    can_bus->send_data(CanNode::ESPClient->canId, use_extended_id, data);
}

/**
 * @brief Publishes a new \c value for the given \c property on the CAN bus. The message is sent
 *        with the CAN id of the \c node.
 * @todo  Instead of passing value as std:.uint16_t, pass a SimpleVariant and automatically convert
 *        the type.
 */
void sendData(esphome::canbus::Canbus* can_bus, const CanNode* node, const Property property,
              const std::uint16_t value) {
    if (can_bus == nullptr || node == nullptr) {
        return;
    }
    const auto use_extended_id{false};
    const auto [IdByte1, IdByte2] = asBytes(node->getWriteId());
    const auto [IndexByte1, IndexByte2] = asBytes(property.id);
    const auto [ValueByte1, ValueByte2] = asBytes(value);
    std::vector<std::uint8_t> data{IdByte1, IdByte2, 0xfa, IndexByte1, IndexByte2, ValueByte1, ValueByte2};

    can_bus->send_data(CanNode::ESPClient->canId, use_extended_id, data);
    // Request the value again to make sure the sensor is updated, with a delay of 10s to allow the heatpump to react.
    scheduleRequest(node, property, std::chrono::seconds(10));
}
