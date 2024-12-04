#if !defined(PROPERTY_H)
#define PROPERTY_H

#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_map>
#include "type.h"

namespace detail {
struct Property {
    std::string_view name;
    std::uint16_t id{0U};
    Type type{Type::et_default};

    constexpr Property(const std::string_view _name, const std::uint16_t _id, const Type _type)
        : name(_name), id(_id), type(_type) {}
    constexpr Property(const std::string_view _name, const std::uint16_t _id) : name(_name), id(_id) {}

    constexpr operator std::uint16_t() const { return id; }

    Property& operator=(const Property& p) {
        name = p.name;
        id = p.id;
        type = p.type;
        return *this;
    }

   protected:
    Property getProperty(const std::uint16_t id);
};

class Mapper {
   public:
    Mapper() = default;
    inline Mapper(const Property other) { map.insert({other.id, other}); }

    Property getProperty(const std::uint16_t id);

   private:
    static inline std::unordered_map<std::uint16_t, Property> map;
};

}  // namespace detail

#define PROPERTY(NAME, VALUE, ...)                                          \
    static constexpr detail::Property k##NAME{#NAME, VALUE, ##__VA_ARGS__}; \
    static constexpr bool unique##VALUE{true};                              \
    static inline detail::Mapper k##NAME##_MAPPING {                        \
        k##NAME                                                             \
    }

struct Property : public detail::Property {
    constexpr Property(const Property& p) : detail::Property{p.name, p.id, p.type} {}
    constexpr Property(const detail::Property& p) : detail::Property{p.name, p.id, p.type} {}
    Property(const std::uint16_t _id) : detail::Property{getProperty(_id)} {}

    PROPERTY(INDEX_NOT_FOUND, 0x0000);

    PROPERTY(RUECKLAUFISTTEMP, 0x0016, Type::et_dec_val);
    PROPERTY(AUSSENTEMP, 0x000c, Type::et_dec_val);
    PROPERTY(QUELLENDRUCK, 0x79c1, Type::et_cent_val);
    PROPERTY(TAUPUNKTTEMPERATUR, 0x4fa6, Type::et_dec_val);
    PROPERTY(KOMFORTTEMPERATUR_HK1, 0xc2d5,Type::et_dec_val);
    // Here we can gradually add new values and then add the sensors in the yaml
    // We would need to find out which id belongs to for example VORLAUF_IST to be able to identify the can ID for HK1
    // also nice would be BETRIEBSSCHALTER to know which can id is kessel. Once these wotk we can try to request the data actively
    // by sending messages to the heatpump
    // to find new values you need to navigate through the diplay in the heatpump and match the values (e.g. 322 for 32.2°)
};

#endif
