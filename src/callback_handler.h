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
    void addCallback(const std::pair<CanMember, Property> key, std::function<void(const SimpleVariant&)> value) {
        callbacks.insert({std::move(key), std::move(value)});
    }

    std::function<void(const SimpleVariant&)> getCallback(const std::pair<CanMember, Property> key) const {
        auto it = callbacks.find(key);
        if (it != callbacks.end()) {
            return it->second;
        }
        ESP_LOGI("CallbackHandler", "Callback not found for %s 0x%04x", key.first.name.c_str(), key.second);
        return [](const SimpleVariant&) -> void {
        };
    }

    static CallbackHandler& instance() {
        static CallbackHandler c;
        return c;
    }

   private:
    CallbackHandler() {}
    std::map<std::pair<CanMember, Property>, std::function<void(const SimpleVariant&)>> callbacks;
};

#endif
