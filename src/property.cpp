#include "property.h"
#include <unordered_map>

namespace oe32trta {
namespace detail {
Property Mapper::getProperty(const std::uint16_t id) {
    if (auto it = map.find(id); it != map.end()) {
        return it->second;
    }
    return {"UNKNOWN", id};
}

Property Property::getProperty(const std::uint16_t id) {
    return Mapper().getProperty(id);
}
}  // namespace detail
}  // namespace oe32trta
