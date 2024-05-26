#if !defined(CALLBACK_HANDLER_H)
#define CALLBACK_HANDLER_H

#include <functional>
#include <map>
#include <utility>

#include "communication.h"
#include "simple_variant.h"

class CallbackHandler {
   public:
    void addCallback(const std::pair<CanMember, std::uint16_t>& key, std::function<void(const SimpleVariant&)> value) {
        callbacks.insert({key, value});
    }

    std::function<void(const SimpleVariant&)> getCallback(const std::pair<CanMember, std::uint16_t>& key) const {
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
    std::map<std::pair<CanMember, std::uint16_t>, std::function<void(const SimpleVariant&)>> callbacks;

};

void publishValue(esphome::template_::TemplateSensor* sensor, const SimpleVariant& value) {

    const std::type_info& ti = value.type();

    if (ti == typeid(bool)) {
        sensor->publish_state(value.get<bool>());
    } else if (ti == typeid(std::uint8_t)) {
        sensor->publish_state(value.get<std::uint8_t>());
    } else if (ti == typeid(std::int16_t)) {
        sensor->publish_state(value.get<std::int16_t>());
    } else if (ti == typeid(std::uint16_t)) {
        sensor->publish_state(value.get<std::uint16_t>());
    } else if (ti == typeid(float)) {
        sensor->publish_state(value.get<float>());
    } else {
        ESP_LOGE("ERROR", "Unknown type to publish for Template Sensor.");
    }

}


void publishValue(esphome::template_::TemplateNumber* sensor, const SimpleVariant& value) {

    const std::type_info& ti = value.type();

    if (ti == typeid(std::uint8_t)) {
        sensor->publish_state(value.get<std::uint8_t>());
    } else if (ti == typeid(std::int16_t)) {
        sensor->publish_state(value.get<std::int16_t>());
    } else if (ti == typeid(std::uint16_t)) {
        sensor->publish_state(value.get<std::uint16_t>());
    } else if (ti == typeid(float)) {
        sensor->publish_state(value.get<float>());
    } else {
        ESP_LOGE("ERROR", "Unknown type to publish for Template Number.");
    }

}


void publishValue(esphome::text_sensor::TextSensor* sensor, const SimpleVariant& value) {

    const std::type_info& ti = value.type();

    if (ti == typeid(std::string)) {
        sensor->publish_state(value.get<std::string>());
    } else {
        ESP_LOGE("ERROR", "Unknown type to publish for Text Sensor.");
    }

}


#endif
