#include "mapper.h"

std::optional<std::string> Mapper::getError(const std::uint16_t id) const {
    const auto it = errorMap.find(id);
    if (it != errorMap.end()) {
        return it->second;
    }
    return {};
}

std::optional<std::string> Mapper::getBetriebsart(const std::uint16_t id) const {
    const auto it = betriebsartMap.find(id);
    if (it != betriebsartMap.end()) {
        return it->second;
    }
    return {};
}

std::optional<std::uint16_t> Mapper::getBetriebsartId(const std::string& betriebsart) const {
    for (const auto& element : betriebsartMap) {
        if (element.second == betriebsart) {
            return element.first;
        }
    }
    return {};
}

std::optional<std::string> Mapper::getKuehlmodus(const std::uint16_t id) const {
    const auto it = kuehlmodusMap.find(id);
    if (it != kuehlmodusMap.end()) {
        return it->second;
    }
    return {};
}

std::optional<std::uint16_t> Mapper::getKuehlmodusId(const std::string& kuehlmodus) const {
    for (const auto& element : kuehlmodusMap) {
        if (element.second == kuehlmodus) {
            return element.first;
        }
    }
    return {};
}

Type Mapper::getType(const Property property) const {
    const auto it = propertyMap.find(property);
    if (it != propertyMap.end()) {
        return it->second;
    } else {
        return Type::et_default;
    }
}

Mapper::Mapper() {
    errorMap = {{0x0002, "Schuetz klebt"},
                {0x0003, "ERR HD-SENSOR"},
                {0x0004, "Hochdruck"},
                {0x0005, "Verdampferfuehler"},
                {0x0006, "Relaistreiber"},
                {0x0007, "Relaispegel"},
                {0x0008, "Hexschalter"},
                {0x0009, "Drehzahl Luefter"},
                {0x000a, "Lueftertreiber"},
                {0x000b, "Reset Baustein"},
                {0x000c, "ND"},
                {0x000d, "ROM"},
                {0x000e, "QUELLEN MINTEMP"},
                {0x0010, "Abtauen"},
                {0x0012, "ERR T-HEI IWS"},
                {0x0017, "ERR T-FRO IWS"},
                {0x001a, "Niederdruck"},
                {0x001b, "ERR ND-DRUCK"},
                {0x001c, "ERR HD-DRUCK"},
                {0x001d, "HD-SENSOR-MAX"},
                {0x001e, "HEISSGAS-MAX"},
                {0x001f, "ERR HD-SENSOR"},
                {0x0020, "Einfrierschutz"},
                {0x0021, "KEINE LEISTUNG"}};

    betriebsartMap = {{0x0000, "Notbetrieb"},    {0x0100, "Bereitschaft"}, {0x0300, "Tagbetrieb"},
                      {0x0400, "Absenkbetrieb"}, {0x0500, "Warmwasser"},   {0x0B00, "Automatik"},
                      {0x0E00, "Handbetrieb"}};

    kuehlmodusMap = {{0x0000, "Flaechenkuehlung"}, {0x0001, "Geblaesekuehlung"}};

    propertyMap = {{Property::kSPEICHERSOLLTEMP, Type::et_dec_val},
                   {Property::kVORLAUFSOLLTEMP, Type::et_dec_val},
                   {Property::kRAUMSOLLTEMP_I, Type::et_dec_val},
                   {Property::kAUSSENTEMP, Type::et_dec_val},
                   {Property::kSAMMLERISTTEMP, Type::et_dec_val},
                   {Property::kSPEICHERISTTEMP, Type::et_dec_val},
                   {Property::kVORLAUFISTTEMP, Type::et_dec_val},
                   {Property::kRAUMISTTEMP, Type::et_dec_val},
                   {Property::kVERSTELLTE_RAUMSOLLTEMP, Type::et_dec_val},
                   {Property::kEINSTELL_SPEICHERSOLLTEMP, Type::et_dec_val},
                   {Property::kVERDAMPFERTEMP, Type::et_dec_val},
                   {Property::kRUECKLAUFISTTEMP, Type::et_dec_val},
                   {Property::kFEUCHTE, Type::et_dec_val},
                   {Property::kRAUMEINFLUSS, Type::et_little_endian},
                   {Property::kPROGRAMMSCHALTER, Type::et_betriebsart},
                   {Property::kVOLUMENSTROM, Type::et_dec_val},
                   {Property::kPUMPENDREHZAHL_HEIZEN, Type::et_dec_val},
                   {Property::kKUEHL_RAUMSOLL_TAG, Type::et_dec_val},
                   {Property::kKUEHL_RAUMSOLL_ABWESEND, Type::et_dec_val},
                   {Property::kKUEHL_RAUMSOLL_NACHT, Type::et_dec_val},
                   {Property::kDRUCK_HEIZKREIS, Type::et_dec_val},
                   {Property::kANZEIGE_NIEDERDRUCK, Type::et_dec_val},
                   {Property::kEL_AUFNAHMELEISTUNG_WW_TAG_KWH, Type::et_double_val},
                   {Property::kEL_AUFNAHMELEISTUNG_WW_SUM_MWH, Type::et_double_val},
                   {Property::kEL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH, Type::et_double_val},
                   {Property::kEL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH, Type::et_double_val},
                   {Property::kWAERMEERTRAG_2WE_WW_TAG_KWH, Type::et_double_val},
                   {Property::kWAERMEERTRAG_2WE_WW_SUM_MWH, Type::et_double_val},
                   {Property::kWAERMEERTRAG_2WE_HEIZ_TAG_KWH, Type::et_double_val},
                   {Property::kWAERMEERTRAG_2WE_HEIZ_SUM_MWH, Type::et_double_val},
                   {Property::kWAERMEERTRAG_WW_TAG_KWH, Type::et_double_val},
                   {Property::kWAERMEERTRAG_WW_SUM_MWH, Type::et_double_val},
                   {Property::kWAERMEERTRAG_HEIZ_TAG_KWH, Type::et_double_val},
                   {Property::kWAERMEERTRAG_HEIZ_SUM_MWH, Type::et_double_val}};
}
