﻿#include "Browser.h"

uint8_t fontData[] = { // 8x8 Spectrum font with russian characters, codepage Windows-1251
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, // !
	0x00, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, // "
	0x00, 0x24, 0x7E, 0x24, 0x24, 0x7E, 0x24, 0x00, // #
	0x00, 0x08, 0x3E, 0x28, 0x3E, 0x0A, 0x3E, 0x08, // $
	0x00, 0x62, 0x64, 0x08, 0x10, 0x26, 0x46, 0x00, // %
	0x00, 0x10, 0x28, 0x10, 0x28, 0x44, 0x3A, 0x00, // &
	0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // '
	0x00, 0x04, 0x08, 0x08, 0x08, 0x08, 0x04, 0x00, // (
	0x00, 0x20, 0x10, 0x10, 0x10, 0x10, 0x20, 0x00, // )
	0x00, 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14, 0x00, // *
	0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, // +
	0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x10, // ,
	0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, // -
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, // .
	0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, // /
	0x00, 0x3C, 0x46, 0x4A, 0x52, 0x62, 0x3C, 0x00, // 0
	0x00, 0x18, 0x28, 0x08, 0x08, 0x08, 0x3E, 0x00, // 1
	0x00, 0x3C, 0x42, 0x02, 0x3C, 0x40, 0x7E, 0x00, // 2
	0x00, 0x3C, 0x42, 0x0C, 0x02, 0x42, 0x3C, 0x00, // 3
	0x00, 0x08, 0x18, 0x28, 0x48, 0x7E, 0x08, 0x00, // 4
	0x00, 0x7E, 0x40, 0x7C, 0x02, 0x42, 0x3C, 0x00, // 5
	0x00, 0x3C, 0x40, 0x7C, 0x42, 0x42, 0x3C, 0x00, // 6
	0x00, 0x7E, 0x02, 0x04, 0x08, 0x10, 0x10, 0x00, // 7
	0x00, 0x3C, 0x42, 0x3C, 0x42, 0x42, 0x3C, 0x00, // 8
	0x00, 0x3C, 0x42, 0x42, 0x3E, 0x02, 0x3C, 0x00, // 9
	0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, // :
	0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x10, 0x20, // ;
	0x00, 0x00, 0x04, 0x08, 0x10, 0x08, 0x04, 0x00, // <
	0x00, 0x00, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x00, // =
	0x00, 0x00, 0x10, 0x08, 0x04, 0x08, 0x10, 0x00, // >
	0x00, 0x3C, 0x42, 0x04, 0x08, 0x00, 0x08, 0x00, // ?
	0x00, 0x3C, 0x4A, 0x56, 0x5E, 0x40, 0x3C, 0x00, // @
	0x00, 0x3C, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x00, // A
	0x00, 0x7C, 0x42, 0x7C, 0x42, 0x42, 0x7C, 0x00, // B
	0x00, 0x3C, 0x42, 0x40, 0x40, 0x42, 0x3C, 0x00, // C
	0x00, 0x78, 0x44, 0x42, 0x42, 0x44, 0x78, 0x00, // D
	0x00, 0x7E, 0x40, 0x7C, 0x40, 0x40, 0x7E, 0x00, // E
	0x00, 0x7E, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x00, // F
	0x00, 0x3C, 0x42, 0x40, 0x4E, 0x42, 0x3C, 0x00, // G
	0x00, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00, // H
	0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, // I
	0x00, 0x02, 0x02, 0x02, 0x42, 0x42, 0x3C, 0x00, // J
	0x00, 0x44, 0x48, 0x70, 0x48, 0x44, 0x42, 0x00, // K
	0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7E, 0x00, // L
	0x00, 0x42, 0x66, 0x5A, 0x42, 0x42, 0x42, 0x00, // M
	0x00, 0x42, 0x62, 0x52, 0x4A, 0x46, 0x42, 0x00, // N
	0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, // O
	0x00, 0x7C, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x00, // P
	0x00, 0x3C, 0x42, 0x42, 0x52, 0x4A, 0x3C, 0x00, // Q
	0x00, 0x7C, 0x42, 0x42, 0x7C, 0x44, 0x42, 0x00, // R
	0x00, 0x3C, 0x40, 0x3C, 0x02, 0x42, 0x3C, 0x00, // S
	0x00, 0xFE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, // T
	0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, // U
	0x00, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, // V
	0x00, 0x42, 0x42, 0x42, 0x42, 0x5A, 0x24, 0x00, // W
	0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00, // X
	0x00, 0x82, 0x44, 0x38, 0x10, 0x10, 0x10, 0x00, // Y
	0x00, 0x7E, 0x04, 0x08, 0x10, 0x20, 0x7E, 0x00, // Z
	0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x0E, 0x00, // [
	0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, // backslah
	0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x70, 0x00, // ]
	0x00, 0x10, 0x38, 0x54, 0x10, 0x10, 0x10, 0x00, // ^
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, // _
	0x00, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, // `
	0x00, 0x00, 0x38, 0x04, 0x3C, 0x44, 0x3C, 0x00, // a
	0x00, 0x20, 0x20, 0x3C, 0x22, 0x22, 0x3C, 0x00, // b
	0x00, 0x00, 0x1C, 0x20, 0x20, 0x20, 0x1C, 0x00, // c
	0x00, 0x04, 0x04, 0x3C, 0x44, 0x44, 0x3C, 0x00, // d
	0x00, 0x00, 0x38, 0x44, 0x78, 0x40, 0x3C, 0x00, // e
	0x00, 0x0C, 0x10, 0x18, 0x10, 0x10, 0x10, 0x00, // f
	0x00, 0x00, 0x3C, 0x44, 0x44, 0x3C, 0x04, 0x38, // g
	0x00, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x00, // h
	0x00, 0x10, 0x00, 0x30, 0x10, 0x10, 0x38, 0x00, // i
	0x00, 0x04, 0x00, 0x04, 0x04, 0x04, 0x24, 0x18, // j
	0x00, 0x20, 0x28, 0x30, 0x30, 0x28, 0x24, 0x00, // k
	0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0C, 0x00, // l
	0x00, 0x00, 0x68, 0x54, 0x54, 0x54, 0x54, 0x00, // m
	0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x44, 0x00, // n
	0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, // o
	0x00, 0x00, 0x78, 0x44, 0x44, 0x78, 0x40, 0x40, // p
	0x00, 0x00, 0x3C, 0x44, 0x44, 0x3C, 0x04, 0x06, // q
	0x00, 0x00, 0x1C, 0x20, 0x20, 0x20, 0x20, 0x00, // r
	0x00, 0x00, 0x38, 0x40, 0x38, 0x04, 0x78, 0x00, // s
	0x00, 0x10, 0x38, 0x10, 0x10, 0x10, 0x0C, 0x00, // t
	0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, // u
	0x00, 0x00, 0x44, 0x44, 0x28, 0x28, 0x10, 0x00, // v
	0x00, 0x00, 0x44, 0x54, 0x54, 0x54, 0x28, 0x00, // w
	0x00, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, // z
	0x00, 0x00, 0x44, 0x44, 0x44, 0x3C, 0x04, 0x38, // y
	0x00, 0x00, 0x7C, 0x08, 0x10, 0x20, 0x7C, 0x00, // z
	0x00, 0x0E, 0x08, 0x10, 0x08, 0x08, 0x0E, 0x00, // {
	0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, // |
	0x00, 0x70, 0x10, 0x08, 0x10, 0x10, 0x70, 0x00, // }
	0x00, 0x14, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // ~
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x10, 0x10, 0x54, 0x38, 0x10, 0x00, // Arrow down
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x24, 0x00, 0x7E, 0x40, 0x7C, 0x40, 0x7E, 0x00, // Ё
	0x3C, 0x42, 0x99, 0xA1, 0xA1, 0x99, 0x42, 0x3C, // © 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x28, 0x00, 0x38, 0x44, 0x78, 0x40, 0x3C, 0x00, // ё
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3C, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x00, // А
	0x00, 0x7E, 0x40, 0x7C, 0x42, 0x42, 0x7C, 0x00, // Б
	0x00, 0x7C, 0x42, 0x7C, 0x42, 0x42, 0x7C, 0x00, // В
	0x00, 0x7E, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, // Г
	0x00, 0x1C, 0x24, 0x24, 0x24, 0x24, 0x7E, 0x42, // Д
	0x00, 0x7E, 0x40, 0x7C, 0x40, 0x40, 0x7E, 0x00, // Е
	0x00, 0x92, 0x54, 0x38, 0x54, 0x92, 0x92, 0x00, // Ж
	0x00, 0x3C, 0x42, 0x0C, 0x02, 0x42, 0x3C, 0x00, // З
	0x00, 0x00, 0x46, 0x4A, 0x52, 0x62, 0x42, 0x00, // И
	0x18, 0x42, 0x46, 0x4A, 0x52, 0x62, 0x42, 0x00, // Й
	0x00, 0x44, 0x48, 0x70, 0x48, 0x44, 0x42, 0x00, // К
	0x00, 0x1E, 0x22, 0x22, 0x22, 0x22, 0xC2, 0x00, // Л
	0x00, 0x42, 0x66, 0x5A, 0x42, 0x42, 0x42, 0x00, // М
	0x00, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00, // Н
	0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, // О
	0x00, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, // П
	0x00, 0x7C, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x00, // Р
	0x00, 0x3C, 0x42, 0x40, 0x40, 0x42, 0x3C, 0x00, // С
	0x00, 0xFE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, // Т
	0x00, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x3C, 0x00, // У
	0x00, 0x7C, 0x92, 0x92, 0x92, 0x7C, 0x10, 0x00, // Ф
	0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00, // Х
	0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x7E, 0x02, // Ц
	0x00, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x00, // Ч
	0x00, 0x82, 0x92, 0x92, 0x92, 0x92, 0xFE, 0x00, // Ш
	0x00, 0x82, 0x92, 0x92, 0x92, 0x92, 0xFE, 0x02, // Щ
	0x00, 0xC0, 0x40, 0x7C, 0x42, 0x42, 0x7C, 0x00, // Ъ
	0x00, 0x82, 0x82, 0xF2, 0x8A, 0x8A, 0xF2, 0x00, // Ы
	0x00, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x7C, 0x00, // Ь
	0x00, 0x3C, 0x42, 0x0E, 0x02, 0x42, 0x3C, 0x00, // Э
	0x00, 0x9C, 0xA2, 0xE2, 0xA2, 0xA2, 0x9C, 0x00, // Ю
	0x00, 0x3E, 0x42, 0x42, 0x3E, 0x22, 0x42, 0x00, // Я
	0x00, 0x00, 0x38, 0x04, 0x3C, 0x44, 0x3C, 0x00, // а
	0x00, 0x3C, 0x40, 0x38, 0x44, 0x44, 0x38, 0x00, // б
	0x00, 0x00, 0x78, 0x44, 0x78, 0x44, 0x78, 0x00, // в
	0x00, 0x00, 0x7C, 0x40, 0x40, 0x40, 0x40, 0x00, // г
	0x00, 0x00, 0x1C, 0x24, 0x24, 0x24, 0x7E, 0x42, // д
	0x00, 0x00, 0x38, 0x44, 0x78, 0x40, 0x3C, 0x00, // е
	0x00, 0x00, 0x92, 0x54, 0x38, 0x54, 0x92, 0x00, // ж
	0x00, 0x00, 0x78, 0x04, 0x38, 0x04, 0x78, 0x00, // з
	0x00, 0x00, 0x44, 0x4C, 0x54, 0x64, 0x44, 0x00, // и
	0x00, 0x10, 0x44, 0x4C, 0x54, 0x64, 0x44, 0x00, // й
	0x00, 0x00, 0x44, 0x48, 0x70, 0x48, 0x44, 0x00, // к
	0x00, 0x00, 0x1C, 0x24, 0x44, 0x44, 0x44, 0x00, // л
	0x00, 0x00, 0x44, 0x6C, 0x54, 0x44, 0x44, 0x00, // м
	0x00, 0x00, 0x44, 0x44, 0x7C, 0x44, 0x44, 0x00, // н
	0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, // о
	0x00, 0x00, 0x7C, 0x44, 0x44, 0x44, 0x44, 0x00, // п
	0x00, 0x00, 0x78, 0x44, 0x44, 0x78, 0x40, 0x40, // р
	0x00, 0x00, 0x3C, 0x40, 0x40, 0x40, 0x3C, 0x00, // с
	0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x00, // т
	0x00, 0x00, 0x44, 0x44, 0x44, 0x3C, 0x04, 0x38, // у
	0x00, 0x10, 0x7C, 0x92, 0x92, 0x92, 0x7C, 0x10, // ф
	0x00, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, // х
	0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x7E, 0x02, // й
	0x00, 0x00, 0x44, 0x44, 0x3C, 0x04, 0x04, 0x00, // ч
	0x00, 0x00, 0x54, 0x54, 0x54, 0x54, 0x7C, 0x00, // ш
	0x00, 0x00, 0x54, 0x54, 0x54, 0x54, 0x7E, 0x02, // щ
	0x00, 0x00, 0x60, 0x20, 0x3C, 0x22, 0x3C, 0x00, // ъ
	0x00, 0x00, 0x82, 0x82, 0xF2, 0x8A, 0xF2, 0x00, // ы
	0x00, 0x00, 0x40, 0x40, 0x78, 0x44, 0x78, 0x00, // ь
	0x00, 0x00, 0x38, 0x44, 0x1C, 0x44, 0x38, 0x00, // э
	0x00, 0x00, 0x4C, 0x52, 0x72, 0x52, 0x4C, 0x00, // ю
	0x00, 0x00, 0x3C, 0x44, 0x3C, 0x24, 0x44, 0x00  // я
};

void Browser::drawChar(const uint8_t ch, uint8_t posX, uint16_t foreColor, uint16_t backColor)
{
	uint16_t* pBuffer = m_pDisplayInstance->getBuffer(0);
	if (ch < 32 || posX > 39) return;
	uint16_t glyphPos = (uint16_t)(ch - 32) * 8;
	for (int j = 0; j < 8; j++)
	{
		uint8_t glyphByte = fontData[glyphPos + j];
		for (int i = 0; i < 8; i++)
		{
			if (glyphByte & 0x80)
				pBuffer[j * 320 + i + posX * 8] = foreColor;
			else
				pBuffer[j * 320 + i + posX * 8] = backColor;
			glyphByte <<= 1;
		}
	}
}

void Browser::drawString(const String textStr, uint8_t posX, uint8_t posY, uint16_t foreColor, uint16_t backColor, bool isAnsi)
{
	String decodedStr = "";
	int idx = 0;
	if (!isAnsi)
	{
		int len = textStr.length();
		char ch;
		while (idx < len)
		{
			ch = textStr[idx++];
			//if (ch >= 0xD0)
			//	if (textStr[idx] == 0x81 || textStr[idx] == 0x91)
			//		ch = textStr[idx++] + 0x27;
			//	else
			//		ch = textStr[idx++] + (0x30 + 0x40 * (ch & 0x01));
			switch (ch)
			{
			case 0xD0:
				ch = textStr[idx++];
				if (ch == 0x81)
					ch = 0xA8;
				else
					if (ch >= 0x90 && ch <= 0xBF) ch = ch + 0x30;
				break;
			case 0xD1:
				ch = textStr[idx++];
				if (ch == 0x91)
					ch = 0xB8;
				else
					if (ch >= 0x80 && ch <= 0x8F) ch = ch + 0x70;
				break;
			default:
				break;
			}
			decodedStr += ch;
		}
	}
	else
		decodedStr = textStr;
	if (posY > 29) return;
	m_pDisplayInstance->setAddrWindow(0, posY * 8, 319, posY * 8 + 7);
	idx = 0;
	while (decodedStr[idx]) drawChar(decodedStr[idx], posX + idx++, foreColor, backColor);
	m_pDisplayInstance->drawBuffer(0, 320 * 8);
}

void Browser::listFiles()
{
	drawString("                                        ", 0, 0, 0xFFFF, 0x1700);
	for (int i = 0; i < 21; i++)
	{
		String outStr = " ";
		outStr += m_browserWindow[i];
		if (outStr.length() > 38)
		{
			String tmp = outStr;
			tmp.toLowerCase();
			int pos = tmp.lastIndexOf(".tap");
			outStr.remove(33, pos - 33);
			outStr.setCharAt(32, '~');
		}
		for (int j = outStr.length(); j < 38; j++) outStr += " ";
		uint16_t backColor = 0x1700, foreColor = 0xF7BD;
		if (m_currDir + m_browserWindow[i] == m_selectedFile) foreColor = 0xE0FF;
		if (outStr[1] == '/') foreColor = 0xFF07;
		if (i + 1 == m_selectionPos)
		{
			backColor = 0xFF07; foreColor = 0x0000;
		}
		drawString(outStr, 1, i + 1, foreColor, backColor, false);
		drawString("                                        ", 0, 22, 0xFFFF, 0x1700);
	}
}

void Browser::chDir()
{
	m_browseFrom = m_selectionPos = 1;
	getFileList();
	dir();
}

void Browser::dir()
{
	int i, posInList = 0;
	for (i = 0; i < 21; i++) m_browserWindow[i] = "";
	for (i = 0; i < m_browseFrom - 1; i++)
	{
		posInList = m_fileList.indexOf("\n", posInList) + 1;
	}
	for (i = 0; i < 21; i++)
	{
		if (m_fileList[posInList] == '\n' || posInList >= m_fileList.length()) break;
		int endOfStr = m_fileList.indexOf("\n", posInList);
		m_browserWindow[i] = m_fileList.substring(posInList, endOfStr);
		posInList = endOfStr + 1;
	}
}

void Browser::getFileList()
{
	m_fileList = "";
	m_filesCount = 0;
	File dirFile = SD.open(m_currDir), entry;
	if (!dirFile) return;
	dirFile.rewindDirectory();
	if (m_currDir != "/" && dirFile.isDirectory())
	{
		m_fileList = "/.."; m_fileList.concat("\n"); m_filesCount++;
	}
	while (entry = dirFile.openNextFile())
	{
		if (entry.isDirectory())m_fileList.concat("/");
		m_fileList.concat(entry.name()); m_filesCount++;
		m_fileList.concat("\n");
		entry.close();
	}
	dirFile.close();
}

void Browser::drawSettingsString()
{
	String settingString = "  Звук: ";
	settingString += (m_soundOn ? "ВКЛ " : "ВЫКЛ");
	settingString += "                 Лента: ";
	settingString += (m_tapeTurbo ? "2Х  " : "1Х  ");
	drawString(settingString, 0, 25, 0xF7BD, 0x1700, false);
}

void Browser::drawSelectedROM() 
{
	String romString = "  ROM : ";
	romString += m_romFileNames[m_currRom];
	for (int i = romString.length(); i < 38; i++) romString += " ";
	drawString(romString, 0, 26, 0xF7BD, 0x1700);
}

void Browser::drawFooter()
{
	drawString("----------------------------------------", 0, 23, 0xF7BD, 0x1700);
	drawString("                                        ", 0, 24, 0xFFFF, 0x1700);
	drawSettingsString();
	drawSelectedROM();
	drawString("                                        ", 0, 27, 0xFFFF, 0x1700);
	drawString(" F1-Спр F2-Звук F3-Лента F4-ROM  F5-Вых ", 0, 28, 0xE0FF, 0x1700, false);
	drawString("                                        ", 0, 29, 0xFFFF, 0x1700);
}

bool Browser::run()
{
	bool result = false;
	if (!(SD.begin(SS, SPI_FULL_SPEED))) return result;
	chDir();
	listFiles();
	drawFooter();
	while (true)
	{
		if (m_pKeyboardInstance->getData(9) == 0x01)
		{
			LittleFS.begin();
			File helpFile = LittleFS.open("/Help.txt", "r");
			for (int i = 0; i < 26; i++)
			{
				char strData[43];
				helpFile.readBytes(strData, 42);
				strData[40] = 0;
				drawString(strData, 0, i + 2, 0x0000, 0xF7BD);
			}
			helpFile.close();
			LittleFS.end();
			delay(500);
			while (m_pKeyboardInstance->getData(9) != 0x01) yield();
			listFiles();
			drawFooter();
			delay(500);
		}
		if (m_pKeyboardInstance->getData(9) == 0x02)
		{
			m_soundOn ^= true;
			drawSettingsString();
			delay(500);
		}
		if (m_pKeyboardInstance->getData(9) == 0x04)
		{
			m_tapeTurbo ^= true;
			drawSettingsString();
			delay(500);
		}
		if (m_pKeyboardInstance->getData(9) == 0x08)
		{
			m_currRom = (m_currRom + 1) & 0x03;
			drawSelectedROM();
			result = true;
			delay(500);
		}
		if (m_pKeyboardInstance->getData(9) == 0x10) break;
		if (m_pKeyboardInstance->getData(8) == 0x04)
		{
			m_selectionPos++;
			if (m_selectionPos > 21 || m_browserWindow[m_selectionPos - 1] == "")
			{
				m_selectionPos--;
				if (m_browseFrom + 20 < m_filesCount)
				{
					m_browseFrom++; dir(); delay(150);
				}
			}
			else
				delay(150);
			listFiles();
		}
		if (m_pKeyboardInstance->getData(8) == 0x08)
		{
			m_selectionPos--;
			if (m_selectionPos == 0)
			{
				m_selectionPos++;
				if (m_browseFrom > 1)
				{
					m_browseFrom--; dir(); delay(150);
				}
			}
			else
				delay(150);
			listFiles();
		}
		if (m_pKeyboardInstance->getData(8) == 0x10)
		{
			if (m_browserWindow[m_selectionPos - 1][0] == '/')
			{
				if (m_browserWindow[m_selectionPos - 1] == "/..")
				{
					m_currDir.remove(m_currDir.length() - 1);
					m_currDir.remove(m_currDir.lastIndexOf("/") + 1);
				}
				else
				{
					m_currDir.remove(m_currDir.length() - 1);
					m_currDir += m_browserWindow[m_selectionPos - 1];
					m_currDir += "/";
				}
				chDir();
				listFiles();
			}
			else
			{
				m_selectedFile = m_currDir + m_browserWindow[m_selectionPos - 1];
				listFiles();
			}
			delay(500);
		}
		yield();
	}
	SD.end();
	return result;
}