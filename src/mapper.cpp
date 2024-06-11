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

}
