#if !defined(UTIL_H)
#define UTIL_H

#include <unordered_set>
#include "esphome.h"

/**
 * @brief Synchronizes the time of the heat pump to the one received via Home Assistant time platform.
 */
void syncTime(esphome::time::RealTimeClock* time_obj);

/**
 * @brief Singleton access to whitelisted entities that shall be included in analytics.
 */
std::unordered_set<esphome::EntityBase*>& getWhitelistedEntities();

/**
 * @brief Whitelist a set of entities that shall be included in analytics.
 */
template <typename... Args>
void addToWhitelist(Args... args) {
    (getWhitelistedEntities().insert(static_cast<esphome::EntityBase*>(args)), ...);
}

#endif
