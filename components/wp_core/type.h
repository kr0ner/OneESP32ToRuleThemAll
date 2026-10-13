#if !defined(TYPE_H)
#define TYPE_H

#include <cstdint>
#include <optional>
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
    et_passivkuehlung,
    et_zeit,
    et_datum,
    et_time_domain,
    et_dev_nr,
    et_err_nr,
    et_dev_id
};

/**
 * @brief Compile-time classification of the CAN property types according to the
 *        alternative they produce/consume in \c SimpleVariant via
 *        \c GetValueByType / \c GetRawByType. Allows static_asserts in the
 *        ESPHome template lambdas to verify that the expected variant
 *        alternative is used, so no runtime type check is ever needed.
 */
template <Type T, Type... Ts>
inline constexpr bool is_any_of_v = ((T == Ts) || ...);

// Variant-alternative classification: what value.get<T>() will return.
// This is the only guarantee SimpleVariant gives; it says nothing about
// scaling or masking, which are type-specific semantics on top of it.
template <Type T>
inline constexpr bool holds_float = is_any_of_v<T,
                                                Type::et_default,         // raw 16-bit word, unmasked
                                                Type::et_double_val,      // int16 word
                                                Type::et_triple_val,      // int16 word
                                                Type::et_byte,            // low byte masked
                                                Type::et_dec_val,         // signed /10
                                                Type::et_cent_val,        // signed /100
                                                Type::et_mil_val,         // signed /1000
                                                Type::et_little_endian>;  // byte-swapped
template <Type T>
inline constexpr bool holds_bool = is_any_of_v<T, Type::et_bool, Type::et_little_bool>;
template <Type T>
inline constexpr bool holds_string =
    is_any_of_v<T, Type::et_betriebsart, Type::et_passivkuehlung, Type::et_zeit, Type::et_datum, Type::et_time_domain,
                Type::et_dev_nr, Type::et_err_nr, Type::et_dev_id>;

// The C++ type used in value.get<T>() is the normalized variant alternative,
// so compare against that. std::uint16_t / int / etc. are normalized to float.
static_assert(holds_float<Type::et_byte> == std::is_same_v<normalized_t<std::uint16_t>, float>);
static_assert(holds_bool<Type::et_bool> == std::is_same_v<normalized_t<bool>, bool>);
static_assert(holds_string<Type::et_err_nr> == std::is_same_v<normalized_t<std::string>, std::string>);

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
 * @brief Converts a typed \c SimpleVariant to its raw 16-bit representation.
 *
 * Inverse of GetValueByType(). Returns \c std::nullopt on conversion failure.
 */
std::optional<std::uint16_t> GetRawByType(const SimpleVariant& value, const Type type);

/**
 * @brief Toggles endianness of an unsigned 16-bit integer.
 *        Needed whenever payload of type et_little_endian is sent/received
 *        little endian (CAN bus) -> big endian    (ESPHome)
 *        big endian    (ESPHome) -> little endian (CAN bus)
 */
uint16_t toggleEndianness(const uint16_t value);

#endif
