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
#include "type.h"

#include <cstring>
#include <algorithm>

#include "mappings.h"

void SetValueType(char * val, const Type type, const std::uint16_t value)
{
  switch (type)
  {
    case Type::et_byte:
      sprintf(val, "%d", (value & 0xFF));
      break;

    case Type::et_dec_val:
      sprintf(val, "%.1f", (value / 10.0));
      break;

    case Type::et_cent_val:
      sprintf(val, "%.2f", (value / 100.0));
      break;

    case Type::et_mil_val:
      sprintf(val, "%.3f", (value/ 1000.0));
      break;

    case Type::et_little_endian:
      sprintf(val, "%d", ((value >> 8U) & 0xFF) | ((value & 0xff) << 8U));
      break;
      
    case Type::et_little_bool:
      if (value == 0x0100) {
        strcpy(val, "on");
      } else {
        if (!value) {
          strcpy(val, "off");
        }
        else {
          strcpy(val, "?");
        }
      }
      break;
      
    case Type::et_bool:
      if (value == 0x0001) {
        strcpy(val, "on");
      } else {
        if (!value) {
          strcpy(val, "off");
        } else {
          strcpy(val, "?");
        }
      }
      break;
    
    case Type::et_betriebsart:
      {
        const auto it = std::find_if(BetriebsartMappings.begin(), BetriebsartMappings.end(), [value](const BetriebsartMapping& element){ return element.id == value;});
        if (it != BetriebsartMappings.end()) {
          strcpy(val, it->name);
        } else {
          strcpy(val, "?");
        }
      }
      break;

    case Type::et_zeit:{
      sprintf(val, "%2.2d:%2.2d", ((value >> 8U) & 0xff), (value & 0xff));
      break;
    }

    case Type::et_datum:
      sprintf(val, "%2.2d.%2.2d.", ((value >> 8U) & 0xff), (value & 0xff));
      break;

    case Type::et_time_domain:
      if (value & 0x8080) {
        strcpy(val, "not used time domain");
      } else {
        sprintf(val, "%2.2d:%2.2d-%2.2d:%2.2d",
                (value >> 8) / 4, 15*((value >> 8) % 4),
                (value & 0xff) / 4, 15*(value % 4));
      }
      break;

    case Type::et_dev_nr:
      if (value >= 0x80) {
        strcpy(val, "--");
      } else {
        sprintf(val, "%d", value + 1);
      }
      break;

    case Type::et_dev_id:
      sprintf(val, "%d-%2.2d", ((value >> 8U) & 0xff), (value & 0xff));
      break;

    case Type::et_err_nr:
    {
      const auto it = std::find_if(ErrorMappings.begin(), ErrorMappings.end(), [value](const ErrorMapping& element){ return element.id == value;});
      if(it != ErrorMappings.end()) {
        strcpy(val, it->name);
      } else {
        sprintf(val, "ERR %d", value);
      }
      break;
    }

    case Type::et_double_val:
      sprintf(val, "%.3f", value * 1.0f);
      break;

    case Type::et_triple_val:
      sprintf(val, "%.6f", value * 1.0f);
      break;

    case Type::et_default:
    default:
      sprintf(val, "%d", value);
      break;
  }
}
