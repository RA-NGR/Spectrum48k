#include "ZXSpectrum.h"

// Processor
void ZXSpectrum::intZ80()
{
	if (IFF1 && !m_Z80Processor.skipINT)
	{
		if (m_Z80Processor.iff2_read) FL &= ~FLAG_P;
		if (m_Z80Processor.halted) m_Z80Processor.halted = 0;
		IFF1 = IFF2 = 0;
		R++;
		m_Z80Processor.tCount += 7; /* Longer than usual M1 cycle */
		writeMem(--SP, PCH); writeMem(--SP, PCL);
		switch (IM)
		{
		case 0:
			PC = 0x0038;
			break;
		case 1:
			PC = 0x0038;
			break;
		case 2:
		{
			uint16_t inttemp = (0x100 * I) + 0xFF;
			PCL = readMem(inttemp++); PCH = readMem(inttemp);
			break;
		}
		default:
			break;
		}
		m_Z80Processor.memptr.w = PC;
		Q = 0;
	}
}

void ZXSpectrum::stepZ80()
{
	void** pCurrentRegisters;
	void** pCurrentPairs;
	uint8_t opcode = fetchOpcode(PC);
	m_Z80Processor.iff2_read = 0;
	PC++; R++;
	uint8_t last_Q = Q; /* keep Q value from previous opcode for SCF and CCF */
	Q = 0; /* preempt Q value assuming next opcode doesn't set flags */
	uint8_t instruction = instructionTable[opcode];
	if (m_Z80Processor.halted)
	{
		instruction = NOP; PC--;
	}
	switch (m_Z80Processor.skipINT)
	{
	case DD_PREFIX:
	{
		pCurrentRegisters = m_Z80Processor.pDDRegisters;
		pCurrentPairs = m_Z80Processor.pDDPairs;
		break;
	}
	case FD_PREFIX:
	{
		pCurrentRegisters = m_Z80Processor.pFDRegisters;
		pCurrentPairs = m_Z80Processor.pFDPairs;
		break;
	}
	default:
	{
		pCurrentRegisters = m_Z80Processor.pRegisters;
		pCurrentPairs = m_Z80Processor.pPairs;
		break;
	}
	}
	bool repeatLoop;
	do
	{
		m_Z80Processor.skipINT = 0;
		repeatLoop = false;
		switch (instruction)
		{
		case ADD_HL_RR:						/*+*/
		{
			addressOnBus(IR, 7);
			ADD16((*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX])), (*(uint16_t*)(pCurrentPairs[dd(opcode)])));
			break;
		}
		case DEC_8:				/*!*/
		{
			if (r(opcode) == 0x06)
			{
				uint8_t bytetemp;
				if (pCurrentRegisters != m_Z80Processor.pRegisters)
				{
					uint8_t offset = readMem(PC);
					addressOnBus(PC, 5);
					PC++;
					m_Z80Processor.memptr.w = (*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX])) + (int8_t)offset;
					bytetemp = readMem(m_Z80Processor.memptr.w);
					addressOnBus(m_Z80Processor.memptr.w, 1);
					DEC8(bytetemp);
					writeMem(m_Z80Processor.memptr.w, bytetemp);
				}
				else
				{
					bytetemp = readMem(HL);
					addressOnBus(HL, 1);
					DEC8(bytetemp);
					writeMem(HL, bytetemp);
				}
			}
			else
				DEC8((*(uint8_t*)(pCurrentRegisters[r(opcode)])));
			break;
		}
		case DEC_RR:						/*+*/
		{
			addressOnBus(IR, 2);
			(*(uint16_t*)(pCurrentPairs[dd(opcode)]))--;
			break;
		}
		case EX_INDIRECT_SP_HL:				/*!*/
		{
			uint8_t bytetempl = readMem(SP);
			uint8_t bytetemph = readMem(SP + 1);
			addressOnBus(SP + 1, 1);
			writeMem(SP + 1, (*(uint8_t*)(pCurrentRegisters[H_INDEX])));
			writeMem(SP, (*(uint8_t*)(pCurrentRegisters[L_INDEX])));
			addressOnBus(SP, 2);
			(*(uint8_t*)(pCurrentRegisters[L_INDEX])) = m_Z80Processor.memptr.b.l = bytetempl;
			(*(uint8_t*)(pCurrentRegisters[H_INDEX])) = m_Z80Processor.memptr.b.h = bytetemph;
			break;
		}
		case INC_8:							/*!*/
		{
			if (r(opcode) == 0x06)
			{
				uint8_t bytetemp;
				if (pCurrentRegisters != m_Z80Processor.pRegisters)
				{
					uint8_t offset = readMem(PC);
					addressOnBus(PC, 5);
					PC++;
					m_Z80Processor.memptr.w = (*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX])) + (int8_t)offset;
					bytetemp = readMem(m_Z80Processor.memptr.w);
					addressOnBus(m_Z80Processor.memptr.w, 1);
					INC8(bytetemp);
					writeMem(m_Z80Processor.memptr.w, bytetemp);
				}
				else
				{
					bytetemp = readMem(HL);
					addressOnBus(HL, 1);
					INC8(bytetemp);
					writeMem(HL, bytetemp);
				}
			}
			else
				INC8((*(uint8_t*)(pCurrentRegisters[r(opcode)])));
			break;
		}
		case INC_RR:						/*+*/
		{
			addressOnBus(IR, 2);
			(*(uint16_t*)(pCurrentPairs[dd(opcode)]))++;
			break;
		}
		case JP_HL:							/*!*/
		{
			PC = (*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX]));
			break;
		}
		case LD_HL_INDIRECT_NN:				/*!*/
		{
			LD16_RRNN((*(uint8_t*)(pCurrentRegisters[L_INDEX])), (*(uint8_t*)(pCurrentRegisters[H_INDEX])));
			break;
		}
		case LD_INDIRECT_HL_N:				/*!*/
		{
			if (pCurrentRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC++);
				uint8_t value = readMem(PC);
				addressOnBus(PC, 2);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX])) + (int8_t)offset;
				writeMem(m_Z80Processor.memptr.w, value);
			}
			else
				writeMem(HL, readMem(PC++));
			break;
		}
		case LD_INDIRECT_NN_HL:				/*!*/
		{
			LD16_NNRR((*(uint8_t*)(pCurrentRegisters[L_INDEX])), (*(uint8_t*)(pCurrentRegisters[H_INDEX])));
			break;
		}
		case LD_INDIRECT_HL_R:				/*!*/
		{
			if (pCurrentRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				addressOnBus(PC, 5);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX])) + (int8_t)offset;
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				writeMem((*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX])), (*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
			break;
		}
		case LD_R_N:						/*!*/
		{
			(*(uint8_t*)(pCurrentRegisters[r(opcode)])) = readMem(PC++);
			break;
		}
		case LD_R_8:				/*!*/
		{
			if (r_(opcode) == 0x06)
			{
				if (pCurrentRegisters != m_Z80Processor.pRegisters)
				{
					uint8_t offset = readMem(PC);
					addressOnBus(PC, 5);
					PC++;
					m_Z80Processor.memptr.w = (*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX])) + (int8_t)offset;
					(*(uint8_t*)(m_Z80Processor.pRegisters[r(opcode)])) = readMem(m_Z80Processor.memptr.w);
				}
				else
					(*(uint8_t*)(pCurrentRegisters[r(opcode)])) = readMem((*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX])));
			}
			else
				(*(uint8_t*)(pCurrentRegisters[r(opcode)])) = (*(uint8_t*)(pCurrentRegisters[r_(opcode)]));
			break;
		}
		case LD_RR_NN:						/*!*/
		{
			(*(uint8_t*)(pCurrentPairs[dd(opcode)])) = readMem(PC++);
			(*((uint8_t*)(pCurrentPairs[dd(opcode)]) + 1)) = readMem(PC++);
			break;
		}
		case LD_SP_HL:						/*!*/
		{
			addressOnBus(IR, 2);
			SP = (*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX]));
			break;
		}
		case POP_SS:						/*!*/
		{
			uint8_t index = dd(opcode);
			if (index == 3) index++;
			POP16((*(uint8_t*)(pCurrentPairs[index])), (*((uint8_t*)(pCurrentPairs[index]) + 1)));
			break;
		}
		case PUSH_SS:						/*!*/
		{
			uint8_t index = dd(opcode);
			if (index == 3) index++;
			uint16_t tempword = (*(uint16_t*)(pCurrentPairs[index]));
			addressOnBus(IR, 1);
			PUSH16(tempword & 0xFF, tempword >> 8);
			break;
		}
		case AL_R:
		{
			if (r_(opcode) == 0x06)
			{
				uint8_t bytetemp;
				if (pCurrentRegisters != m_Z80Processor.pRegisters)
				{
					uint8_t offset = readMem(PC);
					addressOnBus(PC, 5);
					PC++;
					m_Z80Processor.memptr.w = (*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX])) + (int8_t)offset;
					bytetemp = readMem(m_Z80Processor.memptr.w);
				}
				else
					bytetemp = readMem(HL);
				switch (ALRS_OPERATION_DECODE(opcode))
				{
				case 0:
				{
					ADD(bytetemp);
					break;
				}
				case 1:
				{
					ADC(bytetemp);
					break;
				}
				case 2:
				{
					SUB(bytetemp);
					break;
				}
				case 3:
				{
					SBC(bytetemp);
					break;
				}
				case 4:
				{
					AND(bytetemp);
					break;
				}
				case 5:
				{
					XOR(bytetemp);
					break;
				}
				case 6:
				{
					OR(bytetemp);
					break;
				}
				case 7:
				{
					CP(bytetemp);
					break;
				}
				default:
					break;
				}
			}
			else
				switch (ALRS_OPERATION_DECODE(opcode))
				{
				case 0:
				{
					ADD((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
					break;
				}
				case 1:
				{
					ADC((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
					break;
				}
				case 2:
				{
					SUB((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
					break;
				}
				case 3:
				{
					SBC((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
					break;
				}
				case 4:
				{
					AND((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
					break;
				}
				case 5:
				{
					XOR((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
					break;
				}
				case 6:
				{
					OR((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
					break;
				}
				case 7:
				{
					CP((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
					break;
				}
				default:
					break;
				}
			break;

		}
		case LD_A_INDIRECT:
		{
			if (dd(opcode) == 0x03)
			{
				m_Z80Processor.memptr.b.l = readMem(PC++);
				m_Z80Processor.memptr.b.h = readMem(PC++);
				A = readMem(m_Z80Processor.memptr.w++);
			}
			else
			{
				m_Z80Processor.memptr.w = (*(uint16_t*)(pCurrentPairs[dd(opcode)])) + 1;
				A = readMem(*(uint16_t*)(pCurrentPairs[dd(opcode)]));
			}
			break;
		}
		case LD_INDIRECT_A:
		{
			if (dd(opcode) == 0x03)
			{
				uint16_t wordtemp = readMem(PC++);
				wordtemp |= readMem(PC++) << 8;
				m_Z80Processor.memptr.b.l = wordtemp + 1;
				m_Z80Processor.memptr.b.h = A;
				writeMem(wordtemp, A);
			}
			else
			{
				m_Z80Processor.memptr.b.l = (*(uint16_t*)(pCurrentPairs[dd(opcode)])) + 1;
				m_Z80Processor.memptr.b.h = A;
				writeMem(*(uint16_t*)(pCurrentPairs[dd(opcode)]), A);
			}
			break;
		}
		case LD_A_I_LD_A_R:
		{
			addressOnBus(IR, 1);
			if (opcode == 0x57)
				A = I;
			else
				A = (R & 0x7f) | (R7 & 0x80);
			FL = (FL & FLAG_C) | sz53Table[A] | (IFF2 << 2);
			m_Z80Processor.iff2_read = 1;
			Q = FL;
			break;
		}
		case LD_I_A_LD_R_A:
		{
			addressOnBus(IR, 1);
			if (opcode == 0x47)
				I = A;
			else
				R = R7 = A;
			break;
		}
		case LD_RR_INDIRECT_NN:				/*!*/
		{
			LD16_RRNN((*(uint8_t*)(pCurrentPairs[dd(opcode)])), (*((uint8_t*)(pCurrentPairs[dd(opcode)]) + 1)));
			break;
		}
		case LD_INDIRECT_NN_RR:				/*!*/
		{
			LD16_NNRR((*(uint8_t*)(pCurrentPairs[dd(opcode)])), (*((uint8_t*)(pCurrentPairs[dd(opcode)]) + 1)));
			break;
		}
		case EX_DE_HL:
		{
			uint16_t wordtemp = DE; DE = HL; HL = wordtemp;
			break;
		}
		case EX_AF_AF_PRIME:
		{
			/* Tape saving trap: note this traps the EX AF,AF' at #04d0, not
		   #04d1 as PC has already been incremented
		   0x76 - Timex 2068 save routine in EXROM
				   if (PC == 0x04d1 || PC == 0x0077) {
					   if (tape_save_trap() == 0) break;*/
			uint16_t wordtemp = AF;
			AF = AF_;
			AF_ = wordtemp;
			break;
		}
		case EXX:
		{
			uint16_t wordtemp = BC; BC = BC_; BC_ = wordtemp;
			wordtemp = DE; DE = DE_; DE_ = wordtemp;
			wordtemp = HL; HL = HL_; HL_ = wordtemp;
			break;
		}
		case LDI_LDD:
		{
			uint8_t bytetemp = readMem(HL);
			BC--;
			writeMem(DE, bytetemp);
			addressOnBus(DE, 2);
			if (opcode == 0xA0)
			{
				DE++; HL++;
			}
			else
			{
				DE--; HL--;
			}
			bytetemp += A;
			FL = (FL & (FLAG_C | FLAG_Z | FLAG_S)) | (BC ? FLAG_V : 0) | (bytetemp & FLAG_3) | ((bytetemp & 0x02) ? FLAG_5 : 0);
			Q = FL;
			break;
		}
		case LDIR_LDDR: /* Test passed */
		{
			uint8_t bytetemp = readMem(HL);
			writeMem(DE, bytetemp);
			addressOnBus(DE, 2);
			BC--;
			bytetemp += A;
			FL = (FL & (FLAG_C | FLAG_Z | FLAG_S)) | (BC ? FLAG_V : 0);
			if (BC)
			{
				FL |= (PCH & (FLAG_3 | FLAG_5));
				addressOnBus(DE, 5);
				PC -= 2;
				m_Z80Processor.memptr.w = PC + 1;
			}
			else
				FL |= ((bytetemp & FLAG_3) | ((bytetemp & 0x02) ? FLAG_5 : 0));
			Q = FL;
			if (opcode == 0xB0)
			{
				HL++; DE++;
			}
			else
			{
				HL--; DE--;
			}
			break;
		}
		case CPI_CPD:
		{
			uint8_t value = readMem(HL), bytetemp = A - value, lookup = ((A & 0x08) >> 3) | (((value) & 0x08) >> 2) | ((bytetemp & 0x08) >> 1);
			addressOnBus(HL, 5);
			if (opcode == 0xA1)
			{
				HL++;
				m_Z80Processor.memptr.w++;
			}
			else
			{
				HL--;
				m_Z80Processor.memptr.w--;
			}
			BC--;
			FL = (FL & FLAG_C) | (BC ? (FLAG_V | FLAG_N) : FLAG_N) | halfcarrySubTable[lookup] | (bytetemp ? 0 : FLAG_Z) | (bytetemp & FLAG_S);
			if (FL & FLAG_H) bytetemp--;
			FL |= (bytetemp & FLAG_3) | ((bytetemp & 0x02) ? FLAG_5 : 0);
			Q = FL;
			break;
		}
		case CPIR_CPDR: /* Test passed*/
		{
			uint8_t value = readMem(HL), bytetemp = A - value, lookup = ((A & 0x08) >> 3) | (((value) & 0x08) >> 2) | ((bytetemp & 0x08) >> 1);
			addressOnBus(HL, 5);
			BC--;
			FL = (FL & FLAG_C) | (BC ? (FLAG_V | FLAG_N) : FLAG_N) | halfcarrySubTable[lookup] | (bytetemp ? 0 : FLAG_Z) | (bytetemp & FLAG_S);
			if (FL & FLAG_H) bytetemp--;
			if (BC && bytetemp)
				FL |= (PCH & (FLAG_3 | FLAG_5));
			else
				FL |= ((bytetemp & FLAG_3) | ((bytetemp & 0x02) ? FLAG_5 : 0));
			Q = FL;
			if ((FL & (FLAG_V | FLAG_Z)) == FLAG_V)
			{
				addressOnBus(HL, 5);
				PC -= 2;
				m_Z80Processor.memptr.w = PC + 1;
			}
			else
			{
				if (opcode == 0xB1)
					m_Z80Processor.memptr.w++;
				else
					m_Z80Processor.memptr.w--;
			}
			if (opcode == 0xB1)
				HL++;
			else
				HL--;
			break;
		}
		case AL_N:
		{
			uint8_t bytetemp = readMem(PC++);
			switch (ALRS_OPERATION_DECODE(opcode))
			{
			case 0:
			{
				ADD(bytetemp);
				break;
			}
			case 1:
			{
				ADC(bytetemp);
				break;
			}
			case 2:
			{
				SUB(bytetemp);
				break;
			}
			case 3:
			{
				SBC(bytetemp);
				break;
			}
			case 4:
			{
				AND(bytetemp);
				break;
			}
			case 5:
			{
				XOR(bytetemp);
				break;
			}
			case 6:
			{
				OR(bytetemp);
				break;
			}
			case 7:
			{
				CP(bytetemp);
				break;
			}
			default:
				break;
			}
			break;
		}
		case ADC_HL_RR:
		{
			addressOnBus(IR, 7);
			ADC16((*(uint16_t*)(pCurrentPairs[dd(opcode)])));
			break;
			break;
		}
		case SBC_HL_RR:
		{
			addressOnBus(IR, 7);
			SBC16((*(uint16_t*)(pCurrentPairs[dd(opcode)])));
			break;
		}
		case DAA:
		{
			uint8_t add = 0, carry = (FL & FLAG_C);
			if ((FL & FLAG_H) || ((A & 0x0f) > 9)) add = 6;
			if (carry || (A > 0x99)) add |= 0x60;
			if (A > 0x99) carry = FLAG_C;
			if (FL & FLAG_N)
			{
				SUB(add);
			}
			else
			{
				ADD(add);
			}
			FL = (FL & ~(FLAG_C | FLAG_P)) | carry | parityTable[A];
			Q = FL;
			break;
		}
		case CPL:
		{
			A ^= 0xff;
			FL = (FL & (FLAG_C | FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | (FLAG_N | FLAG_H);
			Q = FL;
			break;
		}
		case NEG:
		{
			uint8_t bytetemp = A;
			A = 0;
			SUB(bytetemp);
			break;
		}
		case CCF:
		{
			FL = (FL & (FLAG_P | FLAG_Z | FLAG_S)) | ((FL & FLAG_C) ? FLAG_H : FLAG_C) | (((last_Q ^ FL) | A) & (FLAG_3 | FLAG_5));
			Q = FL;
			break;
		}
		case SCF:
		{
			FL = (FL & (FLAG_P | FLAG_Z | FLAG_S)) | (((last_Q ^ FL) | A) & (FLAG_3 | FLAG_5)) | FLAG_C;
			Q = FL;
			break;
		}
		case HALT:
		{
			m_Z80Processor.halted = 1;
			break;
		}
		case DI:
		{
			IFF1 = IFF2 = 0;
			break;
		}
		case EI:
		{
			IFF1 = IFF2 = 1;
			m_Z80Processor.skipINT = EI;
			break;
		}
		case IM_N:
		{
			if (q(opcode) < 0x02)
				IM = 0;
			else
				IM = q(opcode) - 1;
			break;
		}
		case RLCA:
		{
			A = (A << 1) | (A >> 7);
			FL = (FL & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_C | FLAG_3 | FLAG_5));
			Q = FL;
			break;
		}
		case RLA:
		{
			uint8_t bytetemp = A;
			A = (A << 1) | (FL & FLAG_C);
			FL = (FL & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | (bytetemp >> 7);
			Q = FL;
			break;
		}
		case RRCA:
		{
			FL = (FL & (FLAG_P | FLAG_Z | FLAG_S)) | (A & FLAG_C);
			A = (A >> 1) | (A << 7);
			FL |= (A & (FLAG_3 | FLAG_5));
			Q = FL;
			break;
		}
		case RRA:
		{
			uint8_t bytetemp = A;
			A = (A >> 1) | (FL << 7);
			FL = (FL & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | (bytetemp & FLAG_C);
			Q = FL;
			break;
		}
		case RS_R:							/*!!*/
		{
			uint8_t bytetemp;
			if (pCurrentRegisters != m_Z80Processor.pRegisters)
			{
				if (r_(opcode) == 0x06)
				{
					bytetemp = readMem(m_Z80Processor.memptr.w);
					addressOnBus(m_Z80Processor.memptr.w, 1);
					switch (ALRS_OPERATION_DECODE(opcode))
					{
					case 0:
					{
						RLC(bytetemp);
						break;
					}
					case 1:
					{
						RRC(bytetemp);
						break;
					}
					case 2:
					{
						RL(bytetemp);
						break;
					}
					case 3:
					{
						RR(bytetemp);
						break;
					}
					case 4:
					{
						SLA(bytetemp);
						break;
					}
					case 5:
					{
						SRA(bytetemp);
						break;
					}
					case 6:
					{
						SLL(bytetemp);
						break;
					}
					case 7:
					{
						SRL(bytetemp);
						break;
					}
					default:
						break;
					}
					writeMem(m_Z80Processor.memptr.w, bytetemp);
				}
				else
				{
					(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w);
					addressOnBus(m_Z80Processor.memptr.w, 1);
					switch (ALRS_OPERATION_DECODE(opcode))
					{
					case 0:
					{
						RLC((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
						break;
					}
					case 1:
					{
						RRC((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
						break;
					}
					case 2:
					{
						RL((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
						break;
					}
					case 3:
					{
						RR((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
						break;
					}
					case 4:
					{
						SLA((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
						break;
					}
					case 5:
					{
						SRA((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
						break;
					}
					case 6:
					{
						SLL((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
						break;
					}
					case 7:
					{
						SRL((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
						break;
					}
					default:
						break;
					}
					writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
				}
			}
			else
			{
				if (r_(opcode) == 0x06)
				{
					bytetemp = readMem(HL);
					addressOnBus(HL, 1);
					switch (ALRS_OPERATION_DECODE(opcode))
					{
					case 0:
					{
						RLC(bytetemp);
						break;
					}
					case 1:
					{
						RRC(bytetemp);
						break;
					}
					case 2:
					{
						RL(bytetemp);
						break;
					}
					case 3:
					{
						RR(bytetemp);
						break;
					}
					case 4:
					{
						SLA(bytetemp);
						break;
					}
					case 5:
					{
						SRA(bytetemp);
						break;
					}
					case 6:
					{
						SLL(bytetemp);
						break;
					}
					case 7:
					{
						SRL(bytetemp);
						break;
					}
					default:
						break;
					}
					writeMem(HL, bytetemp);
				}
				else
					switch (ALRS_OPERATION_DECODE(opcode))
					{
					case 0:
					{
						RLC((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
						break;
					}
					case 1:
					{
						RRC((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
						break;
					}
					case 2:
					{
						RL((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
						break;
					}
					case 3:
					{
						RR((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
						break;
					}
					case 4:
					{
						SLA((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
						break;
					}
					case 5:
					{
						SRA((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
						break;
					}
					case 6:
					{
						SLL((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
						break;
					}
					case 7:
					{
						SRL((*(uint8_t*)(pCurrentRegisters[r_(opcode)])));
						break;
					}
					default:
						break;
					}
			}
			break;
		}
		case RLD_RRD:
		{
			uint8_t bytetemp = readMem(HL);
			addressOnBus(HL, 4);
			if (opcode == 0x6F)
			{
				writeMem(HL, (bytetemp << 4) | (A & 0x0F));
				A = (A & 0xF0) | (bytetemp >> 4);
			}
			else
			{
				writeMem(HL, (A << 4) | (bytetemp >> 4));
				A = (A & 0xF0) | (bytetemp & 0x0F);
			}
			FL = (FL & FLAG_C) | sz53pTable[A];
			Q = FL;
			m_Z80Processor.memptr.w = HL + 1;
			break;
		}
		case BIT_B_R:						/*!!*/
		{
			uint8_t bytetemp;
			if (pCurrentRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w);
				addressOnBus(m_Z80Processor.memptr.w, 1);
				BIT_MEMPTR(r(opcode), bytetemp);
			}
			else
			{
				if (r_(opcode) == 0x06)
				{
					bytetemp = readMem(HL);
					addressOnBus(HL, 1);
					BIT_MEMPTR(r(opcode), bytetemp);
				}
				else
				{
					FL = (FL & FLAG_C) | FLAG_H | ((*(uint8_t*)(pCurrentRegisters[r_(opcode)])) & (FLAG_3 | FLAG_5)); 
					if (!((*(uint8_t*)(pCurrentRegisters[r_(opcode)])) & (0x01 << (r(opcode))))) FL |= FLAG_P | FLAG_Z;
					if ((r(opcode)) == 7 && ((*(uint8_t*)(pCurrentRegisters[r_(opcode)]))) & 0x80) FL |= FLAG_S; Q = FL;
				}
			}
			break;
		}
		case SR_B_R:						/*!!*/
		{
			uint8_t bitMask = 1 << r(opcode), bytetemp;
			if (pCurrentRegisters != m_Z80Processor.pRegisters)
			{
				if (r_(opcode) == 0x06)
				{
					bytetemp = readMem(m_Z80Processor.memptr.w);
					bytetemp = (opcode & 0x40 ? bytetemp | bitMask : bytetemp & (~bitMask));
					addressOnBus(m_Z80Processor.memptr.w, 1);
					writeMem(m_Z80Processor.memptr.w, bytetemp);
				}
				else
				{
					(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w);
					(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = (opcode & 0x40 ? (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) | bitMask :
						(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) & (~bitMask));
					addressOnBus(m_Z80Processor.memptr.w, 1);
					writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
				}
			}
			else
			{
				if (r_(opcode) == 0x06)
				{
					bytetemp = readMem(HL);
					bytetemp = (opcode & 0x40 ? bytetemp | bitMask : bytetemp & (~bitMask));
					addressOnBus(HL, 1);
					writeMem(HL, bytetemp);
				}
				else
					(*(uint8_t*)(pCurrentRegisters[r_(opcode)])) = (opcode & 0x40 ? (*(uint8_t*)(pCurrentRegisters[r_(opcode)])) | bitMask : (*(uint8_t*)(pCurrentRegisters[r_(opcode)])) & (~bitMask));;
			}
			break;
		}
		case JP_NN:
		{
			m_Z80Processor.memptr.b.l = readMem(PC++);
			m_Z80Processor.memptr.b.h = readMem(PC);
			if (opcode == 0xC3 || CC(r(opcode)))
				PC = m_Z80Processor.memptr.w;
			else
				PC++;
			break;
		}
		case JR_E:
		{
			int8_t jrtemp = readMem(PC);
			if (opcode == 0x18 || CC(q(opcode)))
			{
				PC += jrtemp; addressOnBus(PC, 5); PC++; m_Z80Processor.memptr.w = PC;
			}
			else
			{
				PC++;
			}
			break;
		}
		case DJNZ_E:
		{
			addressOnBus(IR, 1);
			int8_t jrtemp = readMem(PC);
			if (--B)
			{
				PC += jrtemp; addressOnBus(PC, 5); PC++; m_Z80Processor.memptr.w = PC;
			}
			else
			{
				PC++;
			}
			break;
		}
		case CALL_NN:
		{
			m_Z80Processor.memptr.b.l = readMem(PC++);
			m_Z80Processor.memptr.b.h = readMem(PC);
			if (opcode == 0xCD || CC(r(opcode)))
			{
				PC++; addressOnBus(PC, 1); PUSH16(PCL, PCH); PC = m_Z80Processor.memptr.w;
			}
			else
			{
				PC++;
			}
			break;
		}
		case RET:
		{
			if (opcode != 0xC9) addressOnBus(IR, 1);
			if (opcode == 0xC9 || CC(r(opcode)))
			{
				POP16(PCL, PCH); m_Z80Processor.memptr.w = PC;
			}
			break;
		}
		case RETI_RETN:
		{
			IFF1 = IFF2;
			POP16(PCL, PCH); m_Z80Processor.memptr.w = PC;
			break;
		}
		case RST_P:
		{
			addressOnBus(IR, 1);
			RST(rstTable[r(opcode)]);
			break;
		}
		case IN_A_N:
		{
			uint16_t intemp = readMem(PC++) + (A << 8);
			A = readPort(intemp);
			m_Z80Processor.memptr.w = intemp + 1;
			break;
		}
		case IN_R_C:
		{
			if (r(opcode) != 0b110)
			{
				INP((*(uint8_t*)(pCurrentRegisters[r(opcode)])), BC);
			}
			else
			{
				uint8_t bytetemp;
				INP(bytetemp, BC);
			}
			break;
		}
		case INI_IND:
		{
			addressOnBus(IR, 1);
			uint8_t initemp = readPort(BC), initemp2;
			writeMem(HL, initemp);
			if (opcode == 0xA2)
			{
				m_Z80Processor.memptr.w = BC + 1;
				HL++;
				initemp2 = initemp + C + 1;
			}
			else
			{
				m_Z80Processor.memptr.w = BC - 1;
				HL--;
				initemp2 = initemp + C - 1;
			}
			B--;
			FL = (initemp & 0x80 ? FLAG_N : 0) | ((initemp2 < initemp) ? FLAG_H | FLAG_C : 0) | (parityTable[(initemp2 & 0x07) ^ B] ? FLAG_P : 0) | sz53Table[B];
			Q = FL;
			break;
		}
		case INIR_INDR:
		{
			addressOnBus(IR, 1);
			uint8_t initemp = readPort(BC), initemp2;
			writeMem(HL, initemp);
			if (opcode == 0xB2)
			{
				m_Z80Processor.memptr.w = BC + 1;
				initemp2 = initemp + C + 1;
			}
			else
			{
				m_Z80Processor.memptr.w = BC - 1;
				initemp2 = initemp + C - 1;
			}
			B--;
			FL = (initemp & 0x80 ? FLAG_N : 0);
			if (B)
			{
				addressOnBus(HL, 5); 
				PC -= 2;
				FL |= ((B & FLAG_S) | (PCH & (FLAG_3 | FLAG_5)) | (initemp2 < initemp ? FLAG_C | (initemp & 0x80 ? ((B & 0x0F) == 0 ? FLAG_H : 0) | parityTable[((initemp2 & 0x07) ^ B) ^
					((B - 1) & 7)] : ((B & 0xF) == 0xF ? FLAG_H : 0) | parityTable[((initemp2 & 0x07) ^ B) ^ ((B + 1) & 7)]) : parityTable[((initemp2 & 0x07) ^ B) ^ (B & 7)]));
				m_Z80Processor.memptr.w = PC + 1;
			}
			else
				FL |= (FLAG_Z | (initemp2 < initemp ? (FLAG_H | FLAG_C) : 0) | parityTable[(initemp2 & 0x07) ^ B]);
			Q = FL;
			if (opcode == 0xB2)
				HL++;
			else
				HL--;
			break;
		}
		case OUT_N_A:
		{
			uint8_t nn = readMem(PC++);
			uint16_t outtemp = nn | (A << 8);
			m_Z80Processor.memptr.b.h = A;
			m_Z80Processor.memptr.b.l = (nn + 1);
			writePort(outtemp, A);
			break;
		}
		case OUT_C_R:
		{
			if (r(opcode) != 0b110)
			{
				writePort(BC, (*(uint8_t*)(pCurrentRegisters[r(opcode)])));
			}
			else
				writePort(BC, 0);
			m_Z80Processor.memptr.w = BC + 1;
			break;
		}
		case OUTI_OUTD:
		{
			addressOnBus(IR, 1);
			uint8_t outitemp = readMem(HL);
			B--; /* This does happen first, despite what the specs say */
			if (opcode == 0xA3)
			{
				m_Z80Processor.memptr.w = BC + 1;
				HL++;
			}
			else
			{
				m_Z80Processor.memptr.w = BC - 1;
				HL--;
			}
			writePort(BC, outitemp);
			uint8_t outitemp2 = outitemp + L;
			FL = (outitemp & 0x80 ? FLAG_N : 0) | ((outitemp2 < outitemp) ? FLAG_H | FLAG_C : 0) | (parityTable[(outitemp2 & 0x07) ^ B] ? FLAG_P : 0) | sz53Table[B];
			Q = FL;
			break;
		}
		case OTIR_OTDR:
		{
			addressOnBus(IR, 1);
			uint8_t outitemp = readMem(HL), outitemp2;
			B--; /* This does happen first, despite what the specs say */
			if (opcode == 0xB3)
				m_Z80Processor.memptr.w = BC + 1;
			else
				m_Z80Processor.memptr.w = BC - 1;
			writePort(BC, outitemp);
			if (opcode == 0xB3)
				HL++;
			else
				HL--;
			outitemp2 = outitemp + L;
			FL = (outitemp & 0x80 ? FLAG_N : 0);
			if (B)
			{
				addressOnBus(BC, 5); 
				PC -= 2;
				FL |= ((B & FLAG_S) | (PCH & (FLAG_3 | FLAG_5)) | (outitemp2 < outitemp ? FLAG_C | (outitemp & 0x80 ? ((B & 0x0F) == 0 ? FLAG_H : 0) | parityTable[((outitemp2 & 0x07) ^ B) ^
					((B - 1) & 7)] : ((B & 0xF) == 0xF ? FLAG_H : 0) | parityTable[((outitemp2 & 0x07) ^ B) ^ ((B + 1) & 7)]) : parityTable[((outitemp2 & 0x07) ^ B) ^ (B & 7)]));
				m_Z80Processor.memptr.w = PC + 1;
			}
			else
				FL |= (FLAG_Z | (outitemp2 < outitemp ? (FLAG_H | FLAG_C) : 0) | parityTable[(outitemp2 & 0x07) ^ B]);
			Q = FL;
			break;
		}
		case CB_PREFIX:
		{
			if (pCurrentRegisters != m_Z80Processor.pRegisters)
			{
				m_Z80Processor.memptr.w = (*(uint16_t*)(pCurrentPairs[HL_IX_IY_INDEX])) + (int8_t)(readMem(PC));
				PC++;
				opcode = readMem(PC);
				addressOnBus(PC, 2);
			}
			else
			{
				opcode = fetchOpcode(PC);
				R++;
			}
			instruction = cbInstructionTable[opcode];
			PC++;
			repeatLoop = true;
			break;
		}
		case DD_PREFIX:
		{
			m_Z80Processor.skipINT = DD_PREFIX;
			break;
		}
		case ED_PREFIX:
		{
			opcode = fetchOpcode(PC);
			instruction = edInstructionTable[opcode];
			PC++;
			R++;
			repeatLoop = true;
			break;
		}
		case FD_PREFIX:
		{
			m_Z80Processor.skipINT = FD_PREFIX;
			break;
		}
		case NOP:
		case ED_UNDEFINED:
		default:
			break;
		}
	} while (repeatLoop);
}

void ZXSpectrum::resetZ80()
{
	stopTape();
	m_outPortFE.rawData = m_outPort7FFD.rawData = 0;
	setMachineType(m_emulSettings.emulSettins.machineType);
	m_Z80Processor = { 0 };
	m_Z80Processor.pRegisters[0] = m_Z80Processor.pDDRegisters[0] = m_Z80Processor.pFDRegisters[0] = &B;
	m_Z80Processor.pRegisters[1] = m_Z80Processor.pDDRegisters[1] = m_Z80Processor.pFDRegisters[1] = &C;
	m_Z80Processor.pRegisters[2] = m_Z80Processor.pDDRegisters[2] = m_Z80Processor.pFDRegisters[2] = &D;
	m_Z80Processor.pRegisters[3] = m_Z80Processor.pDDRegisters[3] = m_Z80Processor.pFDRegisters[3] = &E;
	m_Z80Processor.pRegisters[4] = &H; m_Z80Processor.pDDRegisters[4] = &IXH; m_Z80Processor.pFDRegisters[4] = &IYH;
	m_Z80Processor.pRegisters[5] = &L; m_Z80Processor.pDDRegisters[5] = &IXL; m_Z80Processor.pFDRegisters[5] = &IYL;
	m_Z80Processor.pRegisters[7] = m_Z80Processor.pDDRegisters[7] = m_Z80Processor.pFDRegisters[7] = &A;
	m_Z80Processor.pPairs[0] = m_Z80Processor.pDDPairs[0] = m_Z80Processor.pFDPairs[0] = &BC;
	m_Z80Processor.pPairs[1] = m_Z80Processor.pDDPairs[1] = m_Z80Processor.pFDPairs[1] = &DE;
	m_Z80Processor.pPairs[2] = &HL; m_Z80Processor.pDDPairs[2] = &IX; m_Z80Processor.pFDPairs[2] = &IY;
	m_Z80Processor.pPairs[3] = m_Z80Processor.pDDPairs[3] = m_Z80Processor.pFDPairs[3] = &SP;
	m_Z80Processor.pPairs[4] = m_Z80Processor.pDDPairs[4] = m_Z80Processor.pFDPairs[4] = &AF;
	m_defaultPortVal = 0xFF;
	m_borderColor = m_colorLookup[7];
	AF = AF_ = 0xffff;
	SP = 0xffff;
	m_maxEmulTime = 0; 
	m_drawStage = TopBlank;
	m_virtualRegsAY[m_outPortFFFD & 0x0F] = { 0 }; m_virtualRegsAY[7] = 0xFD; m_virtualRegsAY[14] = 0xFF;
//	DBG_PRINTLN(rp2040.cpuid());
	rp2040.fifo.push(RESET | m_emulSettings.emulSettins.machineType);
} 
// Tape 
void ZXSpectrum::processTape()
{
	m_tapeBit ^= 0x01;
	m_ZXTape.statesCount--;
	m_ZXTape.stateCycles = m_tapeStates[m_ZXTape.tapeState].stateCycles + m_ZXTape.stateCycles; // restore state cycles
	if (m_ZXTape.statesCount > 0) return;
	if (m_ZXTape.tapeState > 1 && m_ZXTape.tapeState < 4) m_ZXTape.tapeState++; // Next state (PILOT->SYNCRO HIGH->SYNCRO LOW)
	else
	{
		if (!m_TAPSection.isLastBit)
		{
			m_ZXTape.tapeState = (m_TAPSection.dataBuffer[m_TAPSection.bit >> 3] & (1 << (7 - (m_TAPSection.bit & 7)))) ? 1 : 0;
			m_TAPSection.bit++; 
			if (m_TAPSection.bit >= (m_TAPSection.dataSize << 3))
				if (!fetchTapeData()) m_TAPSection.isLastBit = true;
		}
		else
			stopTape();
	}
	m_ZXTape.stateCycles = m_tapeStates[m_ZXTape.tapeState].stateCycles; // renew state cycles
	m_ZXTape.statesCount = m_tapeStates[m_ZXTape.tapeState].statesCount; // set state count
}

bool ZXSpectrum::fetchTapeData()
{
	uint32_t bytesToRead = (m_TAPSection.sectionSize > m_bufferSize ? m_bufferSize : m_TAPSection.sectionSize);
	size_t readed = 0;
	if (!bytesToRead) return false;
	if ((readed = m_TAPSection.pFile->readBytes((char*)m_TAPSection.dataBuffer, bytesToRead)) != bytesToRead) return false;
	m_TAPSection.sectionSize -= bytesToRead;
	m_TAPSection.dataSize = bytesToRead;
	m_TAPSection.bit = 0;
	return true;
}

void ZXSpectrum::startTape(File* pFile, uint16_t sectionSize)
{
	m_TAPSection = { 0 };
	m_TAPSection.pFile = pFile; m_TAPSection.sectionSize = sectionSize;
	if (!fetchTapeData()) return;
	m_ZXTape.isTapeActive = true; // start
	m_ZXTape.tapeState = 2; // PILOT tone
	m_ZXTape.stateCycles = m_tapeStates[m_ZXTape.tapeState].stateCycles;
	m_ZXTape.statesCount = m_tapeStates[m_ZXTape.tapeState].statesCount;
	m_tapeBit = 0;
	gpio_put(LED_BUILTIN, 1);
}

 void ZXSpectrum::setMachineType(bool is128)
{
	 // 128K is late timigs int - 1, contention start + 1 
	m_emulSettings.tStatesPerLoop = (!is128 ? 69888 : 70908);
	m_emulSettings.tStatesPerLine = (!is128 ? 224 : 228);
	m_emulSettings.irqLength = (!is128 ? 32 : 35);
	m_emulSettings.contentionStart = (!is128 ? 14335 : 14361 + 1);
	m_emulSettings.contentionEnd = m_emulSettings.contentionStart + 191 * m_emulSettings.tStatesPerLine + 128 - 2 - 1;
	m_emulSettings.borderStart = (!is128 ? m_emulSettings.contentionStart + 1 - BORDER_SHIFT - 24 * m_emulSettings.tStatesPerLine: 
								  m_emulSettings.contentionStart + 1 - BORDER_SHIFT - 24 * m_emulSettings.tStatesPerLine);
	m_emulSettings.borderEnd = m_emulSettings.borderStart + 240 * m_emulSettings.tStatesPerLine;
	//m_emulSettings.borderStart = (!is128 ? m_emulSettings.contentionStart + 1 - 24 * m_emulSettings.tStatesPerLine :
	//							  m_emulSettings.contentionStart + 1 - 24 * m_emulSettings.tStatesPerLine);
	//m_emulSettings.borderEnd = m_emulSettings.borderStartOld + 240 * m_emulSettings.tStatesPerLine;
	m_emulSettings.audioStatesDivider = (!is128 ? 16 : 19);
	if (!is128)
		setMemPageAddr(0, (uint8_t*)zx48ROM);
//		setMemPageAddr(0, (uint8_t*)zxTestROM);
	else
		setMemPageAddr(0, (uint8_t*)zx128ROM0);
//		setMemPageAddr(0, (uint8_t*)zxTestROM);
	m_pageContended[0] = false;
	setMemPageAddr(1, m_pRAMBanks[5]); m_pageContended[1] = true;
	setMemPageAddr(2, m_pRAMBanks[2]); m_pageContended[2] = false;
	setMemPageAddr(3, m_pRAMBanks[0]); m_pageContended[3] = false;
	//m_bufferSize = (!is128 ? 16384 : 24567); // Fu****g magic
	m_bufferSize = 24567; // Fu****g magic
 }

void ZXSpectrum::writeMem(uint16_t address, uint8_t data)
{
	contendedAccess(address, 3);
	if (address >> 14)
	{
		uint8_t oldData = *memoryAddress(address);
		*memoryAddress(address) = data;
		if (address >> 15) return;
		//uint16_t posY = m_Z80Processor.tCount / m_emulSettings.tStatesPerLine, posX = (m_Z80Processor.tCount % m_emulSettings.tStatesPerLine) / 4;
		//if (posY < SCREENOFFSET + 24 || posX > 31) return;
		//if (/*((address - 0x4000 >= m_pixelsMemOffset[posY - SCREENOFFSET] + posX) && (address - 0x4000 < m_pixelsMemOffset[posY - SCREENOFFSET] + 32)) ||*/ 
		//	((address - 0x4000 >= m_attributesMemOffset[posY - SCREENOFFSET]) && (address - 0x4000 <= m_attributesMemOffset[posY - SCREENOFFSET] + posX)))
		//{
		//	//DBG_PRINTF("T: %6d X: %2d Y: %3d addr:%04X\n", m_Z80Processor.tCount, posX, posY - SCREENOFFSET, address - 0x4000);
		//	struct MemoryChange memChng;
		//	memChng.x = address - 0x4000 - m_attributesMemOffset[posY - SCREENOFFSET];//posX;
		//	memChng.y = posY / 8;
		//	memChng.value = oldData;
		//	m_videoMemChange.putData(memChng);
		//}
	}
}

uint8_t ZXSpectrum::readMem(uint16_t address)
{
	contendedAccess(address, 3);
	return *memoryAddress(address);
}

uint8_t ZXSpectrum::fetchOpcode(uint16_t address)
{
	contendedAccess(address, 4);
	return *memoryAddress(address);
}

//uint8_t ZXSpectrum::unattachedPort()
//{
//	//uint16_t posY = (m_Z80Processor.tCount / m_tStatesPerLine) - (64 - m_emulSettings.machineType);
//	uint32_t posY = (m_Z80Processor.tCount / 224) - 64;
//	if (posY >= 192) return 0xFF;
//	DBG_PRINTLN(posY);
//	//uint8_t pixOffset = 3 * (m_emulSettings.machineType ^ 1);
//	//uint8_t halfPix = (m_Z80Processor.tCount % m_tStatesPerLine) - pixOffset;
//	uint8_t halfPix = (m_Z80Processor.tCount % 224) - 3;
//	//if ((halfPix >= (128 - pixOffset)) || (halfPix & 0x04)) return 0xFF;
//	if ((halfPix >= 125) || (halfPix & 0x04)) return 0xFF;
//	int hpOffset = (halfPix >> 2) + ((halfPix >> 1) & 0x01);
//	if (halfPix & 0x01) return *screenAddress(m_attributesMemOffset[posY + 24] + hpOffset);
//	return *screenAddress(m_pixelsMemOffset[posY + 24] + hpOffset);
//
//}

uint8_t ZXSpectrum::unattachedPort()
{
	int posY, tCount, posX;

	if (m_Z80Processor.tCount < m_emulSettings.contentionStart - 15) return 0xFF;
	posY = (m_Z80Processor.tCount - (m_emulSettings.contentionStart - 15)) / m_emulSettings.tStatesPerLine;
	if (posY >= 192) return 0xFF;
	tCount = m_Z80Processor.tCount - (posY * m_emulSettings.tStatesPerLine + m_emulSettings.contentionStart - 15) + 8;
	if (tCount < 24 || tCount >= 152) return 0xFF;
	posX = ((tCount - 24) / 8) * 2;
	switch (tCount % 8)
	{
	case 5:
		posX++;
	case 3:
		return *screenAddress(m_attributesMemOffset[posY + 24] + posX);
	case 4:
		posX++;
	case 2:
		return *screenAddress(m_pixelsMemOffset[posY + 24] + posX);
	case 0:
	case 1:
	case 6:
	case 7:
		return 0xFF;
	}
	return 0;
}

void ZXSpectrum::writePort(uint16_t port, uint8_t data)
{
	contendedAccess(port, 1);
	if (!(port & 0x0001)) // ULA ports
	{
		if (m_outPortFE.borderColor != (data & 7))
		{
			if (m_Z80Processor.tCount >= m_emulSettings.borderStart && m_Z80Processor.tCount < m_emulSettings.borderEnd)
			{
				struct BorderColors value;
				value.y = (m_Z80Processor.tCount - m_emulSettings.borderStart) / m_emulSettings.tStatesPerLine;
				value.x = ((m_Z80Processor.tCount - m_emulSettings.borderStart) % m_emulSettings.tStatesPerLine) / 4;
				value.color = m_colorLookup[data & 0x07]; 
				m_borderColors.putData(value);
			}
			else
				m_borderColor = m_colorLookup[data & 0x07];
		}
		if (m_outPortFE.soundOut != ((data >> 4) & 1))
			rp2040.fifo.push_nb((m_Z80Processor.tCount / m_emulSettings.audioStatesDivider) & 0x00007FFF | WR_PORT | (data & 0x10) << 11);
		m_outPortFE.rawData = data;
		//m_defaultPortVal = (data & 0x18 ? 0xFF : 0xBF); // issue 2
		m_defaultPortVal = (data & 0x10 ? 0xFF : 0xBF); // issue 3
		contendedAccess(CONTENDED, 2);
	}
	else
	{
		if (m_emulSettings.emulSettins.machineType)
		{
			if ((port & 0x8002) == 0) // 128K memory port
			{
				if (!m_outPort7FFD.disabled)
				{
					m_outPort7FFD.rawData = data;
					setMemPageAddr(0, (uint8_t*)(m_outPort7FFD.rom ? zx128ROM1 : zx128ROM0));
//					setMemPageAddr(0, (uint8_t*)(m_outPort7FFD.rom ? zxTestROM : zxTestROM));
					setMemPageAddr(3, m_pRAMBanks[m_outPort7FFD.page]); m_pageContended[3] = m_outPort7FFD.page & 1 ? true : false;
				}
			}
			//if ((port & 0xC002) == 0xC000) // AY port
			//{
			//	m_outPortFFFD = data; 
			//	rp2040.fifo.push_nb(WR_PORT | AY_PORT | data << 16 | (m_Z80Processor.tCount / m_emulSettings.audioStatesDivider) & 0x00007FFF);
			//}
			//if ((port & 0xC002) == 0x8000) // AY port
			//{
			//	m_virtualRegsAY[m_outPortFFFD & 0x0F] = data; 
			//	rp2040.fifo.push_nb(WR_PORT | AY_PORT | AY_DATA | data << 16 | (m_Z80Processor.tCount / m_emulSettings.audioStatesDivider) & 0x00007FFF);
			//}
		}
		if (m_pageContended[port >> 14])
		{
			addressOnBus(CONTENDED, 2);
			contendedAccess(CONTENDED, 0);
		}
		else
			m_Z80Processor.tCount += 2;
	}
	m_Z80Processor.tCount++;
}

uint8_t ZXSpectrum::readPort(uint16_t port)
{
	uint8_t retVal = m_defaultPortVal;

	contendedAccess(port, 1);
	if (!(port & 0x0001)) // ULA ports
	{
		contendedAccess(CONTENDED, 2);
		for (int i = 0; i < 8; i++) if (!((port >> (i + 8)) & 0x01)) retVal &= m_pInPorts[i];// &0xBF);
		if (m_tapeBit) retVal ^= 0x40;
	}
	else
	{
		if (m_pageContended[port >> 14])
		{
			addressOnBus(CONTENDED, 2); contendedAccess(CONTENDED, 0);
		}
		else
			m_Z80Processor.tCount += 2;
		retVal = unattachedPort();
		if (m_emulSettings.emulSettins.machineType)
		{
			// Reads from port 0x7ffd cause a crash, as the 128's HAL10H8 chip does not distinguish between reads and writes to this port, resulting in a floating data bus being used to set the paging registers.
			if ((port & 0x0001) && (port & 0x8002) == 0 && (!m_outPort7FFD.disabled))
			{
				m_outPort7FFD.rawData = retVal;
				setMemPageAddr(0, (uint8_t*)(m_outPort7FFD.rom ? zx128ROM1 : zx128ROM0));
				setMemPageAddr(3, m_pRAMBanks[m_outPort7FFD.page]); m_pageContended[3] = m_outPort7FFD.page & 1 ? true : false;
			}
			//if ((port & 0xC002) == 0xC000) // AY port
			//{
			//	retVal = m_virtualRegsAY[m_outPortFFFD & 0x0F];
			//}
		}
		if ((port & 0x00E0) == 0x00) retVal = m_pInPorts[8]; // Joystic port
	}
	m_Z80Processor.tCount++;
	return retVal;
}

void ZXSpectrum::loopZ80()
{
	uint64_t startTime = micros();
	int32_t usedCycles;
	rp2040.fifo.push(START_FRAME);
	while (m_Z80Processor.tCount < m_emulSettings.borderStart)
	{
		if (m_Z80Processor.tCount < m_emulSettings.irqLength) intZ80();
		usedCycles = m_Z80Processor.tCount;
		stepZ80();
		usedCycles = m_Z80Processor.tCount - usedCycles;
		if (m_ZXTape.isTapeActive)
		{
			m_ZXTape.stateCycles -= usedCycles;	if (m_ZXTape.stateCycles <= 0) processTape();
		}
	}
	while (m_Z80Processor.tCount < m_emulSettings.borderEnd + (m_emulSettings.tStatesPerLine << 1))
	{
		usedCycles = m_Z80Processor.tCount;
		stepZ80();
		usedCycles = m_Z80Processor.tCount - usedCycles;
		if (m_ZXTape.isTapeActive)
		{
			m_ZXTape.stateCycles -= usedCycles; if (m_ZXTape.stateCycles <= 0) processTape();
		}
		int16_t scanLine = m_Z80Processor.tCount / m_emulSettings.tStatesPerLine - 1;
		if (m_scanLine != scanLine)
		{
			m_scanLine = scanLine;
			if (m_scanLine >= SCREENOFFSET && m_scanLine <= SCREENOFFSET + 239) drawLine(m_scanLine - SCREENOFFSET);
		}
	}
	while (m_Z80Processor.tCount < m_emulSettings.tStatesPerLoop)
	{
		usedCycles = m_Z80Processor.tCount;
		stepZ80();
		usedCycles = m_Z80Processor.tCount - usedCycles;
		if (m_ZXTape.isTapeActive)
		{
			m_ZXTape.stateCycles -= usedCycles; if (m_ZXTape.stateCycles <= 0) processTape();
		}
	}
	rp2040.fifo.push_nb(STOP_FRAME); m_drawStage = TopBlank;
	m_Z80Processor.tCount -= m_emulSettings.tStatesPerLoop;
	m_frameCounter = (++m_frameCounter) & 0x1F;
	m_emulationTime = micros() - startTime;
	if (m_maxEmulTime < m_emulationTime && !m_ZXTape.isTapeActive) m_maxEmulTime = m_emulationTime;
	while (!(rp2040.fifo.pop() & STOP_FRAME));
}

// Graphics out
void ZXSpectrum::drawLine(int posY)
{
	uint8_t* pPixelData = screenAddress(m_pixelsMemOffset[posY]);
	uint8_t* pAttrData = screenAddress(m_attributesMemOffset[posY]);
	int flashAttr = (m_frameCounter >> 4) & 1;
	uint32_t* pScreenBuffer = m_pScreenBuffer;
	int posX;
	struct BorderColors value;
	for (posX = 0; posX < 4; posX++) // Left border
	{
		if (m_borderColors.getData(value) && posY == value.y && posX == value.x) { m_borderColor = value.color; m_borderColors.completeGet(); }
		*pScreenBuffer++ = m_borderColor; *pScreenBuffer++ = m_borderColor; *pScreenBuffer++ = m_borderColor; *pScreenBuffer++ = m_borderColor;
	}
	for (; posX < 32 + 4; posX++) // Main area
	{
		if (m_borderColors.getData(value) && posY == value.y && posX == value.x) { m_borderColor = value.color; m_borderColors.completeGet(); }
		if (posY < 24 || posY > 215)
		{
			*pScreenBuffer++ = m_borderColor; *pScreenBuffer++ = m_borderColor;	*pScreenBuffer++ = m_borderColor; *pScreenBuffer++ = m_borderColor;
		}
		else
		{
			uint8_t attrData, pixelData;
			//struct MemoryChange oldMem;
			attrData = *pAttrData++;
			pixelData = *pPixelData++ ^ m_colorInvertMask[(attrData >> 7) & flashAttr];
			//while(true)
			//{
			//	if (!m_videoMemChange.getData(oldMem)) break;
			//	if (posX - 4 == oldMem.x && posY / 8 == oldMem.y)
			//	{
			//		attrData = oldMem.value;
			//		m_videoMemChange.completeGet();
			//	}
			//	else
			//		break;
			//}
			uint8_t	bgColorIndex = (attrData >> 3) & 0xF, fgColorIndex = (attrData & 7) | (bgColorIndex & 0x8);
			uint32_t bgColorMask, bgColor = m_colorLookup[bgColorIndex], fgColorMask, fgColor = m_colorLookup[fgColorIndex];
			fgColorMask = m_pixelBitMask[(pixelData >> 6) & 3];	bgColorMask = ~fgColorMask;
			*pScreenBuffer++ = (fgColorMask & fgColor) | (bgColorMask & bgColor);
			fgColorMask = m_pixelBitMask[(pixelData >> 4) & 3];	bgColorMask = ~fgColorMask;
			*pScreenBuffer++ = (fgColorMask & fgColor) | (bgColorMask & bgColor);
			fgColorMask = m_pixelBitMask[(pixelData >> 2) & 3];	bgColorMask = ~fgColorMask;
			*pScreenBuffer++ = (fgColorMask & fgColor) | (bgColorMask & bgColor);
			fgColorMask = m_pixelBitMask[pixelData & 3]; bgColorMask = ~fgColorMask;
			*pScreenBuffer++ = (fgColorMask & fgColor) | (bgColorMask & bgColor);
		}
	}
	for (; posX < 36 + 4; posX++) // Right border
	{
		if (m_borderColors.getData(value) && posY == value.y && posX == value.x) { m_borderColor = value.color; m_borderColors.completeGet(); }
		*pScreenBuffer++ = m_borderColor; *pScreenBuffer++ = m_borderColor;	*pScreenBuffer++ = m_borderColor; *pScreenBuffer++ = m_borderColor;
	}
	for (; posX < 40 + 16 + m_emulSettings.emulSettins.machineType; posX++) // Retrace
		if (m_borderColors.getData(value) && posY == value.y && posX == value.x) { m_borderColor = value.color; m_borderColors.completeGet(); }
	m_pDisplayInstance->drawBuffer(320);
}

void ZXSpectrum::init(Display* pDisplayInstance, Keyboard* pKeyboardInstance)
{
	int i;
	m_emulSettings.emulSettins.machineType = 0;
	m_pDisplayInstance = pDisplayInstance;
	m_pScreenBuffer = (uint32_t*)m_pDisplayInstance->getBuffer();// m_pDrawBufferEnd = m_pScreenBuffer + 320;
	for (i = 0; i < 8; i++)
	{
		if ((m_pRAMBanks[i] = (uint8_t*)malloc(16384)) == NULL)
		{
			DBG_PRINTLN("Error allocating RAM banks"); return;
		}
	}
	m_pInPorts = pKeyboardInstance->getBuffer(); 
	gpio_init(LED_BUILTIN);
	gpio_set_dir(LED_BUILTIN, true);

}
// Misc & Diag
void ZXSpectrum::storeState(const char* pFileName)
{
	int i;
	LittleFS.begin();
	File storeFile = LittleFS.open(pFileName, "w");
	for (i = 0; i < 8; i++)	if (storeFile.write(m_pRAMBanks[i], 16384) != 16384) break;
	if (i < 8 || storeFile.write((uint8_t*)&m_Z80Processor, sizeof(m_Z80Processor)) != sizeof(m_Z80Processor) || storeFile.write((uint8_t*)&m_outPortFE.rawData, 1) != 1 ||
		storeFile.write((uint8_t*)&m_outPort7FFD.rawData, 1) != 1)
	{
		DBG_PRINTLN("Error saving state file");
	}
	storeFile.close();
	LittleFS.end();
	stopTape();
}

void ZXSpectrum::restoreState(const char* pFileName)
{
	int i;
	LittleFS.begin();
	File storeFile = LittleFS.open(pFileName, "r");
	for (i = 0; i < 8; i++)	if (storeFile.read(m_pRAMBanks[i], 16384) != 16384) break;
	if (i < 8 || storeFile.read((uint8_t*)&m_Z80Processor, sizeof(m_Z80Processor)) != sizeof(m_Z80Processor) || storeFile.read((uint8_t*)&m_outPortFE.rawData, 1) != 1 ||
		storeFile.read((uint8_t*)&m_outPort7FFD.rawData, 1) != 1)
	{
		DBG_PRINTLN("Error loading state file");
	}
	m_borderColor = m_colorLookup[m_outPortFE.borderColor];
	storeFile.close();
	LittleFS.end();
}

bool ZXSpectrum::toggleDebug()
{
	m_debugActvie = !m_debugActvie; 
	return m_debugActvie; 
}


void ZXSpectrum::drawFunc(int32_t tStates, bool isContended)
{
	switch (m_drawStage)
	{
	case TopBlank:
	{
		if (m_Z80Processor.tCount >= m_emulSettings.borderStart)
			m_drawStage = TopBorder; 
		else
		{
			break;
		}
	}
	case TopBorder:
	{
		if (m_Z80Processor.tCount >= m_emulSettings.contentionStart)
			m_drawStage = MainScreen;
		else
		{
			//drawTopBorder
			break;
		}
	}
	case MainScreen:
	{
		if (m_Z80Processor.tCount > m_emulSettings.contentionEnd)
			m_drawStage = BottomBorder;
		else
		{
			//drawMiddle
			if (isContended) m_Z80Processor.tCount += contPattern[(m_Z80Processor.tCount - m_emulSettings.contentionStart) % m_emulSettings.tStatesPerLine];
			break;
		}
	}
	case BottomBorder:
	{
		if (m_Z80Processor.tCount >= m_emulSettings.borderEnd)
			m_drawStage = BottomBlank;
		else
		{
			//drawBottomBorder
			break;
		}
	}
	case BottomBlank:
	default:
		break;
	}
	m_Z80Processor.tCount += tStates;
	//if (isContended && m_Z80Processor.tCount >= m_emulSettings.contentionStart && m_Z80Processor.tCount <= m_emulSettings.contentionEnd) 
	//	m_Z80Processor.tCount += contPattern[(m_Z80Processor.tCount - m_emulSettings.contentionStart) % m_emulSettings.tStatesPerLine]; 
	//m_Z80Processor.tCount += tStates;
}
//void ZXSpectrum::drawFunc(int32_t tStates, bool isContended)
//{
//	static bool flag = false;
//	int32_t currTStates = m_Z80Processor.tCount;
//	switch (m_drawStage)
//	{
//	case TopBlank:
//	{
//		if (currTStates + tStates >= m_emulSettings.borderStart)
//		{
//			int32_t tStatesToBorder = m_emulSettings.borderStart - currTStates;
//			//DBG_PRINTF("Border start: %d MustBe:%d From=%d, Add=%d, ToBorder=%d\n", m_emulSettings.borderStart, currTStates + tStates, currTStates, tStates, tStatesToBorder);
//			currTStates = m_Z80Processor.tCount += tStatesToBorder;
//			tStates -= tStatesToBorder;
//			//DBG_PRINTF("              New=%d, Remain=%d\n", currTStates, tStates);
//			//flag = true;
//			m_drawStage = TopBorder;
//		}
//		else
//		{
//			m_Z80Processor.tCount += tStates;
//			break;
//		}
//	}
//	case TopBorder:
//	{
//		if (currTStates + tStates >= m_emulSettings.contentionStart)
//		{
//			int32_t tStatesToMainScreen = m_emulSettings.contentionStart - currTStates;
//			currTStates = m_Z80Processor.tCount += tStatesToMainScreen;
//			tStates -= tStatesToMainScreen;
//			m_drawStage = MainScreen;
//		}
//		else
//		{
//			//drawTopBorder
//			m_Z80Processor.tCount += tStates;
//			break;
//		}
//	}
//	case MainScreen:
//	{
//		if (currTStates + tStates >= m_emulSettings.contentionEnd)
//		{
//			int32_t tStatesToBorder = m_emulSettings.contentionEnd - currTStates;
//			m_Z80Processor.tCount += contPattern[(m_Z80Processor.tCount - m_emulSettings.contentionStart) % m_emulSettings.tStatesPerLine];
//			currTStates = m_Z80Processor.tCount += tStatesToBorder;
//			tStates -= tStatesToBorder;
//			m_drawStage = BottomBorder;
//		}
//		else
//		{
//			//drawMiddle
////			DBG_PRINTF("Contention: %d-%d, CurrState: % d\n", m_emulSettings.contentionStart, m_emulSettings.contentionEnd, m_Z80Processor.tCount);
//			m_Z80Processor.tCount += contPattern[(m_Z80Processor.tCount - m_emulSettings.contentionStart) % m_emulSettings.tStatesPerLine];
//			m_Z80Processor.tCount += tStates;
//			break;
//		}
//	}
//	case BottomBorder:
//	{
//		if (currTStates + tStates >= m_emulSettings.borderEnd)
//		{
//			int32_t tStatesToBlank = m_emulSettings.borderEnd - currTStates;
//			currTStates = m_Z80Processor.tCount += tStatesToBlank;
//			tStates -= tStatesToBlank;
//			m_drawStage = BottomBlank;
//		}
//		else
//		{
//			//drawBottomBorder
//			m_Z80Processor.tCount += tStates;
//			break;
//		}
//	}
//	case BottomBlank:
//	default:
//		m_Z80Processor.tCount += tStates;
//		break;
//	}
//	//m_Z80Processor.tCount += tStates;
//	//if (flag)
//	//{
//	//	DBG_PRINTF("              result=%d\n\n", m_Z80Processor.tCount);
//	//	flag = false;
//	//}
//	//if (isContended && m_Z80Processor.tCount >= m_emulSettings.contentionStart && m_Z80Processor.tCount <= m_emulSettings.contentionEnd) 
//	//	m_Z80Processor.tCount += contPattern[(m_Z80Processor.tCount - m_emulSettings.contentionStart) % m_emulSettings.tStatesPerLine]; 
//	//m_Z80Processor.tCount += tStates;
//}