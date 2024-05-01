#include "Common.h"
#include "ZXSpectrum.h"
#include "ZXPeripherals.h"
#include "Browser.h"

Display g_mainDisplay;
Sound g_mainSpeaker;
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
	uint16_t tapSize;
	File tapFile;
	bool tapActive;
	int32_t tapePause;
	String fileName;
} g_zxTape = {
		.tapSize = 0,
		.tapActive = false,
		.tapePause = -1,
		.fileName = ""
};

void setup()
{
//	vreg_set_voltage(VREG_VOLTAGE_1_15);
//	set_sys_clock_khz(250000, true);
#if defined(DBG)
	Serial.begin(115200);
	delay(1000);
#endif // DBG

	g_mainDisplay.init();
	delay(100);
	g_mainKeyboard.init();
	g_zxEmulator.init(&g_mainDisplay, &g_mainKeyboard);
	g_cardBrowser.loadSettings();
	g_zxEmulator.setMachine(g_cardBrowser.getMachinType());
	g_zxEmulator.enableSound(g_cardBrowser.getSoundState());
	rp2040.fifo.push(SET_VOL | g_cardBrowser.getSoundVolume());
	g_zxEmulator.resetZ80();
}

bool readTAPSection(File& file)
{
	if (file.readBytes((char*)&g_zxTape.tapSize, 2) != 2) return false;
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
					SD.end();
				}
				else
				{
					g_zxEmulator.startTape(&g_zxTape.tapFile, g_zxTape.tapSize);
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
					case 's': // Show stats
					case 'S':
						emulTime = g_zxEmulator.getEmulationTime();
						maxTime = g_zxEmulator.getMaxEmulationTime();
						DBG_PRINTF("Free mem: %6d, ", rp2040.getFreeHeap());
						DBG_PRINTF("Core temp: %.2f'C, FPS: %3.1f (min: %3.1f)\n", analogReadTemp(), 1000000.0 / emulTime, 1000000.0 / maxTime);
						break;
					case 'd': // For debug purposes
					case 'D':
						//DBG_PRINTF("Debug %s\n", (g_zxEmulator.toggleDebug() ? "On" : "Off"));
						break;
					//case 'w':
					//case 'W':
					//{
					//	LittleFS.begin();
					//	SD.begin(SS, SPI_FULL_SPEED);
					//	File inFile, outFile;
					//	inFile = LittleFS.open("/soundout.bin", "r");
					//	outFile = SD.open("/soundout.bin", "w");
					//	uint8_t data;
					//	int i;
					//	for(i = 0;;i++)
					//	{
					//		if (inFile.read(&data, 1) == 0) break;
					//		outFile.write(data);
					//	}
					//	DBG_PRINTF("Readed %d bytes, wrote %d bytes to SD\n", i, outFile.size());
					//	outFile.close();
					//	inFile.close();
					//	SD.end();
					//	LittleFS.end();
					//	break;
					//}
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
			}
			else
			{
				if (SD.begin(SS, SPI_FULL_SPEED) && g_zxTape.fileName.length() != 0 && (g_zxTape.tapFile = SD.open(g_zxTape.fileName)))
				{

					if (readTAPSection(g_zxTape.tapFile))
					{
						g_zxTape.tapActive = true; g_zxTape.tapePause = -1;
						g_zxEmulator.startTape(&g_zxTape.tapFile, g_zxTape.tapSize);
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
		bool isComputerChanged = g_cardBrowser.run();
		delay(500);
		if (!isComputerChanged)
			g_zxEmulator.restoreState("/state");
		else
		{
			g_zxEmulator.setMachine(g_cardBrowser.getMachinType());
			g_zxEmulator.resetZ80();
		}
		g_zxTape.fileName = g_cardBrowser.getSelectedFile();
		g_zxEmulator.enableSound(g_cardBrowser.getSoundState());
		rp2040.fifo.push(SET_VOL | g_cardBrowser.getSoundVolume());
		g_mainDisplay.setAddrWindow(0, 0, 319, 239);
		g_sysMode = systemMode::modeEmulator;
	}
}

void setup1()
{
	g_mainSpeaker.init();
}

void loop1()
{
	if (g_sysMode == modeEmulator) g_mainSpeaker.update();
}
