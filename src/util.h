#if !defined(UTIL_H)
#define UTIL_H

#include "communication.h"
#include "esp_log.h"
#include "esphome.h"
#include "property.h"
#include "type.h"

/**
 * @brief Synchronizes the time of the heat pump to the one received via sntp.
 */
void syncTime() {
    const auto time = id(sntp_time).now();
    if (!time.is_valid()) {
        ESP_LOGE("TIMESYNC", "Cannot sync time. SNTP time is invalid.");
        return;
    }
    ESP_LOGI("TIMESYNC", "Synchronizing heat pump time to %d.%d.%d %d:%d", time.day_of_month, time.month, time.year,
             time.hour, time.minute);
    queueTransmission(Kessel, Property::kJAHR, toggleEndianness(time.year - 2000U));
    queueTransmission(Kessel, Property::kMONAT, toggleEndianness(time.month));
    queueTransmission(Kessel, Property::kTAG, toggleEndianness(time.day_of_month));
    queueTransmission(Kessel, Property::kSTUNDE, toggleEndianness(time.hour));
    queueTransmission(Kessel, Property::kMINUTE, toggleEndianness(time.minute));
}

#endif
