#include "WDC6502AnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "WDC6502Analyzer.h"
#include "WDC6502AnalyzerSettings.h"
#include "WDC6502AnalyzerOpcodes.h"
#include <iostream>
#include <sstream>
#include <stdio.h>

opcode_detail_struct opcodes[] = {
    "BRK",  2,  7,  STK_INT,            true,  true,  
    "ORA",  2,  6,  DP_IDX_IND_X,       true,  true,  
    "COP",  2,  7,  STK_INT,            false, false, 
    "ORA",  2,  4,  STK_REL,            false, false, 
    "TSB",  2,  5,  DP,                 false, true,  
    "ORA",  2,  3,  DP,                 true,  true,  
    "ASL",  2,  5,  DP,                 true,  true,  
    "ORA",  2,  6,  DP_IND_LONG,        false, false, 
    "PHP",  1,  3,  STK,                true,  true,  
    "ORA",  2,  2,  IMM,                true,  true,  
    "ASL",  1,  2,  ACC,                true,  true,  
    "PHD",  1,  4,  STK,                false, false, 
    "TSB",  3,  6,  ABS,                false, true,  
    "ORA",  3,  4,  ABS,                true,  true,  
    "ASL",  3,  6,  ABS,                true,  true,  
    "ORA",  4,  5,  ABS_LONG,           false, false, 
    "BPL",  2,  2,  PC_REL,             true,  true,  
    "ORA",  2,  5,  DP_IND_IDX_Y,       true,  true,  
    "ORA",  2,  5,  DP_IND,             false, true,  
    "ORA",  2,  7,  STK_IND_INX_Y,      false, false, 
    "TRB",  2,  5,  DP,                 false, true,  
    "ORA",  2,  4,  DP_IDX_X,           true,  true,  
    "ASL",  2,  6,  DP_IDX_X,           true,  true,  
    "ORA",  2,  6,  DP_IND_LONG_INX_Y,  false, false, 
    "CLC",  1,  2,  IMP,                true,  true,  
    "ORA",  3,  4,  ABS_INX_Y,          true,  true,  
    "INC",  1,  2,  ACC,                false, true,  
    "TAS",  1,  2,  IMP,                false, false, 
    "TRB",  3,  6,  ABS,                false, true,  
    "ORA",  3,  4,  ABS_INX_X,          true,  true,  
    "ASL",  3,  7,  ABS_INX_X,          true,  true,  
    "ORA",  4,  5,  ABS_LONG_INX_X,     false, false, 
    "JSR",  3,  6,  ABS,                true,  true,  
    "AND",  2,  6,  DP_IDX_IND_X,       true,  true,  
    "JSL",  4,  8,  ABS_LONG,           false, false, 
    "AND",  2,  4,  STK_REL,            false, false, 
    "BIT",  2,  3,  DP,                 true,  true,  
    "AND",  2,  3,  DP,                 true,  true,  
    "ROL",  2,  5,  DP,                 true,  true,  
    "AND",  2,  6,  DP_IND_LONG,        false, false, 
    "PLP",  1,  4,  STK,                true,  true,  
    "AND",  2,  2,  IMM,                true,  true,  
    "ROL",  1,  2,  ACC,                true,  true,  
    "PLD",  1,  5,  STK,                false, false, 
    "BIT",  3,  4,  ABS,                true,  true,  
    "AND",  3,  4,  ABS,                true,  true,  
    "ROL",  3,  6,  ABS,                true,  true,  
    "AND",  4,  5,  ABS_LONG,           false, false, 
    "BMI",  2,  2,  PC_REL,             true,  true,  
    "AND",  2,  5,  DP_IND_IDX_Y,       true,  true,  
    "AND",  2,  5,  DP_IND,             false, true,  
    "AND",  2,  7,  STK_IND_INX_Y,      false, false, 
    "BIT",  2,  4,  DP_IDX_X,           false, true,  
    "AND",  2,  4,  DP_IDX_X,           true,  true,  
    "ROL",  2,  6,  DP_IDX_X,           true,  true,  
    "AND",  2,  6,  DP_IND_LONG_INX_Y,  false, false, 
    "SEC",  1,  2,  IMP,                true,  true,  
    "AND",  3,  4,  ABS_INX_Y,          true,  true,  
    "DEC",  1,  2,  ACC,                false, true,  
    "TSA",  1,  2,  IMP,                false, false, 
    "BIT",  3,  4,  ABS_INX_X,          false, true,  
    "AND",  3,  4,  ABS_INX_X,          true,  true,  
    "ROL",  3,  7,  ABS_INX_X,          true,  true,  
    "AND",  4,  5,  ABS_LONG_INX_X,     false, false, 
    "RTI",  1,  6,  STK,                true,  true,  
    "EOR",  2,  6,  DP_IDX_IND_X,       true,  true,  
    "WDM",  2,  2,  WDM,                false, false, 
    "EOR",  2,  4,  STK_REL,            false, false, 
    "MVP",  3,  3,  BLOCK,              false, false, 
    "EOR",  2,  3,  DP,                 true,  true,  
    "LSR",  2,  5,  DP,                 true,  true,  
    "EOR",  2,  6,  DP_IND_LONG,        false, false, 
    "PHA",  1,  3,  STK,                true,  true,  
    "EOR",  2,  2,  IMM,                true,  true,  
    "LSR",  1,  2,  ACC,                true,  true,  
    "PHK",  1,  3,  STK,                false, false, 
    "JMP",  3,  3,  ABS,                true,  true,  
    "EOR",  3,  4,  ABS,                true,  true,  
    "LSR",  3,  6,  ABS,                true,  true,  
    "EOR",  4,  5,  ABS_LONG,           false, false, 
    "BVC",  2,  2,  PC_REL,             true,  true,  
    "EOR",  2,  5,  DP_IND_IDX_Y,       true,  true,  
    "EOR",  2,  5,  DP_IND,             false, true,  
    "EOR",  2,  7,  STK_IND_INX_Y,      false, false, 
    "MVN",  3,  3,  BLOCK,              false, false, 
    "EOR",  2,  4,  DP_IDX_X,           true,  true,  
    "LSR",  2,  6,  DP_IDX_X,           true,  true,  
    "EOR",  2,  6,  DP_IND_LONG_INX_Y,  false, false, 
    "CLI",  1,  2,  IMP,                true,  true,  
    "EOR",  3,  4,  ABS_INX_Y,          true,  true,  
    "PHY",  1,  3,  STK,                false, true,  
    "TAD",  1,  2,  IMP,                false, false, 
    "JML",  4,  4,  ABS_LONG,           false, false, 
    "EOR",  3,  4,  ABS_INX_X,          true,  true,  
    "LSR",  3,  7,  ABS_INX_X,          true,  true,  
    "EOR",  4,  5,  ABS_LONG_INX_X,     false, false, 
    "RTS",  1,  6,  STK,                true,  true,  
    "ADC",  2,  6,  DP_IDX_IND_X,       true,  true,  
    "PER",  3,  6,  STK_PC_REL_LONG,    false, false, 
    "ADC",  2,  4,  STK_REL,            false, false, 
    "STZ",  2,  3,  DP,                 false, true,  
    "ADC",  2,  3,  DP,                 true,  true,  
    "ROR",  2,  5,  DP,                 true,  true,  
    "ADC",  2,  6,  DP_IND_LONG,        false, false, 
    "PLA",  1,  4,  STK,                true,  true,  
    "ADC",  2,  2,  IMM,                true,  true,  
    "ROR",  1,  2,  ACC,                true,  true,  
    "RTL",  1,  6,  STK,                false, false, 
    "JMP",  3,  5,  ABS_IND,            true,  true,  
    "ADC",  3,  4,  ABS,                true,  true,  
    "ROR",  3,  6,  ABS,                true,  true,  
    "ADC",  4,  5,  ABS_LONG,           false, false, 
    "BVS",  2,  2,  PC_REL,             true,  true,  
    "ADC",  2,  5,  DP_IND_IDX_Y,       true,  true,  
    "ADC",  2,  5,  DP_IND,             false, true,  
    "ADC",  2,  7,  STK_IND_INX_Y,      false, false, 
    "STZ",  2,  4,  DP_IDX_X,           false, true,  
    "ADC",  2,  4,  DP_IDX_X,           true,  true,  
    "ROR",  2,  6,  DP_IDX_X,           true,  true,  
    "ADC",  2,  6,  DP_IND_LONG_INX_Y,  false, false, 
    "SEI",  1,  2,  IMP,                true,  true,  
    "ADC",  3,  4,  ABS_INX_Y,          true,  true,  
    "PLY",  1,  4,  STK,                false, true,  
    "TDA",  1,  2,  IMP,                false, false, 
    "JMP",  3,  6,  ABS_INX_IND,        false, true,  
    "ADC",  3,  4,  ABS_INX_X,          true,  true,  
    "ROR",  3,  7,  ABS_INX_X,          true,  true,  
    "ADC",  4,  5,  ABS_LONG_INX_X,     false, false, 
    "BRA",  2,  3,  PC_REL,             false, true,  
    "STA",  2,  6,  DP_IDX_IND_X,       true,  true,  
    "BRL",  3,  4,  PC_REL_LONG,        false, false, 
    "STA",  2,  4,  STK_REL,            false, false, 
    "STY",  2,  3,  DP,                 true,  true,  
    "STA",  2,  3,  DP,                 true,  true,  
    "STX",  2,  3,  DP,                 true,  true,  
    "STA",  2,  6,  DP_IND_LONG,        false, false, 
    "DEY",  1,  2,  IMP,                true,  true,  
    "BIT",  2,  2,  IMM,                false, true,  
    "TXA",  1,  2,  IMP,                true,  true,  
    "PHB",  1,  3,  STK,                false, false, 
    "STY",  3,  4,  ABS,                true,  true,  
    "STA",  3,  4,  ABS,                true,  true,  
    "STX",  3,  4,  ABS,                true,  true,  
    "STA",  4,  5,  ABS_LONG,           false, false, 
    "BCC",  2,  2,  PC_REL,             true,  true,  
    "STA",  2,  6,  DP_IND_IDX_Y,       true,  true,  
    "STA",  2,  5,  DP_IND,             false, true,  
    "STA",  2,  7,  STK_IND_INX_Y,      false, false, 
    "STY",  2,  4,  DP_IDX_X,           true,  true,  
    "STA",  2,  4,  DP_IDX_X,           true,  true,  
    "STX",  2,  4,  DP_IDX_Y,           true,  true,  
    "STA",  2,  6,  DP_IND_LONG_INX_Y,  false, false, 
    "TYA",  1,  2,  IMP,                true,  true,  
    "STA",  3,  5,  ABS_INX_Y,          true,  true,  
    "TXS",  1,  2,  IMP,                true,  true,  
    "TXY",  1,  2,  IMP,                false, false, 
    "STZ",  3,  4,  ABS,                false, true,  
    "STA",  3,  5,  ABS_INX_X,          true,  true,  
    "STZ",  3,  5,  ABS_INX_X,          false, true,  
    "STA",  4,  5,  ABS_LONG_INX_X,     false, false, 
    "LDY",  2,  2,  IMM,                true,  true,  
    "LDA",  2,  6,  DP_IDX_IND_X,       true,  true,  
    "LDX",  2,  2,  IMM,                true,  true,  
    "LDA",  2,  4,  STK_REL,            false, false, 
    "LDY",  2,  3,  DP,                 true,  true,  
    "LDA",  2,  3,  DP,                 true,  true,  
    "LDX",  2,  3,  DP,                 true,  true,  
    "LDA",  2,  6,  DP_IND_LONG,        false, false, 
    "TAY",  1,  2,  IMP,                true,  true,  
    "LDA",  2,  2,  IMM,                true,  true,  
    "TAX",  1,  2,  IMP,                true,  true,  
    "PLB",  1,  4,  STK,                false, false, 
    "LDY",  3,  4,  ABS,                true,  true,  
    "LDA",  3,  4,  ABS,                true,  true,  
    "LDX",  3,  4,  ABS,                true,  true,  
    "LDA",  4,  5,  ABS_LONG,           false, false, 
    "BCS",  2,  2,  PC_REL,             true,  true,  
    "LDA",  2,  5,  DP_IND_IDX_Y,       true,  true,  
    "LDA",  2,  5,  DP_IND,             false, true,  
    "LDA",  2,  7,  STK_IND_INX_Y,      false, false, 
    "LDY",  2,  4,  DP_IDX_X,           true,  true,  
    "LDA",  2,  4,  DP_IDX_X,           true,  true,  
    "LDX",  2,  4,  DP_IDX_Y,           true,  true,  
    "LDA",  2,  6,  DP_IND_LONG_INX_Y,  false, false, 
    "CLV",  1,  2,  IMP,                true,  true,  
    "LDA",  3,  4,  ABS_INX_Y,          true,  true,  
    "TSX",  1,  2,  IMP,                true,  true,  
    "TYX",  1,  2,  IMP,                false, false, 
    "LDY",  3,  4,  ABS_INX_X,          true,  true,  
    "LDA",  3,  4,  ABS_INX_X,          true,  true,  
    "LDX",  3,  4,  ABS_INX_Y,          true,  true,  
    "LDA",  4,  5,  ABS_LONG_INX_X,     false, false, 
    "CPY",  2,  2,  IMM,                true,  true,  
    "CMP",  2,  6,  DP_IDX_IND_X,       true,  true,  
    "REP",  2,  3,  IMM,                false, false, 
    "CMP",  2,  4,  STK_REL,            false, false, 
    "CPY",  2,  3,  DP,                 true,  true,  
    "CMP",  2,  3,  DP,                 true,  true,  
    "DEC",  2,  5,  DP,                 true,  true,  
    "CMP",  2,  6,  DP_IND_LONG,        false, false, 
    "INY",  1,  2,  IMP,                true,  true,  
    "CMP",  2,  2,  IMM,                true,  true,  
    "DEX",  1,  2,  IMP,                true,  true,  
    "WAI",  1,  3,  IMP,                false, false, 
    "CPY",  3,  4,  ABS,                true,  true,  
    "CMP",  3,  4,  ABS,                true,  true,  
    "DEC",  3,  6,  ABS,                true,  true,  
    "CMP",  4,  5,  ABS_LONG,           false, false, 
    "BNE",  2,  2,  PC_REL,             true,  true,  
    "CMP",  2,  5,  DP_IND_IDX_Y,       true,  true,  
    "CMP",  2,  5,  DP_IND,             false, true,  
    "CMP",  2,  7,  STK_IND_INX_Y,      false, false, 
    "PEI",  2,  6,  STK_DP_IND,         false, false, 
    "CMP",  2,  4,  DP_IDX_X,           true,  true,  
    "DEC",  2,  6,  DP_IDX_X,           true,  true,  
    "CMP",  2,  6,  DP_IND_LONG_INX_Y,  false, false, 
    "CLD",  1,  2,  IMP,                true,  true,  
    "CMP",  3,  4,  ABS_INX_Y,          true,  true,  
    "PHX",  1,  3,  STK,                false, true,  
    "STP",  1,  3,  IMP,                false, false, 
    "JMP",  3,  6,  ABS_IND_LONG,       false, false, 
    "CMP",  3,  4,  ABS_INX_X,          true,  true,  
    "DEC",  3,  7,  ABS_INX_X,          true,  true,  
    "CMP",  4,  5,  ABS_LONG_INX_X,     false, false, 
    "CPX",  2,  2,  IMM,                true,  true,  
    "SBC",  2,  6,  DP_IDX_IND_X,       true,  true,  
    "SEP",  2,  3,  IMM,                false, false, 
    "SBC",  2,  4,  STK_REL,            false, false, 
    "CPX",  2,  3,  DP,                 true,  true,  
    "SBC",  2,  3,  DP,                 true,  true,  
    "INC",  2,  5,  DP,                 true,  true,  
    "SBC",  2,  6,  DP_IND_LONG,        false, false, 
    "INX",  1,  2,  IMP,                true,  true,  
    "SBC",  2,  2,  IMM,                true,  true,  
    "NOP",  1,  2,  IMP,                true,  true,  
    "XBA",  1,  3,  IMP,                false, false, 
    "CPX",  3,  4,  ABS,                true,  true,  
    "SBC",  3,  4,  ABS,                true,  true,  
    "INC",  3,  6,  ABS,                true,  true,  
    "SBC",  4,  5,  ABS_LONG,           false, false, 
    "BEQ",  2,  2,  PC_REL,             true,  true,  
    "SBC",  2,  5,  DP_IND_IDX_Y,       true,  true,  
    "SBC",  2,  5,  DP_IND,             false, true,  
    "SBC",  2,  7,  STK_IND_INX_Y,      false, false, 
    "PEA",  3,  5,  STK_ABS,            false, false, 
    "SBC",  2,  4,  DP_IDX_X,           true,  true,  
    "INC",  2,  6,  DP_IDX_X,           true,  true,  
    "SBC",  2,  6,  DP_IND_LONG_INX_Y,  false, false, 
    "SED",  1,  2,  IMP,                true,  true,  
    "SBC",  3,  4,  ABS_INX_Y,          true,  true,  
    "PLX",  1,  4,  STK,                false, true,  
    "XCE",  1,  2,  IMP,                false, false, 
    "JSR",  3,  8,  ABS_INX_IND,        false, false, 
    "SBC",  3,  4,  ABS_INX_X,          true,  true,  
    "INC",  3,  7,  ABS_INX_X,          true,  true,  
    "SBC",  4,  5,  ABS_LONG_INX_X,     false, false, 
}; 

const char *AddrText [WDM+1][2] =
{
    {"", ""},       // ABS 
    {"(", ")"},     // ABS_IND
    {"[", "]"},     // ABS_IND_LONG
    {"(", ",X)"},   // ABS_INX_IND
    {"", ",X"},     // ABS_INX_X
    {"", ",Y"},     // ABS_INX_Y
    {"", ""},       // ABS_LONG
    {"", ",X"},     // ABS_LONG_INX_X
    {"", ""},       // ACC
    {"", ""},       // BLOCK
    {"", ""},       // DP
    {"(", ",X)"},   // DP_IDX_IND_X
    {"", ",X"},     // DP_IDX_X
    {"(", "),Y"},   // DP_IDX_Y
    {"(", ")"},     // DP_IND
    {"(", "),Y"},   // DP_IND_IDX_Y
    {"[", "]"},     // DP_IND_LONG
    {"[", "],Y"},   // DP_IND_LONG_INX_Y
    {"#", ""},      // IMM
    {"", ""},       // IMP
    {"", ""},       // PC_REL
    {"", ""},       // PC_REL_LONG 
    {"", ""},       // STK
    {"", ""},       // STK_ABS
    {"(", ")"},     // STK_DP_IND
    {"(", ",S),Y"}, // STK_IND_INX_Y
    {"", ""},       // STK_INT
    {"", ""},       // STK_PC_REL_LONG
    {"", ",S"},     // STK_REL
    {"", ""}        // WDM
};

WDC6502AnalyzerResults::WDC6502AnalyzerResults(WDC6502Analyzer *analyzer, WDC6502AnalyzerSettings *settings)
    :   AnalyzerResults(),
        mSettings(settings),
        mAnalyzer(analyzer)
{
}

WDC6502AnalyzerResults::~WDC6502AnalyzerResults()
{
}

void WDC6502AnalyzerResults::GenerateBubbleText(U64 frame_index, Channel & /*channel*/, DisplayBase display_base)   //unrefereced vars commented out to remove warnings.
{
    //char number_str[128];
    char result_str[128];

    ClearResultStrings();

    Frame frame = GetFrame( frame_index );

    if ( frame.mType == CycleAnalyzerEnums::Opcode_Read ) {
        GenerateOpcodeTxt(result_str, sizeof(result_str), (U32) frame.mData2, frame.mFlags);
    }
    else {
        AnalyzerHelpers::GetNumberString( frame.mData2, display_base, 8, result_str, sizeof(result_str));
    }

    //snprintf(result_str, sizeof(result_str), "%s", number_str);
    AddResultString(result_str);
}

void WDC6502AnalyzerResults::GenerateExportFile(const char *file, DisplayBase display_base, U32 /*export_type_user_id*/)
{

}

void WDC6502AnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
    char byte1_str[32] = "";
    
    ClearTabularText();
    Frame frame = GetFrame( frame_index );

    switch( frame.mType ){
        case(CycleAnalyzerEnums::Opcode_Read):
            {
                GenerateOpcodeTxt(byte1_str, sizeof(byte1_str), (U32) frame.mData2, frame.mFlags);
                AddTabularText(byte1_str);
            }
            break;
        case(CycleAnalyzerEnums::Data_Read):
            snprintf(byte1_str, sizeof(byte1_str), "%#X", (U8) frame.mData2);
            AddTabularText("DATA READ = ", byte1_str);
            break;
        case(CycleAnalyzerEnums::Data_Write):
            snprintf(byte1_str, sizeof(byte1_str), "%#X", (U8) frame.mData2);
            AddTabularText("DATA WRITE = ", byte1_str);
            break;  
        case(CycleAnalyzerEnums::Not_Valid):
            snprintf(byte1_str, sizeof(byte1_str), " %#X  ", (U32)frame.mData2);
            AddTabularText("INVALID = ", byte1_str);          
    }
      
}

void WDC6502AnalyzerResults::GeneratePacketTabularText(U64 /*packet_id*/, DisplayBase /*display_base*/)    //unrefereced vars commented out to remove warnings.
{
    ClearResultStrings();
    AddResultString("not supported");
}

void WDC6502AnalyzerResults::GenerateTransactionTabularText(U64 /*transaction_id*/, DisplayBase /*display_base*/)    //unrefereced vars commented out to remove warnings.
{
    ClearResultStrings();
    AddResultString("not supported");
}

void WDC6502AnalyzerResults::GenerateOpcodeTxt(char* Text, size_t Text_Size, U32 RawOpcodeData, U8 Flags)
{   
    // Generate the text from the opcode data
    U8 Opcode_value = 0;
    U8 Byte2 = 0;
    U8 Byte3 = 0;
    U8 Byte4 = 0;
    char OpcodeText[16];
    char OpcodeDataText[16];
    char OpcodeEndText[16] = "";

    if ((Flags & FOUR_BYTE_FLAG) != 0) {
        Byte4 = RawOpcodeData & 0xFF;
        RawOpcodeData = RawOpcodeData >> 8;
    }
    if  (   ((Flags & THREE_BYTE_FLAG) != 0) ||
            ((Flags & FOUR_BYTE_FLAG) != 0) ) {
        Byte3 = RawOpcodeData & 0xFF;
        RawOpcodeData = RawOpcodeData >> 8;
    }
    if (    ((Flags & TWO_BYTE_FLAG) != 0) ||
            ((Flags & THREE_BYTE_FLAG) != 0) ||
            ((Flags & FOUR_BYTE_FLAG) != 0) ) {
        Byte2 = RawOpcodeData & 0xFF;
        RawOpcodeData = RawOpcodeData >> 8;
    }

    Opcode_value = (U8)( RawOpcodeData & 0xFF);

    //OpcodeText
    AddFirstPartOpcode(OpcodeText, sizeof(OpcodeText), Opcode_value);
    AddLastPartOpcode(OpcodeEndText, sizeof(OpcodeEndText), Opcode_value);

    if ((Flags & TWO_BYTE_FLAG) != 0) {
        // One extra byte in opcode
        snprintf(OpcodeDataText, sizeof(OpcodeDataText), "$%X", Byte2 );
    }
    if ((Flags & THREE_BYTE_FLAG) != 0) {
        // Two extra bytes in opcode
        snprintf(OpcodeDataText, sizeof(OpcodeDataText), "$%04X", (Byte3<<8)+Byte2 );
    }       
    if ((Flags & FOUR_BYTE_FLAG) != 0) {
        // Three extra bytes in opcode
        snprintf(OpcodeDataText, sizeof(OpcodeDataText), "$%02X::%04X", Byte4, (Byte3<<8)+Byte2 );
    }         

    // Place all in output string
    snprintf(Text, Text_Size, "%s%s%s", OpcodeText, OpcodeDataText, OpcodeEndText);
}

void WDC6502AnalyzerResults::AddFirstPartOpcode(char * Text, size_t Text_Size, U8 OpcodeValue)
{
    Address_Modes_t Addr_Mode = ABS;
    
    Addr_Mode = opcodes[OpcodeValue].Address_Mode;
    
    // Setup the first part of the opcode text
    snprintf(Text, Text_Size, "%s %s", 
                opcodes[OpcodeValue].OpcodeText,
                AddrText[Addr_Mode][0]);
}

void WDC6502AnalyzerResults::AddLastPartOpcode(char * Text, size_t Text_Size, U8 OpcodeValue)
{
    Address_Modes_t Addr_Mode = ABS;
    
    Addr_Mode = opcodes[OpcodeValue].Address_Mode;
    
    // Setup the first part of the opcode text
    snprintf(Text, Text_Size, "%s", 
                AddrText[Addr_Mode][1]);
}