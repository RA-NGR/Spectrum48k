#include "ZXSpectrum.h"

ZXSpectrum::~ZXSpectrum()
{
	free(m_pZXMemory);
//#ifdef CONT_TABLE
//	free(m_pContendTable);
//#endif // CONT_TABLE
}

void ZXSpectrum::drawLine(int posY)
{
	uint16_t posX, zxPixelMapAddr, zxColorAttribAddr, tftMemAddr;
	uint8_t i, zxColorAttrib, zxPixelMap, zxBrightFlag, zxInkColor, zxPaperColor, bitPos;
	int buffSwitch = (posY / DMA_BUFF_SIZE) & 1;
	int buffOffset = (posY % DMA_BUFF_SIZE) * 320;

	for (posX = 0; posX < 4; posX++) // Left border
	{
		if (m_pbRIndex != m_pbWIndex && posY == m_borderColors[m_pbRIndex].y && posX == m_borderColors[m_pbRIndex].x)
		{
			m_borderColor = m_borderColors[m_pbRIndex].color; m_pbRIndex = (++m_pbRIndex) & (BORDER_BUFFER_SIZE - 1);
		}
		for (i = 0; i < 8; i++) m_pScreenBuffer[buffSwitch][((posX * 8 + i) ^ 1) + buffOffset] = m_borderColor;
	}
	for (posX = 0; posX < 32; posX++) // Main area
	{
		if (m_pbRIndex != m_pbWIndex && posY == m_borderColors[m_pbRIndex].y && posX + 4 == m_borderColors[m_pbRIndex].x)
		{
			m_borderColor = m_borderColors[m_pbRIndex].color; m_pbRIndex = (++m_pbRIndex) & (BORDER_BUFFER_SIZE - 1);
		}
		if (posY < 24 || posY > 215)
			for (i = 0; i < 8; i++) m_pScreenBuffer[buffSwitch][((posX * 8 + i + 32) ^ 1) + buffOffset] = m_borderColor;
		else
		{
			zxColorAttribAddr = 0x5800 + (((posY - 24) / 8) * 32) + posX; zxColorAttrib = m_pZXMemory[zxColorAttribAddr];
			zxBrightFlag = (zxColorAttrib & 0x40) >> 3;
			zxPixelMapAddr = 0x4000 + (((posY - 24) % 64) / 8) * 32 + ((posY - 24) % 8) * 256 + ((posY - 24) / 64) * 2048 + posX;
			zxPixelMap = m_pZXMemory[zxPixelMapAddr] ^ ((zxColorAttrib & 0x80) ? ((m_frameCounter & 0x10) ? 255 : 0) : 0);
			zxInkColor = zxColorAttrib & 0x07; zxPaperColor = (zxColorAttrib & 0x38) >> 3;
			for (i = 0; i < 8; i++)
			{
				tftMemAddr = posX * 8 + i + 32;	bitPos = (0x80 >> i);
				if ((zxPixelMap & bitPos) != 0)
					m_pScreenBuffer[buffSwitch][tftMemAddr + buffOffset] = m_colorLookup[zxInkColor + zxBrightFlag];
				else
					m_pScreenBuffer[buffSwitch][tftMemAddr + buffOffset] = m_colorLookup[zxPaperColor + zxBrightFlag];
			}
		}
	}
	for (posX = 0; posX < 4; posX++) // Right border
	{
		if (m_pbRIndex != m_pbWIndex && posY == m_borderColors[m_pbRIndex].y && posX + 36 == m_borderColors[m_pbRIndex].x)
		{
			m_borderColor = m_borderColors[m_pbRIndex].color; m_pbRIndex = (++m_pbRIndex) & (BORDER_BUFFER_SIZE - 1);
		}
		for (i = 0; i < 8; i++) m_pScreenBuffer[buffSwitch][((posX * 8 + i + 288) ^ 1) + buffOffset] = m_borderColor;
	}
	for (posX = 0; posX < 16; posX++) // Retrace
	{
		if (m_pbRIndex != m_pbWIndex && posY == m_borderColors[m_pbRIndex].y && posX + 40 == m_borderColors[m_pbRIndex].x)
		{
			m_borderColor = m_borderColors[m_pbRIndex].color; m_pbRIndex = (++m_pbRIndex) & (BORDER_BUFFER_SIZE - 1);
		}
	}
	if (posY % DMA_BUFF_SIZE == DMA_BUFF_SIZE - 1) m_pDisplayInstance->drawBuffer(buffSwitch, 320 * DMA_BUFF_SIZE);
}

int8_t ZXSpectrum::intZ80()
{
	uint16_t inttemp;

	if (IFF1 && m_Z80Processor.tCount < IRQ_LENGTH)
	{
		if (m_Z80Processor.tCount == m_Z80Processor.intEnabledAt) return 0;
		if (m_Z80Processor.halted)
		{
			PC++; m_Z80Processor.halted = 0;
		}
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
			inttemp = (0x100 * I) + 0xFF;
			PCL = readMem(inttemp++); PCH = readMem(inttemp);
			break;
		default:
			break;
		}
		m_Z80Processor.memptr.w = PC;
		Q = 0;
		return 1;
	}
	return 0;
}

void ZXSpectrum::processTape()
{
	m_tapeIn ^= 0x40;
	m_ZXTape.statesCount--;
	m_ZXTape.stateCycles = m_tapeStates[m_ZXTape.tapeState].stateCycles + m_ZXTape.stateCycles; // restore state cycles
	if (m_ZXTape.statesCount > 0) return;
	if (m_ZXTape.tapeState > 1 && m_ZXTape.tapeState < 4)
		m_ZXTape.tapeState++; // Next state (PILOT->SYNCRO HIGH->SYNCRO LOW)
	else
	{
		m_ZXTape.tapeState = (m_TAPSection.data[m_TAPSection.bit >> 3] & (1 << (7 - (m_TAPSection.bit & 7)))) ? 1 : 0;
		m_TAPSection.bit++;
		if (m_TAPSection.bit > (m_TAPSection.size << 3)) stopTape();
	}
	m_ZXTape.stateCycles = m_tapeStates[m_ZXTape.tapeState].stateCycles; // renew state cycles
	m_ZXTape.statesCount = m_tapeStates[m_ZXTape.tapeState].statesCount; // set state count
}

void ZXSpectrum::writeMem(uint16_t address, uint8_t data)
{
	contendedAccess(address, 3);
	if (address >= 0x4000) m_pZXMemory[address] = data;
}

uint8_t ZXSpectrum::readMem(uint16_t address)
{
	contendedAccess(address, 3);
	return m_pZXMemory[address];
}

uint8_t ZXSpectrum::unattachedPort()
{
	int posY, tCount, posX;

	if (m_Z80Processor.tCount < 14320) return 0xFF;
	posY = (m_Z80Processor.tCount - 14320) / 224;
	if (posY >= 192) return 0xFF;
	tCount = m_Z80Processor.tCount - (posY * 224 + 14320) + 8;
	if (tCount < 24 || tCount >= 152) return 0xFF;
	posX = ((tCount - 24) / 8) * 2;
	switch (tCount % 8)
	{
	case 5:
		posX++;
	case 3:
		return m_pZXMemory[0x5800 + ((posY / 8) * 32) + posX];
	case 4:
		posX++;
	case 2:
		return m_pZXMemory[0x4000 + ((posY % 64) / 8) * 32 + (posY % 8) * 256 + (posY / 64) * 2048 + posX];
	case 0:
	case 1:
	case 6:
	case 7:
		return 0xFF;
	}
	return 0;
}

uint8_t ZXSpectrum::readPort(uint16_t port)
{
	uint8_t retVal = 0xFF;
	uint32_t periphData = 0x0000001F, timeOut = 0;

	contendedAccess(port, 1);
	if (!(port & 0x0001))
	{
		contendedAccess(CONTENDED, 2);
		retVal &= m_tapeIn;
		for (int i = 0; i < 8; i++) if (!((port >> (i + 8)) & 0x01)) retVal &= m_pInPort[i];
	}
	else
	{
		if ((port & 0xC000) == 0x4000)
		{
			contendedAccess(CONTENDED, 1); contendedAccess(CONTENDED, 1); contendedAccess(CONTENDED, 0);
		}
		else
		{
			m_Z80Processor.tCount += 2;
			retVal = unattachedPort();
		}

	}
	if ((port & 0x00FF) <= 0x1F) retVal = m_pInPort[8];
	m_Z80Processor.tCount++;
	return retVal;
}

void ZXSpectrum::writePort(uint16_t port, uint8_t data)
{
	contendedAccess(port, 1);
	if ((port & 0x00FF) == 0x00FE)
	{
		if (m_outPortFE.borderColor != (data & 7))
		{
			if (m_Z80Processor.tCount >= STARTSCREEN && m_Z80Processor.tCount <= ENDSCREEN)
			{
				m_borderColors[m_pbWIndex].y = (m_Z80Processor.tCount - STARTSCREEN) / 224; m_borderColors[m_pbWIndex].x = ((m_Z80Processor.tCount - STARTSCREEN) % 224) / 4;
				m_borderColors[m_pbWIndex].color = m_colorLookup[data & 0x07]; m_pbWIndex = (++m_pbWIndex) & (BORDER_BUFFER_SIZE - 1);
			}
			else
				m_borderColor = m_colorLookup[data & 0x07];
		}
		if (m_emulSettings.soundEnabled && m_outPortFE.soundOut != ((data >> 4) & 1)) rp2040.fifo.push_nb(m_Z80Processor.tCount & 0x00FFFFFF | WR_PORT);
		m_outPortFE.rawData = data;
	}
	if (!(port & 0x0001))
	{
		contendedAccess(CONTENDED, 2);
	}
	else
	{
		if ((port & 0xC000) == 0x4000)
		{
			contendedAccess(CONTENDED, 1); contendedAccess(CONTENDED, 1); contendedAccess(CONTENDED, 0);
		}
		else
			m_Z80Processor.tCount += 2;
	}
	m_Z80Processor.tCount++;
}

void ZXSpectrum::stepZ80()
{
	void** pRegisters = m_Z80Processor.pRegisters;
	void** pPair = m_Z80Processor.pPair;
	contendedAccess(PC, 4);
	uint8_t opcode = m_pZXMemory[PC];
	PC++; R++;
	uint8_t last_Q = Q; /* keep Q value from previous opcode for SCF and CCF */
	Q = 0; /* preempt Q value assuming next opcode doesn't set flags */
	uint8_t instruction = instructionTable[opcode];
	bool repeatLoop;
	do
	{
		repeatLoop = false;
		switch (instruction)
		{
		case NOP:
		{
			break;
		}
		case SCF:
		{
			FL = (FL & (FLAG_P | FLAG_Z | FLAG_S)) | (((last_Q ^ FL) | A) & (FLAG_3 | FLAG_5)) | FLAG_C;
			Q = FL;
			break;
		}
		case CCF:
		{
			FL = (FL & (FLAG_P | FLAG_Z | FLAG_S)) | ((FL & FLAG_C) ? FLAG_H : FLAG_C) | (((last_Q ^ FL) | A) & (FLAG_3 | FLAG_5));
			Q = FL;
			break;
		}
		case RLCA:
		{
			A = (A << 1) | (A >> 7);
			FL = (FL & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_C | FLAG_3 | FLAG_5));
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
		case RLA:
		{
			uint8_t bytetemp = A;
			A = (A << 1) | (FL & FLAG_C);
			FL = (FL & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | (bytetemp >> 7);
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
		case INC_R:							/*!*/
		{
			INC8((*(uint8_t*)(pRegisters[r(opcode)])));
			break;
		}
		case INC_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				INC8(bytetemp);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				uint8_t bytetemp = readMem(HL);
				contendedAccess(HL, 1);
				INC8(bytetemp);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case DEC_R:							/*!*/
		{
			DEC8((*(uint8_t*)(pRegisters[r(opcode)])));
			break;
		}
		case DEC_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				DEC8(bytetemp);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				uint8_t bytetemp = readMem(HL);
				contendedAccess(HL, 1);
				DEC8(bytetemp);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case LD_R_R:						/*!*/
		{
			(*(uint8_t*)(pRegisters[r(opcode)])) = (*(uint8_t*)(pRegisters[r_(opcode)]));
			break;
		}
		case LD_R_N:						/*!*/
		{
			(*(uint8_t*)(pRegisters[r(opcode)])) = readMem(PC++);
			break;
		}
		case LD_R_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				(*(uint8_t*)(m_Z80Processor.pRegisters[r(opcode)])) = readMem(m_Z80Processor.memptr.w);
			}
			else
				(*(uint8_t*)(pRegisters[r(opcode)])) = readMem((*(uint16_t*)(pPair[HL_IX_IY_INDEX])));
			break;
		}
		case LD_INDIRECT_HL_R:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				writeMem((*(uint16_t*)(pPair[HL_IX_IY_INDEX])), (*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case HALT:
		{
			m_Z80Processor.halted = 1;
			PC--;
			break;
		}
		case ADD_N:
		{
			uint8_t bytetemp = readMem(PC++);
			ADD(bytetemp);
			break;
		}
		case ADD_R:
		{
			ADD((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case ADD_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				ADD(bytetemp);
			}
			else
			{
				uint8_t bytetemp = readMem(HL);
				ADD(bytetemp);
			}
			break;
		}
		case ADC_N:
		{
			uint8_t bytetemp = readMem(PC++);
			ADC(bytetemp);
			break;
		}
		case ADC_R:							/*!*/
		{
			ADC((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case ADC_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				ADC(bytetemp);
			}
			else
			{
				uint8_t bytetemp = readMem(HL);
				ADC(bytetemp);
			}
			break;
		}
		case SUB_N:
		{
			uint8_t bytetemp = readMem(PC++);
			SUB(bytetemp);
			break;
		}
		case SUB_R:							/*!*/
		{
			SUB((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case SUB_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				SUB(bytetemp);
			}
			else
			{
				uint8_t bytetemp = readMem(HL);
				SUB(bytetemp);
			}
			break;
		}
		case SBC_N:
		{
			uint8_t bytetemp = readMem(PC++);
			SBC(bytetemp);
			break;
		}
		case SBC_R:							/*!*/
		{
			SBC((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case SBC_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				SBC(bytetemp);
			}
			else
			{
				uint8_t bytetemp = readMem(HL);
				SBC(bytetemp);
			}
			break;
		}
		case AND_N:
		{
			uint8_t bytetemp = readMem(PC++);
			AND(bytetemp);
			break;
		}
		case AND_R:							/*!*/
		{
			AND((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case AND_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				AND(bytetemp);
			}
			else
			{
				uint8_t bytetemp = readMem(HL);
				AND(bytetemp);
			}
			break;
		}
		case XOR_N:
		{
			uint8_t bytetemp = readMem(PC++);
			XOR(bytetemp);
			break;
		}
		case XOR_R:							/*!*/
		{
			XOR((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case XOR_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				XOR(bytetemp);
			}
			else
			{
				uint8_t bytetemp = readMem(HL);
				XOR(bytetemp);
			}
			break;
		}
		case OR_N:
		{
			uint8_t bytetemp = readMem(PC++);
			OR(bytetemp);
			break;
		}
		case OR_R:							/*!*/
		{
			OR((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case OR_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				OR(bytetemp);
			}
			else
			{
				uint8_t bytetemp = readMem(HL);
				OR(bytetemp);
			}
			break;
		}
		case CPL:
		{
			A ^= 0xff;
			FL = (FL & (FLAG_C | FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | (FLAG_N | FLAG_H);
			Q = FL;
			break;
		}
		case CP_N:
		{
			uint8_t bytetemp = readMem(PC++);
			CP(bytetemp);
			break;
		}
		case CP_R:							/*!*/
		{
			CP((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case CP_INDIRECT_HL:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1); contendedAccess(PC, 1);
				contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				CP(bytetemp);
			}
			else
			{
				uint8_t bytetemp = readMem(HL);
				CP(bytetemp);
			}
			break;
		}
		case PUSH_SS:						/*!*/
		{
			uint8_t index = dd(opcode);
			if (index == 3) index++;
			uint16_t tempword = (*(uint16_t*)(pPair[index]));
			contendedAccess(IR, 1);
			PUSH16(tempword & 0xFF, tempword >> 8);
			break;
		}
		case POP_SS:						/*!*/
		{
			uint8_t index = dd(opcode);
			if (index == 3) index++;
			POP16((*(uint8_t*)(pPair[index])), (*((uint8_t*)(pPair[index]) + 1)));
			break;
		}
		case INC_RR:						/*!*/
		{
			contendedAccess(IR, 1); contendedAccess(IR, 1);
			(*(uint16_t*)(pPair[dd(opcode)]))++;
			break;
		}
		case DEC_RR:						/*!*/
		{
			contendedAccess(IR, 1); contendedAccess(IR, 1);
			(*(uint16_t*)(pPair[dd(opcode)]))--;
			break;
		}
		case ADD_HL_RR:						/*!*/
		{
			contendedAccess(IR, 1); contendedAccess(IR, 1); contendedAccess(IR, 1); contendedAccess(IR, 1);
			contendedAccess(IR, 1); contendedAccess(IR, 1); contendedAccess(IR, 1);
			ADD16((*(uint16_t*)(pPair[HL_IX_IY_INDEX])), (*(uint16_t*)(pPair[dd(opcode)])));
			break;
		}
		case LD_RR_NN:						/*!*/
		{
			(*(uint8_t*)(pPair[dd(opcode)])) = readMem(PC++);
			(*((uint8_t*)(pPair[dd(opcode)]) + 1)) = readMem(PC++);
			break;
		}
		case RET:
		{
			RET();
			break;
		}
		case RET_CC:
		{
			contendedAccess(IR, 1);
			if (CC(r(opcode)))
			{
				RET();
			}
			break;
		}
		case CALL_NN:
		{
			m_Z80Processor.memptr.b.l = readMem(PC++);
			m_Z80Processor.memptr.b.h = readMem(PC);
			CALL();
			break;
		}
		case CALL_CC_NN:
		{
			m_Z80Processor.memptr.b.l = readMem(PC++);
			m_Z80Processor.memptr.b.h = readMem(PC);
			if (CC(r(opcode)))
			{
				CALL();
			}
			else
			{
				PC++;
			}
			break;
		}
		case JP_NN:
		{
			m_Z80Processor.memptr.b.l = readMem(PC++);
			m_Z80Processor.memptr.b.h = readMem(PC);
			JP();
			break;
		}
		case JP_CC_NN:
		{
			m_Z80Processor.memptr.b.l = readMem(PC++);
			m_Z80Processor.memptr.b.h = readMem(PC);
			if (CC(r(opcode)))
			{
				JP();
			}
			else
			{
				PC++;
			}
			break;
		}
		case DJNZ_E:
		{
			contendedAccess(IR, 1);
			B--;
			if (B)
			{
				JR();
			}
			else
			{
				contendedAccess(PC, 3);
				PC++;
			}
			break;
		}
		case JR_DD_E:
		{
			if (CC(q(opcode)))
			{
				JR();
			}
			else
			{
				contendedAccess(PC, 3);
				PC++;
			}
			break;
		}
		case JR_E:
		{
			JR();
			break;
		}
		case RST_P:
		{
			contendedAccess(IR, 1);
			RST(rstTable[r(opcode)]);
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
		case DI:
		{
			IFF1 = IFF2 = 0;
			break;
		}
		case EI:
		{
			IFF1 = IFF2 = 1;
			m_Z80Processor.intEnabledAt = m_Z80Processor.tCount;
			break;
		}
		case LD_SP_HL:						/*!*/
		{
			contendedAccess(IR, 1); contendedAccess(IR, 1);
			SP = (*(uint16_t*)(pPair[HL_IX_IY_INDEX]));
			break;
		}
		case LD_INDIRECT_NN_HL:				/*!*/
		{
			LD16_NNRR((*(uint8_t*)(pRegisters[L_INDEX])), (*(uint8_t*)(pRegisters[H_INDEX])));
			break;
		}
		case LD_HL_INDIRECT_NN:				/*!*/
		{
			LD16_RRNN((*(uint8_t*)(pRegisters[L_INDEX])), (*(uint8_t*)(pRegisters[H_INDEX])));
			break;
		}
		case LD_A_INDIRECT_BC:
		{
			m_Z80Processor.memptr.w = BC + 1;
			A = readMem(BC);
			break;
		}
		case LD_INDIRECT_BC_A:
		{
			m_Z80Processor.memptr.b.l = BC + 1;
			m_Z80Processor.memptr.b.h = A;
			writeMem(BC, A);
			break;
		}
		case LD_A_INDIRECT_DE:
		{
			m_Z80Processor.memptr.w = DE + 1;
			A = readMem(DE);
			break;
		}
		case LD_INDIRECT_DE_A:
		{
			m_Z80Processor.memptr.b.l = DE + 1;
			m_Z80Processor.memptr.b.h = A;
			writeMem(DE, A);
			break;
		}
		case LD_A_INDIRECT_NN:
		{
			m_Z80Processor.memptr.b.l = readMem(PC++);
			m_Z80Processor.memptr.b.h = readMem(PC++);
			A = readMem(m_Z80Processor.memptr.w++);
			break;
		}
		case LD_INDIRECT_NN_A:
		{
			uint16_t wordtemp = readMem(PC++);
			wordtemp |= readMem(PC++) << 8;
			m_Z80Processor.memptr.b.l = wordtemp + 1;
			m_Z80Processor.memptr.b.h = A;
			writeMem(wordtemp, A);
			break;
		}
		case LD_INDIRECT_HL_N:				/*!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t offset = readMem(PC++);
				uint8_t value = readMem(PC);
				contendedAccess(PC, 1); contendedAccess(PC, 1);
				PC++;
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)offset;
				writeMem(m_Z80Processor.memptr.w, value);
			}
			else
				writeMem(HL, readMem(PC++));
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
		case EXX:
		{
			uint16_t wordtemp = BC; BC = BC_; BC_ = wordtemp;
			wordtemp = DE; DE = DE_; DE_ = wordtemp;
			wordtemp = HL; HL = HL_; HL_ = wordtemp;
			break;
		}
		case EX_DE_HL:
		{
			uint16_t wordtemp = DE; DE = HL; HL = wordtemp;
			break;
		}
		case EX_INDIRECT_SP_HL:				/*!*/
		{
			uint8_t bytetempl = readMem(SP);
			uint8_t bytetemph = readMem(SP + 1);
			contendedAccess(SP + 1, 1);
			writeMem(SP + 1, (*(uint8_t*)(pRegisters[H_INDEX])));
			writeMem(SP, (*(uint8_t*)(pRegisters[L_INDEX])));
			contendedAccess(SP, 1); contendedAccess(SP, 1);
			(*(uint8_t*)(pRegisters[L_INDEX])) = m_Z80Processor.memptr.b.l = bytetempl;
			(*(uint8_t*)(pRegisters[H_INDEX])) = m_Z80Processor.memptr.b.h = bytetemph;
			break;
		}
		case JP_HL:							/*!*/
		{
			PC = (*(uint16_t*)(pPair[HL_IX_IY_INDEX]));
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
		case IN_A_N:
		{
			uint16_t intemp = readMem(PC++) + (A << 8);
			A = readPort(intemp);
			m_Z80Processor.memptr.w = intemp + 1;
			break;
		}
		case RLC_R:							/*!!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				RLC((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				RLC((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case RLC_INDIRECT_HL:				/*!!*/
		{
			uint8_t bytetemp;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				RLC(bytetemp);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				bytetemp = readMem(HL);
				contendedAccess(HL, 1);
				RLC(bytetemp);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case RRC_R:							/*!!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				RRC((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				RRC((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case RRC_INDIRECT_HL:				/*!!*/
		{
			uint8_t bytetemp;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				RRC(bytetemp);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				bytetemp = readMem(HL);
				contendedAccess(HL, 1);
				RRC(bytetemp);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case RL_R:							/*!!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				RL((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				RL((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case RL_INDIRECT_HL:				/*!!*/
		{
			uint8_t bytetemp;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				RL(bytetemp);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				bytetemp = readMem(HL);
				contendedAccess(HL, 1);
				RL(bytetemp);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case RR_R:							/*!!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				RR((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				RR((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case RR_INDIRECT_HL:				/*!!*/
		{
			uint8_t bytetemp;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				RR(bytetemp);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				bytetemp = readMem(HL);
				contendedAccess(HL, 1);
				RR(bytetemp);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case SRA_R:							/*!!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				SRA((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				SRA((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case SRA_INDIRECT_HL:				/*!!*/
		{
			uint8_t bytetemp;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				SRA(bytetemp);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				bytetemp = readMem(HL);
				contendedAccess(HL, 1);
				SRA(bytetemp);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case SLA_R:							/*!!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				SLA((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				SLA((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case SLA_INDIRECT_HL:				/*!!*/
		{
			uint8_t bytetemp;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				SLA(bytetemp);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				bytetemp = readMem(HL);
				contendedAccess(HL, 1);
				SLA(bytetemp);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case SRL_R:							/*!!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				SRL((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				SRL((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case SRL_INDIRECT_HL:				/*!!*/
		{
			uint8_t bytetemp;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				SRL(bytetemp);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				bytetemp = readMem(HL);
				contendedAccess(HL, 1);
				SRL(bytetemp);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case SLL_R:							/*!!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				SLL((*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				SLL((*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case SLL_INDIRECT_HL:				/*!!*/
		{
			uint8_t bytetemp;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				SLL(bytetemp);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				bytetemp = readMem(HL);
				contendedAccess(HL, 1);
				SLL(bytetemp);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case BIT_B_R:						/*!!*/
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				uint8_t bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
				BIT_MEMPTR(r(opcode), bytetemp);
			}
			else
				BIT_REG(r(opcode), (*(uint8_t*)(pRegisters[r_(opcode)])));
			break;
		}
		case BIT_B_INDIRECT_HL:				/*!!*/
		{
			uint8_t bytetemp;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w);
				contendedAccess(m_Z80Processor.memptr.w, 1);
			}
			else
			{
				bytetemp = readMem(HL);
				contendedAccess(HL, 1);
			}
			BIT_MEMPTR(r(opcode), bytetemp);
			break;
		}
		case RES_B_R:						/*!!*/
		{
			uint8_t bitMask = 1 << r(opcode);
			bitMask ^= 0xFF;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w) & bitMask;
				contendedAccess(m_Z80Processor.memptr.w, 1);
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				(*(uint8_t*)(pRegisters[r_(opcode)])) &= bitMask;
			break;
		}
		case RES_B_INDIRECT_HL:				/*!!*/
		{
			uint8_t bitMask = 1 << r(opcode), bytetemp;
			bitMask ^= 0xFF;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w) & bitMask;
				contendedAccess(m_Z80Processor.memptr.w, 1);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				bytetemp = readMem(HL) & bitMask;
				contendedAccess(HL, 1);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case SET_B_R:						/*!!*/
		{
			uint8_t bitMask = 1 << r(opcode);
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				(*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])) = readMem(m_Z80Processor.memptr.w) | bitMask;
				contendedAccess(m_Z80Processor.memptr.w, 1);
				writeMem(m_Z80Processor.memptr.w, (*(uint8_t*)(m_Z80Processor.pRegisters[r_(opcode)])));
			}
			else
				(*(uint8_t*)(pRegisters[r_(opcode)])) |= bitMask;
			break;
		}
		case SET_B_INDIRECT_HL:				/*!!*/
		{
			uint8_t bitMask = 1 << r(opcode), bytetemp;
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				bytetemp = readMem(m_Z80Processor.memptr.w) | bitMask;
				contendedAccess(m_Z80Processor.memptr.w, 1);
				writeMem(m_Z80Processor.memptr.w, bytetemp);
			}
			else
			{
				bytetemp = readMem(HL) | bitMask;
				contendedAccess(HL, 1);
				writeMem(HL, bytetemp);
			}
			break;
		}
		case ED_PREFIX:
		{
			contendedAccess(PC, 4);
			opcode = m_pZXMemory[PC];
			instruction = edInstructionTable[opcode];
			PC++;
			R++;
			repeatLoop = true;
			break;
		}
		case IN_R_C:
		{
			if (r(opcode) != 0b110)
			{
				INP((*(uint8_t*)(pRegisters[r(opcode)])), BC);
			}
			else
			{
				uint8_t bytetemp;
				INP(bytetemp, BC);
			}
			break;
		}
		case OUT_C_R:
		{
			if (r(opcode) != 0b110)
			{
				writePort(BC, (*(uint8_t*)(pRegisters[r(opcode)])));
			}
			else
				writePort(BC, 0);
			m_Z80Processor.memptr.w = BC + 1;
			break;
		}
		case SBC_HL_RR:
		{
			contendedAccess(IR, 1); contendedAccess(IR, 1); contendedAccess(IR, 1); contendedAccess(IR, 1);
			contendedAccess(IR, 1); contendedAccess(IR, 1); contendedAccess(IR, 1);
			SBC16((*(uint16_t*)(pPair[dd(opcode)])));
			break;
		}
		case ADC_HL_RR:
		{
			contendedAccess(IR, 1); contendedAccess(IR, 1); contendedAccess(IR, 1); contendedAccess(IR, 1);
			contendedAccess(IR, 1); contendedAccess(IR, 1); contendedAccess(IR, 1);
			ADC16((*(uint16_t*)(pPair[dd(opcode)])));
			break;
			break;
		}
		case LD_INDIRECT_NN_RR:				/*!*/
		{
			LD16_NNRR((*(uint8_t*)(pPair[dd(opcode)])), (*((uint8_t*)(pPair[dd(opcode)]) + 1)));
			break;
		}
		case LD_RR_INDIRECT_NN:				/*!*/
		{
			LD16_RRNN((*(uint8_t*)(pPair[dd(opcode)])), (*((uint8_t*)(pPair[dd(opcode)]) + 1)));
			break;
		}
		case NEG:
		{
			uint8_t bytetemp = A;
			A = 0;
			SUB(bytetemp);
			break;
		}
		case RETI_RETN:
		{
			IFF1 = IFF2;
			RET();
			break;
		}
		case RLD_RRD:
		{
			uint8_t bytetemp = readMem(HL);
			contendedAccess(HL, 1); contendedAccess(HL, 1); contendedAccess(HL, 1); contendedAccess(HL, 1);
			if (opcode == 0x6F)
			{
				writeMem(HL, (bytetemp << 4) | (A & 0x0f));
				A = (A & 0xf0) | (bytetemp >> 4);
			}
			else
			{
				writeMem(HL, (A << 4) | (bytetemp >> 4));
				A = (A & 0xf0) | (bytetemp & 0x0f);
			}
			FL = (FL & FLAG_C) | sz53pTable[A];
			Q = FL;
			m_Z80Processor.memptr.w = HL + 1;
			break;
		}
		case LD_I_A_LD_R_A:
		{
			contendedAccess(IR, 1);
			if (opcode == 0x47)
				I = A;
			else
				R = R7 = A;
			break;
		}
		case LD_A_I_LD_A_R:
		{
			contendedAccess(IR, 1);
			if (opcode == 0x57)
				A = I;
			else
				A = (R & 0x7f) | (R7 & 0x80);
			FL = (FL & FLAG_C) | sz53Table[A] | (IFF2 ? FLAG_V : 0);
			Q = FL;
			m_Z80Processor.iff2_read = 1;
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
		case LDI_LDD:
		{
			uint8_t bytetemp = readMem(HL);
			BC--;
			writeMem(DE, bytetemp);
			contendedAccess(DE, 1); contendedAccess(DE, 1);
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
		case CPI_CPD:
		{
			uint8_t value = readMem(HL);
			contendedAccess(HL, 1); contendedAccess(HL, 1); contendedAccess(HL, 1); contendedAccess(HL, 1);
			contendedAccess(HL, 1);
			uint8_t bytetemp = A - value;
			uint8_t lookup = ((A & 0x08) >> 3) | (((value) & 0x08) >> 2) | ((bytetemp & 0x08) >> 1);
			if (opcode == 0xA1)
				HL++;
			else
				HL--;
			BC--;
			FL = (FL & FLAG_C) | (BC ? (FLAG_V | FLAG_N) : FLAG_N) | halfcarrySubTable[lookup] | (bytetemp ? 0 : FLAG_Z) | (bytetemp & FLAG_S);
			if (FL & FLAG_H) bytetemp--;
			FL |= (bytetemp & FLAG_3) | ((bytetemp & 0x02) ? FLAG_5 : 0);
			Q = FL;
			m_Z80Processor.memptr.w++;
			break;
		}
		case INI_IND:
		{
			contendedAccess(IR, 1);
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
		case OUTI_OUTD:
		{
			contendedAccess(IR, 1);
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
		case LDIR_LDDR:
		{
			uint8_t bytetemp = readMem(HL);
			writeMem(DE, bytetemp);
			contendedAccess(DE, 1); contendedAccess(DE, 1);
			BC--;
			bytetemp += A;
			FL = (FL & (FLAG_C | FLAG_Z | FLAG_S)) | (BC ? FLAG_V : 0) | (bytetemp & FLAG_3) | ((bytetemp & 0x02) ? FLAG_5 : 0);
			Q = FL;
			if (BC)
			{
				contendedAccess(DE, 1); contendedAccess(DE, 1); contendedAccess(DE, 1); contendedAccess(DE, 1);
				contendedAccess(DE, 1);
				PC -= 2;
				m_Z80Processor.memptr.w = PC + 1;
			}
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
		case CPIR_CPDR:
		{
			uint8_t value = readMem(HL);
			uint8_t bytetemp = A - value;
			uint8_t lookup = ((A & 0x08) >> 3) | (((value) & 0x08) >> 2) | ((bytetemp & 0x08) >> 1);
			contendedAccess(HL, 1); contendedAccess(HL, 1); contendedAccess(HL, 1); contendedAccess(HL, 1);
			contendedAccess(HL, 1);
			BC--;
			FL = (FL & FLAG_C) | (BC ? (FLAG_V | FLAG_N) : FLAG_N) | halfcarrySubTable[lookup] | (bytetemp ? 0 : FLAG_Z) | (bytetemp & FLAG_S);
			if (FL & FLAG_H) bytetemp--;
			FL |= (bytetemp & FLAG_3) | ((bytetemp & 0x02) ? FLAG_5 : 0);
			Q = FL;
			if ((FL & (FLAG_V | FLAG_Z)) == FLAG_V)
			{
				contendedAccess(HL, 1); contendedAccess(HL, 1); contendedAccess(HL, 1); contendedAccess(HL, 1);
				contendedAccess(HL, 1);
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
		case INIR_INDR:
		{
			contendedAccess(IR, 1);
			uint8_t initemp = readPort(BC), initemp2;
			writeMem(HL, initemp);
			if (opcode == 0xB2)
				m_Z80Processor.memptr.w = BC + 1;
			else
				m_Z80Processor.memptr.w = BC - 1;
			B--;
			if (opcode == 0xB2)
				initemp2 = initemp + C + 1;
			else
				initemp2 = initemp + C - 1;
			FL = (initemp & 0x80 ? FLAG_N : 0) | ((initemp2 < initemp) ? FLAG_H | FLAG_C : 0) | (parityTable[(initemp2 & 0x07) ^ B] ? FLAG_P : 0) | sz53Table[B];
			Q = FL;
			if (B)
			{
				contendedAccess(HL, 1); contendedAccess(HL, 1); contendedAccess(HL, 1); contendedAccess(HL, 1);
				contendedAccess(HL, 1);
				PC -= 2;
			}
			if (opcode == 0xB2)
				HL++;
			else
				HL--;
			break;
		}
		case OTIR_OTDR:
		{
			contendedAccess(IR, 1);
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
			FL = (outitemp & 0x80 ? FLAG_N : 0) | ((outitemp2 < outitemp) ? FLAG_H | FLAG_C : 0) | (parityTable[(outitemp2 & 0x07) ^ B] ? FLAG_P : 0) | sz53Table[B];
			Q = FL;
			if (B)
			{
				contendedAccess(BC, 1); contendedAccess(BC, 1); contendedAccess(BC, 1); contendedAccess(BC, 1);
				contendedAccess(BC, 1);
				PC -= 2;
			}
			break;
		}
		case ED_UNDEFINED:
		{
			break;
		}
		case CB_PREFIX:
		{
			if (pRegisters != m_Z80Processor.pRegisters)
			{
				contendedAccess(PC, 3);
				m_Z80Processor.memptr.w = (*(uint16_t*)(pPair[HL_IX_IY_INDEX])) + (int8_t)m_pZXMemory[PC];
				PC++;
				contendedAccess(PC, 3);
				opcode = m_pZXMemory[PC];;
				contendedAccess(PC, 1); contendedAccess(PC, 1);
			}
			else
			{
				contendedAccess(PC, 4);
				opcode = m_pZXMemory[PC];
				R++;
			}
			instruction = cbInstructionTable[opcode];
			PC++;
			repeatLoop = true;
			break;
		}
		case DD_PREFIX:
		{
			contendedAccess(PC, 4);
			opcode = m_pZXMemory[PC];
			instruction = instructionTable[opcode];
			if (instruction <= CB_PREFIX)
			{
				PC++;
				R++;
				pRegisters = m_Z80Processor.pDDRegisters;
				pPair = m_Z80Processor.pDDPair;
				repeatLoop = true;
			}
			else
			{
				DBG_PRINTLN("DD Backtrack");
			}
			break;
		}
		case FD_PREFIX:
		{
			contendedAccess(PC, 4);
			opcode = m_pZXMemory[PC];
			instruction = instructionTable[opcode];
			if (instruction <= CB_PREFIX)
			{
				PC++;
				R++;
				pRegisters = m_Z80Processor.pFDRegisters;
				pPair = m_Z80Processor.pFDPair;
				repeatLoop = true;
			}
			else
			{
				DBG_PRINTLN("FD Backtrack");
			}
			break;
		}
		default:
			break;
		}
	} while (repeatLoop);
}
/// Public
bool ZXSpectrum::init(Display* pDisplayInstance, Keyboard* pKeyboardInstance)
{
	char romFile[] = "/BASIC82.rom";
	m_pDisplayInstance = pDisplayInstance;
	for (uint8_t i = 0; i < 2; i++) m_pScreenBuffer[i] = m_pDisplayInstance->getBuffer(i);
	m_pInPort = pKeyboardInstance->getBuffer();
	if ((m_pZXMemory = (uint8_t*)malloc(65536)) == NULL) { printf("Error allocating ZXMemory"); return false; }
//#ifdef CONT_TABLE
//	if ((m_pContendTable = (uint8_t*)malloc(42910)) == NULL) { printf("Error allocating contended access table"); return false; }
//	memset(m_pContendTable, 0, 42910);
//	uint8_t contPattern[] = { 6, 5, 4, 3, 2, 1, 0, 0 };
//	for (uint32_t i = 0; i < 42910; i++) m_pContendTable[i] = (((i % 224) > 127) ? 0 : contPattern[(i % 224) % 8]);
//#endif // CONT_TABLE
	if (!loadROMFile(romFile)) return false;
	m_initComplete = true;
	return m_initComplete;
}

bool ZXSpectrum::loadROMFile(const char* pFileName)
{
	if (!LittleFS.begin()) { DBG_PRINTLN("FS Mount Failed"); return false; }
	if (!LittleFS.exists(pFileName)) { DBG_PRINTLN("ROM image not found"); return false; }
	File romFile = LittleFS.open(pFileName, "r");
	if (!(romFile.read(m_pZXMemory, 16384) == 16384)) { DBG_PRINTLN("Error reading ROM image"); romFile.close(); return false; }
	romFile.close();
	LittleFS.end();
	return true;
}

void ZXSpectrum::resetZ80()
{
	stopTape();
	m_Z80Processor = { 0 };
	void** pRegisters = m_Z80Processor.pRegisters;
	AF = AF_ = 0xffff;
	SP = 0xffff;
	m_Z80Processor.intEnabledAt = -1;
	m_maxEmulTime = 0;
	m_Z80Processor.pRegisters[0] = m_Z80Processor.pDDRegisters[0] = m_Z80Processor.pFDRegisters[0] = &B;
	m_Z80Processor.pRegisters[1] = m_Z80Processor.pDDRegisters[1] = m_Z80Processor.pFDRegisters[1] = &C;
	m_Z80Processor.pRegisters[2] = m_Z80Processor.pDDRegisters[2] = m_Z80Processor.pFDRegisters[2] = &D;
	m_Z80Processor.pRegisters[3] = m_Z80Processor.pDDRegisters[3] = m_Z80Processor.pFDRegisters[3] = &E;
	m_Z80Processor.pRegisters[4] = &H; m_Z80Processor.pDDRegisters[4] = &IXH; m_Z80Processor.pFDRegisters[4] = &IYH;
	m_Z80Processor.pRegisters[5] = &L; m_Z80Processor.pDDRegisters[5] = &IXL; m_Z80Processor.pFDRegisters[5] = &IYL;
	m_Z80Processor.pRegisters[7] = m_Z80Processor.pDDRegisters[7] = m_Z80Processor.pFDRegisters[7] = &A;
	m_Z80Processor.pPair[0] = m_Z80Processor.pDDPair[0] = m_Z80Processor.pFDPair[0] = &BC;
	m_Z80Processor.pPair[1] = m_Z80Processor.pDDPair[1] = m_Z80Processor.pFDPair[1] = &DE;
	m_Z80Processor.pPair[2] = &HL; m_Z80Processor.pDDPair[2] = &IX; m_Z80Processor.pFDPair[2] = &IY;
	m_Z80Processor.pPair[3] = m_Z80Processor.pDDPair[3] = m_Z80Processor.pFDPair[3] = &SP;
	m_Z80Processor.pPair[4] = m_Z80Processor.pDDPair[4] = m_Z80Processor.pFDPair[4] = &AF;
}

void ZXSpectrum::loopZ80()
{
	uint64_t startTime = micros();
	int32_t usedCycles;
	rp2040.fifo.push(START_FRAME);
	intZ80();
	while (m_Z80Processor.tCount < LOOPCYCLES)
	{
		usedCycles = m_Z80Processor.tCount;
		stepZ80();
		usedCycles = m_Z80Processor.tCount - usedCycles;
		if (m_ZXTape.isTapeActive)
		{
			m_ZXTape.stateCycles -= usedCycles;
			if (m_ZXTape.stateCycles <= 0) processTape();
		}
		int16_t scanLine = m_Z80Processor.tCount / 224 - 1;
		if (m_scanLine != scanLine)
		{
			m_scanLine = scanLine;
			if (m_scanLine >= SCREENOFFSET && m_scanLine <= SCREENOFFSET + 239) drawLine(m_scanLine - SCREENOFFSET);
		}
	}
	while (m_pbRIndex != m_pbWIndex)
	{
		m_borderColor = m_borderColors[m_pbRIndex].color; m_pbRIndex = (++m_pbRIndex) & (BORDER_BUFFER_SIZE - 1);
	}
	m_Z80Processor.tCount -= LOOPCYCLES;
	m_frameCounter = (++m_frameCounter) & 0x1F;
	if (m_Z80Processor.intEnabledAt >= 0) m_Z80Processor.intEnabledAt -= LOOPCYCLES;
	m_emulationTime = micros() - startTime;
	if (m_maxEmulTime < m_emulationTime) m_maxEmulTime = m_emulationTime;
	while (!(rp2040.fifo.pop() & STOP_FRAME));
}

void ZXSpectrum::startTape(uint8_t* pBuffer, uint32_t bufferSize)
{
	m_TAPSection = { 0 };
	m_TAPSection.data = pBuffer; m_TAPSection.size = bufferSize;
	m_TAPSection.bit = 0;
	m_ZXTape.isTapeActive = true; // start
	m_ZXTape.tapeState = 2; // PILOT tone
	m_ZXTape.stateCycles = m_tapeStates[m_ZXTape.tapeState].stateCycles;
	m_ZXTape.statesCount = m_tapeStates[m_ZXTape.tapeState].statesCount;
}

void ZXSpectrum::tapeMode(bool isTurbo)
{
	if (isTurbo)
	{
		m_pZXMemory[1409] = 206;
		m_pZXMemory[1416] = 227;
		m_pZXMemory[1424] = 228;
		m_pZXMemory[1432] = 236;
		m_pZXMemory[1446] = 216;
		m_pZXMemory[1479] = 217;
		m_pZXMemory[1487] = 229;
		m_pZXMemory[1492] = 215;
		m_pZXMemory[1512] = 5;
		m_tapeStates[0] = { 427, 2 };
		m_tapeStates[1] = { 855, 2 };
		m_tapeStates[2] = { 1084, 4846 };
		m_tapeStates[3] = { 333, 1 };
		m_tapeStates[4] = { 367, 1 };
	}
	else
	{
		m_pZXMemory[1409] = 156;
		m_pZXMemory[1416] = 198;
		m_pZXMemory[1424] = 201;
		m_pZXMemory[1432] = 212;
		m_pZXMemory[1446] = 176;
		m_pZXMemory[1479] = 178;
		m_pZXMemory[1487] = 203;
		m_pZXMemory[1492] = 176;
		m_pZXMemory[1512] = 22;
		m_tapeStates[0] = { 855, 2 };
		m_tapeStates[1] = { 1710, 2 };
		m_tapeStates[2] = { 2168, 4846 };
		m_tapeStates[3] = { 667, 1 };
		m_tapeStates[4] = { 735, 1 };
	}
}

void ZXSpectrum::storeState(const char* pFileName)
{
	LittleFS.begin();
	File storeFile = LittleFS.open(pFileName, "w");
	if (storeFile.write(m_pZXMemory + 16384, 49152) != 49152 || storeFile.write((uint8_t*)&m_Z80Processor, sizeof(m_Z80Processor)) != sizeof(m_Z80Processor) || storeFile.write((uint8_t*)&m_borderColor, 2) != 2)
	{
		DBG_PRINTLN("Error saving state file");
	}
	storeFile.close();
	LittleFS.end();
	//stopTape();
}

void ZXSpectrum::restoreState(const char* pFileName)
{
	LittleFS.begin();
	File storeFile = LittleFS.open(pFileName, "r");
	if (storeFile.read(m_pZXMemory + 16384, 49152) != 49152 || storeFile.read((uint8_t*)&m_Z80Processor, sizeof(m_Z80Processor)) != sizeof(m_Z80Processor) || storeFile.read((uint8_t*)&m_borderColor, 2) != 2)
	{
		DBG_PRINTLN("Error loading state file");
	}
	storeFile.close();
	LittleFS.end();
}
