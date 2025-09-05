#include "ULA.h"

#include "Video.h"
#include "VidPrecalc.h"
#include "CPU.h"
#include "MemESP.h"
#include "Config.h"
#include "OSDMain.h"
#include "hardconfig.h"
#include "hardpins.h"
#include "Z80_JLS/z80.h"
#include "Z80_JLS/z80operations.h"
#include "psram_spi.h"

#pragma GCC optimize("O3")

//extern "C" uint8_t* getLineBuffer(int line) {
//    if (!VIDEO::vga.frameBuffer) return 0;
//    return (uint8_t*)VIDEO::vga.frameBuffer[line];
//}

uint16_t VIDEO::spectrum_colors[NUM_SPECTRUM_COLORS] = {
    BLACK,     BLUE,     RED,     MAGENTA,     GREEN,     CYAN,     YELLOW,     WHITE,
    BRI_BLACK, BRI_BLUE, BRI_RED, BRI_MAGENTA, BRI_GREEN, BRI_CYAN, BRI_YELLOW, BRI_WHITE,
    ORANGE
};

uint8_t VIDEO::borderColor = 0;
uint32_t VIDEO::brd;
uint32_t VIDEO::border32[8];
uint8_t VIDEO::flashing = 0;
uint8_t VIDEO::flash_ctr = 0;
uint8_t VIDEO::tStatesPerLine;
int VIDEO::tStatesScreen;
int VIDEO::tStatesBorder;
uint8_t* VIDEO::grmem;
uint16_t VIDEO::offBmp[SPEC_H];
uint16_t VIDEO::offAtt[SPEC_H];
int VIDEO::VsyncFinetune[2];
uint32_t VIDEO::framecnt = 0;
uint8_t VIDEO::dispUpdCycle;
uint8_t VIDEO::att1;
uint8_t VIDEO::bmp1;
uint8_t VIDEO::att2;
uint8_t VIDEO::bmp2;
bool VIDEO::snow_att = false;
bool VIDEO::dbl_att = false;
// bool VIDEO::opCodeFetch;
uint8_t VIDEO::lastbmp;
uint8_t VIDEO::lastatt;
uint8_t VIDEO::snowpage;
uint8_t VIDEO::snowR;
bool VIDEO::snow_toggle = false;

static uint32_t* lineptr32;

static unsigned int tstateDraw; // Drawing start point (in Tstates)
static unsigned int linedraw_cnt;
static unsigned int lin_end, lin_end2 /*, lin_end3*/;
static unsigned int coldraw_cnt;
static unsigned int col_end;
static unsigned int video_rest;
static unsigned int video_opcode_rest;
static unsigned int curline;

static unsigned int bmpOffset;  // offset for bitmap in graphic memory
static unsigned int attOffset;  // offset for attrib in graphic memory

static const uint8_t wait_st[128] = {
    6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
    6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
    6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
    6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
    6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
    6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
    6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
    6, 5, 4, 3, 2, 1, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0,
}; // sequence of wait states

void (*VIDEO::Draw)(unsigned int, bool) = &VIDEO::Blank;
void (*VIDEO::Draw_Opcode)(bool) = &VIDEO::Blank_Opcode;
void (*VIDEO::Draw_OSD169)(unsigned int, bool) = &VIDEO::MainScreen;
void (*VIDEO::Draw_OSD43)() = &VIDEO::BottomBorder;

void (*VIDEO::DrawBorder)() = &VIDEO::TopBorder_Blank;


static uint32_t* brdptr32;
static uint16_t* brdptr16;

uint32_t VIDEO::lastBrdTstate;
bool VIDEO::brdChange = false;
bool VIDEO::brdnextframe = true;

#define ULA_SWAP(y) ((y & 0xC0) | ((y & 0x38) >> 3) | ((y & 0x07) << 3))
void precalcULASWAP() 
{
    for (int i = 0; i < SPEC_H; i++) 
    {
        VIDEO::offBmp[i] = ULA_SWAP(i) << 5;
        VIDEO::offAtt[i] = ((i >> 3) << 5) + 0x1800;
    }
}

void precalcborder32()
{
    for (int i = 0; i < 8; i++) {
        uint8_t border = zxColor(i, 0);
        VIDEO::border32[i] = border | (border << 8) | (border << 16) | (border << 24);
    }
}

void VIDEO::vgataskinit(void* unused) 
{
    uint8_t Mode;
    Mode = 16 + ((Config::arch == "48K") ? 0 : (Config::arch == "128K" || Config::arch == "ALF" ? 2 : 4)) + (Config::aspect_16_9 ? 1 : 0);
    OSD::scrW = vidmodes[Mode][vmodeproperties::hRes];
    OSD::scrH = vidmodes[Mode][vmodeproperties::vRes] / vidmodes[Mode][vmodeproperties::vDiv];
    vga.VGA6Bit_useinterrupt = true; // ????
    // Init mode
    vga.init(Mode, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
    for (;;) {}
}

void VIDEO::Init() 
{
    //int Mode = Config::aspect_16_9 ? 2 : 0;
    //Mode += Config::scanlines;
    //OSD::scrW = vidmodes[Mode][vmodeproperties::hRes];
    //OSD::scrH = (vidmodes[Mode][vmodeproperties::vRes] / vidmodes[Mode][vmodeproperties::vDiv]) >> Config::scanlines;
    //vga.VGA6Bit_useinterrupt = false;
    //vga.init(Mode, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);

    //// Precalculate colors for current VGA mode
    //for (int i = 0; i < NUM_SPECTRUM_COLORS; i++) {
    //    // printf("RGBAXMask: %d, SBits: %d\n",(int)VIDEO::vga.RGBAXMask,(int)VIDEO::vga.SBits);
    //    // printf("Before: %d -> %d, ",i,(int)spectrum_colors[i]);
    //    spectrum_colors[i] = (spectrum_colors[i] & VIDEO::vga.RGBAXMask) | VIDEO::vga.SBits;
    //    // printf("After : %d -> %d\n",i,(int)spectrum_colors[i]);
    //}

    //for (int n = 0; n < 16; n++)
    //    AluByte[n] = (unsigned int*)AluBytes[bitRead(VIDEO::vga.SBits, 7)][n];

    precalcULASWAP();   // precalculate ULA SWAP values
    precalcborder32();  // Precalc border 32 bits values
}

void VIDEO::Reset() {

    borderColor = 7;
    brd = border32[7];


    if (Config::arch == "48K") 
    {
        tStatesPerLine = TSTATES_PER_LINE;
        tStatesScreen = TS_SCREEN_48;
        tStatesBorder = TS_BORDER_320x240;
        DrawBorder = TopBorder_Blank;
    }
    else 
    {
        tStatesPerLine = TSTATES_PER_LINE_128;
        tStatesScreen = TS_SCREEN_128;
        tStatesBorder = TS_BORDER_320x240_128;
        VsyncFinetune[0] = is169 ? 0 : 0;
        VsyncFinetune[1] = is169 ? 0 : 0;

        DrawBorder = TopBorder_Blank;
    }

        lin_end = 24;
        lin_end2 = 216;

    //grmem = MemESP::videoLatch ? MemESP::ram[7].direct() : MemESP::ram[5].direct();

 //   VIDEO::snow_toggle = Config::arch != "P1024" && Config::arch != "P512" && Config::arch != "Pentagon" ? Config::render : false;

    //if (VIDEO::snow_toggle) {
    //    Draw = &Blank_Snow;
    //    Draw_Opcode = &Blank_Snow_Opcode;
    //}
    //else {
        Draw = &Blank;
        Draw_Opcode = &Blank_Opcode;
    //}

    // Restart border drawing
    lastBrdTstate = tStatesBorder;
    brdChange = false;
    brdnextframe = true;
}

//  VIDEO DRAW FUNCTIONS
VIDEO::MainScreen_Blank(unsigned int statestoadd, bool contended) 
{

    CPU::tstates += statestoadd;

    if (CPU::tstates >= tstateDraw) {

        if (brdChange) DrawBorder(); // Needed to avoid tearing in demos like Gabba (Pentagon)

        lineptr32 = (uint32_t*)(vga.frameBuffer[linedraw_cnt]) + (is169 ? 13 : 8);

        coldraw_cnt = 0;

        curline = linedraw_cnt - lin_end;
        bmpOffset = offBmp[curline];
        attOffset = offAtt[curline];

#ifdef DIRTY_LINES
        // Force line draw (for testing)
        // dirty_lines[curline] = 1;
#endif // DIRTY_LINES

        Draw = linedraw_cnt >= 176 && linedraw_cnt <= 191 ? Draw_OSD169 : MainScreen;
        Draw_Opcode = MainScreen_Opcode;

        video_rest = CPU::tstates - tstateDraw;
        Draw(0, false);

    }

}

IRAM_ATTR void VIDEO::MainScreen_Blank_Opcode(bool contended) { MainScreen_Blank(4, contended); }

IRAM_ATTR void VIDEO::MainScreen_Blank_Snow(unsigned int statestoadd, bool contended) {

    CPU::tstates += statestoadd;

    if (CPU::tstates >= tstateDraw) {

        if (brdChange) DrawBorder();

        lineptr32 = (uint32_t*)(vga.frameBuffer[linedraw_cnt]) + (is169 ? 13 : 8);

        coldraw_cnt = 0;

        curline = linedraw_cnt - lin_end;
        bmpOffset = offBmp[curline];
        attOffset = offAtt[curline];

        snowpage = MemESP::videoLatch ? 7 : 5;

        dispUpdCycle = 0; // For ULA cycle perfect emulation

#ifdef DIRTY_LINES
        // Force line draw (for testing)
        // dirty_lines[curline] = 1;
#endif // DIRTY_LINES

        Draw = &MainScreen_Snow;
        Draw_Opcode = &MainScreen_Snow_Opcode;

        // For ULA cycle perfect emulation
        int vid_rest = CPU::tstates - tstateDraw;
        if (vid_rest) {
            CPU::tstates = tstateDraw;
            Draw(vid_rest, false);
        }

    }

}

IRAM_ATTR void VIDEO::MainScreen_Blank_Snow_Opcode(bool contended) {

    CPU::tstates += 4;

    if (CPU::tstates >= tstateDraw) {

        if (brdChange) DrawBorder();

        lineptr32 = (uint32_t*)(vga.frameBuffer[linedraw_cnt]) + (is169 ? 13 : 8);

        coldraw_cnt = 0;

        curline = linedraw_cnt - lin_end;
        bmpOffset = offBmp[curline];
        attOffset = offAtt[curline];

        snowpage = MemESP::videoLatch ? 7 : 5;

        dispUpdCycle = 0; // For ptime-128 compliant version

#ifdef DIRTY_LINES
        // Force line draw (for testing)
        // dirty_lines[curline] = 1;
#endif // DIRTY_LINES

        Draw = &MainScreen_Snow;
        Draw_Opcode = &MainScreen_Snow_Opcode;

        // For ULA cycle perfect emulation
        video_opcode_rest = CPU::tstates - tstateDraw;
        if (video_opcode_rest) {
            CPU::tstates = tstateDraw;
            Draw_Opcode(false);
            video_opcode_rest = 0;
        }

    }

}

#ifndef DIRTY_LINES

// ----------------------------------------------------------------------------------
// Fast video emulation with no ULA cycle emulation and no snow effect support
// ----------------------------------------------------------------------------------
/*IRAM_ATTR*/ void VIDEO::MainScreen(unsigned int statestoadd, bool contended) {

    if (contended) statestoadd += wait_st[CPU::tstates - tstateDraw];

    CPU::tstates += statestoadd;
    statestoadd += video_rest;
    video_rest = statestoadd & 0x03;
    unsigned int loopCount = statestoadd >> 2;
    coldraw_cnt += loopCount;

    if (coldraw_cnt >= 32) {
        tstateDraw += tStatesPerLine;
        if (++linedraw_cnt == lin_end2) {
            Draw = &Blank;
            Draw_Opcode = &Blank_Opcode;
        }
        else {
            Draw = &MainScreen_Blank;
            Draw_Opcode = &MainScreen_Blank_Opcode;
        }
        loopCount -= coldraw_cnt - 32;
    }

    for (; loopCount--;) {
        uint8_t att = grmem[attOffset++];
        uint8_t bmp = att & flashing ? ~grmem[bmpOffset++] : grmem[bmpOffset++];
        *lineptr32++ = AluByte[bmp >> 4][att];
        *lineptr32++ = AluByte[bmp & 0xF][att];
    }

}

IRAM_ATTR void VIDEO::MainScreen_OSD(unsigned int statestoadd, bool contended) {

    if (contended) statestoadd += wait_st[CPU::tstates - tstateDraw];

    CPU::tstates += statestoadd;
    statestoadd += video_rest;
    video_rest = statestoadd & 0x03;
    unsigned int loopCount = statestoadd >> 2;
    unsigned int coldraw_osd = coldraw_cnt;
    coldraw_cnt += loopCount;

    if (coldraw_cnt >= 32) {
        tstateDraw += tStatesPerLine;
        if (++linedraw_cnt == lin_end2) {
            Draw = &Blank;
            Draw_Opcode = &Blank_Opcode;
        }
        else {
            Draw = &MainScreen_Blank;
            Draw_Opcode = &MainScreen_Blank_Opcode;
        }
        loopCount -= coldraw_cnt - 32;
    }

    for (; loopCount--;) {
        if (coldraw_osd >= 13 && coldraw_osd <= 30) {
            lineptr32 += 2;
            attOffset++;
            bmpOffset++;
        }
        else {
            uint8_t att = grmem[attOffset++];
            uint8_t bmp = (att & flashing) ? ~grmem[bmpOffset++] : grmem[bmpOffset++];
            *lineptr32++ = AluByte[bmp >> 4][att];
            *lineptr32++ = AluByte[bmp & 0xF][att];
        }
        coldraw_osd++;
    }

}

IRAM_ATTR void VIDEO::MainScreen_Opcode(bool contended) { Draw(4, contended); }

// ----------------------------------------------------------------------------------
// ULA cycle perfect emulation with snow effect support
// ----------------------------------------------------------------------------------
IRAM_ATTR void VIDEO::MainScreen_Snow(unsigned int statestoadd, bool contended) {

    bool do_stats = false;

    if (contended) statestoadd += wait_st[coldraw_cnt]; // [CPU::tstates - tstateDraw];

    CPU::tstates += statestoadd;

    unsigned int col_osd = coldraw_cnt >> 2;
    if (linedraw_cnt >= 176 && linedraw_cnt <= 191) do_stats = (VIDEO::Draw_OSD169 == VIDEO::MainScreen_OSD);

    coldraw_cnt += statestoadd;

    if (coldraw_cnt >= 128) {
        tstateDraw += tStatesPerLine;
        if (++linedraw_cnt == lin_end2) {
            Draw = &Blank_Snow;
            Draw_Opcode = &Blank_Snow_Opcode;
        }
        else {
            Draw = &MainScreen_Blank_Snow;
            Draw_Opcode = &MainScreen_Blank_Snow_Opcode;
        }
        statestoadd -= coldraw_cnt - 128;
    }

    for (; statestoadd--;) {

        switch (dispUpdCycle) {

            // In Weiv's Spectramine cycle starts in 2 and half black strip shows at 14349 in ptime-128.tap (early timings).
            // In SpecEmu cycle starts in 3, black strip at 14350. Will use Weiv's data for now.
        case 2:
            bmp1 = grmem[bmpOffset++];
            lastbmp = bmp1;
            break;
        case 3:
            if (snow_att) {
                att1 = MemESP::ram[snowpage].direct()[(attOffset++ & 0xff80) | snowR];  // get attribute byte
                snow_att = false;
            }
            else
                att1 = grmem[attOffset++];  // get attribute byte                

            lastatt = att1;

            if (do_stats && (col_osd >= 13 && col_osd <= 30)) {
                lineptr32 += 2;
            }
            else {
                if (att1 & flashing) bmp1 = ~bmp1;
                *lineptr32++ = AluByte[bmp1 >> 4][att1];
                *lineptr32++ = AluByte[bmp1 & 0xF][att1];
            }

            col_osd++;

            break;
        case 4:
            bmp2 = grmem[bmpOffset++];
            break;
        case 5:
            if (dbl_att) {
                att2 = lastatt;
                attOffset++;
                dbl_att = false;
            }
            else
                att2 = grmem[attOffset++];  // get attribute byte

            if (do_stats && (col_osd >= 13 && col_osd <= 30)) {
                lineptr32 += 2;
            }
            else {
                if (att2 & flashing) bmp2 = ~bmp2;
                *lineptr32++ = AluByte[bmp2 >> 4][att2];
                *lineptr32++ = AluByte[bmp2 & 0xF][att2];

            }

            col_osd++;

        }

        ++dispUpdCycle &= 0x07; // Update the cycle counter.

    }

}

// ----------------------------------------------------------------------------------
// ULA cycle perfect emulation with snow effect support
// ----------------------------------------------------------------------------------
IRAM_ATTR void VIDEO::MainScreen_Snow_Opcode(bool contended) {

    int snow_effect = 0;
    unsigned int addr;
    bool do_stats = false;

    unsigned int statestoadd = video_opcode_rest ? video_opcode_rest : 4;

    if (contended) statestoadd += wait_st[coldraw_cnt]; // [CPU::tstates - tstateDraw];

    CPU::tstates += statestoadd;

    unsigned int col_osd = coldraw_cnt >> 2;
    if (linedraw_cnt >= 176 && linedraw_cnt <= 191) do_stats = (VIDEO::Draw_OSD169 == VIDEO::MainScreen_OSD);

    coldraw_cnt += statestoadd;

    if (coldraw_cnt >= 128) {
        tstateDraw += tStatesPerLine;
        if (++linedraw_cnt == lin_end2) {
            Draw = &Blank_Snow;
            Draw_Opcode = &Blank_Snow_Opcode;
        }
        else {
            Draw = &MainScreen_Blank_Snow;
            Draw_Opcode = &MainScreen_Blank_Snow_Opcode;
        }

        statestoadd -= coldraw_cnt - 128;

    }

    if (dispUpdCycle == 6) {
        dispUpdCycle = 2;
        return;
    }

    // Determine if snow effect can be applied
    uint8_t page = Z80::getRegI() & 0xc0;
    if (page == 0x40) { // Snow 48K, 128K
        snow_effect = 1;
        snowpage = MemESP::videoLatch ? 7 : 5;
    }
    else if (Z80Ops::is128 && (MemESP::bankLatch & 0x01) && page == 0xc0) {  // Snow 128K
        snow_effect = 1;
        if (MemESP::bankLatch == 1 || MemESP::bankLatch == 3)
            snowpage = MemESP::videoLatch ? 3 : 1;
        else
            snowpage = MemESP::videoLatch ? 7 : 5;
    }

    for (; statestoadd--;) {

        switch (dispUpdCycle) {

            // In Weiv's Spectramine cycle starts in 2 and half black strip shows at 14349 in ptime-128.tap (early timings).
            // In SpecEmu cycle starts in 3, black strip at 14350. Will use Weiv's data for now.

        case 2:

            if (snow_effect && statestoadd == 0) {
                snowR = Z80::getRegR() & 0x7f;
                bmp1 = MemESP::ram[snowpage].direct()[(bmpOffset++ & 0xff80) | snowR];
                snow_att = true;
            }
            else
                bmp1 = grmem[bmpOffset++];

            lastbmp = bmp1;

            break;

        case 3:

            if (snow_att) {
                att1 = MemESP::ram[snowpage].direct()[(attOffset++ & 0xff80) | snowR];  // get attribute byte
                snow_att = false;
            }
            else
                att1 = grmem[attOffset++];  // get attribute byte                

            lastatt = att1;

            if (do_stats && (col_osd >= 13 && col_osd <= 30)) {
                lineptr32 += 2;
            }
            else {
                if (att1 & flashing) bmp1 = ~bmp1;
                *lineptr32++ = AluByte[bmp1 >> 4][att1];
                *lineptr32++ = AluByte[bmp1 & 0xF][att1];
            }

            col_osd++;

            break;

        case 4:

            if (snow_effect && statestoadd == 0) {
                bmp2 = lastbmp;
                bmpOffset++;
                dbl_att = true;
            }
            else
                bmp2 = grmem[bmpOffset++];

            break;

        case 5:

            if (dbl_att) {
                att2 = lastatt;
                attOffset++;
                dbl_att = false;
            }
            else
                att2 = grmem[attOffset++];  // get attribute byte

            if (do_stats && (col_osd >= 13 && col_osd <= 30)) {
                lineptr32 += 2;
            }
            else {
                if (att2 & flashing) bmp2 = ~bmp2;
                *lineptr32++ = AluByte[bmp2 >> 4][att2];
                *lineptr32++ = AluByte[bmp2 & 0xF][att2];
            }

            col_osd++;

        }

        ++dispUpdCycle &= 0x07; // Update the cycle counter.

    }

}

#else

// IRAM_ATTR void VIDEO::MainScreen(unsigned int statestoadd, bool contended) {    

//     if (contended) statestoadd += wait_st[CPU::tstates - tstateDraw];

//     CPU::tstates += statestoadd;
//     statestoadd += video_rest;
//     video_rest = statestoadd & 0x03;
//     unsigned int loopCount = statestoadd >> 2;
//     coldraw_cnt += loopCount;

//     if (coldraw_cnt >= 32) {
//         tstateDraw += tStatesPerLine;
//         Draw = ++linedraw_cnt == lin_end2 ? &Blank : &MainScreen_Blank;
//         if (dirty_lines[curline]) {
//             loopCount -= coldraw_cnt - 32;
//             for (;loopCount--;) {
//                 uint8_t att = grmem[attOffset++];
//                 uint8_t bmp = att & flashing ? ~grmem[bmpOffset++] : grmem[bmpOffset++];
//                 *lineptr32++ = AluByte[bmp >> 4][att];
//                 *lineptr32++ = AluByte[bmp & 0xF][att];
//             }
//             dirty_lines[curline] &= 0x80;
//         }
//         return;
//     }

//     if (dirty_lines[curline]) {
//         for (;loopCount--;) {
//             uint8_t att = grmem[attOffset++];
//             uint8_t bmp = att & flashing ? ~grmem[bmpOffset++] : grmem[bmpOffset++];
//             *lineptr32++ = AluByte[bmp >> 4][att];
//             *lineptr32++ = AluByte[bmp & 0xF][att];
//         }
//     } else {
//         attOffset += loopCount;
//         bmpOffset += loopCount;
//         lineptr32 += loopCount << 1;
//     }

// }

#endif

IRAM_ATTR void VIDEO::Blank(unsigned int statestoadd, bool contended) { CPU::tstates += statestoadd; }
IRAM_ATTR void VIDEO::Blank_Opcode(bool contended) { CPU::tstates += 4; }
IRAM_ATTR void VIDEO::Blank_Snow(unsigned int statestoadd, bool contended) { CPU::tstates += statestoadd; }
IRAM_ATTR void VIDEO::Blank_Snow_Opcode(bool contended) { CPU::tstates += 4; }

IRAM_ATTR void VIDEO::EndFrame() {

    linedraw_cnt = is169 ? 4 : 24;

    tstateDraw = tStatesScreen;

    if (VIDEO::snow_toggle) {
        Draw = &MainScreen_Blank_Snow;
        Draw_Opcode = &MainScreen_Blank_Snow_Opcode;
    }
    else {
        Draw = &MainScreen_Blank;
        Draw_Opcode = &MainScreen_Blank_Opcode;
    }

    if (brdChange) {
        DrawBorder();
        brdnextframe = true;
    }
    else {
        if (brdnextframe) {
            DrawBorder();
            brdnextframe = false;
        }
    }

    // Restart border drawing
    DrawBorder = Z80Ops::isPentagon ? &TopBorder_Blank_Pentagon : &TopBorder_Blank;
    lastBrdTstate = tStatesBorder;
    brdChange = false;

    framecnt++;
}

//----------------------------------------------------------------------------------------------------------------
// Border Drawing
//----------------------------------------------------------------------------------------------------------------

// IRAM_ATTR void VIDEO::DrawBorderFast() {

//     uint8_t border = zxColor(borderColor,0);

//     int i = 0;

//     // Top border
//     for (; i < lin_end; i++) memset((uint32_t *)(vga.frameBuffer[i]),border, vga.xres);

//     // Paper border
//     int brdsize = (vga.xres - SPEC_W) >> 1;
//     for (; i < lin_end2; i++) {
//         memset((uint32_t *)(vga.frameBuffer[i]), border, brdsize);
//         memset((uint32_t *)(vga.frameBuffer[i] + vga.xres - brdsize), border, brdsize);
//     }

//     // Bottom border
//     for (; i < OSD::scrH; i++) memset((uint32_t *)(vga.frameBuffer[i]),border, vga.xres);

// }

static int brdcol_cnt = 0;
static int brdlin_cnt = 0;

IRAM_ATTR void VIDEO::TopBorder_Blank() {
    if (CPU::tstates >= tStatesBorder) {
        brdcol_cnt = 0;
        brdlin_cnt = 0;
        brdptr32 = (uint32_t*)(vga.frameBuffer[brdlin_cnt]) + (is169 ? 5 : 0);
        DrawBorder = &TopBorder;
        DrawBorder();
    }
}

IRAM_ATTR void VIDEO::TopBorder() {

    while (lastBrdTstate <= CPU::tstates) {

        *brdptr32++ = brd;
        *brdptr32++ = brd;

        lastBrdTstate += 4;

        brdcol_cnt++;

        if (brdcol_cnt == 40) {
            brdlin_cnt++;
            brdptr32 = (uint32_t*)(vga.frameBuffer[brdlin_cnt]) + (is169 ? 5 : 0);
            brdcol_cnt = 0;
            lastBrdTstate += Z80Ops::is128 ? 68 : 64;
            if (brdlin_cnt == (is169 ? 4 : 24)) {
                DrawBorder = &MiddleBorder;
                MiddleBorder();
                return;
            }
        }

    }

}

IRAM_ATTR void VIDEO::MiddleBorder() {
    while (lastBrdTstate <= CPU::tstates) {

        *brdptr32++ = brd;
        *brdptr32++ = brd;

        lastBrdTstate += 4;

        brdcol_cnt++;

        if (brdcol_cnt == 4) {
            lastBrdTstate += 128;
            brdptr32 += 64;
            brdcol_cnt = 36;
        }
        else if (brdcol_cnt == 40) {
            brdlin_cnt++;
            brdptr32 = (uint32_t*)(vga.frameBuffer[brdlin_cnt]) + (is169 ? 5 : 0);
            brdcol_cnt = 0;
            lastBrdTstate += Z80Ops::is128 ? 68 : 64;
            if (brdlin_cnt == (is169 ? 196 : 216)) {
                DrawBorder = Draw_OSD43;
                DrawBorder();
                return;
            }
        }

    }

}

IRAM_ATTR void VIDEO::BottomBorder() {

    while (lastBrdTstate <= CPU::tstates) {

        *brdptr32++ = brd;
        *brdptr32++ = brd;

        lastBrdTstate += 4;

        brdcol_cnt++;

        if (brdcol_cnt == 40) {
            brdlin_cnt++;
            brdptr32 = (uint32_t*)(vga.frameBuffer[brdlin_cnt]) + (is169 ? 5 : 0);
            brdcol_cnt = 0;
            lastBrdTstate += Z80Ops::is128 ? 68 : 64;
            if (brdlin_cnt == (is169 ? 200 : 240)) {
                DrawBorder = &Border_Blank;
                return;
            }
        }

    }

}

IRAM_ATTR void VIDEO::BottomBorder_OSD() {

    while (lastBrdTstate <= CPU::tstates) {

        if (brdlin_cnt < 220 || brdlin_cnt > 235) {
            *brdptr32++ = brd;
            *brdptr32++ = brd;
        }
        else if (brdcol_cnt < 21 || brdcol_cnt > 38) {
            *brdptr32++ = brd;
            *brdptr32++ = brd;
        }
        else {
            brdptr32 += 2;
        }

        lastBrdTstate += 4;

        brdcol_cnt++;

        if (brdcol_cnt == 40) {
            brdlin_cnt++;
            brdptr32 = (uint32_t*)(vga.frameBuffer[brdlin_cnt]) + (is169 ? 5 : 0);
            brdcol_cnt = 0;
            lastBrdTstate += Z80Ops::is128 ? 68 : 64;
            if (brdlin_cnt == 240) {
                DrawBorder = &Border_Blank;
                return;
            }
        }

    }

}

IRAM_ATTR void VIDEO::Border_Blank() {

}

static int brdcol_end = 0;
static int brdcol_end1 = 0;

IRAM_ATTR void VIDEO::TopBorder_Blank_Pentagon() {

    if (CPU::tstates >= tStatesBorder) {
        brdcol_cnt = is169 ? 10 : 0;
        brdcol_end = is169 ? 170 : 160;
        brdcol_end1 = is169 ? 26 : 16;
        brdlin_cnt = 0;
        brdptr16 = (uint16_t*)(vga.frameBuffer[brdlin_cnt]);
        DrawBorder = &TopBorder_Pentagon;
        DrawBorder();
    }

}

IRAM_ATTR void VIDEO::TopBorder_Pentagon() {

    while (lastBrdTstate <= CPU::tstates) {

        brdptr16[brdcol_cnt ^ 1] = (uint16_t)brd;

        lastBrdTstate++;

        brdcol_cnt++;

        if (brdcol_cnt == brdcol_end) {
            brdlin_cnt++;
            brdptr16 = (uint16_t*)(vga.frameBuffer[brdlin_cnt]);
            brdcol_cnt = is169 ? 10 : 0;
            lastBrdTstate += 64;
            if (brdlin_cnt == (is169 ? 4 : 24)) {
                DrawBorder = &MiddleBorder_Pentagon;
                MiddleBorder_Pentagon();
                return;
            }
        }

    }

}

IRAM_ATTR void VIDEO::MiddleBorder_Pentagon() {

    while (lastBrdTstate <= CPU::tstates) {

        brdptr16[brdcol_cnt ^ 1] = (uint16_t)brd;

        lastBrdTstate++;

        brdcol_cnt++;

        if (brdcol_cnt == brdcol_end1) {
            lastBrdTstate += 128;
            brdcol_cnt = 144 + (is169 ? 10 : 0);
        }
        else if (brdcol_cnt == brdcol_end) {
            brdlin_cnt++;
            brdptr16 = (uint16_t*)(vga.frameBuffer[brdlin_cnt]);
            brdcol_cnt = is169 ? 10 : 0;
            lastBrdTstate += 64;
            if (brdlin_cnt == (is169 ? 196 : 216)) {
                DrawBorder = Draw_OSD43;
                DrawBorder();
                return;
            }
        }

    }

}

IRAM_ATTR void VIDEO::BottomBorder_Pentagon() {

    while (lastBrdTstate <= CPU::tstates) {

        brdptr16[brdcol_cnt ^ 1] = (uint16_t)brd;

        lastBrdTstate++;

        brdcol_cnt++;

        if (brdcol_cnt == brdcol_end) {
            brdlin_cnt++;
            brdptr16 = (uint16_t*)(vga.frameBuffer[brdlin_cnt]);
            brdcol_cnt = is169 ? 10 : 0;
            lastBrdTstate += 64;
            if (brdlin_cnt == (is169 ? 200 : 240)) {
                DrawBorder = &Border_Blank;
                return;
            }
        }

    }

}

IRAM_ATTR void VIDEO::BottomBorder_OSD_Pentagon() {
    while (lastBrdTstate <= CPU::tstates) {
        if (brdlin_cnt < 220 || brdlin_cnt > 235)
            brdptr16[brdcol_cnt ^ 1] = (uint16_t)brd;
        else if (brdcol_cnt < 84 || brdcol_cnt > 155)
            brdptr16[brdcol_cnt ^ 1] = (uint16_t)brd;
        lastBrdTstate++;
        brdcol_cnt++;
        if (brdcol_cnt == brdcol_end) {
            brdlin_cnt++;
            brdptr16 = (uint16_t*)(vga.frameBuffer[brdlin_cnt]);
            brdcol_cnt = is169 ? 10 : 0;
            brdcol_cnt = 0;
            lastBrdTstate += 64;
            if (brdlin_cnt == 240) {
                DrawBorder = &Border_Blank;
                return;
            }
        }
    }
}

#define PSRAM_SHIFT_RAM (2ul << 20)

void SaveRectT::save(int16_t x, int16_t y, int16_t w, int16_t h) {
    if (offsets.empty()) {
        offsets.push_back(0);
    }
    x -= 2; if (x < 0) x = 0; // W/A
    w += 4; // W/A
    size_t off = offsets.back();
    if (butter_psram_size() >= PSRAM_SHIFT_RAM) {
        off += PSRAM_SHIFT_RAM;
        *(int16_t*)(PSRAM_DATA + off) = x; off += 2;
        *(int16_t*)(PSRAM_DATA + off) = y; off += 2;
        *(int16_t*)(PSRAM_DATA + off) = w; off += 2;
        *(int16_t*)(PSRAM_DATA + off) = h; off += 2;
        for (size_t line = y; line < y + h; ++line) {
            uint8_t* backbuffer = VIDEO::vga.frameBuffer[line];
            memcpy((void*)(PSRAM_DATA + off), backbuffer, w);
            for (int i = 0; i < w; ++i) {
                PSRAM_DATA[off + i] = VIDEO::vga.frameBuffer[line][x + i];
            }
            off += w;
        }
        offsets.push_back(off - PSRAM_SHIFT_RAM);
        return;
    }
    if (psram_size() >= PSRAM_SHIFT_RAM) {
        off += PSRAM_SHIFT_RAM;
        write16psram(off, x); off += 2;
        write16psram(off, y); off += 2;
        write16psram(off, w); off += 2;
        write16psram(off, h); off += 2;
        for (size_t line = y; line < y + h; ++line) {
            uint8_t* backbuffer = VIDEO::vga.frameBuffer[line];
            writepsram(off, backbuffer + x, w);
            off += w;
        }
        offsets.push_back(off - PSRAM_SHIFT_RAM);
        return;
    }
    if (FileUtils::fsMount) {
        FIL f;
        f_open(&f, "/tmp/save_rect.tmp", FA_WRITE | FA_CREATE_ALWAYS);
        f_lseek(&f, off);
        UINT bw;
        f_write(&f, &x, 2, &bw);
        f_write(&f, &y, 2, &bw);
        f_write(&f, &w, 2, &bw);
        f_write(&f, &h, 2, &bw);
        off += 8;
        for (size_t line = y; line < y + h; ++line) {
            uint8_t* backbuffer = VIDEO::vga.frameBuffer[line];
            f_write(&f, backbuffer + x, w, &bw);
            off += w;
        }
        f_close(&f);
        offsets.push_back(off);
    }
}
void SaveRectT::restore_last() {
    if (offsets.empty()) return; /// ???
    offsets.pop_back();
    size_t off = offsets.back();
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    if (butter_psram_size() >= PSRAM_SHIFT_RAM) {
        off += PSRAM_SHIFT_RAM;
        x = *(int16_t*)(PSRAM_DATA + off); off += 2;
        y = *(int16_t*)(PSRAM_DATA + off); off += 2;
        w = *(int16_t*)(PSRAM_DATA + off); off += 2;
        h = *(int16_t*)(PSRAM_DATA + off); off += 2;
        if (!w || !h) return;
        for (size_t line = y; line < y + h; ++line) {
            for (int i = 0; i < w; ++i) {
                VIDEO::vga.frameBuffer[line][x + i] = PSRAM_DATA[off + i];
            }
            off += w;
        }
    }
    else if (psram_size() >= PSRAM_SHIFT_RAM) {
        off += PSRAM_SHIFT_RAM;
        x = read16psram(off); off += 2;
        y = read16psram(off); off += 2;
        w = read16psram(off); off += 2;
        h = read16psram(off); off += 2;
        if (!w || !h) return;
        for (size_t line = y; line < y + h; ++line) {
            readpsram(VIDEO::vga.frameBuffer[line] + x, off, w);
            off += w;
        }
    }
    else if (FileUtils::fsMount) {
        FIL f;
        f_open(&f, "/tmp/save_rect.tmp", FA_READ);
        f_lseek(&f, off);
        UINT br;
        f_read(&f, &x, 2, &br);
        f_read(&f, &y, 2, &br);
        f_read(&f, &w, 2, &br);
        f_read(&f, &h, 2, &br);
        if (!w || !h) {
            f_close(&f);
            return;
        }
        for (size_t line = y; line < y + h; ++line) {
            f_read(&f, VIDEO::vga.frameBuffer[line] + x, w, &br);
        }
        f_close(&f);
    }
    if (offsets.empty()) {
        offsets.push_back(0);
    }
}

void SaveRectT::store_ram(const void* p, size_t sz) {
    if (offsets.empty()) {
        offsets.push_back(0);
    }
    size_t off = offsets.back();
    UINT bw;
    FIL f;
    f_open(&f, "/tmp/save_rect.tmp", FA_WRITE | FA_CREATE_ALWAYS);
    f_lseek(&f, off);
    f_write(&f, p, sz, &bw);
    f_close(&f);
}

void SaveRectT::restore_ram(void* p, size_t sz) {
    if (offsets.empty()) return;
    offsets.pop_back();
    size_t off = offsets.back();
    UINT br;
    FIL f;
    f_open(&f, "/tmp/save_rect.tmp", FA_READ);
    f_lseek(&f, off);
    f_read(&f, p, sz, &br);
    f_close(&f);
    if (offsets.empty()) {
        offsets.push_back(0);
    }
}
}