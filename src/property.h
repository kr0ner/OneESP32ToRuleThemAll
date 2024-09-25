#if !defined(PROPERTY_H)
#define PROPERTY_H

#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_map>
#include "type.h"

namespace detail {
struct Property;
struct Mapper {
    Mapper(const Property* other);

   protected:
    Mapper() = default;

   protected:
    static inline std::unordered_map<std::uint16_t, const Property*> map;
};

struct Property : public Mapper {
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
    Property getProperty(const std::uint16_t id) {
        if (auto it = map.find(id); it != map.end()) {
            return *(it->second);
        }
        return {"UNKNOWN", id};
    }
};

inline Mapper::Mapper(const Property* other) {
    map.insert({other->id, other});
}
}  // namespace detail

#define PROPERTY(NAME, VALUE, ...)                                          \
    static constexpr detail::Property k##NAME{#NAME, VALUE, ##__VA_ARGS__}; \
    static inline detail::Mapper k##NAME##_MAPPING {                        \
        &k##NAME                                                            \
    }

struct Property : public detail::Property {
    Property(const Property& p) : detail::Property{p.name, p.id, p.type} {}
    Property(const detail::Property& p) : detail::Property{p.name, p.id, p.type} {}
    Property(const std::uint16_t _id) : detail::Property{getProperty(_id)} {}

    PROPERTY(INDEX_NOT_FOUND, 0x0000);
    PROPERTY(FEHLERMELDUNG, 0x0001);
    PROPERTY(SPEICHERSOLLTEMP, 0x0003, Type::et_dec_val);
    PROPERTY(VORLAUFSOLLTEMP, 0x0004, Type::et_dec_val);
    PROPERTY(RAUMSOLLTEMP_I, 0x0005, Type::et_dec_val);
    PROPERTY(RAUMSOLLTEMP_NACHT, 0x0008, Type::et_dec_val);
    PROPERTY(AUSSENTEMP, 0x000c, Type::et_dec_val);
    PROPERTY(SAMMLERISTTEMP, 0x000d, Type::et_dec_val);
    PROPERTY(SPEICHERISTTEMP, 0x000e, Type::et_dec_val);
    PROPERTY(VORLAUFISTTEMP, 0x000f, Type::et_dec_val);
    PROPERTY(RAUMISTTEMP, 0x0011, Type::et_dec_val);
    PROPERTY(VERSTELLTE_RAUMSOLLTEMP, 0x0012, Type::et_dec_val);
    PROPERTY(EINSTELL_SPEICHERSOLLTEMP, 0x0013, Type::et_dec_val);
    PROPERTY(VERDAMPFERTEMP, 0x0014, Type::et_dec_val);
    PROPERTY(RUECKLAUFISTTEMP, 0x0016, Type::et_dec_val);
    PROPERTY(FEUCHTE, 0x0075, Type::et_dec_val);
    PROPERTY(RAUMEINFLUSS, 0x010f, Type::et_little_endian);
    PROPERTY(PROGRAMMSCHALTER, 0x0112, Type::et_betriebsart);
    PROPERTY(WOCHENTAG, 0x0121);
    PROPERTY(TAG, 0x0122, Type::et_little_endian);
    PROPERTY(MONAT, 0x0123, Type::et_little_endian);
    PROPERTY(JAHR, 0x0124, Type::et_little_endian);
    PROPERTY(STUNDE, 0x0125, Type::et_little_endian);
    PROPERTY(MINUTE, 0x0126, Type::et_little_endian);
    PROPERTY(HYSTERESE_WW, 0x0140, Type::et_dec_val);
    PROPERTY(BETRIEBS_STATUS, 0x0176);
    PROPERTY(VOLUMENSTROM, 0x01da, Type::et_dec_val);
    PROPERTY(TAUPUNKT_HK1, 0x0264, Type::et_dec_val);
    PROPERTY(HEIZGASTEMPERATUR, 0x0265); 
    PROPERTY(KUEHLMODE, 0x0287);
    PROPERTY(PUMPENDREHZAHL_HEIZEN, 0x02cb, Type::et_dec_val);
    PROPERTY(WAERMEERTRAG_WRG_TAG_WH, 0x03ae);
    PROPERTY(WAERMEERTRAG_WRG_TAG_KWH, 0x03af);
    PROPERTY(WAERMEERTRAG_WRG_SUM_KWH, 0x03b0);
    PROPERTY(WAERMEERTRAG_WRG_SUM_MWH, 0x03b1);
    PROPERTY(KUEHL_RAUMSOLL_TAG, 0x0569, Type::et_dec_val);
    PROPERTY(KUEHL_RAUMSOLL_ABWESEND, 0x056a, Type::et_dec_val);
    PROPERTY(KUEHL_RAUMSOLL_NACHT, 0x056b, Type::et_dec_val);
    PROPERTY(LUEFT_STUFE_TAG, 0x056c);
    PROPERTY(LUEFT_STUFE_NACHT, 0x056d);
    PROPERTY(LUEFT_STUFE_PARTY, 0x0570);
    PROPERTY(PASSIVKUEHLUNG, 0x0575);
    PROPERTY(NE_STUFE_WW, 0x058a);
    PROPERTY(ZULUFT_SOLL, 0x0596);
    PROPERTY(ZULUFT_IST, 0x0597);
    PROPERTY(ABLUFT_SOLL, 0x0598);
    PROPERTY(ABLUFT_IST, 0x0599);
    PROPERTY(FORTLUFT_SOLL, 0x059a);
    PROPERTY(FORTLUFT_IST, 0x059b);
    PROPERTY(VERFLUESSIGERTEMPERATUR, 0x059c, Type::et_dec_val);
    PROPERTY(VERDICHTER_HEIZEN, 0x05a4); 
    PROPERTY(VERDICHTER_KUEHLEN, 0x05a5); 
    PROPERTY(VERDICHTER_WW, 0x05a6); 
    PROPERTY(EL_NE_HEIZEN, 0x05a7); 
    PROPERTY(EL_NE_WW, 0x05a8); 
    PROPERTY(PUMPENZYKLEN_MIN_AUSSENT, 0x05bb);
    PROPERTY(KUEHLSYSTEM, 0x0613);
    PROPERTY(DRUCK_HEIZKREIS, 0x064a, Type::et_dec_val);
    PROPERTY(LEISTUNG_AUSLEGUNG_KUEHLEN, 0x0692);
    PROPERTY(MOTORSTROM, 0x069f);
    PROPERTY(MOTORLEISTUNG, 0x06a0, Type::et_cent_val);
    PROPERTY(MOTORSPANNUNG, 0x06a1);
    PROPERTY(OELSUMPFTEMPERATUR, 0x0693c, Type::et_dec_val); 
    PROPERTY(ABLUFTTEMP, 0x0694, Type::et_dec_val);
    PROPERTY(HOCHDRUCK, 0x07a6, Type::et_cent_val); 
    PROPERTY(NIEDERDRUCK, 0x07a7, Type::et_cent_val); 
    PROPERTY(EL_AUFNAHMELEISTUNG_WW_TAG_WH, 0x091a);
    PROPERTY(EL_AUFNAHMELEISTUNG_WW_TAG_KWH, 0x091b, Type::et_double_val);
    PROPERTY(EL_AUFNAHMELEISTUNG_WW_SUM_KWH, 0x091c);
    PROPERTY(EL_AUFNAHMELEISTUNG_WW_SUM_MWH, 0x091d, Type::et_double_val);
    PROPERTY(EL_AUFNAHMELEISTUNG_HEIZ_TAG_WH, 0x091e);
    PROPERTY(EL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH, 0x091f, Type::et_double_val);
    PROPERTY(EL_AUFNAHMELEISTUNG_HEIZ_SUM_KWH, 0x0920);
    PROPERTY(EL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH, 0x0921, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_2WE_WW_TAG_WH, 0x0922);
    PROPERTY(WAERMEERTRAG_2WE_WW_TAG_KWH, 0x0923, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_2WE_WW_SUM_KWH, 0x0924);
    PROPERTY(WAERMEERTRAG_2WE_WW_SUM_MWH, 0x0925, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_2WE_HEIZ_TAG_WH, 0x0926);
    PROPERTY(WAERMEERTRAG_2WE_HEIZ_TAG_KWH, 0x0927, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_2WE_HEIZ_SUM_KWH, 0x0928);
    PROPERTY(WAERMEERTRAG_2WE_HEIZ_SUM_MWH, 0x0929, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_WW_TAG_WH, 0x092a);
    PROPERTY(WAERMEERTRAG_WW_TAG_KWH, 0x092b, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_WW_SUM_KWH, 0x092c);
    PROPERTY(WAERMEERTRAG_WW_SUM_MWH, 0x092d, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_HEIZ_TAG_WH, 0x092e);
    PROPERTY(WAERMEERTRAG_HEIZ_TAG_KWH, 0x092f, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_HEIZ_SUM_KWH, 0x0930);
    PROPERTY(WAERMEERTRAG_HEIZ_SUM_MWH, 0x0931, Type::et_double_val);
    PROPERTY(HEIZ_KUEHL_LEISTUNG, 0xc0ee, Type::et_cent_val);
    PROPERTY(ABLUFTFEUCHTE, 0xc0ef);
    PROPERTY(LEISTUNG_AUSLEGUNG_HEIZUNG, 0xc0f1);
    PROPERTY(VERDICHTER_STARTS, 0xc0f4);
    PROPERTY(VERDICHTER_STARTS_K, 0xc0f5);
    PROPERTY(ABLUFTTAUPUNKT, 0xc0f6, Type::et_dec_val);
    PROPERTY(DIFFERENZDRUCK, 0xc11e);
    PROPERTY(LAUFZEIT_FILTER, 0xc111);
    PROPERTY(BETRIEBS_STATUS_2, 0xc356);
};

#endif
