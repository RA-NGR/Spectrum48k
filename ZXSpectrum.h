#pragma once

#include "Common.h"
#include "ZXMacros.h"
#include "ZXPeripherals.h"

enum {
	ADC_INDIRECT_HL,
	ADC_R,
	ADD_HL_RR,
	ADD_INDIRECT_HL,
	ADD_R,
	AND_INDIRECT_HL,
	AND_R,
	CP_INDIRECT_HL,
	CP_R,
	DEC_INDIRECT_HL,
	DEC_R,
	DEC_RR,
	EX_INDIRECT_SP_HL,
	INC_INDIRECT_HL,
	INC_R,
	INC_RR,
	JP_HL,
	LD_HL_INDIRECT_NN,
	LD_INDIRECT_HL_N,
	LD_INDIRECT_NN_HL,
	LD_INDIRECT_HL_R,
	LD_R_INDIRECT_HL,
	LD_R_N,
	LD_R_R,
	LD_RR_NN,
	LD_SP_HL,
	OR_INDIRECT_HL,
	OR_R,
	POP_SS,
	PUSH_SS,
	SBC_INDIRECT_HL,
	SBC_R,
	SUB_INDIRECT_HL,
	SUB_R,
	XOR_INDIRECT_HL,
	XOR_R,
	CB_PREFIX,
// ------------- End of DD/FD affected instructions --------
	LD_A_INDIRECT_BC,
	LD_A_INDIRECT_DE,
	LD_A_INDIRECT_NN,
	LD_INDIRECT_BC_A,
	LD_INDIRECT_DE_A,
	LD_INDIRECT_NN_A,
	LD_A_I_LD_A_R,
	LD_I_A_LD_R_A,
	LD_RR_INDIRECT_NN,
	LD_INDIRECT_NN_RR,
	EX_DE_HL,
	EX_AF_AF_PRIME,
	EXX,
	LDI_LDD,
	LDIR_LDDR,
	CPI_CPD,
	CPIR_CPDR,
	ADD_N,
	ADC_N,
	SUB_N,
	SBC_N,
	AND_N,
	XOR_N,
	OR_N,
	CP_N,
	ADC_HL_RR,
	SBC_HL_RR,
	DAA,
	CPL,
	NEG,
	CCF,
	SCF,
	NOP,
	HALT,
	DI,
	EI,
	IM_N,
	RLCA,
	RLA,
	RRCA,
	RRA,
	RLC_R,
	RLC_INDIRECT_HL,
	RL_R,
	RL_INDIRECT_HL,
	RRC_R,
	RRC_INDIRECT_HL,
	RR_R,
	RR_INDIRECT_HL,
	SLA_R,
	SLA_INDIRECT_HL,
	SLL_R,
	SLL_INDIRECT_HL,
	SRA_R,
	SRA_INDIRECT_HL,
	SRL_R,
	SRL_INDIRECT_HL,
	RLD_RRD,
	BIT_B_R,
	BIT_B_INDIRECT_HL,
	SET_B_R,
	SET_B_INDIRECT_HL,
	RES_B_R,
	RES_B_INDIRECT_HL,
	JP_NN,
	JP_CC_NN,
	JR_E,
	JR_DD_E,
	DJNZ_E,
	CALL_NN,
	CALL_CC_NN,
	RET,
	RET_CC,
	RETI_RETN,
	RST_P,
	IN_A_N,
	IN_R_C,
	INI_IND,
	INIR_INDR,
	OUT_N_A,
	OUT_C_R,
	OUTI_OUTD,
	OTIR_OTDR,
	DD_PREFIX,
	FD_PREFIX,
	ED_PREFIX,
	ED_UNDEFINED
};
const uint8_t __attribute__((section(".time_critical." "tables"))) instructionTable[256] = {
	NOP, LD_RR_NN, LD_INDIRECT_BC_A, INC_RR, INC_R, DEC_R, LD_R_N, RLCA,																/* 0x00 - 0x07 */
	EX_AF_AF_PRIME, ADD_HL_RR, LD_A_INDIRECT_BC, DEC_RR, INC_R, DEC_R, LD_R_N, RRCA,													/* 0x08 - 0x0F */
	DJNZ_E, LD_RR_NN, LD_INDIRECT_DE_A, INC_RR, INC_R, DEC_R, LD_R_N, RLA,																/* 0x10 - 0x17 */
	JR_E, ADD_HL_RR, LD_A_INDIRECT_DE, DEC_RR, INC_R, DEC_R, LD_R_N, RRA,																/* 0x18 - 0x1F */
	JR_DD_E, LD_RR_NN, LD_INDIRECT_NN_HL, INC_RR, INC_R, DEC_R, LD_R_N, DAA,															/* 0x20 - 0x27 */
	JR_DD_E, ADD_HL_RR, LD_HL_INDIRECT_NN, DEC_RR, INC_R, DEC_R, LD_R_N, CPL,															/* 0x28 - 0x2F */
	JR_DD_E, LD_RR_NN, LD_INDIRECT_NN_A, INC_RR, INC_INDIRECT_HL, DEC_INDIRECT_HL, LD_INDIRECT_HL_N, SCF,								/* 0x30 - 0x37 */
	JR_DD_E, ADD_HL_RR, LD_A_INDIRECT_NN, DEC_RR, INC_R, DEC_R, LD_R_N, CCF,															/* 0x38 - 0x3F */
	LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_INDIRECT_HL, LD_R_R,															/* 0x40 - 0x47 */
	LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_INDIRECT_HL, LD_R_R,															/* 0x48 - 0x4F */
	LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_INDIRECT_HL, LD_R_R,															/* 0x50 - 0x57 */
	LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_INDIRECT_HL, LD_R_R,															/* 0x58 - 0x5F */
	LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_INDIRECT_HL, LD_R_R,															/* 0x60 - 0x67 */
	LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_INDIRECT_HL, LD_R_R,															/* 0x68 - 0x6F */
	LD_INDIRECT_HL_R, LD_INDIRECT_HL_R, LD_INDIRECT_HL_R, LD_INDIRECT_HL_R, LD_INDIRECT_HL_R, LD_INDIRECT_HL_R, HALT,LD_INDIRECT_HL_R,  /* 0x70 - 0x77 */
	LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_R, LD_R_INDIRECT_HL, LD_R_R,															/* 0x78 - 0x7F */
	ADD_R, ADD_R, ADD_R, ADD_R, ADD_R, ADD_R, ADD_INDIRECT_HL, ADD_R,																	/* 0x80 - 0x87 */
	ADC_R, ADC_R, ADC_R, ADC_R, ADC_R, ADC_R, ADC_INDIRECT_HL, ADC_R,																	/* 0x88 - 0x8F */
	SUB_R, SUB_R, SUB_R, SUB_R, SUB_R, SUB_R, SUB_INDIRECT_HL, SUB_R,																	/* 0x90 - 0x97 */
	SBC_R, SBC_R, SBC_R, SBC_R, SBC_R, SBC_R, SBC_INDIRECT_HL, SBC_R,																	/* 0x98 - 0x9F */
	AND_R, AND_R, AND_R, AND_R, AND_R, AND_R, AND_INDIRECT_HL, AND_R,																	/* 0xA0 - 0xA7 */
	XOR_R, XOR_R, XOR_R, XOR_R, XOR_R, XOR_R, XOR_INDIRECT_HL, XOR_R,																	/* 0xA8 - 0xAF */
	OR_R, OR_R, OR_R, OR_R, OR_R, OR_R, OR_INDIRECT_HL, OR_R,																			/* 0xB0 - 0xB7 */
	CP_R, CP_R, CP_R, CP_R, CP_R, CP_R, CP_INDIRECT_HL, CP_R,																			/* 0xB8 - 0xBF */
	RET_CC, POP_SS, JP_CC_NN, JP_NN, CALL_CC_NN, PUSH_SS, ADD_N, RST_P,																	/* 0xC0 - 0xC7 */
	RET_CC, RET, JP_CC_NN, CB_PREFIX, CALL_CC_NN, CALL_NN, ADC_N, RST_P,																/* 0xC8 - 0xCF */
	RET_CC, POP_SS, JP_CC_NN, OUT_N_A, CALL_CC_NN, PUSH_SS, SUB_N, RST_P,																/* 0xD0 - 0xD7 */
	RET_CC, EXX, JP_CC_NN, IN_A_N, CALL_CC_NN, DD_PREFIX, SBC_N, RST_P,																	/* 0xD8 - 0xDF */
	RET_CC, POP_SS, JP_CC_NN, EX_INDIRECT_SP_HL, CALL_CC_NN, PUSH_SS, AND_N, RST_P,														/* 0xE0 - 0xE7 */
	RET_CC, JP_HL, JP_CC_NN, EX_DE_HL, CALL_CC_NN, ED_PREFIX, XOR_N, RST_P,																/* 0xE8 - 0xEF */
	RET_CC, POP_SS, JP_CC_NN, DI, CALL_CC_NN, PUSH_SS, OR_N, RST_P,																		/* 0xF0 - 0xF7 */
	RET_CC, LD_SP_HL, JP_CC_NN, EI, CALL_CC_NN, FD_PREFIX, CP_N, RST_P																	/* 0xF8 - 0xFF */
};
const uint8_t __attribute__((section(".time_critical." "tables"))) cbInstructionTable[256] = {
	RLC_R, RLC_R, RLC_R, RLC_R, RLC_R, RLC_R, RLC_INDIRECT_HL, RLC_R,					/* 0x00 - 0x07 */
	RRC_R, RRC_R, RRC_R, RRC_R, RRC_R, RRC_R, RRC_INDIRECT_HL, RRC_R,					/* 0x08 - 0x0F */
	RL_R, RL_R, RL_R, RL_R, RL_R, RL_R, RL_INDIRECT_HL, RL_R,							/* 0x10 - 0x17 */
	RR_R, RR_R, RR_R, RR_R, RR_R, RR_R, RR_INDIRECT_HL, RR_R,							/* 0x18 - 0x1F */
	SLA_R, SLA_R, SLA_R, SLA_R, SLA_R, SLA_R, SLA_INDIRECT_HL, SLA_R,					/* 0x20 - 0x27 */
	SRA_R, SRA_R, SRA_R, SRA_R, SRA_R, SRA_R, SRA_INDIRECT_HL, SRA_R,					/* 0x28 - 0x2F */
	SLL_R, SLL_R, SLL_R, SLL_R, SLL_R, SLL_R, SLL_INDIRECT_HL, SLL_R,					/* 0x30 - 0x37 */
	SRL_R, SRL_R, SRL_R, SRL_R, SRL_R, SRL_R, SRL_INDIRECT_HL, SRL_R,					/* 0x38 - 0x3F */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_INDIRECT_HL, BIT_B_R,	/* 0x40 - 0x47 */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_INDIRECT_HL, BIT_B_R,	/* 0x48 - 0x4F */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_INDIRECT_HL, BIT_B_R,	/* 0x50 - 0x57 */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_INDIRECT_HL, BIT_B_R,	/* 0x58 - 0x5F */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_INDIRECT_HL, BIT_B_R,	/* 0x60 - 0x67 */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_INDIRECT_HL, BIT_B_R,	/* 0x68 - 0x6F */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_INDIRECT_HL, BIT_B_R,	/* 0x70 - 0x77 */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_INDIRECT_HL, BIT_B_R,	/* 0x78 - 0x7F */
	RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_INDIRECT_HL, RES_B_R,	/* 0x80 - 0x87 */
	RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_INDIRECT_HL, RES_B_R,	/* 0x88 - 0x8F */
	RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_INDIRECT_HL, RES_B_R,	/* 0x90 - 0x97 */
	RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_INDIRECT_HL, RES_B_R,	/* 0x98 - 0x9F */
	RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_INDIRECT_HL, RES_B_R,	/* 0xA0 - 0xA7 */
	RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_INDIRECT_HL, RES_B_R,	/* 0xA8 - 0xAF */
	RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_INDIRECT_HL, RES_B_R,	/* 0xB0 - 0xB7 */
	RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_R, RES_B_INDIRECT_HL, RES_B_R,	/* 0xB8 - 0xBF */
	SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_INDIRECT_HL, SET_B_R,	/* 0xC0 - 0xC7 */
	SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_INDIRECT_HL, SET_B_R,	/* 0xC8 - 0xCF */
	SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_INDIRECT_HL, SET_B_R,	/* 0xD0 - 0xD7 */
	SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_INDIRECT_HL, SET_B_R,	/* 0xD8 - 0xDF */
	SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_INDIRECT_HL, SET_B_R,	/* 0xE0 - 0xE7 */
	SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_INDIRECT_HL, SET_B_R,	/* 0xE8 - 0xEF */
	SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_INDIRECT_HL, SET_B_R,	/* 0xF0 - 0xF7 */
	SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_R, SET_B_INDIRECT_HL, SET_B_R,	/* 0xF8 - 0xFF */
};
const uint8_t __attribute__((section(".time_critical." "tables"))) edInstructionTable[256] = {
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	IN_R_C, OUT_C_R, SBC_HL_RR, LD_INDIRECT_NN_RR, NEG, RETI_RETN, IM_N, LD_I_A_LD_R_A,
	IN_R_C, OUT_C_R, ADC_HL_RR, LD_RR_INDIRECT_NN, NEG, RETI_RETN, IM_N, LD_I_A_LD_R_A,
	IN_R_C, OUT_C_R, SBC_HL_RR, LD_INDIRECT_NN_RR, NEG, RETI_RETN, IM_N, LD_A_I_LD_A_R,
	IN_R_C, OUT_C_R, ADC_HL_RR, LD_RR_INDIRECT_NN, NEG, RETI_RETN, IM_N, LD_A_I_LD_A_R,
	IN_R_C, OUT_C_R, SBC_HL_RR, LD_INDIRECT_NN_RR, NEG, RETI_RETN, IM_N, RLD_RRD,
	IN_R_C, OUT_C_R, ADC_HL_RR, LD_RR_INDIRECT_NN, NEG, RETI_RETN, IM_N, RLD_RRD,
	IN_R_C, OUT_C_R, SBC_HL_RR, LD_INDIRECT_NN_RR, NEG, RETI_RETN, IM_N, ED_UNDEFINED,
	IN_R_C, OUT_C_R, ADC_HL_RR, LD_RR_INDIRECT_NN, NEG, RETI_RETN, IM_N, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	LDI_LDD, CPI_CPD, INI_IND, OUTI_OUTD, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	LDI_LDD, CPI_CPD, INI_IND, OUTI_OUTD, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	LDIR_LDDR, CPIR_CPDR, INIR_INDR, OTIR_OTDR, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	LDIR_LDDR, CPIR_CPDR, INIR_INDR, OTIR_OTDR, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED
};

const uint8_t __attribute__((section(".time_critical." "tables"))) halfcarryAddTable[8] = { 0x00, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x10 };
const uint8_t __attribute__((section(".time_critical." "tables"))) halfcarrySubTable[8] = { 0x00, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x10 };
const uint8_t __attribute__((section(".time_critical." "tables"))) overflowAddTable[8] = { 0x00, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x00 };
const uint8_t __attribute__((section(".time_critical." "tables"))) overflowSubTable[8] = { 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00 };
const uint8_t __attribute__((section(".time_critical." "tables"))) sz53Table[256] = {
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
	0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0,
	0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,
	0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0,
	0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
	0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0,
	0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,
	0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0,
	0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8
};
const uint8_t __attribute__((section(".time_critical." "tables"))) parityTable[256] = {
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04
};
const uint8_t __attribute__((section(".time_critical." "tables"))) sz53pTable[256] = {
	0x44, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x08, 0x0C, 0x0C, 0x08, 0x0C, 0x08, 0x08, 0x0C,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x0C, 0x08, 0x08, 0x0C, 0x08, 0x0C, 0x0C, 0x08,
	0x20, 0x24, 0x24, 0x20, 0x24, 0x20, 0x20, 0x24,
	0x2C, 0x28, 0x28, 0x2C, 0x28, 0x2C, 0x2C, 0x28,
	0x24, 0x20, 0x20, 0x24, 0x20, 0x24, 0x24, 0x20,
	0x28, 0x2C, 0x2C, 0x28, 0x2C, 0x28, 0x28, 0x2C,
	0x00, 0x04, 0x04, 0x00, 0x04, 0x00, 0x00, 0x04,
	0x0C, 0x08, 0x08, 0x0C, 0x08, 0x0C, 0x0C, 0x08,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x00,
	0x08, 0x0C, 0x0C, 0x08, 0x0C, 0x08, 0x08, 0x0C,
	0x24, 0x20, 0x20, 0x24, 0x20, 0x24, 0x24, 0x20,
	0x28, 0x2C, 0x2C, 0x28, 0x2C, 0x28, 0x28, 0x2C,
	0x20, 0x24, 0x24, 0x20, 0x24, 0x20, 0x20, 0x24,
	0x2C, 0x28, 0x28, 0x2C, 0x28, 0x2C, 0x2C, 0x28,
	0x80, 0x84, 0x84, 0x80, 0x84, 0x80, 0x80, 0x84,
	0x8C, 0x88, 0x88, 0x8C, 0x88, 0x8C, 0x8C, 0x88,
	0x84, 0x80, 0x80, 0x84, 0x80, 0x84, 0x84, 0x80,
	0x88, 0x8C, 0x8C, 0x88, 0x8C, 0x88, 0x88, 0x8C,
	0xA4, 0xA0, 0xA0, 0xA4, 0xA0, 0xA4, 0xA4, 0xA0,
	0xA8, 0xAC, 0xAC, 0xA8, 0xAC, 0xA8, 0xA8, 0xAC,
	0xA0, 0xA4, 0xA4, 0xA0, 0xA4, 0xA0, 0xA0, 0xA4,
	0xAC, 0xA8, 0xA8, 0xAC, 0xA8, 0xAC, 0xAC, 0xA8,
	0x84, 0x80, 0x80, 0x84, 0x80, 0x84, 0x84, 0x80,
	0x88, 0x8C, 0x8C, 0x88, 0x8C, 0x88, 0x88, 0x8C,
	0x80, 0x84, 0x84, 0x80, 0x84, 0x80, 0x80, 0x84,
	0x8C, 0x88, 0x88, 0x8C, 0x88, 0x8C, 0x8C, 0x88,
	0xA0, 0xA4, 0xA4, 0xA0, 0xA4, 0xA0, 0xA0, 0xA4,
	0xAC, 0xA8, 0xA8, 0xAC, 0xA8, 0xAC, 0xAC, 0xA8,
	0xA4, 0xA0, 0xA0, 0xA4, 0xA0, 0xA4, 0xA4, 0xA0,
	0xA8, 0xAC, 0xAC, 0xA8, 0xAC, 0xA8, 0xA8, 0xAC
};
const uint8_t __attribute__((section(".time_critical." "tables"))) xorConditionTable[8] = { FLAG_Z, 0, FLAG_C, 0, FLAG_P, 0, FLAG_S, 0 };
const uint8_t __attribute__((section(".time_critical." "tables"))) andConditionTable[8] = { FLAG_Z, FLAG_Z, FLAG_C, FLAG_C, FLAG_P, FLAG_P, FLAG_S, FLAG_S };
const uint8_t __attribute__((section(".time_critical." "tables"))) rstTable[8] = { 0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38 };

const uint8_t __attribute__((section(".time_critical." "tables"))) contPattern[224] = { 
	6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
	6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
	6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
	6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

class ZXSpectrum
{
	const uint16_t m_colorLookup[16] = { 0x0000, 0x1700, 0x00B8, 0x17B8, 0xE005, 0xF705, 0xE0BD, 0xF7BD,
										 0x0000, 0x1F00, 0x00F8, 0x1FF8, 0xE007, 0xFF07, 0xE0FF, 0xFFFF };
	typedef union
	{
		struct { uint8_t l, h; } b; // Little Endian
		uint16_t w;
	} pair;
	struct Z80 {
		pair af, bc, de, hl;
		pair af_, bc_, de_, hl_;
		pair ix, iy;
		uint8_t i;
		uint8_t r;
		uint8_t r7;
		pair sp, pc;
		pair memptr;
		int iff2_read;
		uint8_t iff1, iff2, im;
		int halted;
		uint8_t q;
		int32_t intEnabledAt;
		int32_t tCount;
		void* pRegisters[8];
		void* pPairs[5];
		void* pDDRegisters[8];
		void* pDDPairs[5];
		void* pFDRegisters[8];
		void* pFDPairs[5];
	} m_Z80Processor;
	struct Tape
	{
		bool isTapeActive; // Tape running
		int  stateCycles; // CPU cycles per state
		int  statesCount; //  Number of states repeat
		uint8_t tapeState; // Phase of tape (pilot, syncro etc.)
	} m_ZXTape;
	struct TapeStates
	{
		int stateCycles; // CPU cycles per state
		int statesCount; // Number of states repeat
	} m_tapeStates[5] = {
		{ 855, 2 },     // 0
		{ 1710, 2 },    // 1
		{ 2168, 4846 }, // Pilot
		{ 667, 1 },     // Sync HIGH
		{ 735, 1 }      // Sync LOW    
	};
	struct TAPData
	{
		uint16_t size;
		uint8_t* data;
		uint32_t bit;
	} m_TAPSection;
	struct BorderColors
	{
		uint16_t x;
		uint8_t y;
		uint16_t color;
	} m_borderColors[BORDER_BUFFER_SIZE];
	uint8_t m_pbWIndex = 0;
	uint8_t m_pbRIndex = 0;
	uint16_t m_borderColor = m_colorLookup[7];
	uint8_t m_frameCounter = 0;
	uint8_t* m_pZXMemory;
	uint16_t* m_pScreenBuffer[2];
//#ifdef CONT_TABLE
//	uint8_t* m_pContendTable;
//#else
////	void __attribute__((section(".time_critical." "contendedAccess"))) contendedAccess(uint16_t address, int32_t time);
//#endif // CONT_TABLE
	bool m_initComplete = false;
	int16_t m_scanLine = -1;
	uint32_t m_emulationTime = 0, m_maxEmulTime = 0;
	union PortFE
	{
		struct
		{
			uint8_t borderColor : 3;
			uint8_t tapeOut : 1;
			uint8_t soundOut : 1;
			uint8_t unused : 3;
		};
		uint8_t rawData;
	} m_outPortFE;
	uint8_t m_tapeIn = 0xBF;
	uint8_t* m_pInPort;
	Display* m_pDisplayInstance;
	union ZXSettings
	{
		struct
		{
			uint8_t soundEnabled : 1;
			uint8_t unused : 7;
		};
		uint8_t rawData;
	} m_emulSettings = { 0x01 };
	bool m_soundEnabled = true;
	void __attribute__((section(".time_critical." "drawLine"))) drawLine(int posY);
	int8_t intZ80();
	void processTape();
	void __attribute__((section(".time_critical." "writeMem"))) writeMem(uint16_t address, uint8_t data);
	uint8_t __attribute__((section(".time_critical." "readMem"))) readMem(uint16_t address);
	uint8_t unattachedPort();
	uint8_t readPort(uint16_t port);
	void writePort(uint16_t port, uint8_t data);
	void stepZ80();
public:
	ZXSpectrum() {};
	~ZXSpectrum();
	bool init(Display* pDisplayInstance, Keyboard* pKeyboardInstance);
	bool loadROMFile(const char* pFileName);
	void resetZ80();
	void loopZ80();
	uint32_t getEmulationTime() { return m_emulationTime; };
	uint32_t getMaxEmulationTime() { return m_maxEmulTime; };
	void enableSound(bool isEnable = true) { m_emulSettings.soundEnabled = (isEnable ? 1 : 0); };
	void startTape(uint8_t* pBuffer, uint32_t bufferSize);
	void stopTape() { m_ZXTape.isTapeActive = false; m_tapeIn = 0xBF; };
	bool tapeActive() { return m_ZXTape.isTapeActive; };
	void tapeMode(bool isTurbo = false);
	void storeState(const char* pFileName);
	void restoreState(const char* pFileName);
};