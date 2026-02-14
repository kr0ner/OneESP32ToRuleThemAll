#if !defined(UTIL_H)
#define UTIL_H

#include <unordered_set>
#include "communication.h"
#include "esp_log.h"
#include "esphome.h"
#include "property.h"
#include "type.h"

/**
 * @brief Synchronizes the time of the heat pump to the one received via Home Assistant time platform.
 */
void syncTime() {
    const auto time = id(homeassistant_time).now();
    if (!time.is_valid()) {
        ESP_LOGE("TIMESYNC", "Cannot sync time. Home Assistant time is invalid.");
        return;
    }
    ESP_LOGI("TIMESYNC", "Synchronizing heat pump time to %d.%d.%d %d:%d", time.day_of_month, time.month, time.year,
             time.hour, time.minute);
    queueTransmission(Kessel, Property::kSEKUNDE, toggleEndianness(time.second));
    queueTransmission(Kessel, Property::kMINUTE, toggleEndianness(time.minute));
    queueTransmission(Kessel, Property::kSTUNDE, toggleEndianness(time.hour));
    queueTransmission(Kessel, Property::kTAG, toggleEndianness(time.day_of_month));
    queueTransmission(Kessel, Property::kMONAT, toggleEndianness(time.month));
    queueTransmission(Kessel, Property::kJAHR, toggleEndianness(time.year - 2000U));
}

/**
 * @brief Singleton access to whitelisted entities that shall be included in analytics.
 */
std::unordered_set<esphome::EntityBase*>& getWhitelistedEntities() {
    static std::unordered_set<esphome::EntityBase*> entities;
    return entities;
}

/**
 * @brief Whitelist a set of entities that shall be included in analytics.
 */
template <typename... Args>
void addToWhitelist(Args... args) {
    (getWhitelistedEntities().insert(static_cast<esphome::EntityBase*>(args)), ...);
}

#endif
