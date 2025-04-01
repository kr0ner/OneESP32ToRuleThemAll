#if !defined(CALLBACK_HANDLER_H)
#define CALLBACK_HANDLER_H

#include <functional>
#include <unordered_map>
#include <utility>

#include "communication.h"
#include "esphome.h"
#include "property.h"
#include "simple_variant.h"

namespace std {
template <>
struct hash<CanMember> {
    size_t operator()(const CanMember& rhs) const {
        size_t h1 = hash<std::string_view>{}(rhs.name());
        size_t h2 = hash<CANId>{}(rhs.canId());
        return h1 ^ (h2 << 1);
    }
};

template <>
struct hash<Property> {
    size_t operator()(const Property& rhs) const {
        size_t h1 = hash<std::string_view>{}(rhs.name());
        size_t h2 = hash<std::uint16_t>{}(rhs.id());
        size_t h3 = hash<Type>{}(rhs.type());
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

template <>
struct hash<std::pair<CanMember, Property>> {
    size_t operator()(const std::pair<CanMember, Property>& rhs) const {
        size_t h1 = std::hash<CanMember>()(rhs.first);
        size_t h2 = std::hash<Property>()(rhs.second);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};
}  // namespace std

class CallbackHandler {
   public:
    using Key = std::pair<CanMember, Property>;
    using Callback = std::function<void(const SimpleVariant&)>;

    /**
     * @brief For every given key adds a callback that will be executed every time a CAN message
     *        with the specified \c Property is received for the given \c CanMember. The callback
     *        needs to ensure that only one source is actively handled at a time to prevent toggling.
     */
    void addCallbacks(const std::vector<Key>& keys, Callback callback) {
        for (const auto& key : keys) {
            addCallback(key, callback);
        }
    }

    /**
     * @brief Adds a callback that will be executed every time a CAN message with the specified
     *        \c Property is received for the given \c CanMember. The callback usually takes care of
     *        publishing the value to Home Assistant and is registered once during the startup.
     *        Registering multiple callbacks is possible. They are executed in the same order as they
     *        were added.
     */
    void addCallback(const Key key, Callback callback) {
        // check if callback already exists
        auto it = callbacks.find(key);
        if (it == callbacks.end()) {
            // add callback
            callbacks.insert({std::move(key), std::move(callback)});
        } else {
            // merge with existing callback
            auto existingCallback = getCallback(key);
            callbacks.insert_or_assign(std::move(key), [existingCallback = std::move(existingCallback),
                                                        newCallback = std::move(callback)](const SimpleVariant& value) {
                existingCallback(value);
                newCallback(value);
            });
        }
    }

    /**
     * @brief Replaces already existing callbacks with the new one.
     */
    void replaceCallback(const Key key, Callback callback) {
        callbacks.insert_or_assign(std::move(key), std::move(callback));
    }

    /**
     * @brief Obtains the callback that has been registered for a specific \c Property and
     *        \c CanMember. If no callback could be found, it will return an empty lambda.
     * @note  Unknown property ids will be silently ignored.
     */
    [[nodiscard]] std::function<void(const SimpleVariant&)> getCallback(const Key key) const {
        if (key.second != Property::kINDEX_NOT_FOUND) {
            auto it = callbacks.find(key);
            if (it != callbacks.end()) {
                return it->second;
            }
            ESP_LOGI("CallbackHandler", "Callback not found for %s %s (0x%04x)", key.first.name(), key.second.name(),
                     key.second.id());
        }
        return [](const SimpleVariant&) -> void {
        };
    }

    /**
     * @brief Removes the callback that has been registered for a specific \c Property and
     *        \c CanMember. The already registered callback will be returned. If no callback could be found,
     *        an empty lambda will be returned.
     */
    std::function<void(const SimpleVariant&)> removeCallback(const Key key) {
        if (auto it = callbacks.find(key); it != callbacks.end()) {
            auto callback = std::move(it->second);
            callbacks.erase(key);
            return callback;
        }
        return [](const SimpleVariant&) -> void {
        };
    }

    /**
     * @brief This implements the singleton pattern for the \c CallbackHandler. Ensures that there
     *        will be only one instance available.
     */
    static CallbackHandler& instance() {
        static CallbackHandler c;
        return c;
    }

   private:
    CallbackHandler() {}
    std::unordered_map<Key, Callback> callbacks;
};

#endif
