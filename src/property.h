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
    inline Mapper(const Property& other) { map.insert({other.id, other}); }

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
    PROPERTY(FEHLERMELDUNG, 0x0001);
    PROPERTY(SPEICHERSOLLTEMP, 0x0003, Type::et_dec_val);
    PROPERTY(VORLAUFSOLLTEMP, 0x0004, Type::et_dec_val);
    PROPERTY(AUSSENTEMP, 0x000c, Type::et_dec_val);
    PROPERTY(SPEICHERISTTEMP, 0x000e, Type::et_dec_val);
    PROPERTY(VERSTELLTE_RAUMSOLLTEMP, 0x0012, Type::et_dec_val);
    PROPERTY(STEIGUNG_HK1, 0x010e, Type::et_cent_val);
    PROPERTY(WOCHENTAG, 0x0121, Type::et_little_endian);
    PROPERTY(TAG, 0x0122, Type::et_little_endian);
    PROPERTY(MONAT, 0x0123, Type::et_little_endian);
    PROPERTY(JAHR, 0x0124, Type::et_little_endian);
    PROPERTY(STUNDE, 0x0125, Type::et_little_endian);
    PROPERTY(MINUTE, 0x0126, Type::et_little_endian);
    PROPERTY(ANTEIL_VORLAUF_HK1, 0x059d);
    PROPERTY(FUSSPUNKT_HK1, 0x059e, Type::et_dec_val);
    PROPERTY(EL_ENERGIEAUFNAHME_WW_TAG_WH, 0x091a);
    PROPERTY(EL_ENERGIEAUFNAHME_WW_TAG_KWH, 0x091b, Type::et_double_val);
    PROPERTY(EL_ENERGIEAUFNAHME_WW_SUM_KWH, 0x091c);
    PROPERTY(EL_ENERGIEAUFNAHME_WW_SUM_MWH, 0x091d, Type::et_double_val);
    PROPERTY(EL_ENERGIEAUFNAHME_HEIZ_TAG_WH, 0x091e);
    PROPERTY(EL_ENERGIEAUFNAHME_HEIZ_TAG_KWH, 0x091f, Type::et_double_val);
    PROPERTY(EL_ENERGIEAUFNAHME_HEIZ_SUM_KWH, 0x0920);
    PROPERTY(EL_ENERGIEAUFNAHME_HEIZ_SUM_MWH, 0x0921, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_WW_TAG_WH, 0x092a);
    PROPERTY(WAERMEERTRAG_WW_TAG_KWH, 0x092b, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_WW_SUM_KWH, 0x092c);
    PROPERTY(WAERMEERTRAG_WW_SUM_MWH, 0x092d, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_HEIZ_TAG_WH, 0x092e);
    PROPERTY(WAERMEERTRAG_HEIZ_TAG_KWH, 0x092f, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_HEIZ_SUM_KWH, 0x0930);
    PROPERTY(WAERMEERTRAG_HEIZ_SUM_MWH, 0x0931, Type::et_double_val);

#if defined(THZ_504) || defined(THZ_404) || defined(THZ_5_5_ECO) || defined(TTF_07_C)
    PROPERTY(RAUMISTTEMP, 0x0011, Type::et_dec_val);
    PROPERTY(VERDAMPFERTEMP, 0x0014, Type::et_dec_val);
    PROPERTY(RUECKLAUFISTTEMP, 0x0016, Type::et_dec_val);
    PROPERTY(RAUMFEUCHTE, 0x0075, Type::et_dec_val);
    PROPERTY(RAUMEINFLUSS, 0x010f, Type::et_little_endian);
    PROPERTY(PROGRAMMSCHALTER, 0x0112, Type::et_betriebsart);
#endif

#if defined(THZ_504) || defined(THZ_404) || defined(THZ_5_5_ECO)
    PROPERTY(RAUMSOLLTEMP_TAG, 0x0005, Type::et_dec_val);
    PROPERTY(RAUMSOLLTEMP_NACHT, 0x0008, Type::et_dec_val);
    PROPERTY(SAMMLERISTTEMP, 0x000d, Type::et_dec_val);
    PROPERTY(VORLAUFISTTEMP, 0x000f, Type::et_dec_val);
    PROPERTY(SPEICHERSOLLTEMP_TAG, 0x0013, Type::et_dec_val);
    PROPERTY(SOLLWERT_MAX_HK1, 0x0028, Type::et_dec_val);
    PROPERTY(SOMMERBETRIEB_TEMP, 0x0116, Type::et_dec_val);
    PROPERTY(SOLLWERT_MIN_HK1, 0x012b, Type::et_dec_val);
    PROPERTY(HYSTERESE_WW, 0x0140, Type::et_dec_val);
    PROPERTY(BETRIEBS_STATUS, 0x0176);
    PROPERTY(VOLUMENSTROM, 0x01da, Type::et_dec_val);
    PROPERTY(TAUPUNKT_HK1, 0x0264, Type::et_dec_val);
    PROPERTY(KUEHLMODE, 0x0287);
    PROPERTY(PUMPENDREHZAHL_HEIZEN, 0x02cb, Type::et_dec_val);
    PROPERTY(LAUFZEIT_FILTER_TAGE, 0x0341);
    PROPERTY(WAERMEERTRAG_RUECKGE_TAG_WH, 0x03ae);
    PROPERTY(WAERMEERTRAG_RUECKGE_TAG_KWH, 0x03af);
    PROPERTY(WAERMEERTRAG_RUECKGE_SUM_KWH, 0x03b0);
    PROPERTY(WAERMEERTRAG_RUECKGE_SUM_MWH, 0x03b1);
    PROPERTY(KUEHL_RAUMSOLL_TAG, 0x0569, Type::et_dec_val);
    PROPERTY(KUEHL_RAUMSOLL_ABWESEND, 0x056a, Type::et_dec_val);
    PROPERTY(KUEHL_RAUMSOLL_NACHT, 0x056b, Type::et_dec_val);
    PROPERTY(LUEFT_STUFE_TAG, 0x056c);
    PROPERTY(LUEFT_STUFE_NACHT, 0x056d);
    PROPERTY(LUEFT_STUFE_BEREITSCHAFT, 0x056f);
    PROPERTY(LUEFT_STUFE_PARTY, 0x0570);
    PROPERTY(PASSIVKUEHLUNG, 0x0575);
    PROPERTY(LUEFT_ZEIT_AUSSERPLAN_STUFE1, 0x0572);
    PROPERTY(LUEFT_ZEIT_AUSSERPLAN_STUFE2, 0x0573);
    PROPERTY(LUEFT_ZEIT_AUSSERPLAN_STUFE3, 0x0574);
    PROPERTY(LUEFT_ZULUFT_STUFE1, 0x0576);
    PROPERTY(LUEFT_ZULUFT_STUFE2, 0x0577);
    PROPERTY(LUEFT_ZULUFT_STUFE3, 0x0578);
    PROPERTY(LUEFT_ABLUFT_STUFE1, 0x0579);
    PROPERTY(LUEFT_ABLUFT_STUFE2, 0x057a);
    PROPERTY(LUEFT_ABLUFT_STUFE3, 0x057b);
    PROPERTY(NE_STUFE_WW, 0x058a);
    PROPERTY(ZULUFT_SOLL, 0x0596);
    PROPERTY(ZULUFT_IST, 0x0597);
    PROPERTY(ABLUFT_SOLL, 0x0598);
    PROPERTY(ABLUFT_IST, 0x0599);
    PROPERTY(FORTLUFT_SOLL, 0x059a);
    PROPERTY(FORTLUFT_IST, 0x059b);
    PROPERTY(SOMMERBETRIEB_HYSTERESE, 0x05a2, Type::et_dec_val);
    PROPERTY(PUMPENZYKLEN_MIN_AUSSENT, 0x05bb);
    PROPERTY(SPEICHERSOLLTEMP_NACHT, 0x05bf, Type::et_dec_val);
    PROPERTY(LUEFT_STUFE_HAND, 0x0612);
    PROPERTY(KUEHLSYSTEM, 0x0613);
    PROPERTY(DRUCK_HEIZKREIS, 0x064a, Type::et_dec_val);
    PROPERTY(LEISTUNG_AUSLEGUNG_KUEHLEN, 0x0692);
    PROPERTY(HEIZLEISTUNG_RELATIV, 0x069a, Type::et_double_val);
    PROPERTY(VERDICHTERDREHZAHL, 0x069e);
    PROPERTY(MOTORSTROM, 0x069f);
    PROPERTY(MOTORLEISTUNG, 0x06a0, Type::et_cent_val);
    PROPERTY(MOTORSPANNUNG, 0x06a1);
    PROPERTY(ABLUFTTEMP, 0x0694, Type::et_dec_val);
    PROPERTY(ANZEIGE_NIEDERDRUCK, 0x07a7, Type::et_dec_val);
    PROPERTY(WAERMEERTRAG_2WE_WW_TAG_WH, 0x0922);
    PROPERTY(WAERMEERTRAG_2WE_WW_TAG_KWH, 0x0923, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_2WE_WW_SUM_KWH, 0x0924);
    PROPERTY(WAERMEERTRAG_2WE_WW_SUM_MWH, 0x0925, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_2WE_HEIZ_TAG_WH, 0x0926);
    PROPERTY(WAERMEERTRAG_2WE_HEIZ_TAG_KWH, 0x0927, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_2WE_HEIZ_SUM_KWH, 0x0928);
    PROPERTY(WAERMEERTRAG_2WE_HEIZ_SUM_MWH, 0x0929, Type::et_double_val);
    PROPERTY(HEIZ_KUEHL_LEISTUNG, 0xc0ee, Type::et_cent_val);
    PROPERTY(ABLUFTFEUCHTE, 0xc0ef);
    PROPERTY(LEISTUNG_AUSLEGUNG_HEIZUNG, 0xc0f1);
    PROPERTY(VERDICHTER_STARTS, 0xc0f4);
    PROPERTY(VERDICHTER_STARTS_K, 0xc0f5);
    PROPERTY(ABLUFT_TAUPUNKT, 0xc0f6, Type::et_dec_val);
    PROPERTY(LAUFZEIT_FILTER, 0xc111);
    PROPERTY(DIFFERENZDRUCK, 0xc11e);
    PROPERTY(BETRIEBS_STATUS_2, 0xc356);
#endif

#if defined(TTF_07_C)
    PROPERTY(WW_KOMF_TEMP, 0x0013, Type::et_dec_val);
    PROPERTY(WW_HYSTERESE, 0x0022, Type::et_dec_val);
    PROPERTY(PUFFERISTTEMP, 0x0078, Type::et_dec_val);
    PROPERTY(QUELLE_IST, 0x01d4, Type::et_dec_val);
    PROPERTY(PUFFERSOLLTEMP, 0x01d5, Type::et_dec_val);
    PROPERTY(MAXVORLAUFTEMP, 0x01e8, Type::et_dec_val);
    PROPERTY(HKSOLLTEMP, 0x01d7, Type::et_dec_val);
    PROPERTY(HDSENSORMAX, 0x01a2, Type::et_dec_val);
    PROPERTY(QUELLENTEMP_MIN, 0x01b0, Type::et_dec_val);
    PROPERTY(HEIZGASTEMP, 0x0265, Type::et_dec_val);
    PROPERTY(DRUCK_HOCHDRUCK, 0x0268, Type::et_dec_val);
    PROPERTY(HKISTTEMP, 0x02ca, Type::et_dec_val);
    PROPERTY(VOLUMENSTROM, 0x0673, Type::et_cent_val);
    PROPERTY(DRUCK_HEIZKREIS, 0x0674, Type::et_cent_val);
    PROPERTY(QUELLENDRUCK, 0x0675, Type::et_cent_val);
    PROPERTY(VORLAUFISTTEMP, 0x06a1, Type::et_dec_val);
    PROPERTY(KUEHLEN_EINGESCHALTET, 0x06Aa, Type::et_little_bool);
    PROPERTY(LEISTUNG_HEIZKREISPUMPE, 0x070a, Type::et_byte);
    PROPERTY(LEISTUNG_WARMWASSERPUMPE, 0x070b, Type::et_byte);
    PROPERTY(LEISTUNG_SOLEPUMPE, 0x070c, Type::et_byte);
    PROPERTY(EINPHASIGER_BETRIEB, 0x070e, Type::et_bool);
    PROPERTY(VERDICHTER_STARTS, 0x071d);
    PROPERTY(VERDICHTER_STARTS_K, 0x071c);
    PROPERTY(DRUCK_NIEDERDRUCK, 0x07a5, Type::et_dec_val);
    PROPERTY(ANLAGENFROST, 0x0a00, Type::et_dec_val);
    PROPERTY(BETRIEBS_STATUS, 0x0a20, Type::et_default);
    PROPERTY(WW_ECO_TEMP, 0x0a06, Type::et_dec_val);
    PROPERTY(QUELLENMEDIUM, 0xfdae, Type::et_byte);
    PROPERTY(REGLERDYNAMIK, 0xfdb0, Type::et_little_endian);
    PROPERTY(VERDICHTER_STILLSTAND, 0xfdb1, Type::et_little_endian);
#endif

#if defined(WPL_13)
    PROPERTY(EINSTELL_SPEICHERSOLLTEMP, 0x0013, Type::et_dec_val);
    PROPERTY(PUFFERISTTEMPERATUR, 0x0078, Type::et_dec_val);
    PROPERTY(SOLLTEMP_HK1, 0x01d5, Type::et_dec_val);
    PROPERTY(VORLAUFTEMP, 0x01d6, Type::et_dec_val);
    PROPERTY(MAXVORLAUFTEMP, 0x01e8, Type::et_dec_val);
    PROPERTY(HEISSGASTEMP, 0x0265, Type::et_dec_val);
    PROPERTY(EL_NACHERW_ANZ_STUFEN, 0x059f);
    PROPERTY(EL_NACHERW_VERZ_ZEIT, 0x05a0);
    PROPERTY(VERDICHTER_EINTRITTSTEMP, 0x06d9, Type::et_dec_val);
    PROPERTY(ISTDREHZAHL_VERDICHTER, 0x06eb);
    PROPERTY(SOLLDREHZAHL_VERDICHTER, 0x06ec);
    PROPERTY(VERDAMPFERTEMP, 0x07a9, Type::et_dec_val);
    PROPERTY(FROSTSCHUTZ, 0xfe07, Type::et_dec_val);
    PROPERTY(VERFLUESSIGERTEMP, 0x0a37, Type::et_dec_val);
    PROPERTY(OELSUMPFTEMP, 0x0a39, Type::et_dec_val);
    PROPERTY(WAERMEPUMPEN_STATUS, 0x02e2);
    PROPERTY(MINTEMP, 0x4ea4, Type::et_dec_val);
    PROPERTY(MAXTEMP, 0x4ea8, Type::et_dec_val);
    PROPERTY(MISCHERDYNAMIK, 0x4ea9);
    PROPERTY(SOLLTEMP_HK2, 0x4eb0, Type::et_dec_val);
    PROPERTY(ISTTEMPERATUR, 0x4eb4, Type::et_dec_val);
    PROPERTY(KOMFORTTEMPERATUR, 0x4eb8, Type::et_dec_val);
    PROPERTY(ECOTEMPERATUR, 0x4eb9, Type::et_dec_val);
    PROPERTY(RAUMISTTEMP, 0x4ec7, Type::et_dec_val);
    PROPERTY(RAUMSOLLTEMP, 0x4ece, Type::et_dec_val);
    PROPERTY(RAUMFEUCHTE, 0x4ec8, Type::et_dec_val);
    PROPERTY(TAUPUNKTTEMP, 0x4ee0, Type::et_dec_val);
    PROPERTY(RAUMEINFLUSS, 0x4f33);
    PROPERTY(HEIZUNGSDRUCK, 0x4f46, et_cent_val);
    PROPERTY(VOLUMENSTROM, 0x4f47, Type::et_dec_val);
    PROPERTY(HEIZEN_EFFIZIENZ_TAG, 0x501d, Type::et_cent_val);
    PROPERTY(HEIZEN_EFFIZIENZ_JAHR, 0x501e, Type::et_cent_val);
    PROPERTY(RAUMSOLLTEMP_KUEHLEN, 0x4f04, Type::et_dec_val);
    PROPERTY(RUECKLAUFTEMP_QUELLE, 0x4fa6, Type::et_dec_val);
    PROPERTY(VORLAUFTEMP_QUELLE, 0x4fa7, Type::et_dec_val);
    PROPERTY(QUELLENDRUCK, 0x4fa8, Type::et_dec_val);
    PROPERTY(LEISTUNG_QUELLENPUMPE, 0x4fa9, Type::et_dec_val);
    PROPERTY(STROM_INVERTER, 0x06b2, Type::et_dec_val);
    PROPERTY(SPANNUNG_INVERTER, 0x06b1, Type::et_dec_val);
    PROPERTY(DRUCK_HOCHDRUCK, 0x07a6, Type::et_dec_val);
    PROPERTY(DRUCK_NIEDERDRUCK, 0x07a7, Type::et_dec_val);
    PROPERTY(VORLAUFISTTEMP, 0xfdf3, Type::et_dec_val);
    PROPERTY(BETRIEBS_STATUS, 0x4ecd);
    PROPERTY(VERDICHTER_STARTS, 0x4ef1);
    PROPERTY(VERDICHTER_STARTS_K, 0x4ef0);
    PROPERTY(HYSTERESE_VORLAUFTEMP_KUEHLEN, 0x4f00, Type::et_dec_val);
    PROPERTY(PROGRAMMSCHALTER, 0x4f1b, Type::et_betriebsart);
    PROPERTY(VORLAUFISTTEMP_WP, 0x4f41, Type::et_dec_val);
    PROPERTY(RUECKLAUFISTTEMP_WP, 0x4f43, Type::et_dec_val);
    PROPERTY(LEISTUNG_KUEHLEN, 0x7a40, Type::et_dec_val);
    PROPERTY(RUECKLAUFISTTEMP, 0xfdf4, Type::et_dec_val);
#endif
};

#endif
