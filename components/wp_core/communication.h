#if !defined(COMMUNICATION_H)
#define COMMUNICATION_H
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <list>
#include <string_view>
#include <vector>

#include "property.h"
#include "type.h"

namespace esphome::canbus {
class Canbus;
}  // namespace esphome::canbus

using CANId = std::uint16_t;

struct CanNode {
    CANId canId{0x123};
    const std::string_view name;

    std::uint16_t getWriteId() const { return (((canId & 0x7c0) << 5U) + (canId & 0x3f)); }
    std::uint16_t getReadId() const { return getWriteId() | 0x100; }
    std::uint16_t getResponseId() const { return getWriteId() | 0x200; }

    static std::array<CanNode, 8U> all_nodes;

    static CanNode* ESPClient;
    static CanNode* Heizmodul;
    static CanNode* Kessel;
    static CanNode* HK1;
    static CanNode* HK2;
    static CanNode* FET;
    static CanNode* MFG;
    static CanNode* Manager;
};

using Task = std::pair<const CanNode*, const Property>;
struct ConditionalRequest {
    ConditionalRequest(Task task) : _task(std::move(task)){};
    ConditionalRequest(Task task, std::function<bool()> condition)
        : _task(std::move(task)), _condition(std::move(condition)){};

    Task _task;
    std::function<bool()> _condition = []() {
        return true;
    };
};

struct ConditionalTransmission {
    ConditionalTransmission(Task task, std::uint16_t value) : _task(std::move(task)), _value(value){};
    ConditionalTransmission(Task task, std::function<bool()> condition, std::uint16_t value)
        : _task(std::move(task)), _condition(std::move(condition)), _value(value){};

    Task _task;
    std::uint16_t _value;
    std::function<bool()> _condition = nullptr;
};

/**
 * @brief Singleton accessor to ensure all files share the same list.
 */
std::list<ConditionalRequest>& getConditionalRequests();

/**
 * @brief Singleton accessor to ensure all files share the same list.
 */
std::list<ConditionalTransmission>& getConditionalTransmissions();

/**
 * @brief Tries to find the CanNode with the given CANId.
 *
 * @param The CANId to look up.
 * @return A pointer to the CanNode.
 */
const CanNode* getCanNodeByCanId(const CANId id);

/**
 * @brief Tries to find the CanNode with the given name.
 *
 * @param The name to look up.
 * @return A pointer to the CanNode.
 */
const CanNode* getCanNodeByName(const std::string& name);

/**
 * @brief Puts a request towards the \c node for the given \c property in the request queue.
 *        This will effectively prevent that requests are all sent at the same time, which might
 *        cause issues. The list of requests is processed one element at a time and according to the interval set in the
 *        common.yaml
 */
void queueRequest(const CanNode* node, const Property& property);

/**
 * @brief Puts a conditional request towards the \c node for the given \c property. The request will be placed once
 *        the \c condition evaluates to true and there are no preceding requests pending. The list of requests is
 *        processed one element at a time and according to the interval set in the common.yaml
 */
void queueConditionalRequest(const CanNode* node, const Property& property, std::function<bool()> condition);

/**
 * @brief Puts a conditional request towards the \c node for the given \c property. The request will be placed once
 *        the given \c time in seconds has passed and there are no preceding requests pending. The list of requests is
 *        processed one element at a time and according to the interval set in the common.yaml
 */
void scheduleRequest(const CanNode* node, const Property& property, std::chrono::seconds seconds);

/**
 * @brief Puts a send request towards the \c node for the given \c property  with the \c value in the task queue. This
 *        will effectively prevent that CAN messages are all sent at the same time, which might cause issues.
 * @note  The list of tasks is processed one element at a time and according to the interval set in the common.yaml
 */
void queueTransmission(const CanNode* node, const Property& property, const std::uint16_t value);

/**
 * @brief Attempts to parse the CAN message and extracts the property and the value. The value will
 *        be converted to the correct type automatically, based the type that was defined for this
 *        entity in \c property.h
 */
std::pair<Property, SimpleVariant> processCanMessage(const std::vector<std::uint8_t>& msg);

/**
 * @brief Sends a request over the CAN bus to read a specific \c property from the given \c node.
 * The request message is transmitted using the CAN id of the ESPClient.
 */
void requestData(esphome::canbus::Canbus* can_bus, const CanNode* node, const Property& property);

/**
 * @brief Publishes a new \c value for the given \c property on the CAN bus. The message is sent
 *        with the CAN id of the \c node.
 * @todo  Instead of passing value as std::uint16_t, pass a SimpleVariant and automatically convert
 *        the type.
 */
void sendData(esphome::canbus::Canbus* can_bus, const CanNode* node, const Property property,
              const std::uint16_t value);

#endif
