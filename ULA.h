#ifndef VIDEO_h
#define VIDEO_h

#include <inttypes.h>
//#include "ESPectrum.h"
//#include "VGA/VGA6Bit.h"
#include <list>

#define SPEC_W 256
#define SPEC_H 192

#define TSTATES_PER_LINE 224
#define TSTATES_PER_LINE_128 228
#define TSTATES_PER_LINE_PENTAGON 224

#define TS_SCREEN_48           14335  // START OF ULA DRAW PAPER 48K
#define TS_SCREEN_128          14361  // START OF ULA DRAW PAPER 128K
#define TS_SCREEN_PENTAGON     17983  // START OF ULA DRAW PAPER PENTAGON

#define TS_BORDER_320x240 8948  // START OF BORDER 48
#define TS_BORDER_320x240_128 8878  // START OF BORDER 128
#define TS_BORDER_320x240_PENTAGON 12595  // START OF BORDER PENTAGON

#define TS_BORDER_360x200 13428  // START OF BORDER 48
#define TS_BORDER_360x200_128 13438  // START OF BORDER 128
#define TS_BORDER_360x200_PENTAGON 17075  // START OF BORDER PENTAGON

// Colors for 6 bit VGA mode
//                  //  BBGGRR 
#define BLACK       0b00000000
#define BLUE        0b00100000
#define RED         0b00000010
#define MAGENTA     0b00100010
#define GREEN       0b00001000
#define CYAN        0b00101000
#define YELLOW      0b00001010
#define WHITE       0b00101010
#define BRI_BLACK   0b00000000
#define BRI_BLUE    0b00110000
#define BRI_RED     0b00000011
#define BRI_MAGENTA 0b00110011
#define BRI_GREEN   0b00001100
#define BRI_CYAN    0b00111100
#define BRI_YELLOW  0b00001111
#define BRI_WHITE   0b00111111
#define ORANGE      0b00000111 // used in ESPectrum logo text

#define NUM_SPECTRUM_COLORS 17

class VIDEO
{
public:

    // Initialize video
    static void Init();

    // Reset video
    static void Reset();

    // Video draw functions
    static void EndFrame();
    static void Blank(unsigned int statestoadd, bool contended);
    static void Blank_Opcode(bool contended);
    static void Blank_Snow(unsigned int statestoadd, bool contended);
    static void Blank_Snow_Opcode(bool contended);
    // 48 / 128
    static void MainScreen_Blank(unsigned int statestoadd, bool contended);
    static void MainScreen_Blank_Opcode(bool contended);
    static void MainScreen(unsigned int statestoadd, bool contended);
//    static void MainScreen_OSD(unsigned int statestoadd, bool contended);
    static void MainScreen_Opcode(bool contended);
//    static void MainScreen_OSD_Opcode(bool contended);
    static void MainScreen_Blank_Snow(unsigned int statestoadd, bool contended);
    static void MainScreen_Blank_Snow_Opcode(bool contended);
    static void MainScreen_Snow(unsigned int statestoadd, bool contended);
    static void MainScreen_Snow_Opcode(bool contended);

    // static void DrawBorderFast();

    static void TopBorder_Blank();
    static void TopBorder();
    static void MiddleBorder();
    static void BottomBorder();
    static void BottomBorder_OSD();
    static void Border_Blank();

    static void TopBorder_Blank_Pentagon();
    static void TopBorder_Pentagon();
    static void MiddleBorder_Pentagon();
    static void BottomBorder_Pentagon();
    static void BottomBorder_OSD_Pentagon();

    static void (*Draw)(unsigned int, bool);
    static void (*Draw_Opcode)(bool);
    static void (*Draw_OSD169)(unsigned int, bool);
    static void (*Draw_OSD43)();

    static void (*DrawBorder)();

    static void vgataskinit(void* unused);

    static uint8_t* grmem;

    static uint16_t spectrum_colors[NUM_SPECTRUM_COLORS];

    static uint16_t offBmp[SPEC_H];
    static uint16_t offAtt[SPEC_H];

    static uint8_t borderColor;
    static uint32_t border32[8];
    static uint32_t brd;
    static bool brdChange;
    static bool brdnextframe;
    static uint32_t lastBrdTstate;

    static uint8_t tStatesPerLine;
    static int tStatesScreen;
    static int tStatesBorder;

    static uint8_t flashing;
    static uint8_t flash_ctr;

    static uint8_t att1;
    static uint8_t bmp1;
    static uint8_t att2;
    static uint8_t bmp2;
    // static bool opCodeFetch;

    static uint8_t dispUpdCycle;
    static bool snow_att;
    static bool dbl_att;
    static uint8_t lastbmp;
    static uint8_t lastatt;
    static uint8_t snowpage;
    static uint8_t snowR;
    static bool snow_toggle;

    static int VsyncFinetune[2];

    static uint32_t framecnt; // Frames elapsed

};

#define zxColor(color,bright) VIDEO::spectrum_colors[bright ? color + 8 : color]

#endif // VIDEO_h
