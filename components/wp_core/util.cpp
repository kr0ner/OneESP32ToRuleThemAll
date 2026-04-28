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
#if !defined(THZ_304)
    queueTransmission(CanNode::Kessel, Property::kSEKUNDE, toggleEndianness(time.second));
#endif
    queueTransmission(CanNode::Kessel, Property::kMINUTE, toggleEndianness(time.minute));
    queueTransmission(CanNode::Kessel, Property::kSTUNDE, toggleEndianness(time.hour));
    queueTransmission(CanNode::Kessel, Property::kTAG, toggleEndianness(time.day_of_month));
    queueTransmission(CanNode::Kessel, Property::kMONAT, toggleEndianness(time.month));
    queueTransmission(CanNode::Kessel, Property::kJAHR, toggleEndianness(time.year - 2000U));
}

std::unordered_set<esphome::EntityBase*>& getWhitelistedEntities() {
    static std::unordered_set<esphome::EntityBase*> entities;
    return entities;
}
