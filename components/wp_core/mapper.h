
#if !defined(MAPPER_H)
#define MAPPER_H

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include "property.h"
#include "type.h"

class Mapper {
   public:
    std::optional<std::string> getError(const std::uint16_t id) const;

    std::optional<std::uint16_t> getErrorId(const std::string& error) const;

    std::optional<std::string> getBetriebsart(const std::uint16_t id) const;

    std::optional<std::uint16_t> getBetriebsartId(const std::string& betriebsart) const;

    std::optional<std::string> getKuehlmodus(const std::uint16_t id) const;

    std::optional<std::uint16_t> getKuehlmodusId(const std::string& kuehlmodus) const;

    std::optional<std::string> getPassivkuehlung(const std::uint16_t id) const;

    std::optional<std::uint16_t> getPassivkuehlungId(const std::string& passivkuehlung) const;

    static Mapper& instance() {
        static Mapper mapper;
        return mapper;
    }

   private:
    Mapper();
    std::unordered_map<std::uint16_t, std::string> errorMap;
    std::unordered_map<std::uint16_t, std::string> betriebsartMap;
    std::unordered_map<std::uint16_t, std::string> kuehlmodusMap;
    std::unordered_map<std::uint16_t, std::string> passivkuehlungMap;
};

#endif
