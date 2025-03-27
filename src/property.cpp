#include "property.h"
#include <unordered_map>

namespace detail {
Property Mapper::getProperty(const std::uint16_t id) const {
    if (auto it = map.find(id); it != map.end()) {
        return it->second;
    }
    return ::Property::kINDEX_NOT_FOUND;
}

Property Property::getProperty(const std::uint16_t id) const {
    return Mapper().getProperty(id);
}
}  // namespace detail
