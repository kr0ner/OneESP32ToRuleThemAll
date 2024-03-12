/*
 *  Copyright (C) 2023 Bastian Stahmer
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

#if !defined(heatingpump_H)
#define heatingpump_H
#include <cstdint>
#include <vector>
#include <string>
#include <queue>

#include "mappings.h"
#include "property.h"
#include "type.h"

using CANID=std::uint16_t;

struct CanMember
{
    CANID CanId;
    std::uint8_t ReadId[2U];
    std::uint8_t WriteId[2U];
    std::uint8_t ConfirmationId[2U];
};

//  Name                          CanId      ReadId          WriteId         ConfirmationID
static const CanMember ESPClient {0x6a2,    {0x00, 0x00},   {0x00, 0x00},   {0xE2, 0x00}}; //The ESP Home Client, thus no valid read/write IDs
static const CanMember Anfrage   {0x6a1,    {0x00, 0x00},   {0x00, 0x00},   {0x00, 0x00}};
static const CanMember Kessel    {0x180,    {0x31, 0x00},   {0x30, 0x00},   {0x00, 0x00}};
static const CanMember HK1       {0x301,    {0x61, 0x01},   {0x60, 0x01},   {0x00, 0x00}};
static const CanMember HK2       {0x302,    {0x61, 0x02},   {0x60, 0x02},   {0x00, 0x00}};

using Request = std::pair<const CanMember, const Property>;
static std::queue<Request> request_queue;

void queueRequest(const CanMember& member, const Property& property) {
    ESP_LOGI("QUEUE","Requesting data for %04x", property);
    request_queue.push(std::make_pair(member,property));
}

std::pair<Property,SimpleVariant> processCanMessage(const std::vector<std::uint8_t>& msg)
{
    Property property{Property::kINDEX_NOT_FOUND};
    std::uint8_t byte1{0U};
    std::uint8_t byte2{0U};

    // Return if the message is too small
    if(msg.size() < 7U) {
        return {Property::kINDEX_NOT_FOUND,{}};
    }

    if(msg[2U] == 0xfa) {
        byte1 = msg[5U];
        byte2 = msg[6U];
        property = static_cast<Property>(msg[4U] | (msg[3U] << 8U));
    } else {
        byte1 = msg[3U];
        byte2 = msg[4U];
        property = static_cast<Property>(msg[2U]);
    }

    const auto it = std::find_if(PropertyTypeMappings.begin(),
                                 PropertyTypeMappings.end(),
                                 [property](const PropertyTypeMapping property_type_mapping){
        return property_type_mapping.property == property;
    });
    if(it != PropertyTypeMappings.end()) {
        const std::uint16_t value{static_cast<std::uint16_t>((byte1<<8U) | byte2)};
        return {property,GetValueByType(value,it->type)};
    }
    return {Property::kINDEX_NOT_FOUND,{}};
}

void requestData(const CanMember& member, const Property& property) {
    const auto use_extended_id{false}; //No use of extended ID
    const std::uint8_t IdByte1{member.ReadId[0U]};
    const std::uint8_t IdByte2{member.ReadId[1U]};
    const std::uint8_t IndexByte1{static_cast<std::uint8_t>((property >> 8U) & 0xff)};
    const std::uint8_t IndexByte2{static_cast<std::uint8_t>(property & 0xff)};
    std::vector<std::uint8_t> data;

    data.insert(data.end(), {
        IdByte1,
        IdByte2,
        0xfa,
        IndexByte1,
        IndexByte2,
        0x00,
        0x00
    });

    id(my_mcp2515).send_data(ESPClient.CanId, use_extended_id, data);
}

void sendData(const CanMember& member, const Property property, const std::uint16_t value) {
    const auto use_extended_id{false}; //No use of extended ID
    const std::uint8_t IdByte1{member.WriteId[0U]};
    const std::uint8_t IdByte2{member.WriteId[1U]};
    const std::uint8_t IndexByte1{static_cast<std::uint8_t>((property >> 8U) & 0xff)};
    const std::uint8_t IndexByte2{static_cast<std::uint8_t>(property & 0xff)};
    const std::uint8_t ValueByte1{static_cast<std::uint8_t>((value>>8U) & 0xff)};
    const std::uint8_t ValueByte2{static_cast<std::uint8_t>(value & 0xff)};
    std::vector<std::uint8_t> data;

    data.insert(data.end(), {
        IdByte1,
        IdByte2,
        0xfa,
        IndexByte1,
        IndexByte2,
        ValueByte1,
        ValueByte2
    });
    
    id(my_mcp2515).send_data(ESPClient.CanId, use_extended_id, data);
}

#endif