#if !defined(COMMUNICATION_H)
#define COMMUNICATION_H
#include <chrono>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include "esphome.h"
#include "mapper.h"
#include "property.h"
#include "type.h"

using CANId = std::uint16_t;

struct CanMember {
    CANId canId;
    std::string name;
    bool operator<(const CanMember& other) const { return canId < other.canId; }
    std::uint16_t getWriteId() const { return (((canId & 0x7c0) << 5U) + (canId & 0x3f)); }
    std::uint16_t getReadId() const { return getWriteId() | 0x100; }
    std::uint16_t getResponseId() const { return getWriteId() | 0x200; }
};

static const CanMember ESPClient{ESPCLIENT_ID, "ESPClient"};
static const CanMember Manager{MANAGER_ID, "Manager"};
static const CanMember Kessel{KESSEL_ID, "Kessel"};
static const CanMember HK1{HK1_ID, "HK1"};
static const CanMember HK2{HK2_ID, "HK2"};
static const CanMember FET{0x402, "FET"};    // TODO: make configurable and double check
static const CanMember MFG{0x700, "MFG"};    // TODO: make configurable and double check
static const CanMember WPM2{0x480, "WPM2"};  // TODO: make configurable and double check

static const std::vector<std::reference_wrapper<const CanMember>> canMembers{Kessel,    HK1, HK2, Manager,
                                                                             ESPClient, FET, MFG, WPM2};

using Request = std::pair<const CanMember, const Property>;
struct ConditionalRequest {
    ConditionalRequest(Request request) : _request(request){};
    ConditionalRequest(Request request, std::function<bool()> condition)
        : _request(request), _condition(std::move(condition)){};

    Request _request;
    std::function<bool()> _condition = []() {
        return true;
    };
};

static std::list<ConditionalRequest> conditionalRequests;

/**
 * @brief Tries to find the CANMember with the given CANId.
 *
 * @param The CANId to look up.
 * @return A reference to the CANMember wrapped in an optional.
 */
std::optional<std::reference_wrapper<const CanMember>> getCanMemberByCanId(CANId canId) {
    const auto it = std::find_if(canMembers.cbegin(), canMembers.cend(),
                                 [canId](const auto& member) { return member.get().canId == canId; });
    if (it != canMembers.cend()) {
        return it->get();
    }
    return std::nullopt;
}

/**
 * @brief Tries to find the CANMember with the given name.
 *
 * @param The name to look up.
 * @return A reference to the CANMember wrapped in an optional.
 */
std::optional<std::reference_wrapper<const CanMember>> getCanMemberByName(const std::string& name) {
    const auto it = std::find_if(canMembers.cbegin(), canMembers.cend(),
                                 [name](const auto& member) { return member.get().name == name; });
    if (it != canMembers.cend()) {
        return it->get();
    }
    return std::nullopt;
}

/**
 * @brief Checks if the message is a request, sent by another CAN participant.
 */
bool isRequest(const std::vector<std::uint8_t>& msg) {
    const auto id{msg[1U] | (msg[0U] << 8U)};
    const auto it = std::find_if(canMembers.cbegin(), canMembers.cend(),
                                 [id](const auto& member) { return member.get().getReadId() == id; });
    return it != canMembers.cend();
}

/**
 * @brief Checks if the message is a response to a previously sent request. Responses contain the actual sensor value.
 */
bool isResponse(const std::vector<std::uint8_t>& msg) {
    const auto id{msg[1U] | (msg[0U] << 8U)};
    const auto it = std::find_if(canMembers.cbegin(), canMembers.cend(),
                                 [id](const auto& member) { return member.get().getResponseId() == id; });
    return it != canMembers.cend();
}

/**
 * @brief Puts a request towards the \c member for the given \c property in the request queue.
 *        This will effectively prevent that requests are all sent at the same time, which might
 *        cause issues. The list of requests is processed one element at a time and according to the interval set in the
 *        common.yaml
 */
void queueRequest(const CanMember& member, const Property& property) {
    ESP_LOGI("QUEUE", "Requesting data from %s for %s", member.name.c_str(), std::string(property.name).c_str());
    conditionalRequests.emplace_back(std::make_pair(member, property));
}

/**
 * @brief Puts a conditional request towards the \c member for the given \c property. The request will be placed once
 *        the \c condition evaluates to true and there are no preceding requests pending. The list of requests is
 *        processed one element at a time and according to the interval set in the common.yaml
 */
void queueConditionalRequest(const CanMember& member, const Property& property, std::function<bool()> condition) {
    ESP_LOGI("QUEUE", "Adding conditional request for data from %s for %s", member.name.c_str(),
             std::string(property.name).c_str());
    conditionalRequests.emplace_back(std::make_pair(member, property), std::move(condition));
}

/**
 * @brief Puts a conditional request towards the \c member for the given \c property. The request will be placed once
 *        the given \c time in seconds has passed and there are no preceding requests pending. The list of requests is
 *        processed one element at a time and according to the interval set in the common.yaml
 */
void scheduleRequest(const CanMember& member, const Property& property, std::chrono::seconds seconds) {
    ESP_LOGI("QUEUE", "Scheduling request for data from %s for %s in %lld seconds", member.name.c_str(),
             std::string(property.name).c_str(), seconds.count());
    conditionalRequests.emplace_back(
        std::make_pair(member, property),
        [t = std::chrono::steady_clock::now() + seconds]() { return t < std::chrono::steady_clock::now(); });
}

/**
 * @brief Attempts to parse the CAN message and extracts the property and the value. The value will
 *        be converted to the correct type automatically, based the type that was defined for this
 *        entity in \c property.h
 */
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
                 "Message received: Read/Write ID 0x%02x 0x%02x(0x%03x) for property %s (0x%04x) with raw value: %d",
                 msg[0U], msg[1U], canId, std::string(property.name).c_str(), property.id, value);
        return {Property::kINDEX_NOT_FOUND, value};
    }
    ESP_LOGI("Communication",
             "Message received: Read/Write ID 0x%02x 0x%02x(0x%03x) for property %s (0x%04x) with raw value: %d",
             msg[0U], msg[1U], canId, std::string(property.name).c_str(), property.id, value);
    return {property, GetValueByType(value, property.type)};
}

void requestData(const CanMember& member, const Property& property) {
    const auto use_extended_id{false};  //No use of extended ID
    const std::uint8_t IdByte1{static_cast<std::uint8_t>((member.getReadId() >> 8U) & 0xff)};
    const std::uint8_t IdByte2{static_cast<std::uint8_t>(member.getReadId() & 0xff)};
    const std::uint8_t IndexByte1{static_cast<std::uint8_t>((property >> 8U) & 0xff)};
    const std::uint8_t IndexByte2{static_cast<std::uint8_t>(property & 0xff)};
    std::vector<std::uint8_t> data;

    data.insert(data.end(), {IdByte1, IdByte2, 0xfa, IndexByte1, IndexByte2, 0x00, 0x00});

    id(wp_can).send_data(ESPClient.canId, use_extended_id, data);
}

/**
 * @brief Publishes a new \c value for the given \c property on the CAN bus. The message is sent
 *        with the CAN id of the \c member.
 * @todo  Instead of passing value as std:.uint16_t, pass a SimpleVariant and automatically convert
 *        the type.
 */
void sendData(const CanMember& member, const Property property, const std::uint16_t value) {
    const auto use_extended_id{false};  //No use of extended ID
    const std::uint8_t IdByte1{static_cast<std::uint8_t>((member.getWriteId() >> 8U) & 0xff)};
    const std::uint8_t IdByte2{static_cast<std::uint8_t>(member.getWriteId() & 0xff)};
    const std::uint8_t IndexByte1{static_cast<std::uint8_t>((property >> 8U) & 0xff)};
    const std::uint8_t IndexByte2{static_cast<std::uint8_t>(property & 0xff)};
    const std::uint8_t ValueByte1{static_cast<std::uint8_t>((value >> 8U) & 0xff)};
    const std::uint8_t ValueByte2{static_cast<std::uint8_t>(value & 0xff)};
    std::vector<std::uint8_t> data;

    data.insert(data.end(), {IdByte1, IdByte2, 0xfa, IndexByte1, IndexByte2, ValueByte1, ValueByte2});

    id(wp_can).send_data(ESPClient.canId, use_extended_id, data);
    // Request the value again to make sure the sensor is updated, with a delay of 10s to allow the heatpump to react.
    scheduleRequest(member, property, std::chrono::seconds(10));
}

#endif
