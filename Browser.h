#pragma once
#include "Common.h"
#include "ZXPeripherals.h"

class Browser
{
	Display* m_pDisplayInstance = NULL;
	Keyboard* m_pKeyboardInstance = NULL;
	String m_currDir = "/";
	String m_fileList;
	String m_browserWindow[21];
	int m_browseFrom, m_selectionPos, m_filesCount;
	uint8_t bufferSwitch = 0;
	String m_selectedFile = "";
	union
	{
		struct
		{
			uint8_t soundOn : 1;
			uint8_t soundVol : 2;
			uint8_t machineType : 1;
			uint8_t unused : 4;
		} settings;
		uint8_t raw;
	} m_settingsData = { .raw = 0x07 };
	void drawChar(const uint8_t ch, uint8_t posX, uint16_t foreColor, uint16_t backColor);
	void listFiles();
	void dir();
	void getFileList();
	void chDir();
	void drawSettingsString();
	void drawSelectedMachine();
	void drawFooter();
public:
	Browser(Display* pDisplayInstance, Keyboard* pKeyboardInstance) : m_pDisplayInstance(pDisplayInstance), m_pKeyboardInstance(pKeyboardInstance) {};
	~Browser() {};
	void drawString(const String textStr, uint8_t posX, uint8_t posY, uint16_t foreColor, uint16_t backColor, bool isAnsi = true);
	bool run();
	String getSelectedFile() { return m_selectedFile; };
	bool getSoundState() { return m_settingsData.settings.soundOn; };
	uint8_t getSoundVolume() { return m_settingsData.settings.soundVol; };
	bool getMachinType() { return m_settingsData.settings.machineType; };
	void saveSettings();
	void loadSettings();
};

