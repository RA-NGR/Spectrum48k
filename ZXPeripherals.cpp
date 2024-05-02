#include "ZXPeripherals.h"

Sound::~Sound()
{
    cancel_repeating_timer(&m_clockTimer);
    alarm_pool_destroy(m_pAlarmPool);
}

bool Sound::init()
{
    m_pAlarmPool = alarm_pool_create_with_unused_hardware_alarm(1);
    gpio_set_function(SND_PIN, GPIO_FUNC_PWM);
    const int audio_pin_slice = pwm_gpio_to_slice_num(SND_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 1.0f);
    pwm_config_set_wrap(&config, 255/* + 255 + 255*/);
    pwm_init(audio_pin_slice, &config, true);
    return true;
}

void Sound::update()
{
    uint32_t ctrlData = 0;
    if (rp2040.fifo.pop_nb(&ctrlData))
    {
        if ((ctrlData & 0xFFFFFFFE) == RESET)
        {
            m_samplesPerLoop = 4368 - 636 * (ctrlData & 1); m_samplesPerOut = 7 - (ctrlData & 1); m_enableAY = (ctrlData & 1);
            //if (!(ctrlData & 1))
            //{
            //    m_samplesPerLoop = 69888 / 16; m_samplesPerOut = 7; m_enableAY = false;
            //}
            //else
            //{
            //    m_samplesPerLoop = 70908 / 19; m_samplesPerOut = 6; m_enableAY = true;
            //}
            m_cyclesDone = m_prevBit = 0; m_regsAY = { 0 }; m_regsAY.r8[7] = 0xFD; m_regsAY.r8[14] = 0xFF;
            m_selectedRegAY = 0;
        }
        if (ctrlData & START_FRAME) alarm_pool_add_repeating_timer_us(m_pAlarmPool, -32, onTimer, this, &m_clockTimer);
        if (ctrlData & WR_PORT)
        {
            if (ctrlData & AY_PORT)
            {
                if (ctrlData & AY_DATA)
                    m_regsAY.r8[m_selectedRegAY] = ctrlData & 0x000000FF;
                else
                    m_selectedRegAY = ctrlData & 0x0000000F;
            }
            else
            {
                m_ringBuffer[m_rbWrIndex] = ctrlData & 0x0000FFFF; m_rbWrIndex = (++m_rbWrIndex) & (SOUND_BUFFER_SIZE - 1);
            }
        }
        if (ctrlData & SET_VOL) m_soundVol = ((ctrlData & 0x0000FFFF) + 1) * 64 - 1;
    }
}

bool Sound::onTimer(struct repeating_timer* pTimer)
{
    Sound* pInstance = (Sound*)pTimer->user_data;
    uint16_t samplesBuffer[7], soundBit = pInstance->m_prevBit;
    pInstance->m_cyclesDone += pInstance->m_samplesPerOut;
    int i = 0;
    while (pInstance->m_rbRdIndex != pInstance->m_rbWrIndex && (pInstance->m_ringBuffer[pInstance->m_rbRdIndex] & 0x00007FFF) <= pInstance->m_cyclesDone)
    {
        soundBit = (pInstance->m_ringBuffer[pInstance->m_rbRdIndex] & 0x00008000) >> 15;
        for(; i < (pInstance->m_ringBuffer[pInstance->m_rbRdIndex] & 0x00007FFF) - (pInstance->m_cyclesDone - pInstance->m_samplesPerOut); i++) 
            samplesBuffer[i] = pInstance->m_prevBit;
        pInstance->m_prevBit = soundBit;
        pInstance->m_rbRdIndex = (++pInstance->m_rbRdIndex) & (SOUND_BUFFER_SIZE - 1);
    }
    for (; i < pInstance->m_samplesPerOut; i++) samplesBuffer[i] = pInstance->m_prevBit;
    uint16_t sample = 0;
    for (int i = 0; i < pInstance->m_samplesPerOut; i++) sample += (samplesBuffer[i] * pInstance->m_soundVol);
    pwm_set_gpio_level(SND_PIN, sample / pInstance->m_samplesPerOut);
    if (pInstance->m_cyclesDone < pInstance->m_samplesPerLoop) return true;
    pInstance->m_cyclesDone -= (pInstance->m_samplesPerLoop);
    if (pInstance->m_rbRdIndex != pInstance->m_rbWrIndex && (pInstance->m_ringBuffer[pInstance->m_rbRdIndex] & 0x00007FFF) >= pInstance->m_samplesPerLoop)
        pInstance->m_ringBuffer[pInstance->m_rbRdIndex] -= pInstance->m_samplesPerLoop;
    rp2040.fifo.push_nb(STOP_FRAME);
    return false;
}

Keyboard::~Keyboard()
{
    cancel_repeating_timer(&m_clockTimer);
}

bool Keyboard::init()
{
	Wire1.begin(); Wire1.setClock(1000000);
	writeReg(0x0A, 0x20); // Disable SEQOP on port A
	writeReg(0x0B, 0x20); // Disable SEQOP on port B
	writeReg(0x00, 0x00); // I/O direction register A - all bits to output, Spectrum A8...A15 
	writeReg(0x01, 0x1F); // I/O direction register B - bits 0...4 to input, Spectrum D0...D4
	writeReg(0x04, 0x00); // Disable INT on port A 
	writeReg(0x05, 0x00); // Disable INT on port B
	writeReg(0x0D, 0x1F); // Pullup input bits
	writeReg(0x14, 0xFF); // Set latches to high for all bits port A
	writeReg(0x15, 0xE0); // Set latches to high for bits 5...7 portB
	return add_repeating_timer_us(-KBD_CLOCK, onTimer, this, &m_clockTimer);

}

void Keyboard::writeReg(uint8_t reg, uint8_t data)
{
	Wire1.beginTransmission(0x20);
	Wire1.write(reg); Wire1.write(data);
	Wire1.endTransmission();
}

uint8_t Keyboard::readKeys()
{
	Wire1.beginTransmission(0x20); Wire1.write(0x13); Wire1.endTransmission(); // Request GPIOB state
	Wire1.requestFrom(0x20, 1);
	return (Wire1.read() | 0xE0);
}

bool Keyboard::onTimer(struct repeating_timer* pTimer)
{
	Keyboard* pInstance = (Keyboard*)pTimer->user_data;
	uint8_t regAddr = 0x14, regFlush = 0xFF;
	if (pInstance->m_portScanIdx > 7)
	{
		regAddr = 0x15; regFlush = 0xE0;
	}
	pInstance->writeReg(regAddr, pInstance->m_portScanMask[pInstance->m_portScanIdx]);
    if (pInstance->m_portScanIdx < 8)
        pInstance->m_portData[pInstance->m_portScanIdx] = pInstance->readKeys();// &tapeBit;
	else
		pInstance->m_portData[pInstance->m_portScanIdx] = pInstance->readKeys() ^ 0xFF;
	pInstance->writeReg(regAddr, regFlush);
	if (++pInstance->m_portScanIdx >= 10) pInstance->m_portScanIdx = 0;
	return true;
}

const uint8_t dispInitSeq[] = {  // Cmd, params count, params list OR cmd 255 and delay time in ms
    0x01, 255, 5,																	  					 // ILI9341 Reset, wait 5 ms
    0x28,   0,																		  					 // ILI9341_DISPOFF
    0xCF,   3, 0x00, 0xC1, 0x30,													  					 // ILI9341 Power control B
    0xED,   4, 0x64, 0x03, 0x12, 0x81,												  					 // ILI9341 Power on sequence control 
    0xE8,   3, 0x85, 0x00, 0x78,													  					 // ILI9341 Driver timing control A
    0xCB,   5, 0x39, 0x2C, 0x00, 0x34, 0x02,										  					 // ILI9341 Power control A 
    //0xF7,   1, 0x20,																  					 // ILI9341 pump ratio control
    0xF7,   1, 0x30,																  					 // ILI9341 pump ratio control
    0xEA,   2, 0x00, 0x00,															  					 // ILI9341 Driver timing control B 
    //0xC0,   1, 0x09,																  					 // ILI9341_PWCTR1
    0xC0,   1, 0x23,																  					 // ILI9341_PWCTR1
    //0xC1,   1, 0x00,																  					 // ILI9341_PWCTR2
    0xC1,   1, 0x10,																  					 // ILI9341_PWCTR2
    //0xC5,   2, 0x18, 0x3C,															  					 // ILI9341_VMCTR1
    0xC5,   2, 0x3E, 0x28,															  					 // ILI9341_VMCTR1
    //0xC7,   1, 0x9D,																  					 // ILI9341_VMCTR2
    0xC7,   1, 0x86,																  					 // ILI9341_VMCTR2
    0x36,   1, 0xE8,																  					 // ILI9341_MADCTL <- ILI9341_MADCTL_MV | ILI9341_MADCTL_MY | TFT_MAD_MX | ILI9341_MADCTL_BGR
    0x3A,   1, 0x55,																  					 // ILI9341_PIXFMT
    0xB1,   2, 0x00, TFT_REFRESHRATE,										  					         // ILI9341_FRMCTR1 <- frame rate n Hz
    //0x35,   1, 0x00,																  					 // ILI9341_TEON <- vsync + hsync
    0xB6,   3, 0x08, 0x82, 0x27,													  					 // ILI9341_DFUNCTR
    //0xF2,   1, 0x00,																  					 // ILI9341_ENABLE_3G
    0xF2,   1, 0x02,																  					 // ILI9341_ENABLE_3G
    //0x26,   1, 0x00,																  					 // ILI9341_GAMMASET
    0x26,   1, 0x01,																  					 // ILI9341_GAMMASET
    0xE0,  15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // ILI9341_GMCTRP1
    0xE1,  15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // ILI9341_GMCTRN1
    0x11, 255, 120,																						 // ILI9341_SLPOUT, wait 120 ms
    0x29,   0,																							 // ILI9341_DISPON
    0x2A,   4, 0x00, 0x00, 0x01, 0x3F,																  	 // ILI9341_CASET 0 - 319
    0x2B,   4, 0x00, 0x00, 0x00, 0xEF,																	 // ILI9341_PASET 0 - 239
    0x2C,   0                                                                                            // ILI93441_RAMWR
};

Display::~Display()
{
    dma_channel_unclaim(m_dmaChannel);
    pio_sm_unclaim(m_pio, m_pioSM);
    for (int i = 0; i < 2; i++) if (m_pDMABuffers[i]) free(m_pDMABuffers[i]);
}

bool Display::init()
{
    for (int i = 0; i < 2; i++)
        if ((m_pDMABuffers[i] = (uint16_t*)calloc(320 * DMA_BUFF_SIZE, sizeof(uint16_t))) == NULL) { DBG_PRINTLN("Error allocating DMA buffers"); return m_initComplete; };
    m_pio = pio0;
    if ((m_pioSM = pio_claim_unused_sm(m_pio, false)) < 0)
    {
        m_pio = pio1;
        if ((m_pioSM = pio_claim_unused_sm(m_pio, true)) < 0) { DBG_PRINTLN("Unable to get PIO"); return m_initComplete; };
    }
    uint32_t programOffset = 0;
    programOffset = pio_add_program(m_pio, &m_pioProgram);
    pio_gpio_init(m_pio, TFT_DC); pio_gpio_init(m_pio, TFT_WR);
    for (int i = 0; i < 16; i++) pio_gpio_init(m_pio, TFT_DATA + i);
    pio_sm_set_consecutive_pindirs(m_pio, m_pioSM, TFT_DC, 1, true);
    pio_sm_set_consecutive_pindirs(m_pio, m_pioSM, TFT_WR, 1, true);
    pio_sm_set_consecutive_pindirs(m_pio, m_pioSM, TFT_DATA, 16, true);
    pio_sm_config smCfg = pio_get_default_sm_config();
    sm_config_set_sideset(&smCfg, 2, true, false);
    sm_config_set_set_pins(&smCfg, TFT_DC, 1);
    sm_config_set_sideset_pins(&smCfg, TFT_WR);
    sm_config_set_out_pins(&smCfg, TFT_DATA, 16);
    sm_config_set_clkdiv_int_frac(&smCfg, PIO_CLK_DIV, PIO_CLK_FRACT);
    sm_config_set_fifo_join(&smCfg, PIO_FIFO_JOIN_TX);
    sm_config_set_out_shift(&smCfg, false, false, 0);
    pio_sm_init(m_pio, m_pioSM, programOffset, &smCfg);
    pio_sm_set_enabled(m_pio, m_pioSM, true);
    m_pullStallMask = 1u << (PIO_FDEBUG_TXSTALL_LSB + m_pioSM);
    m_pioInstrSetDC = pio_encode_set((pio_src_dest)0, 1);
    m_pioInstrClrDC = pio_encode_set((pio_src_dest)0, 0);
    uint8_t numArgs;
    uint8_t* dataPtr = (uint8_t*)dispInitSeq;
    while (dataPtr - dispInitSeq < sizeof(dispInitSeq))
    {
        writeCommand(*dataPtr++);
        numArgs = *dataPtr++;
        if (numArgs != 255)
            for (uint8_t i = 0; i < numArgs; i++) writeData(*dataPtr++);
        else
            delay((uint32_t)(*dataPtr++));
    }
    if ((m_dmaChannel = dma_claim_unused_channel(false)) < 0) { DBG_PRINTLN("Unable to get DMA"); return m_initComplete; };
    m_dmaConfig = dma_channel_get_default_config(m_dmaChannel);
    channel_config_set_transfer_data_size(&m_dmaConfig, DMA_SIZE_16);
    channel_config_set_dreq(&m_dmaConfig, pio_get_dreq(m_pio, m_pioSM, true));
    channel_config_set_bswap(&m_dmaConfig, true);
    m_initComplete = true;
    return m_initComplete;
}

void Display::writeCommand(uint8_t cmd)
{
    m_pio->fdebug = m_pullStallMask; while (!(m_pio->fdebug & m_pullStallMask));
    m_pio->sm[m_pioSM].instr = m_pioInstrClrDC;
    m_pio->txf[m_pioSM] = (cmd); m_pio->fdebug = m_pullStallMask; while (!(m_pio->fdebug & m_pullStallMask));
    m_pio->sm[m_pioSM].instr = m_pioInstrSetDC;
}

void Display::drawBuffer(uint8_t bufferIndex, uint16_t bufferSize)
{
    if (!m_initComplete) return;
    while (dma_channel_is_busy(m_dmaChannel));
    dma_channel_configure(m_dmaChannel, &m_dmaConfig, &m_pio->txf[m_pioSM], m_pDMABuffers[bufferIndex & 1], bufferSize, true);
}

void Display::setAddrWindow(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY)
{
    writeCommand(0x2A); 
    uint8_t* pData = (uint8_t*)&startX;
    writeData(pData[1]); writeData(pData[0]);
    pData = (uint8_t*)&endX;
    writeData(pData[1]); writeData(pData[0]);
    writeCommand(0x2B);
    pData = (uint8_t*)&startY;
    writeData(pData[1]); writeData(pData[0]);
    pData = (uint8_t*)&endY;
    writeData(pData[1]); 
    writeData(pData[0]);
    writeCommand(0x2C);
}