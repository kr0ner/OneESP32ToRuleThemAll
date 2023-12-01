#if !defined(TYPE_H)
#define TYPE_H

#include <cstdint>
#include <simple_variant.h>

enum Type : std::uint8_t
{
    et_default = 0,
    et_dec_val,       // Auflösung: xx.x / auch neg. Werte sind möglich
    et_cent_val,      // x.xx
    et_mil_val,       // x.xxx
    et_byte,
    et_bool,          // 0x0000 und 0x0001
    et_little_bool,   // 0x0000 und 0x0100
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

SimpleVariant GetValueByType(const std::uint16_t value, const Type type);

#endif
