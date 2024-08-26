
/*
 *  Copyright (C) 2023 Bastian Stahmer, based heavily on the great work of Jürg Müller, CH-5524 (see below)
 *  Copyright (C) 2014 Jürg Müller, CH-5524
 *
 *  This file is part of ha-stiebel-control.
 *  ha-stiebel-control is free software: : you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see http://www.gnu.org/licenses/ .
 */

// Achtung: Die Tabelle befindet sich im Aufbau! Nicht alle Indizes sind korrekt.
//
// Die Indizes muessen zwingend vor einem produktiven Einsatz ueberprueft werden.
// Die beste Ueberpruefung erhaelt man mit der ComfortSoft und der Simulation
// der eigenen WP. Da werden die Telegramme der Schnittstelle protokolliert.
//
// Die Indizes, die ich mittels ComfortSoft zu meiner WP ueberpruefen konnte,
// sind korrekt. Da ich eine WPL33 ohne PV nutze, gibt es viele Indizes, die Ihr
// selbst ueberpruefen muesst.

// 19.10.2014: et_double_val und et_triple_val umgestellt
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
