#if !defined(CALLBACK_HANDLER_H)
#define CALLBACK_HANDLER_H

#include <functional>
#include <map>
#include <utility>

#include "communication.h"
#include "esphome.h"
#include "property.h"
#include "simple_variant.h"

class CallbackHandler {
   public:
    /**
     * @brief For every given key adds a callback that will be executed every time a CAN message
     *        with the specified \c Property is received for the given \c CanMember. The callback
     *        needs to ensure that only one source is actively handled at a time to prevent toggling.
     */
    void addCallbacks(const std::vector<std::pair<CanMember, Property>>& keys,
                      std::function<void(const SimpleVariant&)> callback) {
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
    void addCallback(const std::pair<CanMember, Property> key, std::function<void(const SimpleVariant&)> callback) {
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
     * @brief Obtains the callback that has been registered for a specific \c Property and
     *        \c CanMember. If no callback could be found, it will return an empty lambda.
     * @note  Unknown property ids will be silently ignored.
     */
    std::function<void(const SimpleVariant&)> getCallback(const std::pair<CanMember, Property> key) const {
        if (key.second != Property::kINDEX_NOT_FOUND) {
            auto it = callbacks.find(key);
            if (it != callbacks.end()) {
                return it->second;
            }
            ESP_LOGI("CallbackHandler", "Callback not found for %s %s (0x%04x)", key.first.name.c_str(),
                     std::string(key.second.name).c_str(), key.second.id);
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
    std::map<std::pair<CanMember, Property>, std::function<void(const SimpleVariant&)>> callbacks;
};

#endif
