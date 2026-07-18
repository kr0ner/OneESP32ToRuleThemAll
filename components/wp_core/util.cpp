#include "util.h"
#include "communication.h"
#include "esp_log.h"
#include "property.h"
#include "type.h"

void syncTime(esphome::time::RealTimeClock* time_obj) {
    const auto time = time_obj->now();
    if (!time.is_valid()) {
        ESP_LOGE("TIMESYNC", "Cannot sync time. Home Assistant time is invalid.");
        return;
    }
    ESP_LOGI("TIMESYNC", "Synchronizing heat pump time to %d.%d.%d %d:%d", time.day_of_month, time.month, time.year,
             time.hour, time.minute);
#if defined(WPL_33)
    const auto* target = CanNode::Manager;
#else
    const auto* target = CanNode::Kessel;
#endif
#if !defined(THZ_304)
    queueTransmission(target, Property::kSEKUNDE, toggleEndianness(time.second));
#endif
    queueTransmission(target, Property::kMINUTE, toggleEndianness(time.minute));
    queueTransmission(target, Property::kSTUNDE, toggleEndianness(time.hour));
    queueTransmission(target, Property::kTAG, toggleEndianness(time.day_of_month));
    queueTransmission(target, Property::kMONAT, toggleEndianness(time.month));
    queueTransmission(target, Property::kJAHR, toggleEndianness(time.year - 2000U));
}

std::unordered_set<esphome::EntityBase*>& getWhitelistedEntities() {
    static std::unordered_set<esphome::EntityBase*> entities;
    return entities;
}
