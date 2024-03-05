#include "Common.h"
#include "ZXSpectrum.h"
#include "ZXPeripherals.h"
#include "Browser.h"

Display g_mainDisplay;
Sound g_soundBeeper;
Keyboard g_mainKeyboard;
ZXSpectrum g_zxEmulator;
Browser g_cardBrowser(&g_mainDisplay, &g_mainKeyboard);

;

enum systemMode
{
	modeEmulator,
	modeBrowser
};
enum systemMode g_sysMode = systemMode::modeEmulator;

struct
{
	uint8_t *pTapBuffer;
	uint16_t tapSize;
	File tapFile;
	bool tapActive;
	int32_t tapePause;
	String fileName;
} g_zxTape = {
		.pTapBuffer = 0,
		.tapSize = 0,
		.tapActive = false,
		.tapePause = -1,
		.fileName = ""
};
//uint8_t tapBuffer[49153] = { 
//	0 };
//uint16_t tapSize = 0;
//File tapFile;
//bool tapActive = false;
//int32_t tapePause = -1;

void setup()
{
//	vreg_set_voltage(VREG_VOLTAGE_1_15);
	set_sys_clock_khz(250000, true);
#if defined(DBG)
	Serial.begin(115200);
	delay(1000);
	DBG_PRINTF("Free mem: %d\n", rp2040.getFreeHeap());
#endif // DBG
	g_mainDisplay.init();
	delay(100);
	g_mainKeyboard.init();
	g_zxEmulator.init(&g_mainDisplay, &g_mainKeyboard);
	g_zxEmulator.resetZ80();
	pinMode(LED_BUILTIN, OUTPUT);
}

bool readTAPSection(File& file)
{
	free(g_zxTape.pTapBuffer); g_zxTape.pTapBuffer = NULL;
	if (file.readBytes((char*)&g_zxTape.tapSize, 2) != 2) return false;
	if ((g_zxTape.pTapBuffer = (uint8_t*)malloc(g_zxTape.tapSize + 1)) == NULL)
	{
		{
			DBG_PRINTF("Error allocating %ld bytes\n", g_zxTape.tapSize);
			return false;
		}

	}
	g_zxTape.pTapBuffer[g_zxTape.tapSize] = 0x00;
	if (file.readBytes((char*)g_zxTape.pTapBuffer, g_zxTape.tapSize) != g_zxTape.tapSize) return false;
	return true;
}

void loop()
{
	bool trace = false;
	if (g_sysMode == modeEmulator)
	{
#ifdef DBG
		static uint32_t loopCounter = 0;
#endif // DBG
		if (g_zxTape.tapActive && !g_zxEmulator.tapeActive())
		{
			if (g_zxTape.tapePause == -1) g_zxTape.tapePause = millis();
			if (millis() - g_zxTape.tapePause > 500)
			{
				if (!readTAPSection(g_zxTape.tapFile))
				{
					g_zxTape.tapFile.close(); g_zxTape.tapActive = false;
					free(g_zxTape.pTapBuffer); g_zxTape.pTapBuffer = NULL;
					SD.end();
				}
				else
				{
					g_zxEmulator.startTape(g_zxTape.pTapBuffer, g_zxTape.tapSize);
					g_zxTape.tapePause = -1;
				}
			}
		}
		int zxKey;
		while ((zxKey = Serial.read()) != -1)
		{
			if (zxKey == '#')
			{
				uint32_t emulTime, maxTime;
				if ((zxKey = Serial.read()) != -1)
					switch (zxKey)
					{
					case '5':
						emulTime = g_zxEmulator.getEmulationTime();
						maxTime = g_zxEmulator.getMaxEmulationTime();
						DBG_PRINTF("Free mem: %6d, ", rp2040.getFreeHeap());
						DBG_PRINTF("Core temp: %.2f'C, FPS: %3.1f (min: %3.1f), Overstates: %d\n", analogReadTemp(), 1000000.0 / emulTime, 1000000.0 / maxTime, g_zxEmulator.getOverstates());
						break;
					default:
						break;
					}
			}
		}
		g_zxEmulator.loopZ80();
		uint8_t fKeys = g_mainKeyboard.getData(9);
		if (fKeys == 0x01 && !g_zxTape.tapActive)
		{
			g_zxEmulator.storeState("/state"); g_sysMode = systemMode::modeBrowser; delay(500);
		}
		if (fKeys == 0x02)
		{
			g_zxEmulator.storeState("/save"); delay(500);
		}
		if (fKeys == 0x04)
		{
			g_zxEmulator.restoreState("/save"); delay(500);
		}
		if (fKeys == 0x18)
		{
			delay(500);
			g_zxEmulator.resetZ80();
		}
		if (fKeys == 0x10)
		{
			delay(500);
			if (g_zxTape.tapActive)
			{
				g_zxEmulator.stopTape();
				g_zxTape.tapFile.close(); g_zxTape.tapActive = false; g_zxTape.tapePause = -1;
				free(g_zxTape.pTapBuffer); g_zxTape.pTapBuffer = NULL;
			}
			else
			{
				if (SD.begin(SS, SPI_FULL_SPEED) && g_zxTape.fileName.length() != 0 && (g_zxTape.tapFile = SD.open(g_zxTape.fileName, "r")))
				{
					if (readTAPSection(g_zxTape.tapFile))
					{
						g_zxTape.tapActive = true; g_zxTape.tapePause = -1;
						g_zxEmulator.startTape(g_zxTape.pTapBuffer, g_zxTape.tapSize);
					}
				}
				else
				{
					DBG_PRINTLN("SD mount failed, error opening file or no file selected.");
				}
			}
		}
#ifdef DBG
		//uint32_t emulTime = g_zxEmulator.getEmulationTime(), maxTime = g_zxEmulator.getMaxEmulationTime();
		//loopCounter++;
		//if (loopCounter > 89)
		//{
		//	DBG_PRINTF("Core temp: %.2f'C, FPS: %3.1f (min: %3.1f)\n", analogReadTemp(), 1000000.0 / emulTime, 1000000.0 / maxTime);
		//	loopCounter = 0;
		//}
#endif // DBG
	}
	else
	{
		bool isROMChanged = g_cardBrowser.run();
		delay(500);
		if (!isROMChanged)
			g_zxEmulator.restoreState("/state");
		else
		{
			g_zxEmulator.loadROMFile(g_cardBrowser.getROMFileName());
			g_zxEmulator.resetZ80();
		}
		g_zxTape.fileName = g_cardBrowser.getSelectedFile();
		g_zxEmulator.enableSound(g_cardBrowser.getSoundState());
		g_zxEmulator.tapeMode(g_cardBrowser.getTapeMode());
		g_mainDisplay.setAddrWindow(0, 0, 319, 239);
		g_sysMode = systemMode::modeEmulator;
	}
}

void setup1()
{
	g_soundBeeper.init();
}

void loop1()
{
	g_soundBeeper.update();
}
