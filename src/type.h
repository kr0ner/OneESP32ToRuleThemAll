#if !defined(TYPE_H)
#define TYPE_H

#include <cstdint>
#include "simple_variant.h"

enum Type : std::uint8_t {
    et_default = 0,  //   + x
    et_dec_val,      // +/- xx.x
    et_cent_val,     // +/- x.xx
    et_mil_val,      // +/- x.xxx
    et_byte,
    et_bool,         // 0x0000 und 0x0001
    et_little_bool,  // 0x0000 und 0x0100
    et_double_val,
    et_triple_val,
    et_little_endian,
    et_betriebsart,
    et_zeit,
    et_datum,
    et_time_domain,
    et_dev_nr,
    et_err_nr,
    et_dev_id
};

/**
 * @brief Helper to get the parameter type of a given function that can then be
 *        used during compile time to decide which conversion function to call.
 */
template <typename C, typename T>
T getParamType(void (C::*)(T));

/**
 * @brief The raw std::uint16_t value is converted to the corresponding type and
 *        returned as \c SimpleVariant.
 */
SimpleVariant GetValueByType(const std::uint16_t value, const Type type);

/**
 * @brief Toggles endianness of an unsigned 16-bit integer.
 *        Needed whenever payload of type et_little_endian is sent/received
 *        little endian (CAN bus) -> big endian    (ESPHome)
 *        big endian    (ESPHome) -> little endian (CAN bus)
 */
uint16_t toggleEndianness(const uint16_t value);

#endif
