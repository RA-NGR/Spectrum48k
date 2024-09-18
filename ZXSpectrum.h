#pragma once

#include "Common.h"
#include "ZXMacros.h"
#include "ZXPeripherals.h"
#include "ROM48.h"
#include "ROM128-0.h"
#include "ROM128-1.h"
#include "RingBuffer.h"

enum {
	ADD_HL_RR,
	AL_R, // 8-bit arithmetic and logical ops
	DEC_8,
	DEC_RR,
	EX_INDIRECT_SP_HL,
	INC_8,
	INC_RR,
	JP_HL,
	LD_HL_INDIRECT_NN,
	LD_INDIRECT_HL_N,
	LD_INDIRECT_NN_HL,
	LD_INDIRECT_HL_R,
	LD_R_N,
	LD_R_8,
	LD_RR_NN,
	LD_SP_HL,
	POP_SS,
	PUSH_SS,
	CB_PREFIX,
// ------------- End of DD/FD affected instructions --------
	LD_A_INDIRECT,
	LD_INDIRECT_A,
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
	AL_N,
	ADC_HL_RR,
	SBC_HL_RR,
	DAA,
	CPL,
	NEG,
	CCF,
	SCF,
	HALT,
	DI,
	EI,
	IM_N,
	RLCA,
	RLA,
	RRCA,
	RRA,
	RS_R, // 8-bit rotate and shift ops
	RLD_RRD,
	BIT_B_R,
	SR_B_R, // 8-bit set and reset bit ops
	JP_NN,
	JR_E,
	DJNZ_E,
	CALL_NN,
	RET,
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
	NOP,
	ED_UNDEFINED
};
const uint8_t __attribute__((section(".time_critical." "tables"))) instructionTable[256] = {
	NOP, LD_RR_NN, LD_INDIRECT_A, INC_RR, INC_8, DEC_8, LD_R_N, RLCA,																	/* 0x00 - 0x07 */
	EX_AF_AF_PRIME, ADD_HL_RR, LD_A_INDIRECT, DEC_RR, INC_8, DEC_8, LD_R_N, RRCA,														/* 0x08 - 0x0F */
	DJNZ_E, LD_RR_NN, LD_INDIRECT_A, INC_RR, INC_8, DEC_8, LD_R_N, RLA,																	/* 0x10 - 0x17 */
	JR_E, ADD_HL_RR, LD_A_INDIRECT, DEC_RR, INC_8, DEC_8, LD_R_N, RRA,																	/* 0x18 - 0x1F */
	JR_E, LD_RR_NN, LD_INDIRECT_NN_HL, INC_RR, INC_8, DEC_8, LD_R_N, DAA,																/* 0x20 - 0x27 */
	JR_E, ADD_HL_RR, LD_HL_INDIRECT_NN, DEC_RR, INC_8, DEC_8, LD_R_N, CPL,																/* 0x28 - 0x2F */
	JR_E, LD_RR_NN, LD_INDIRECT_A, INC_RR, INC_8, DEC_8, LD_INDIRECT_HL_N, SCF,															/* 0x30 - 0x37 */
	JR_E, ADD_HL_RR, LD_A_INDIRECT, DEC_RR, INC_8, DEC_8, LD_R_N, CCF,																	/* 0x38 - 0x3F */
	LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8,																		/* 0x40 - 0x47 */
	LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8,																		/* 0x48 - 0x4F */
	LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8,																		/* 0x50 - 0x57 */
	LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8,																		/* 0x58 - 0x5F */
	LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8,																		/* 0x60 - 0x67 */
	LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8,																		/* 0x68 - 0x6F */
	LD_INDIRECT_HL_R, LD_INDIRECT_HL_R, LD_INDIRECT_HL_R, LD_INDIRECT_HL_R, LD_INDIRECT_HL_R, LD_INDIRECT_HL_R, HALT,LD_INDIRECT_HL_R,  /* 0x70 - 0x77 */
	LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8, LD_R_8,																		/* 0x78 - 0x7F */
	AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R,																						/* 0x80 - 0x87 */
	AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R,																						/* 0x88 - 0x8F */
	AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R,																						/* 0x90 - 0x97 */
	AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R,																						/* 0x98 - 0x9F */
	AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R,																						/* 0xA0 - 0xA7 */
	AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R,																						/* 0xA8 - 0xAF */
	AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R,																						/* 0xB0 - 0xB7 */
	AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R, AL_R,																						/* 0xB8 - 0xBF */
	RET, POP_SS, JP_NN, JP_NN, CALL_NN, PUSH_SS, AL_N, RST_P,																			/* 0xC0 - 0xC7 */
	RET, RET, JP_NN, CB_PREFIX, CALL_NN, CALL_NN, AL_N, RST_P,																			/* 0xC8 - 0xCF */
	RET, POP_SS, JP_NN, OUT_N_A, CALL_NN, PUSH_SS, AL_N, RST_P,																			/* 0xD0 - 0xD7 */
	RET, EXX, JP_NN, IN_A_N, CALL_NN, DD_PREFIX, AL_N, RST_P,																			/* 0xD8 - 0xDF */
	RET, POP_SS, JP_NN, EX_INDIRECT_SP_HL, CALL_NN, PUSH_SS, AL_N, RST_P,																/* 0xE0 - 0xE7 */
	RET, JP_HL, JP_NN, EX_DE_HL, CALL_NN, ED_PREFIX, AL_N, RST_P,																		/* 0xE8 - 0xEF */
	RET, POP_SS, JP_NN, DI, CALL_NN, PUSH_SS, AL_N, RST_P,																				/* 0xF0 - 0xF7 */
	RET, LD_SP_HL, JP_NN, EI, CALL_NN, FD_PREFIX, AL_N, RST_P																			/* 0xF8 - 0xFF */
};
const uint8_t __attribute__((section(".time_critical." "tables"))) cbInstructionTable[256] = {
	RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R,							/* 0x00 - 0x07 */
	RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R,							/* 0x08 - 0x0F */
	RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R,							/* 0x10 - 0x17 */
	RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R,							/* 0x18 - 0x1F */
	RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R,							/* 0x20 - 0x27 */
	RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R,							/* 0x28 - 0x2F */
	RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R,							/* 0x30 - 0x37 */
	RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R, RS_R,							/* 0x38 - 0x3F */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R,	/* 0x40 - 0x47 */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R,	/* 0x48 - 0x4F */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R,	/* 0x50 - 0x57 */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R,	/* 0x58 - 0x5F */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R,	/* 0x60 - 0x67 */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R,	/* 0x68 - 0x6F */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R,	/* 0x70 - 0x77 */
	BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R, BIT_B_R,	/* 0x78 - 0x7F */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0x80 - 0x87 */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0x88 - 0x8F */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0x90 - 0x97 */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0x98 - 0x9F */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xA0 - 0xA7 */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xA8 - 0xAF */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xB0 - 0xB7 */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xB8 - 0xBF */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xC0 - 0xC7 */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xC8 - 0xCF */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xD0 - 0xD7 */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xD8 - 0xDF */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xE0 - 0xE7 */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xE8 - 0xEF */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xF0 - 0xF7 */
	SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R, SR_B_R,			/* 0xF8 - 0xFF */
};
const uint8_t __attribute__((section(".time_critical." "tables"))) edInstructionTable[256] = {
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x00 - 0x07 */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x08 - 0x0F */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x10 - 0x17 */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x18 - 0x1F */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x20 - 0x27 */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x28 - 0x2F */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x30 - 0x37 */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x38 - 0x3F */
	IN_R_C, OUT_C_R, SBC_HL_RR, LD_INDIRECT_NN_RR, NEG, RETI_RETN, IM_N, LD_I_A_LD_R_A,							    /* 0x40 - 0x47 */
	IN_R_C, OUT_C_R, ADC_HL_RR, LD_RR_INDIRECT_NN, NEG, RETI_RETN, IM_N, LD_I_A_LD_R_A,							    /* 0x48 - 0x4F */
	IN_R_C, OUT_C_R, SBC_HL_RR, LD_INDIRECT_NN_RR, NEG, RETI_RETN, IM_N, LD_A_I_LD_A_R,							    /* 0x50 - 0x57 */
	IN_R_C, OUT_C_R, ADC_HL_RR, LD_RR_INDIRECT_NN, NEG, RETI_RETN, IM_N, LD_A_I_LD_A_R,							    /* 0x58 - 0x5F */
	IN_R_C, OUT_C_R, SBC_HL_RR, LD_INDIRECT_NN_RR, NEG, RETI_RETN, IM_N, RLD_RRD,								    /* 0x60 - 0x67 */
	IN_R_C, OUT_C_R, ADC_HL_RR, LD_RR_INDIRECT_NN, NEG, RETI_RETN, IM_N, RLD_RRD,								    /* 0x68 - 0x6F */
	IN_R_C, OUT_C_R, SBC_HL_RR, LD_INDIRECT_NN_RR, NEG, RETI_RETN, IM_N, ED_UNDEFINED,							    /* 0x70 - 0x77 */
	IN_R_C, OUT_C_R, ADC_HL_RR, LD_RR_INDIRECT_NN, NEG, RETI_RETN, IM_N, ED_UNDEFINED,							    /* 0x78 - 0x7F */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x80 - 0x87 */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x88 - 0x8F */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x90 - 0x97 */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0x98 - 0x9F */
	LDI_LDD, CPI_CPD, INI_IND, OUTI_OUTD, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,				    /* 0xA0 - 0xA7 */
	LDI_LDD, CPI_CPD, INI_IND, OUTI_OUTD, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,				    /* 0xA8 - 0xAF */
	LDIR_LDDR, CPIR_CPDR, INIR_INDR, OTIR_OTDR, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,			    /* 0xB0 - 0xB7 */
	LDIR_LDDR, CPIR_CPDR, INIR_INDR, OTIR_OTDR, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED,			    /* 0xB8 - 0xBF */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0xC0 - 0xC7 */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0xC8 - 0xCF */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0xD0 - 0xD7 */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0xD8 - 0xDF */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0xE0 - 0xE7 */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0xE8 - 0xEF */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, /* 0xF0 - 0xF7 */
	ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED, ED_UNDEFINED  /* 0xF8 - 0xFF */
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
const uint8_t __attribute__((section(".time_critical." "tables"))) contPattern[228] = { 
	6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
	6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
	6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
	6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0
};

class ZXSpectrum
{
private:
// Processor
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
		uint8_t iff2_read, iff1, iff2, im;
		uint8_t q;
		uint32_t halted;
		uint32_t skipINT; // Processor is just after EI or within DD/FD prefixed opcode 
		int tCount;
		void* pRegisters[8];
		void* pPairs[5];
		void* pDDRegisters[8];
		void* pDDPairs[5];
		void* pFDRegisters[8];
		void* pFDPairs[5];
	} m_Z80Processor;
	void __attribute__((section(".time_critical." "intZ80"))) intZ80();
	void __attribute__((section(".time_critical." "stepZ80"))) stepZ80();
// Tape 
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
		File* pFile;
		uint16_t sectionSize;
		uint16_t dataSize;
		uint8_t dataBuffer[TAP_BUFFER_SIZE];
		uint32_t bit;
		bool isLastBit;
	} m_TAPSection;
	uint8_t m_tapeBit = 0;
	uint16_t m_bufferSize;
	void processTape();
	bool fetchTapeData();
// Computer
	uint8_t* m_pRAMBanks[8];
	uint8_t* m_pRAMPages[4];
	bool m_pageContended[4];
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
	union Port7FFD
	{
		struct
		{
			uint8_t page : 3;
			uint8_t screen : 1;
			uint8_t rom : 1;
			uint8_t disabled : 1;
			uint8_t unused : 2;
		};
		uint8_t rawData;
	} m_outPort7FFD;
	uint8_t m_outPortFFFD;
	uint8_t m_virtualRegsAY[16];
	uint8_t m_defaultPortVal = 0xFF;
	uint8_t* m_pInPorts;
	void setMachineType(bool is128 = false);
	void setMemPageAddr(uint32_t page, uint8_t* ptr) { m_pRAMPages[page] = ptr - (page << 14); };
	inline uint8_t* memoryAddress(uint32_t address) { return address + m_pRAMPages[address >> 14]; };
	inline uint8_t* screenAddress(uint32_t address) { return address + m_pRAMBanks[5 + ((m_outPort7FFD.screen & !m_outPort7FFD.disabled) << 1)]; };
	void __attribute__((section(".time_critical." "writeMem"))) writeMem(uint16_t address, uint8_t data);
	uint8_t __attribute__((section(".time_critical." "readMem"))) readMem(uint16_t address);
	uint8_t __attribute__((section(".time_critical." "unattachedPort"))) unattachedPort();
	void __attribute__((section(".time_critical." "writePort"))) writePort(uint16_t port, uint8_t data);
	uint8_t __attribute__((section(".time_critical." "readPort"))) readPort(uint16_t port);
// Graphics out
	const uint32_t m_colorLookup[16] = { 0x00000000, 0x17001700, 0x00B800B8, 0x17B817B8, 0xE005E005, 0xF705F705, 0xE0BDE0BD, 0xF7BDF7BD,
										 0x00000000, 0x1F001F00, 0x00F800F8, 0x1FF81FF8, 0xE007E007, 0xFF07FF07, 0xE0FFE0FF, 0xFFFFFFFF };
	const uint32_t m_colorInvertMask[2] = { 0x00, 0xFF };
	const uint32_t m_pixelBitMask[4] = { 0x00000000, 0xFFFF0000, 0x0000FFFF, 0xFFFFFFFF };
	const uint16_t m_pixelsMemOffset[240] = { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
											  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0100, 0x0200, 0x0300, 0x0400, 0x0500, 0x0600, 0x0700,
											  0x0020, 0x0120, 0x0220, 0x0320, 0x0420, 0x0520, 0x0620, 0x0720, 0x0040, 0x0140, 0x0240, 0x0340, 0x0440, 0x0540, 0x0640, 0x0740,
											  0x0060, 0x0160, 0x0260, 0x0360, 0x0460, 0x0560, 0x0660, 0x0760, 0x0080, 0x0180, 0x0280, 0x0380, 0x0480, 0x0580, 0x0680, 0x0780,
											  0x00A0, 0x01A0, 0x02A0, 0x03A0, 0x04A0, 0x05A0, 0x06A0, 0x07A0, 0x00C0, 0x01C0, 0x02C0, 0x03C0, 0x04C0, 0x05C0, 0x06C0, 0x07C0,
											  0x00E0, 0x01E0, 0x02E0, 0x03E0, 0x04E0, 0x05E0, 0x06E0, 0x07E0, 0x0800, 0x0900, 0x0A00, 0x0B00, 0x0C00, 0x0D00, 0x0E00, 0x0F00,
											  0x0820, 0x0920, 0x0A20, 0x0B20, 0x0C20, 0x0D20, 0x0E20, 0x0F20, 0x0840, 0x0940, 0x0A40, 0x0B40, 0x0C40, 0x0D40, 0x0E40, 0x0F40,
											  0x0860, 0x0960, 0x0A60, 0x0B60, 0x0C60, 0x0D60, 0x0E60, 0x0F60, 0x0880, 0x0980, 0x0A80, 0x0B80, 0x0C80, 0x0D80, 0x0E80, 0x0F80,
											  0x08A0, 0x09A0, 0x0AA0, 0x0BA0, 0x0CA0, 0x0DA0, 0x0EA0, 0x0FA0, 0x08C0, 0x09C0, 0x0AC0, 0x0BC0, 0x0CC0, 0x0DC0, 0x0EC0, 0x0FC0,
											  0x08E0, 0x09E0, 0x0AE0, 0x0BE0, 0x0CE0, 0x0DE0, 0x0EE0, 0x0FE0, 0x1000, 0x1100, 0x1200, 0x1300, 0x1400, 0x1500, 0x1600, 0x1700,
											  0x1020, 0x1120, 0x1220, 0x1320, 0x1420, 0x1520, 0x1620, 0x1720, 0x1040, 0x1140, 0x1240, 0x1340, 0x1440, 0x1540, 0x1640, 0x1740,
											  0x1060, 0x1160, 0x1260, 0x1360, 0x1460, 0x1560, 0x1660, 0x1760, 0x1080, 0x1180, 0x1280, 0x1380, 0x1480, 0x1580, 0x1680, 0x1780,
											  0x10A0, 0x11A0, 0x12A0, 0x13A0, 0x14A0, 0x15A0, 0x16A0, 0x17A0, 0x10C0, 0x11C0, 0x12C0, 0x13C0, 0x14C0, 0x15C0, 0x16C0, 0x17C0,
											  0x10E0, 0x11E0, 0x12E0, 0x13E0, 0x14E0, 0x15E0, 0x16E0, 0x17E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
											  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
	};
	const uint16_t m_attributesMemOffset[240] = { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
												  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1800, 0x1800, 0x1800, 0x1800, 0x1800, 0x1800, 0x1800, 0x1800,
												  0x1820, 0x1820, 0x1820, 0x1820, 0x1820, 0x1820, 0x1820, 0x1820, 0x1840, 0x1840, 0x1840, 0x1840, 0x1840, 0x1840, 0x1840, 0x1840,
												  0x1860, 0x1860, 0x1860, 0x1860, 0x1860, 0x1860, 0x1860, 0x1860, 0x1880, 0x1880, 0x1880, 0x1880, 0x1880, 0x1880, 0x1880, 0x1880,
												  0x18A0, 0x18A0, 0x18A0, 0x18A0, 0x18A0, 0x18A0, 0x18A0, 0x18A0, 0x18C0, 0x18C0, 0x18C0, 0x18C0, 0x18C0, 0x18C0, 0x18C0, 0x18C0,
												  0x18E0, 0x18E0, 0x18E0, 0x18E0, 0x18E0, 0x18E0, 0x18E0, 0x18E0, 0x1900, 0x1900, 0x1900, 0x1900, 0x1900, 0x1900, 0x1900, 0x1900,
												  0x1920, 0x1920, 0x1920, 0x1920, 0x1920, 0x1920, 0x1920, 0x1920, 0x1940, 0x1940, 0x1940, 0x1940, 0x1940, 0x1940, 0x1940, 0x1940,
												  0x1960, 0x1960, 0x1960, 0x1960, 0x1960, 0x1960, 0x1960, 0x1960, 0x1980, 0x1980, 0x1980, 0x1980, 0x1980, 0x1980, 0x1980, 0x1980,
												  0x19A0, 0x19A0, 0x19A0, 0x19A0, 0x19A0, 0x19A0, 0x19A0, 0x19A0, 0x19C0, 0x19C0, 0x19C0, 0x19C0, 0x19C0, 0x19C0, 0x19C0, 0x19C0,
												  0x19E0, 0x19E0, 0x19E0, 0x19E0, 0x19E0, 0x19E0, 0x19E0, 0x19E0, 0x1A00, 0x1A00, 0x1A00, 0x1A00, 0x1A00, 0x1A00, 0x1A00, 0x1A00,
												  0x1A20, 0x1A20, 0x1A20, 0x1A20, 0x1A20, 0x1A20, 0x1A20, 0x1A20, 0x1A40, 0x1A40, 0x1A40, 0x1A40, 0x1A40, 0x1A40, 0x1A40, 0x1A40,
												  0x1A60, 0x1A60, 0x1A60, 0x1A60, 0x1A60, 0x1A60, 0x1A60, 0x1A60, 0x1A80, 0x1A80, 0x1A80, 0x1A80, 0x1A80, 0x1A80, 0x1A80, 0x1A80,
												  0x1AA0, 0x1AA0, 0x1AA0, 0x1AA0, 0x1AA0, 0x1AA0, 0x1AA0, 0x1AA0, 0x1AC0, 0x1AC0, 0x1AC0, 0x1AC0, 0x1AC0, 0x1AC0, 0x1AC0, 0x1AC0,
												  0x1AE0, 0x1AE0, 0x1AE0, 0x1AE0, 0x1AE0, 0x1AE0, 0x1AE0, 0x1AE0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
												  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
	};
//	struct BorderColors
	//{
	//	uint16_t x;
	//	uint8_t y;
	//	uint32_t color;
	//} m_borderColors[BORDER_BUFFER_SIZE]; // ring buffer of border colors in visible area (1 color per 8 pixels)
	struct BorderColors
	{
		uint16_t x;
		uint8_t y;
		uint32_t color;
	};
	RingBuffer < BORDER_BUFFER_SIZE, struct BorderColors>  m_borderColors;// ring buffer of border colors in visible area (1 color per 8 pixels)
	uint8_t m_pbWIndex = 0; // wr index of border ring buffer
	uint8_t m_pbRIndex = 0; // rd index of border ring buffer
	uint32_t m_borderColor; // border color out of visible area
	uint8_t m_frameCounter = 0;
	uint32_t* m_pScreenBuffer[2];
	int16_t m_scanLine = -1;
	Display* m_pDisplayInstance;
	void __attribute__((section(".time_critical." "drawLine"))) drawLine(int posY);
// Misc & Diag
	struct ZXSettings
	{
		int tStatesPerLoop;
		int tStatesPerLine;
		int irqLength;
		int contentionStart;
		int contentionEnd;
		int borderStart;
		int borderEnd;
		int audioStatesDivider;
		union 
		{
			struct
			{
				uint8_t machineType : 1; // 0 - Spectrum 48
				uint8_t unused : 7;
			};
			uint8_t rawData;
		} emulSettins;
	} m_emulSettings;
	bool m_debugActvie = false;
	//File m_debugFile;
	uint32_t m_emulationTime = 0, m_maxEmulTime = 0;
public:
// Processor
	void resetZ80();
// Tape
	void startTape(File* pFile, uint16_t sectionSize);
	void stopTape() { m_ZXTape.isTapeActive = false; gpio_put(LED_BUILTIN, 0); };
	bool tapeActive() { return m_ZXTape.isTapeActive; };
// Computer
	void __attribute__((section(".time_critical." "loopZ80"))) loopZ80();
// Graphics out
// Init & deinit
	ZXSpectrum() { };
	~ZXSpectrum() { };
	void init(Display* pDisplayInstance, Keyboard* pKeyboardInstance);
// Misc & Diag
	uint32_t getEmulationTime() { return m_emulationTime; };
	uint32_t getMaxEmulationTime() { return m_maxEmulTime; };
	void setMachine(uint8_t type) { m_emulSettings.emulSettins.machineType = type & 1; };
	void storeState(const char* pFileName);
	void restoreState(const char* pFileName);
	bool toggleDebug();// { m_debugActvie = !m_debugActvie; return m_debugActvie; }
};