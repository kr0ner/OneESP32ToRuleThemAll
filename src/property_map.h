#if !defined(PROPERTY_MAP_H)
#define PROPERTY_MAP_H

#include <cstdint>
#include "type.h"
#include "esphome.h"

using esphome::esp_log_printf_;

// PropertyType class definition
class PropertyType {
public:
    const char* name;
    uint16_t address;
    Type type;

    // Default constructor
    constexpr PropertyType() : name(""), address(0), type(et_default) {}

    // Parameterized constructor
    constexpr PropertyType(const char* n, uint16_t a, Type t) : name(n), address(a), type(t) {}

    // Comparison operator for use in std::map
    bool operator<(const PropertyType& other) const {
        int nameCmp = std::strcmp(name, other.name);
        if (nameCmp < 0) return true;
        if (nameCmp > 0) return false;
        if (address < other.address) return true;
        if (address > other.address) return false;
        return type < other.type;
    }

    bool operator==(const PropertyType& other) const {
        return std::strcmp(name, other.name) == 0 &&
               address == other.address &&
               type == other.type;
    }
};

// Property class definition
class Property {
public:
static constexpr PropertyType kINDEX_NOT_FOUND = PropertyType("kINDEX_NOT_FOUND",0x0000,et_default);
static constexpr PropertyType kFEHLERMELDUNG = PropertyType("kFEHLERMELDUNG",0x0001,et_default);
static constexpr PropertyType kKESSELSOLLTEMP = PropertyType("kKESSELSOLLTEMP",0x0002,et_dec_val);
static constexpr PropertyType kSPEICHERSOLLTEMP = PropertyType("kSPEICHERSOLLTEMP",0x0003,et_dec_val);
static constexpr PropertyType kVORLAUFSOLLTEMP = PropertyType("kVORLAUFSOLLTEMP",0x0004,et_dec_val);
static constexpr PropertyType kRAUMSOLLTEMP_I = PropertyType("kRAUMSOLLTEMP_I",0x0005,et_dec_val);
static constexpr PropertyType kRAUMSOLLTEMP_II = PropertyType("kRAUMSOLLTEMP_II",0x0006,et_dec_val);
static constexpr PropertyType kRAUMSOLLTEMP_III = PropertyType("kRAUMSOLLTEMP_III",0x0007,et_dec_val);
static constexpr PropertyType kRAUMSOLLTEMP_NACHT = PropertyType("kRAUMSOLLTEMP_NACHT",0x0008,et_dec_val);
static constexpr PropertyType kUHRZEIT = PropertyType("kUHRZEIT",0x0009,et_zeit);
static constexpr PropertyType kDATUM = PropertyType("kDATUM",0x000a,et_datum);
static constexpr PropertyType kGERAETE_ID = PropertyType("kGERAETE_ID",0x000b,et_dev_id);
static constexpr PropertyType kAUSSENTEMP = PropertyType("kAUSSENTEMP",0x000c,et_dec_val);
static constexpr PropertyType kSAMMLERISTTEMP = PropertyType("kSAMMLERISTTEMP",0x000d,et_dec_val);
static constexpr PropertyType kSPEICHERISTTEMP = PropertyType("kSPEICHERISTTEMP",0x000e,et_dec_val);
static constexpr PropertyType kVORLAUFISTTEMP = PropertyType("kVORLAUFISTTEMP",0x000f,et_dec_val);
static constexpr PropertyType kGERAETEKONFIGURATION = PropertyType("kGERAETEKONFIGURATION",0x0010,et_default);
static constexpr PropertyType kRAUMISTTEMP = PropertyType("kRAUMISTTEMP",0x0011,et_dec_val);
static constexpr PropertyType kVERSTELLTE_RAUMSOLLTEMP = PropertyType("kVERSTELLTE_RAUMSOLLTEMP",0x0012,et_dec_val);
static constexpr PropertyType kEINSTELL_SPEICHERSOLLTEMP = PropertyType("kEINSTELL_SPEICHERSOLLTEMP",0x0013,et_dec_val);
static constexpr PropertyType kVERDAMPFERTEMP = PropertyType("kVERDAMPFERTEMP",0x0014,et_dec_val);
static constexpr PropertyType kSAMMLERSOLLTEMP = PropertyType("kSAMMLERSOLLTEMP",0x0015,et_dec_val);
static constexpr PropertyType kRUECKLAUFISTTEMP = PropertyType("kRUECKLAUFISTTEMP",0x0016,et_dec_val);
static constexpr PropertyType kSPEICHER_UNTEN_TEMP = PropertyType("kSPEICHER_UNTEN_TEMP",0x0017,et_dec_val);
static constexpr PropertyType kSOLARZONENTEMP = PropertyType("kSOLARZONENTEMP",0x0018,et_dec_val);
static constexpr PropertyType kSPEICHER_OBEN_TEMP = PropertyType("kSPEICHER_OBEN_TEMP",0x0019,et_dec_val);
static constexpr PropertyType kKUNDENKENNUNG = PropertyType("kKUNDENKENNUNG",0x001a,et_default);
static constexpr PropertyType kKOLLEKTORTEMP = PropertyType("kKOLLEKTORTEMP",0x001b,et_dec_val);
static constexpr PropertyType kFESTSTOFFKESSELTEMP = PropertyType("kFESTSTOFFKESSELTEMP",0x001c,et_dec_val);
static constexpr PropertyType kWASSERDRUCK = PropertyType("kWASSERDRUCK",0x001f,et_default);
static constexpr PropertyType kMIN_TEMP_KESSEL = PropertyType("kMIN_TEMP_KESSEL",0x0020,et_dec_val);
static constexpr PropertyType kANFAHRTEMP = PropertyType("kANFAHRTEMP",0x0021,et_dec_val);
static constexpr PropertyType kHYSTERESEZEIT = PropertyType("kHYSTERESEZEIT",0x0022,et_dec_val); // MAX_HYSTERESE
static constexpr PropertyType kMAX_HYSTERESE = PropertyType("kMAX_HYSTERESE",0x0023,et_little_endian);
static constexpr PropertyType kPPL = PropertyType("kPPL",0x0024,et_default);
static constexpr PropertyType kSPEICHERSPERRE = PropertyType("kSPEICHERSPERRE",0x0025,et_default);
static constexpr PropertyType kSPERRZEIT = PropertyType("kSPERRZEIT",0x0026,et_default);
static constexpr PropertyType kHYSTERESE2 = PropertyType("kHYSTERESE2",0x0027,et_default);
static constexpr PropertyType kMAX_TEMP_KESSEL = PropertyType("kMAX_TEMP_KESSEL",0x0028,et_dec_val);
static constexpr PropertyType kMAX_TEMP_HZK = PropertyType("kMAX_TEMP_HZK",0x0029,et_dec_val);
static constexpr PropertyType kKP = PropertyType("kKP",0x002a,et_default);
static constexpr PropertyType kTN = PropertyType("kTN",0x002b,et_little_endian);
static constexpr PropertyType kMISCHERLAUFZEIT = PropertyType("kMISCHERLAUFZEIT",0x002c,et_default);
static constexpr PropertyType kMODGRAD = PropertyType("kMODGRAD",0x002d,et_default);
static constexpr PropertyType kKESSELUEBERHOEHUNG_WW = PropertyType("kKESSELUEBERHOEHUNG_WW",0x002e,et_default);
static constexpr PropertyType kSTAENDIGE_MINIMALBEGRENZUNG = PropertyType("kSTAENDIGE_MINIMALBEGRENZUNG",0x002f,et_default);
static constexpr PropertyType kACCESS_EEPROM = PropertyType("kACCESS_EEPROM",0x0030,et_little_endian);
static constexpr PropertyType kMINDESTABTAUZEIT = PropertyType("kMINDESTABTAUZEIT",0x0031,et_default);
static constexpr PropertyType kACCESS_XRAM = PropertyType("kACCESS_XRAM",0x0032,et_default);
static constexpr PropertyType kACCESS_IRAM = PropertyType("kACCESS_IRAM",0x0033,et_default);
static constexpr PropertyType kMIN_WASSERDRUCK = PropertyType("kMIN_WASSERDRUCK",0x0034,et_default);
static constexpr PropertyType kLEISTUNGSKORREKTUR = PropertyType("kLEISTUNGSKORREKTUR",0x0035,et_default);
static constexpr PropertyType kKOLLEKTORTEMP_2 = PropertyType("kKOLLEKTORTEMP_2",0x0036,et_dec_val);
static constexpr PropertyType kMULTIFUNKTION_ISTTEMP = PropertyType("kMULTIFUNKTION_ISTTEMP",0x0051,et_dec_val);
static constexpr PropertyType kBRENNER = PropertyType("kBRENNER",0x0052,et_little_endian);
static constexpr PropertyType kHZK_PUMPE = PropertyType("kHZK_PUMPE",0x0053,et_default);
static constexpr PropertyType kSPL_PUMPE = PropertyType("kSPL_PUMPE",0x0055,et_default);
static constexpr PropertyType kDCF = PropertyType("kDCF",0x0056,et_little_endian);
static constexpr PropertyType kMISCHER_AUF = PropertyType("kMISCHER_AUF",0x0057,et_little_endian);
static constexpr PropertyType kMISCHER_ZU = PropertyType("kMISCHER_ZU",0x0058,et_little_endian);
static constexpr PropertyType kHEIZKREIS_STATUS = PropertyType("kHEIZKREIS_STATUS",0x0059,et_little_endian);
static constexpr PropertyType kSPEICHER_STATUS = PropertyType("kSPEICHER_STATUS",0x005a,et_little_endian);
static constexpr PropertyType kSCHALTERSTELLUNG = PropertyType("kSCHALTERSTELLUNG",0x005b,et_default);
static constexpr PropertyType kANFAHRENT = PropertyType("kANFAHRENT",0x005d,et_default);
static constexpr PropertyType kTEILVORRANG_WW = PropertyType("kTEILVORRANG_WW",0x005e,et_default);
static constexpr PropertyType kSPEICHERBEDARF = PropertyType("kSPEICHERBEDARF",0x005f,et_default);
static constexpr PropertyType kSCHALTFKT_IWS = PropertyType("kSCHALTFKT_IWS",0x0060,et_default);
static constexpr PropertyType kABTAUUNGAKTIV = PropertyType("kABTAUUNGAKTIV",0x0061,et_default);
static constexpr PropertyType kWAERMEPUMPEN_STATUS = PropertyType("kWAERMEPUMPEN_STATUS",0x0062,et_little_endian);
static constexpr PropertyType kKESSELSTATUS = PropertyType("kKESSELSTATUS",0x0063,et_little_endian);
static constexpr PropertyType kSAMMLER_PUMPE = PropertyType("kSAMMLER_PUMPE",0x0064,et_default);
static constexpr PropertyType kZIRK_PUMPE = PropertyType("kZIRK_PUMPE",0x0065,et_default);
static constexpr PropertyType kMISCHERSTATUS = PropertyType("kMISCHERSTATUS",0x0066,et_little_endian);
static constexpr PropertyType kSONDERKREIS_STATUS = PropertyType("kSONDERKREIS_STATUS",0x0067,et_little_endian);
static constexpr PropertyType kBETRIEBSART = PropertyType("kBETRIEBSART",0x0068,et_default);
static constexpr PropertyType kIO_TEST = PropertyType("kIO_TEST",0x0069,et_default);
static constexpr PropertyType kRESET_KONFIGURATION = PropertyType("kRESET_KONFIGURATION",0x006a,et_default);
static constexpr PropertyType kPARTY_EIN_AUS = PropertyType("kPARTY_EIN_AUS",0x006b,et_default);
static constexpr PropertyType kECO_EIN_AUS = PropertyType("kECO_EIN_AUS",0x006c,et_default);
static constexpr PropertyType kWAHLUMSCHALTUNG = PropertyType("kWAHLUMSCHALTUNG",0x006d,et_default);
static constexpr PropertyType kHEIZKREIS_STATUS_PROGSTELL = PropertyType("kHEIZKREIS_STATUS_PROGSTELL",0x006e,et_default);
static constexpr PropertyType kFERIENBETRIEB = PropertyType("kFERIENBETRIEB",0x006f,et_default);
static constexpr PropertyType kDREHZAHLREG_JA_NEIN = PropertyType("kDREHZAHLREG_JA_NEIN",0x0070,et_default);
static constexpr PropertyType kANFORDERUNG_LEISTUNGSZWANG = PropertyType("kANFORDERUNG_LEISTUNGSZWANG",0x0071,et_default);
static constexpr PropertyType kANTILEG_AKTIV = PropertyType("kANTILEG_AKTIV",0x0072,et_default);
static constexpr PropertyType kBITSCHALTER = PropertyType("kBITSCHALTER",0x0073,et_default);
static constexpr PropertyType kEVU_SPERRE_AKTIV = PropertyType("kEVU_SPERRE_AKTIV",0x0074,et_default);
static constexpr PropertyType kFEUCHTE = PropertyType("kFEUCHTE",0x0075,et_dec_val);
static constexpr PropertyType kPUFFERTEMP_OBEN1 = PropertyType("kPUFFERTEMP_OBEN1",0x0076,et_dec_val);
static constexpr PropertyType kPUFFERTEMP_MITTE1 = PropertyType("kPUFFERTEMP_MITTE1",0x0077,et_dec_val);
static constexpr PropertyType kPUFFERTEMP_UNTEN1 = PropertyType("kPUFFERTEMP_UNTEN1",0x0078,et_dec_val);
static constexpr PropertyType kPUFFERTEMP_OBEN2 = PropertyType("kPUFFERTEMP_OBEN2",0x0079,et_dec_val);
static constexpr PropertyType kPUFFERTEMP_MITTE2 = PropertyType("kPUFFERTEMP_MITTE2",0x007a,et_dec_val);
static constexpr PropertyType kPUFFERTEMP_UNTEN2 = PropertyType("kPUFFERTEMP_UNTEN2",0x007b,et_dec_val);
static constexpr PropertyType kPUFFERTEMP_OBEN3 = PropertyType("kPUFFERTEMP_OBEN3",0x007c,et_dec_val);
static constexpr PropertyType kPUFFERTEMP_MITTE3 = PropertyType("kPUFFERTEMP_MITTE3",0x007d,et_dec_val);
static constexpr PropertyType kPUFFERTEMP_UNTEN3 = PropertyType("kPUFFERTEMP_UNTEN3",0x007e,et_dec_val);
static constexpr PropertyType kEINSTRAHLUNGS_SENSOR = PropertyType("kEINSTRAHLUNGS_SENSOR",0x007f,et_default);
static constexpr PropertyType kECO_AKZEPTANZ_WW = PropertyType("kECO_AKZEPTANZ_WW",0x0080,et_default);
static constexpr PropertyType kECO_AKZEPTANZ_RAUM = PropertyType("kECO_AKZEPTANZ_RAUM",0x0081,et_default);
static constexpr PropertyType kMODGRAD_IST = PropertyType("kMODGRAD_IST",0x008a,et_default);
static constexpr PropertyType kECO_AKZEPTANZ_PUFFER = PropertyType("kECO_AKZEPTANZ_PUFFER",0x008b,et_default);
static constexpr PropertyType kGESAMT_MODGRAD = PropertyType("kGESAMT_MODGRAD",0x008c,et_default);
static constexpr PropertyType kMIN_MOD_KASKADE = PropertyType("kMIN_MOD_KASKADE",0x008d,et_default);
static constexpr PropertyType kFEUCHTE_HYSTERESE = PropertyType("kFEUCHTE_HYSTERESE",0x008e,et_default);
static constexpr PropertyType kLOAD_STANDARD = PropertyType("kLOAD_STANDARD",0x00ef,et_default);
static constexpr PropertyType kONL_CODENUMMER = PropertyType("kONL_CODENUMMER",0x00f0,et_default);
//kERWEITERUNGSTELEGRAMM",0x00fa,et_default);
static constexpr PropertyType kSYSTEM_RESET = PropertyType("kSYSTEM_RESET",0x00fb,et_default); // write only id480
static constexpr PropertyType kCAN_FEHLERMELDUNG = PropertyType("kCAN_FEHLERMELDUNG",0x00fc,et_default);
static constexpr PropertyType kBUSKONFIGURATION = PropertyType("kBUSKONFIGURATION",0x00fd,et_default);
static constexpr PropertyType kINITIALISIERUNG = PropertyType("kINITIALISIERUNG",0x00fe,et_little_endian);
static constexpr PropertyType kUNGUELTIG = PropertyType("kUNGUELTIG",0x00ff,et_default);
static constexpr PropertyType kANTILEGIONELLEN = PropertyType("kANTILEGIONELLEN",0x0101,et_default);
static constexpr PropertyType kAUSSENFUEHLER_VERSORGUNG = PropertyType("kAUSSENFUEHLER_VERSORGUNG",0x0102,et_default);
static constexpr PropertyType kAUFHEIZOPTIMIERUNG = PropertyType("kAUFHEIZOPTIMIERUNG",0x0103,et_default);

static constexpr PropertyType kWOCHENTAG = PropertyType("kWOCHENTAG",0x0121,et_little_endian);
static constexpr PropertyType kTAG = PropertyType("kTAG",0x0122,et_little_endian);
static constexpr PropertyType kMONAT = PropertyType("kMONAT",0x0123,et_little_endian);
static constexpr PropertyType kJAHR = PropertyType("kJAHR",0x0124,et_little_endian); // +2000
static constexpr PropertyType kSTUNDE = PropertyType("kSTUNDE",0x0125,et_little_endian);
static constexpr PropertyType kMINUTE = PropertyType("kMINUTE",0x0126,et_little_endian);
static constexpr PropertyType kSEKUNDE = PropertyType("kSEKUNDE",0x0127,et_little_endian);
static constexpr PropertyType kPROGRAMMSCHALTER = PropertyType("kPROGRAMMSCHALTER",0x0112,et_betriebsart);
static constexpr PropertyType kBETRIEBS_STATUS = PropertyType("kBETRIEBS_STATUS",0x0176,et_default);
static constexpr PropertyType kBETRIEBS_STATUS_2 = PropertyType("kBETRIEBS_STATUS_2",0xc356,et_default);
static constexpr PropertyType kEL_AUFNAHMELEISTUNG_WW_TAG_WH = PropertyType("kEL_AUFNAHMELEISTUNG_WW_TAG_WH",0x091a,et_default);
static constexpr PropertyType kEL_AUFNAHMELEISTUNG_WW_TAG_KWH = PropertyType("kEL_AUFNAHMELEISTUNG_WW_TAG_KWH",0x091b,et_double_val);
static constexpr PropertyType kEL_AUFNAHMELEISTUNG_WW_SUM_KWH = PropertyType("kEL_AUFNAHMELEISTUNG_WW_SUM_KWH",0x091c,et_default);
static constexpr PropertyType kEL_AUFNAHMELEISTUNG_WW_SUM_MWH = PropertyType("kEL_AUFNAHMELEISTUNG_WW_SUM_MWH",0x091d,et_double_val);
static constexpr PropertyType kEL_AUFNAHMELEISTUNG_HEIZ_TAG_WH = PropertyType("kEL_AUFNAHMELEISTUNG_HEIZ_TAG_WH",0x091e,et_default);
static constexpr PropertyType kEL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH = PropertyType("kEL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH",0x091f,et_double_val);
static constexpr PropertyType kEL_AUFNAHMELEISTUNG_HEIZ_SUM_KWH = PropertyType("kEL_AUFNAHMELEISTUNG_HEIZ_SUM_KWH",0x0920,et_default);
static constexpr PropertyType kEL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH = PropertyType("kEL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH",0x0921,et_double_val);
static constexpr PropertyType kLUEFT_STUFE_TAG = PropertyType("kLUEFT_STUFE_TAG",0x056c,et_default);
static constexpr PropertyType kLUEFT_STUFE_NACHT = PropertyType("kLUEFT_STUFE_NACHT",0x056d,et_default);
static constexpr PropertyType kLUEFT_STUFE_BEREITSCHAFT = PropertyType("kLUEFT_STUFE_BEREITSCHAFT",0x056f,et_default); // added for eco 5.5(noconflict)
static constexpr PropertyType kLUEFT_STUFE_ABWESEND = PropertyType("kLUEFT_STUFE_ABWESEND",0x0571,et_default);
static constexpr PropertyType kLUEFT_STUFE_PARTY = PropertyType("kLUEFT_STUFE_PARTY",0x0570,et_default);
#if defined(THZ_TYPE_ECO55)
static constexpr PropertyType kLUEFT_ZEIT_AUSSERPLAN_STUFE1 = PropertyType("kLUEFT_ZEIT_AUSSERPLAN_STUFE1",0x0572,et_default);
static constexpr PropertyType kLUEFT_ZEIT_AUSSERPLAN_STUFE2 = PropertyType("kLUEFT_ZEIT_AUSSERPLAN_STUFE2",0x0573,et_default);
static constexpr PropertyType kLUEFT_ZEIT_AUSSERPLAN_STUFE3 = PropertyType("kLUEFT_ZEIT_AUSSERPLAN_STUFE3",0x0574,et_default);
static constexpr PropertyType kLUEFT_PASSIVKUEHLUNG = PropertyType("kLUEFT_PASSIVKUEHLUNG",0x0575,et_default);
static constexpr PropertyType kLUEFT_ZULUFT_STUFE1 = PropertyType("kLUEFT_ZULUFT_STUFE1",0x0576,et_default);
static constexpr PropertyType kLUEFT_ZULUFT_STUFE2 = PropertyType("kLUEFT_ZULUFT_STUFE2",0x0577,et_default);
static constexpr PropertyType kLUEFT_ZULUFT_STUFE3 = PropertyType("kLUEFT_ZULUFT_STUFE3",0x0578,et_default);
static constexpr PropertyType kLUEFT_ABLUFT_STUFE1 = PropertyType("kLUEFT_ABLUFT_STUFE1",0x0579,et_default);
static constexpr PropertyType kLUEFT_ABLUFT_STUFE2 = PropertyType("kLUEFT_ABLUFT_STUFE2",0x057a,et_default);
static constexpr PropertyType kLUEFT_ABLUFT_STUFE3 = PropertyType("kLUEFT_ABLUFT_STUFE3",0x057b,et_default);
#else
static constexpr PropertyType kLUEFT_ZEIT_AUSSERPLAN_STUFE0 = PropertyType("kLUEFT_ZEIT_AUSSERPLAN_STUFE0",0x0572,et_default);
static constexpr PropertyType kLUEFT_ZEIT_AUSSERPLAN_STUFE1 = PropertyType("kLUEFT_ZEIT_AUSSERPLAN_STUFE1",0x0573,et_default);
static constexpr PropertyType kLUEFT_ZEIT_AUSSERPLAN_STUFE2 = PropertyType("kLUEFT_ZEIT_AUSSERPLAN_STUFE2",0x0574,et_default);
static constexpr PropertyType kLUEFT_ZEIT_AUSSERPLAN_STUFE3 = PropertyType("kLUEFT_ZEIT_AUSSERPLAN_STUFE3",0x0575,et_default);
static constexpr PropertyType kLUEFT_PASSIVKUEHLUNG = PropertyType("kLUEFT_PASSIVKUEHLUNG",0x0576,et_default);
static constexpr PropertyType kLUEFT_ZULUFT_STUFE1 = PropertyType("kLUEFT_ZULUFT_STUFE1",0x0577,et_default);
static constexpr PropertyType kLUEFT_ZULUFT_STUFE2 = PropertyType("kLUEFT_ZULUFT_STUFE2",0x0578,et_default);
static constexpr PropertyType kLUEFT_ZULUFT_STUFE3 = PropertyType("kLUEFT_ZULUFT_STUFE3",0x0579,et_default);
static constexpr PropertyType kLUEFT_ABLUFT_STUFE1 = PropertyType("kLUEFT_ABLUFT_STUFE1",0x057a,et_default);
static constexpr PropertyType kLUEFT_ABLUFT_STUFE2 = PropertyType("kLUEFT_ABLUFT_STUFE2",0x057b,et_default);
static constexpr PropertyType kLUEFT_ABLUFT_STUFE3 = PropertyType("kLUEFT_ABLUFT_STUFE3",0x057c,et_default);
#endif
#if defined(THZ_TYPE_ECO55)
static constexpr PropertyType kZULUFT_SOLL = PropertyType("kZULUFT_SOLL",0x0596,et_default);
static constexpr PropertyType kZULUFT_IST = PropertyType("kZULUFT_IST",0x0597,et_default);
static constexpr PropertyType kABLUFT_SOLL = PropertyType("kABLUFT_SOLL",0x0598,et_default);
static constexpr PropertyType kABLUFT_IST = PropertyType("kABLUFT_IST",0x0599,et_default);
#else
static constexpr PropertyType kPARTY_ENDE_JAHR = PropertyType("kPARTY_ENDE_JAHR",0x0596,et_default);
static constexpr PropertyType kZULUFT_SOLL = PropertyType("kZULUFT_SOLL",0x0597,et_default);
static constexpr PropertyType kZULUFT_IST = PropertyType("kZULUFT_IST",0x0598,et_default);
static constexpr PropertyType kABLUFT_SOLL = PropertyType("kABLUFT_SOLL",0x0599,et_default);
static constexpr PropertyType kABLUFT_IST = PropertyType("kABLUFT_IST",0x059a,et_default);
#endif
#if defined(THZ_TYPE_ECO55)
static constexpr PropertyType kLUEFT_STUFE_HAND = PropertyType("kLUEFT_STUFE_HAND",0x0612,et_default);
#else
static constexpr PropertyType kUNTERD_TEMPM_PUMPENANL = PropertyType("kUNTERD_TEMPM_PUMPENANL",0x0612,et_default);
static constexpr PropertyType kLUEFT_STUFE_HAND = PropertyType("kLUEFT_STUFE_HAND",0x0613,et_default); // Min. Quellentemp. Auf aus /Klaus
#endif
#if defined(THZ_TYPE_ECO55)
static constexpr PropertyType kLAUFZEIT_FILTER = PropertyType("kLAUFZEIT_FILTER",0x0341,et_default);
#else
static constexpr PropertyType kLAUFZEIT_DHC3 = PropertyType("kLAUFZEIT_DHC3",0x0341,et_default);
static constexpr PropertyType kLAUFZEIT_FILTER = PropertyType("kLAUFZEIT_FILTER",0x0342,et_default);
#endif
static constexpr PropertyType kFORTLUFT_SOLL = PropertyType("kFORTLUFT_SOLL",0x059b,et_default);
static constexpr PropertyType kFORTLUFT_IST = PropertyType("kFORTLUFT_IST",0x059c,et_default);
static constexpr PropertyType kSOMMERBETRIEB_TEMP = PropertyType("kSOMMERBETRIEB_TEMP",0x0116,et_default); // changed from kHEIZGRENZE_TAG to kSOMMERBETRIEB_TEMP
static constexpr PropertyType kSOMMERBETRIEB_HYSTERESE = PropertyType("kSOMMERBETRIEB_HYSTERESE",0x05a2,et_default); // changed from kNHZ_LEISTUNG1 to SOMMERBETRIEB_HYSTERESE
static constexpr PropertyType kABLUFT_LUFTFEUCHTIGKEIT = PropertyType("kABLUFT_LUFTFEUCHTIGKEIT",0xc0ef,et_default); // added for eco 5.5
static constexpr PropertyType kABLUFT_TAUPUNKT = PropertyType("kABLUFT_TAUPUNKT",0xc0f6,et_default); // added for eco 5.5
static constexpr PropertyType kFEUCHTEREGELUNG_EIN_AUS = PropertyType("kFEUCHTEREGELUNG_EIN_AUS",0x09d1,et_default);
static constexpr PropertyType kVOLUMENSTROM = PropertyType("kVOLUMENSTROM",0x01da,et_dec_val);
static constexpr PropertyType kABLUFT_TEMP = PropertyType("kABLUFT_TEMP",0x0694,et_default); // added for eco 5.5
static constexpr PropertyType kWAERMEERTRAG_RUECKGE_TAG_WH = PropertyType("kWAERMEERTRAG_RUECKGE_TAG_WH",0x03af,et_default);
static constexpr PropertyType kWAERMEERTRAG_RUECKGE_TAG_KWH = PropertyType("kWAERMEERTRAG_RUECKGE_TAG_KWH",0x03b0,et_default);
static constexpr PropertyType kWAERMEERTRAG_RUECKGE_SUMME_KWH = PropertyType("kWAERMEERTRAG_RUECKGE_SUMME_KWH",0x03b1,et_default);
static constexpr PropertyType kWAERMEERTRAG_RUECKGE_SUMME_MWH = PropertyType("kWAERMEERTRAG_RUECKGE_SUMME_MWH",0x03b6,et_default);
static constexpr PropertyType kWAERMEERTRAG_2WE_WW_TAG_WH = PropertyType("kWAERMEERTRAG_2WE_WW_TAG_WH",0x0922,et_default);
static constexpr PropertyType kWAERMEERTRAG_2WE_WW_TAG_KWH = PropertyType("kWAERMEERTRAG_2WE_WW_TAG_KWH",0x0923,et_double_val);
static constexpr PropertyType kWAERMEERTRAG_2WE_WW_SUM_KWH = PropertyType("kWAERMEERTRAG_2WE_WW_SUM_KWH",0x0924,et_default);
static constexpr PropertyType kWAERMEERTRAG_2WE_WW_SUM_MWH = PropertyType("kWAERMEERTRAG_2WE_WW_SUM_MWH",0x0925,et_double_val);
static constexpr PropertyType kWAERMEERTRAG_2WE_HEIZ_TAG_WH = PropertyType("kWAERMEERTRAG_2WE_HEIZ_TAG_WH",0x0926,et_default);
static constexpr PropertyType kWAERMEERTRAG_2WE_HEIZ_TAG_KWH = PropertyType("kWAERMEERTRAG_2WE_HEIZ_TAG_KWH",0x0927,et_double_val);
static constexpr PropertyType kWAERMEERTRAG_2WE_HEIZ_SUM_KWH = PropertyType("kWAERMEERTRAG_2WE_HEIZ_SUM_KWH",0x0928,et_default);
static constexpr PropertyType kWAERMEERTRAG_2WE_HEIZ_SUM_MWH = PropertyType("kWAERMEERTRAG_2WE_HEIZ_SUM_MWH",0x0929,et_double_val);
static constexpr PropertyType kWAERMEERTRAG_WW_TAG_WH = PropertyType("kWAERMEERTRAG_WW_TAG_WH",0x092a,et_default);
static constexpr PropertyType kWAERMEERTRAG_WW_TAG_KWH = PropertyType("kWAERMEERTRAG_WW_TAG_KWH",0x092b,et_double_val);
static constexpr PropertyType kWAERMEERTRAG_WW_SUM_KWH = PropertyType("kWAERMEERTRAG_WW_SUM_KWH",0x092c,et_default);
static constexpr PropertyType kWAERMEERTRAG_WW_SUM_MWH = PropertyType("kWAERMEERTRAG_WW_SUM_MWH",0x092d,et_double_val);
static constexpr PropertyType kWAERMEERTRAG_HEIZ_TAG_WH = PropertyType("kWAERMEERTRAG_HEIZ_TAG_WH",0x092e,et_default);
static constexpr PropertyType kWAERMEERTRAG_HEIZ_TAG_KWH = PropertyType("kWAERMEERTRAG_HEIZ_TAG_KWH",0x092f,et_double_val);
static constexpr PropertyType kWAERMEERTRAG_HEIZ_SUM_KWH = PropertyType("kWAERMEERTRAG_HEIZ_SUM_KWH",0x0930,et_default);
static constexpr PropertyType kWAERMEERTRAG_HEIZ_SUM_MWH = PropertyType("kWAERMEERTRAG_HEIZ_SUM_MWH",0x0931,et_double_val);
static constexpr PropertyType kANZEIGE_HOCHDRUCK = PropertyType("kANZEIGE_HOCHDRUCK",0x07a6,et_dec_val);
static constexpr PropertyType kANZEIGE_NIEDERDRUCK = PropertyType("kANZEIGE_NIEDERDRUCK",0x07a7,et_dec_val);
static constexpr PropertyType kVERDICHTER = PropertyType("kVERDICHTER",0x07a8,et_dec_val);
static constexpr PropertyType kPUMPENZYKLEN_MAX = PropertyType("kPUMPENZYKLEN_MAX",0x05b8,et_default);
static constexpr PropertyType kPUMPENZYKLEN_MIN = PropertyType("kPUMPENZYKLEN_MIN",0x05b9,et_default);
static constexpr PropertyType kPUMPENZYKLEN_MAX_AUSSENT = PropertyType("kPUMPENZYKLEN_MAX_AUSSENT",0x05ba,et_default);
static constexpr PropertyType kPUMPENZYKLEN_MIN_AUSSENT = PropertyType("kPUMPENZYKLEN_MIN_AUSSENT",0x05bb,et_default);
static constexpr PropertyType kVERDICHTER_STARTS = PropertyType("kVERDICHTER_STARTS",0xc0f4,et_default);
static constexpr PropertyType kVERDICHTER_STARTS_K = PropertyType("kVERDICHTER_STARTS_K",0xc0f5,et_default);
static constexpr PropertyType kPUMPENDREHZAHL_HEIZEN = PropertyType("kPUMPENDREHZAHL_HEIZEN",0x02cb,et_dec_val);
static constexpr PropertyType kRAUMEINFLUSS = PropertyType("kRAUMEINFLUSS",0x010f,et_little_endian);
static constexpr PropertyType kLEISTUNG_AUSLEGUNG_HEIZUNG = PropertyType("kLEISTUNG_AUSLEGUNG_HEIZUNG",0xc0f1,et_default);

};

// Property class definition
class PropertyList {
private:
    // List to store PropertyType values at runtime
    std::vector<PropertyType> properties;

public:

    static PropertyList& instance() {
        static PropertyList list;
        return list;
    }

    // Method to add a PropertyType to the list
    void addProperty(const PropertyType& property) {
        properties.push_back(property);
    }

    // Method to get a reference to a PropertyType by name
    PropertyType& getPropertyByName(const char* name) {
        for (auto& property : properties) {
            if (strcmp(property.name, name) == 0) {
                return property;
            }
        }
        ESP_LOGW("PROPERTY","Property with the name %s not registered by CallbackHandler.", name);
        return const_cast<PropertyType&>(Property::kINDEX_NOT_FOUND); 
    }

    // Method to get a reference to a PropertyType by address
    PropertyType& getPropertyByAddress(uint16_t address) {
        for (auto& property : properties) {
            if (property.address == address) {
                return property;
            }
        }
        ESP_LOGW("PROPERTY","Property with address 0x%04x not registered by CallbackHandler.", address);
        return const_cast<PropertyType&>(Property::kINDEX_NOT_FOUND); 
    }
};

#endif
