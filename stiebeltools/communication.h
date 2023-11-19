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
#include "mappings.h"
#include "type.h"
#include "property.h"
#include <queue>

using CANID=std::uint16_t;

struct CanMember
{
    CANID CanId;
    std::uint8_t ReadId[2U];
    std::uint8_t WriteId[2U];
    std::uint8_t ConfirmationId[2U];
};

/*
#################################################################
#Stiebel Eltron WPL 13 E (2013)
#WPM3i software version 391-08
#FEK software version 416 - 02
#################################################################
#WPL 13 E: CAN ID 180: read - 3100, write - 3000 # Pump
#WPL 13 E: CAN ID 500: read - A100, write - A000 # Heating Module
#WPL 13 E: CAN ID 480: read - 9100, write - 9000 # Manager

#OLD: CAN ID 301: read - 0c01, FEK-device (no active can request, only listening)
#
#OLD: other addresses
#OLD:   180 read: 3100  write: 3000
#OLD:  	301	read: 6101  write: 6001
#OLD: 	480	read: 9100  write: 9000    WMPme Wärmepumpenmanager
#OLD: 	601	read: C101  write: C001
#OLD: 	680	confirmation: D200
#################################################################
*/

//  Name               CanId      ReadId          WriteId         ConfirmationID
static const CanMember ESPClient {0x700,    {0x00, 0x00},   {0x00, 0x00},   {0xE2, 0x00}}; //The ESP Home Client, thus no valid read/write IDs
static const CanMember Kessel    {0x180,    {0x31, 0x00},   {0x30, 0x00},   {0x00, 0x00}};
static const CanMember Manager   {0x480,    {0x91, 0x00},   {0x90, 0x00},   {0x00, 0x00}};
static const CanMember Heizmodul {0x500,    {0xA1, 0x00},   {0xA0, 0x00},   {0x00, 0x00}};

using Request = std::pair<const CanMember, const Property>;
static std::queue<Request> request_queue;

void queueRequest(const CanMember& member, const Property& property) {
    request_queue.push(std::make_pair(member,property));
}

Property processCanMessage(std::string &signalValue, const std::vector<std::uint8_t>& msg)
{
    Property property{Property::kINDEX_NOT_FOUND};
    std::uint8_t byte1{0U};
    std::uint8_t byte2{0U};
    char charValue[16U];

    // Return if the message is too small
    if(msg.size() < 7U) {
        return property;
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

    const auto it = std::find_if(PropertyTypeMappings.begin(), PropertyTypeMappings.end(), [property](const PropertyTypeMapping property_type_mapping){ return property_type_mapping.property == property; });
    if(it != PropertyTypeMappings.end()) {
        const std::uint16_t value{static_cast<std::uint16_t>((byte1<<8U) | byte2)};
        SetValueType(charValue, it->type, value);
        signalValue = std::string(charValue);
        return property;
    }
    return Property::kINDEX_NOT_FOUND;
}

void requestData(const CanMember& member, const Property& property) {
    const auto use_extended_id{false}; //No use of extended ID
    const std::uint8_t IdByte1{member.WriteId[0U]};
    const std::uint8_t IdByte2{member.WriteId[1U]};
    const std::uint8_t IndexByte1{static_cast<std::uint8_t>((property >> 8U) & 0xff)};
    const std::uint8_t IndexByte2{static_cast<std::uint8_t>(property & 0xff)};
    std::vector<std::uint8_t> data;

    if(IndexByte1 == 0x00) {
        data.insert(data.end(), {
            IdByte1,
            IdByte2,
            IndexByte2,
            0x00,
            0x00,
            0x00,
            0x00
        });
    } else {
        data.insert(data.end(), {
            IdByte1,
            IdByte2,
            0xfa,
            IndexByte1,
            IndexByte2,
            0x00,
            0x00
        });
    }

    ESP_LOGD("requestData","%02x %02x %02x %02x %02x %02x %02x", data[0U],data[1U],data[2U],data[3U],data[4U],data[5U],data[6U]);
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

    if(IndexByte1 == 0x00) {
        data.insert(data.end(), {
            IdByte1,
            IdByte2,
            IndexByte2,
            ValueByte1,
            ValueByte2,
            0x00,
            0x00
        });
    } else {
        data.insert(data.end(), {
            IdByte1,
            IdByte2,
            0xfa,
            IndexByte1,
            IndexByte2,
            ValueByte1,
            ValueByte2
        });
    }
    
    ESP_LOGD("sendData","%02x %02x %02x %02x %02x %02x %02x", data[0U],data[1U],data[2U],data[3U],data[4U],data[5U],data[6U]);
    id(my_mcp2515).send_data(ESPClient.CanId, use_extended_id, data);
}

#endif