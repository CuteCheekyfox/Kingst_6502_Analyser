#ifndef WDC6502_ANALYZER_OPCODES_H
#define WDC6502_ANALYZER_OPCODES_H

enum Address_Modes_t { 
    ABS, ABS_IND, ABS_IND_LONG, ABS_INX_IND, ABS_INX_X, ABS_INX_Y,
    ABS_LONG, ABS_LONG_INX_X, ACC, BLOCK, DP, DP_IDX_IND_X,
    DP_IDX_X, DP_IDX_Y, DP_IND, DP_IND_IDX_Y, DP_IND_LONG,
    DP_IND_LONG_INX_Y, IMM, IMP, PC_REL, PC_REL_LONG, STK,
    STK_ABS, STK_DP_IND, STK_IND_INX_Y, STK_INT, STK_PC_REL_LONG,
    STK_REL, WDM
 };

struct opcode_detail_struct {
    char OpcodeText[8];
    U8 No_of_Opcode_bytes;
    U8 Min_No_Cycles;
    Address_Modes_t Address_Mode;
    bool nmos_6502;
    bool cmos_65C02;
} ;

extern opcode_detail_struct opcodes[256];

#endif //WDC6502_ANALYZER_OPCODES_H