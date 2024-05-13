#if !defined(TYPE_H)
#define TYPE_H

#include <cstdint>
#include "property.h"
#include "simple_variant.h"

enum Type : std::uint8_t {
    et_default = 0,
    et_dec_val,   // Auflösung: xx.x / auch neg. Werte sind möglich
    et_cent_val,  // x.xx
    et_mil_val,   // x.xxx
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

SimpleVariant GetValueByType(const std::uint16_t value, const Type type);

template <Property P>
struct TypeMapping {
    using type = std::uint16_t;
};

template <>
struct TypeMapping<Property::kSPEICHERSOLLTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kVORLAUFSOLLTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kRAUMSOLLTEMP_I> {
    using type = float;
};
template <>
struct TypeMapping<Property::kAUSSENTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSAMMLERISTTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSPEICHERISTTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kVORLAUFISTTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kRAUMISTTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kVERSTELLTE_RAUMSOLLTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kEINSTELL_SPEICHERSOLLTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kVERDAMPFERTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kRUECKLAUFISTTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kFEUCHTE> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPROGRAMMSCHALTER> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kVOLUMENSTROM> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUMPENDREHZAHL_HEIZEN> {
    using type = float;
};
template <>
struct TypeMapping<Property::kANZEIGE_NIEDERDRUCK> {
    using type = float;
};
template <>
struct TypeMapping<Property::kDRUCK_HEIZKREIS> {
    using type = float;
};
template <>
struct TypeMapping<Property::kEL_AUFNAHMELEISTUNG_WW_TAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kEL_AUFNAHMELEISTUNG_WW_SUM_MWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kEL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kEL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWAERMEERTRAG_2WE_WW_TAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWAERMEERTRAG_2WE_WW_SUM_MWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWAERMEERTRAG_2WE_HEIZ_TAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWAERMEERTRAG_2WE_HEIZ_SUM_MWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWAERMEERTRAG_WW_TAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWAERMEERTRAG_WW_SUM_MWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWAERMEERTRAG_HEIZ_TAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWAERMEERTRAG_HEIZ_SUM_MWH> {
    using type = float;
};
#endif
