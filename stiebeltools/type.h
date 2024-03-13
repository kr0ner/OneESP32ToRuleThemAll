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
struct TypeMapping<Property::kKESSELSOLLTEMP> {
    using type = float;
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
struct TypeMapping<Property::kRAUMSOLLTEMP_II> {
    using type = float;
};
template <>
struct TypeMapping<Property::kRAUMSOLLTEMP_III> {
    using type = float;
};
template <>
struct TypeMapping<Property::kRAUMSOLLTEMP_NACHT> {
    using type = float;
};
template <>
struct TypeMapping<Property::kUHRZEIT> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kDATUM> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kGERAETE_ID> {
    using type = std::string;
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
    using type = std::uint8_t;
};
template <>
struct TypeMapping<Property::kSAMMLERSOLLTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kRUECKLAUFISTTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSPEICHER_UNTEN_TEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSOLARZONENTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSPEICHER_OBEN_TEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kKOLLEKTORTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kFESTSTOFFKESSELTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kMIN_TEMP_KESSEL> {
    using type = float;
};
template <>
struct TypeMapping<Property::kANFAHRTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kHYSTERESEZEIT> {
    using type = float;
};
template <>
struct TypeMapping<Property::kMAX_HYSTERESE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kMAX_TEMP_KESSEL> {
    using type = float;
};
template <>
struct TypeMapping<Property::kMAX_TEMP_HZK> {
    using type = float;
};
template <>
struct TypeMapping<Property::kTN> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kACCESS_EEPROM> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kKOLLEKTORTEMP_2> {
    using type = float;
};
template <>
struct TypeMapping<Property::kMULTIFUNKTION_ISTTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kBRENNER> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kDCF> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kMISCHER_AUF> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kMISCHER_ZU> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kHEIZKREIS_STATUS> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kSPEICHER_STATUS> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kWAERMEPUMPEN_STATUS> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kKESSELSTATUS> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kMISCHERSTATUS> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kSONDERKREIS_STATUS> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kFEUCHTE> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUFFERTEMP_OBEN1> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUFFERTEMP_MITTE1> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUFFERTEMP_UNTEN1> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUFFERTEMP_OBEN2> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUFFERTEMP_MITTE2> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUFFERTEMP_UNTEN2> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUFFERTEMP_OBEN3> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUFFERTEMP_MITTE3> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUFFERTEMP_UNTEN3> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSOLAR_TAGESERTRAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSOLAR_GESAMTERTRAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSOLAR_GESAMTERTRAG_MWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kINITIALISIERUNG> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kAUSSENTEMPVERZOEGERUNG> {
    using type = float;
};
template <>
struct TypeMapping<Property::kHEIZKURVE> {
    using type = float;
};
template <>
struct TypeMapping<Property::kHZK_KURVENABSTAND> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPROGRAMMSCHALTER> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kSPRACHE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kAUSWAHL_STANDARDTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kESTRICHFUNKTION> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kFERIENANFANG_TAG> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kFERIENANFANG_MONAT> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kFERIENANFANG_JAHR> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kFERIENENDE_TAG> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kFERIENENDE_MONAT> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kFERIENENDE_JAHR> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kWOCHENTAG> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kTAG> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kMONAT> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kJAHR> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kSTUNDE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kMINUTE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kSEKUNDE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kMIN_TEMP_HZK> {
    using type = float;
};
template <>
struct TypeMapping<Property::kFERIEN_ABSENKTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWW_MAXTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWARMWASSERMODE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kKESSELSOLLTEMP_2WE> {
    using type = float;
};
template <>
struct TypeMapping<Property::kABWESENHEITSTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kEINSTELL_SPEICHERSOLLTEMP3> {
    using type = float;
};
template <>
struct TypeMapping<Property::kABGASTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kGERAETEKONFIGURATION_2> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kWW_SCHNELL_START_TEMPERATUR> {
    using type = float;
};
template <>
struct TypeMapping<Property::kMAX_WW_TEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kBIVALENTPARALLELTEMPERATUR_HZG> {
    using type = float;
};
template <>
struct TypeMapping<Property::kBIVALENTPARALLELTEMPERATUR_WW> {
    using type = float;
};
template <>
struct TypeMapping<Property::kBIVALENZALTERNATIVTEMPERATUR_HZG> {
    using type = float;
};
template <>
struct TypeMapping<Property::kBIVALENZALTERNATIVTEMPERATUR_WW> {
    using type = float;
};
template <>
struct TypeMapping<Property::kQUELLENSOLLTEMPERATUR> {
    using type = float;
};
template <>
struct TypeMapping<Property::kAUSSENTEMPERATUR_WARMWASSER> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSOLARDIFFERENZ> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSOLARTEMP_MAX> {
    using type = float;
};
template <>
struct TypeMapping<Property::kESTRICH_SOCKELTEMPERATUR> {
    using type = float;
};
template <>
struct TypeMapping<Property::kESTRICH_MAX_TEMPERATUR> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSW_AUSSENTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kFESTWERT> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kPUMPENSTATUS> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kQUELLE_IST> {
    using type = float;
};
template <>
struct TypeMapping<Property::kPUFFERSOLL> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWPVORLAUFIST> {
    using type = float;
};
template <>
struct TypeMapping<Property::kHILFSKESSELSOLL> {
    using type = float;
};
template <>
struct TypeMapping<Property::kFUEHLER_1> {
    using type = float;
};
template <>
struct TypeMapping<Property::kFUEHLER_2> {
    using type = float;
};
template <>
struct TypeMapping<Property::kVOLUMENSTROM> {
    using type = float;
};
template <>
struct TypeMapping<Property::kMAX_HEIZUNG_TEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kVORLAUFSOLL_GEBLAESE> {
    using type = float;
};
template <>
struct TypeMapping<Property::kRAUMSOLL_GEBLAESE> {
    using type = float;
};
template <>
struct TypeMapping<Property::kHYSTERESE_GEBLAESE> {
    using type = float;
};
template <>
struct TypeMapping<Property::kVORLAUFSOLL_FLAECHE> {
    using type = float;
};
template <>
struct TypeMapping<Property::kRAUMSOLL_FLAECHE> {
    using type = float;
};
template <>
struct TypeMapping<Property::kTAUPUNKT_TEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kHEISSGAS_TEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWW_ECO> {
    using type = bool;
};
template <>
struct TypeMapping<Property::kSOLAR_WOCHENERTRAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSOLAR_WOCHENERTRAG_MWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSOLAR_JAHRESERTRAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSOLAR_JAHRESERTRAG_MWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWAERMEERTRAG_RUECKGE_TAG_KWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kWAERMEERTRAG_RUECKGE_SUMME_MWH> {
    using type = float;
};
template <>
struct TypeMapping<Property::kSOLAR_KOLLEKTORSCHUTZTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kANZEIGE_HOCHDRUCK> {
    using type = float;
};
template <>
struct TypeMapping<Property::kANZEIGE_NIEDERDRUCK> {
    using type = float;
};
template <>
struct TypeMapping<Property::kVERDICHTER> {
    using type = float;
};
template <>
struct TypeMapping<Property::kLZ_VERD_1_HEIZBETRIEB> {
    using type = float;
};
template <>
struct TypeMapping<Property::kLZ_VERD_2_HEIZBETRIEB> {
    using type = float;
};
template <>
struct TypeMapping<Property::kLZ_VERD_1_2_HEIZBETRIEB> {
    using type = float;
};
template <>
struct TypeMapping<Property::kLZ_VERD_1_KUEHLBETRIEB> {
    using type = float;
};
template <>
struct TypeMapping<Property::kLZ_VERD_2_KUEHLBETRIEB> {
    using type = float;
};
template <>
struct TypeMapping<Property::kLZ_VERD_1_2_KUEHLBETRIEB> {
    using type = float;
};
template <>
struct TypeMapping<Property::kVORLAUFISTTEMP_WP_IWS> {
    using type = float;
};
template <>
struct TypeMapping<Property::kRUECKLAUFISTTEMP_WP_IWS> {
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
template <>
struct TypeMapping<Property::kAUSSEN_FROSTTEMP> {
    using type = float;
};
template <>
struct TypeMapping<Property::kEINSTELL_SPEICHERSOLLTEMP2> {
    using type = float;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_5> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_6> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_12> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_13> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_19> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_20> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_26> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_27> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_33> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_34> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_40> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_41> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_47> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_48> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_54> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_55> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_61> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_62> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_68> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_69> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_75> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_76> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_82> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_83> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_89> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_90> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_96> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_97> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_103> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_104> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_110> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_111> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_117> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_118> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_124> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_125> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_131> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_132> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_138> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kFEHLERFELD_139> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_1_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_2_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kHEIZPROG_3_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_1_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_2_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kW_WASSERPROG_3_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_1_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_2_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZIRKPROG_3_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_1_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZBV_PROG_2_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_1_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_2_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_3_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_MO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_MO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_MO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_DI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_DI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_DI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_MI> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_MI_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_MI_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_DO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_DO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_DO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_FR> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_FR_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_FR_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_SA> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_SA_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_SA_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_SO> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_SO_SCHALT_2> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZEITPROG_4_SO_SCHALT_3> {
    using type = std::string;
};
template <>
struct TypeMapping<Property::kZWEITER_WE_STATUS> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kWP_EVU> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kWP_PUMPENSTATUS> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kWP_STATUS> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kDAUERLAUF_PUFFERLADEPUMPE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kSCHALTWERKDYNAMIKZEIT> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kSTILLSTANDZEIT_0> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kPUMPENZYKLEN> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kGEBAEUDEART> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kSOMMERBETRIEB> {
    using type = bool;
};
template <>
struct TypeMapping<Property::kIMPULSRATE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kSOLARBETRIEB> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kWAERMEMENGE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kAUTOMATIK_WARMWASSER> {
    using type = bool;
};
template <>
struct TypeMapping<Property::kWPSTUFEN_WW> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kWW_MIT_2WE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kSPERREN_2WE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kFREIGABE_2WE> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kDYNAMIK> {
    using type = std::uint16_t;
};
template <>
struct TypeMapping<Property::kBETRIEBSART_HZK_PUMPE> {
    using type = std::uint16_t;
};

#endif
