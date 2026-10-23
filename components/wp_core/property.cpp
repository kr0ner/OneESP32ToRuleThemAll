#include "property.h"
#include <map>

namespace oe32trta {
namespace detail {
void Mapper::registerProperty(const CanNode* node, const Property& property) {
    if (node != nullptr) {
        map.insert({{node, property.id}, property});
    }
}

Property Mapper::getProperty(const CanNode* node, const std::uint16_t id) {
    if (node != nullptr) {
        if (auto it = map.find({node, id}); it != map.end()) {
            return it->second;
        }
    }
    for (const auto& [key, property] : map) {
        if (key.second == id) {
            return property;
        }
    }
    return {"UNKNOWN", id};
}

Property Property::getProperty(const CanNode* node, const std::uint16_t id) {
    return Mapper::instance().getProperty(node, id);
}
}  // namespace detail
}  // namespace oe32trta
