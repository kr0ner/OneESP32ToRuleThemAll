#if !defined(UNMAPPED_TRACKER_H)
#define UNMAPPED_TRACKER_H

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>

#include "esphome.h"

// Tracks CAN property ids that arrive on the bus but have no entry in property.h (shown as
// "UNKNOWN" by Mapper::getProperty). Meant purely as a discovery aid for extending property.h --
// disabled by default (see core.yaml's track_unmapped_properties substitution) since it writes to
// flash every time a genuinely new id is seen.
class UnmappedPropertyTracker {
   public:
    static constexpr std::size_t kCapacity{24U};

    struct Entry {
        char canNode[12]{};
        std::uint16_t propertyId{0xFFFFU};
    };

    struct State {
        std::array<Entry, kCapacity> entries{};
        std::uint8_t nextIndex{0U};
    };

    static UnmappedPropertyTracker& instance() {
        static UnmappedPropertyTracker t;
        return t;
    }

    void track(const std::string_view canNode, const std::uint16_t propertyId) {
        for (const auto& entry : state_.entries) {
            if (entry.propertyId == propertyId && canNode == entry.canNode) {
                return;  // already recorded
            }
        }
        auto& slot = state_.entries[state_.nextIndex];
        std::snprintf(slot.canNode, sizeof(slot.canNode), "%.*s", static_cast<int>(canNode.size()), canNode.data());
        slot.propertyId = propertyId;
        state_.nextIndex = static_cast<std::uint8_t>((state_.nextIndex + 1U) % kCapacity);
        pref_.save(&state_);
        ESP_LOGI("UnmappedPropertyTracker", "New unmapped property seen: %.*s (0x%04x)",
                 static_cast<int>(canNode.size()), canNode.data(), propertyId);
    }

    std::string getSummary() const {
        std::string result;
        for (const auto& entry : state_.entries) {
            if (entry.propertyId == 0xFFFFU) {
                continue;
            }
            if (!result.empty()) {
                result += ", ";
            }
            static_assert(sizeof(entry.canNode) == 12U, "adjust the %.11s precision below if this changes");
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%.11s (0x%04x)", entry.canNode, entry.propertyId);
            result += buf;
        }
        return result.empty() ? "none" : result;
    }

   private:
    UnmappedPropertyTracker() {
        pref_ = esphome::global_preferences->make_preference<State>(esphome::fnv1_hash("wp_unmapped_tracker_v1"));
        if (!pref_.load(&state_)) {
            state_ = State{};
        }
    }

    esphome::ESPPreferenceObject pref_;
    State state_{};
};

#endif
