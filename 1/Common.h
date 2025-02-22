#pragma once
#include <Arduino.h>
#include "hardware/gpio.h"
#include "hardware/pwm.h"
//#include <pico/divider.h>
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include <pico/stdlib.h>
#include <hardware/vreg.h>
#include <LittleFS.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <SDFS.h>

//#define NEW_PATH

#define DBG

#define TFT_WR   20		/* Write strobe pin */
#define TFT_DC   21		/* Data Command control pin */

#define TFT_DATA  0		/* Data bus start pin */
#define SND_PIN  22		/* Sound pin */
#define PIO_CLK_DIV   5
#define PIO_CLK_FRACT 0
//#define TFT_REFRESHRATE 0x10 /* 119 Hz */
//#define TFT_REFRESHRATE 0x11 /* 112 Hz */
//#define TFT_REFRESHRATE 0x12 /* 106 Hz */
#define TFT_REFRESHRATE 0x13 /* 100 Hz */
//#define TFT_REFRESHRATE 0x14 /* 95 Hz	 */
//#define TFT_REFRESHRATE 0x15 /* 90 Hz	 */
//#define TFT_REFRESHRATE 0x16 /* 86 Hz	 */
//#define TFT_REFRESHRATE 0x17 /* 83 Hz	 */
//#define TFT_REFRESHRATE 0x18 /* 79 Hz	 */
//#define TFT_REFRESHRATE 0x19 /* 76 Hz	 */
//#define TFT_REFRESHRATE 0x1A /* 73 Hz	 */
//#define TFT_REFRESHRATE 0x1B /* 70 Hz	 */
//#define TFT_REFRESHRATE 0x1C /* 68 Hz	 */
//#define TFT_REFRESHRATE 0x1D /* 65 Hz	 */
//#define TFT_REFRESHRATE 0x1E /* 63 Hz	 */
//#define TFT_REFRESHRATE 0x1F /* 61 Hz	 */


#define BORDER_SHIFT 24

#define DMA_BUFF_SIZE 30

#define RESET		0x00000000
#define START_FRAME 0x01000000
#define STOP_FRAME  0x02000000
#define WR_PORT     0x03000000
#define SET_VOL     0x04000000
#define AY_PORT		0x05000000
#define AY_DATA		0x06000000
#define SCREENOFFSET 40

#define BORDER_BUFFER_SIZE 64
#define KBD_CLOCK 1000
#define BEEPER_BUFFER_SIZE 1024
#define AY_BUFFER_SIZE 512

#define TAP_BUFFER_SIZE 24567

#ifdef DBG
#define DBG_PRINT(a) Serial.print(a)
#define DBG_PRINTLN(a) Serial.println(a)
#define DBG_PRINTF Serial.printf
#else
#define DBG_PRINT(a) 
#define DBG_PRINTLN(a)
#define DBG_PRINTF //
#endif // DBG
