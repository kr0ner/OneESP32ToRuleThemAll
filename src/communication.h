#if !defined(COMMUNICATION_H)
#define COMMUNICATION_H
#include <cstdint>
#include <queue>
#include <string>
#include <vector>

#include "mapper.h"
#include "property.h"
#include "type.h"

using CANID = std::uint16_t;

struct CanMember {
    CANID CanId;
    std::string name;
    bool operator<(const CanMember& other) const { return CanId < other.CanId; }
    std::uint16_t getWriteId() const { return (getWriteId1() | (getWriteId2() << 8U)); }
    std::uint16_t getReadId() const { return (getReadId2() | (getReadId1() << 8U)); }
    std::uint8_t getReadId1() const { return (CanId >> 3) + 1;} // divide by 8 and + 1 for reading a register
    std::uint8_t getReadId2() const { return (CanId & 0xF);} // + last hex digit of Can ID
    std::uint8_t getWriteId1() const { return (CanId >> 3) + 0;} // divide by 8 and + 0 for writing a register
    std::uint8_t getWriteId2() const { return (CanId & 0xF);} // + last hex digit of Can ID
};

// clang-format off
//  Name                          CanId  Name
static const CanMember ESPClient {0x6a2, "ESPClient"};
static const CanMember Anfrage   {0x6a1, "Anfrage"};
static const CanMember Kessel    {0x180, "Kessel"};
static const CanMember HK1       {0x301, "HK1"};
static const CanMember HK2       {0x302, "HK2"};
static const CanMember FEK       {0x401, "FEK"};
// clang-format on

using Request = std::pair<const CanMember, const Property>;
static std::queue<Request> request_queue;

/**
 * @brief Checks if the message is a request, sent by another CAN participant.
 */
bool isRequest(const std::vector<std::uint8_t>& msg) {
    const auto id{msg[1U] | (msg[0U] << 8U)};
    const std::vector<CanMember> members = {ESPClient, Anfrage, Kessel, HK1, HK2, FEK};
    const auto it =
        std::find_if(members.cbegin(), members.cend(), [id](const auto& member) { return member.getWriteId() == id; });
    return it != members.cend();
}

/**
 * @brief Puts a request towards the \c member for the given \c property in the request queue.
 *        This will effectively prevent that requests are all sent at the same time, which might
 *        cause issues. The time inbetween actual requests is defined in yaml.
 */
void queueRequest(const CanMember& member, const Property& property) {
    ESP_LOGI("QUEUE", "Requesting data from %s for %s", member.name.c_str(), std::string(property.name).c_str());
    request_queue.push(std::make_pair(member, property));
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

    const std::uint16_t value{static_cast<std::uint16_t>((byte1 << 8U) | byte2)};
    ESP_LOGI("Communication",
             "Message received: Read/Write ID 0x%02x 0x%02x for property %s (0x%04x) with raw value: %d", msg[0U],
             msg[1U], std::string(property.name).c_str(), property.id, value);
    if (isRequest(msg)) {
        return {Property::kINDEX_NOT_FOUND, value};
    }
    return {property, GetValueByType(value, property.type)};
}

void requestData(const CanMember& member, const Property& property) {
    const auto use_extended_id{false};  //No use of extended ID
    const std::uint8_t IndexByte1{static_cast<std::uint8_t>((property >> 8U) & 0xff)};
    const std::uint8_t IndexByte2{static_cast<std::uint8_t>(property & 0xff)};
    std::vector<std::uint8_t> data;

    data.insert(data.end(), {member.getReadId1(), member.getReadId2(), 0xfa, IndexByte1, IndexByte2, 0x00, 0x00});

    id(my_mcp2515).send_data(ESPClient.CanId, use_extended_id, data);
}

/**
 * @brief Publishes a new \c value for the given \c property on the CAN bus. The message is sent
 *        with the CAN id of the \c member.
 * @todo  Instead of passing value as std:.uint16_t, pass a SimpleVariant and automatically convert
 *        the type.
 */
void sendData(const CanMember& member, const Property property, const std::uint16_t value) {
    const auto use_extended_id{false};  //No use of extended ID
    const std::uint8_t IndexByte1{static_cast<std::uint8_t>((property >> 8U) & 0xff)};
    const std::uint8_t IndexByte2{static_cast<std::uint8_t>(property & 0xff)};
    const std::uint8_t ValueByte1{static_cast<std::uint8_t>((value >> 8U) & 0xff)};
    const std::uint8_t ValueByte2{static_cast<std::uint8_t>(value & 0xff)};
    std::vector<std::uint8_t> data;

    data.insert(data.end(), {member.getWriteId1(), member.getWriteId2(), 0xfa, IndexByte1, IndexByte2, ValueByte1, ValueByte2});

    id(my_mcp2515).send_data(ESPClient.CanId, use_extended_id, data);
}

#endif
