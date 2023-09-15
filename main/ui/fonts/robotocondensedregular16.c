/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#include "lvgl.h"

#ifndef ROBOTOCONDENSEDREGULAR16
#define ROBOTOCONDENSEDREGULAR16 1
#endif

#if ROBOTOCONDENSEDREGULAR16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0x10,

    /* U+0022 "\"" */
    0xb6, 0xd0,

    /* U+0023 "#" */
    0x16, 0x68, 0x97, 0xf2, 0x44, 0x8b, 0x36, 0xfe,
    0x91, 0x22, 0x40,

    /* U+0024 "$" */
    0x10, 0x43, 0x93, 0x45, 0x6, 0xe, 0xc, 0x18,
    0x73, 0x78, 0x41, 0x0,

    /* U+0025 "%" */
    0x60, 0x49, 0x24, 0x92, 0x86, 0x40, 0x40, 0x2c,
    0x29, 0x14, 0x92, 0x49, 0x20, 0x60,

    /* U+0026 "&" */
    0x30, 0x48, 0x48, 0x48, 0x58, 0x70, 0x70, 0xd2,
    0x9e, 0x8e, 0xce, 0x7e,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x29, 0x29, 0x24, 0x92, 0x48, 0x91, 0x0,

    /* U+0029 ")" */
    0x99, 0x22, 0x49, 0x24, 0x94, 0x94, 0x0,

    /* U+002A "*" */
    0x10, 0x23, 0x51, 0xe3, 0x85, 0x0, 0x0,

    /* U+002B "+" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x8,

    /* U+002C "," */
    0x56,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x8, 0x46, 0x21, 0x8, 0x84, 0x23, 0x10, 0x8c,
    0x0,

    /* U+0030 "0" */
    0x79, 0x28, 0x61, 0x86, 0x18, 0x61, 0x86, 0x14,
    0x9e,

    /* U+0031 "1" */
    0x17, 0xd1, 0x11, 0x11, 0x11, 0x11,

    /* U+0032 "2" */
    0x7b, 0x38, 0x61, 0x4, 0x20, 0x84, 0x21, 0x8c,
    0x3f,

    /* U+0033 "3" */
    0x7b, 0x38, 0x41, 0xc, 0xe0, 0xc1, 0x6, 0x1c,
    0xde,

    /* U+0034 "4" */
    0x8, 0x30, 0x61, 0x42, 0x8d, 0x12, 0x64, 0xfe,
    0x10, 0x20, 0x40,

    /* U+0035 "5" */
    0x7d, 0x4, 0x10, 0x79, 0x30, 0x41, 0x7, 0x14,
    0x8e,

    /* U+0036 "6" */
    0x39, 0x84, 0x20, 0xfb, 0x28, 0x61, 0x86, 0x14,
    0x8e,

    /* U+0037 "7" */
    0xfe, 0xc, 0x10, 0x60, 0xc1, 0x6, 0x8, 0x10,
    0x60, 0x83, 0x0,

    /* U+0038 "8" */
    0x7b, 0x38, 0x61, 0xcd, 0xec, 0xe1, 0x86, 0x1c,
    0xde,

    /* U+0039 "9" */
    0x71, 0x28, 0x61, 0x86, 0x14, 0xdf, 0x4, 0x20,
    0x9c,

    /* U+003A ":" */
    0xf0, 0x3, 0xc0,

    /* U+003B ";" */
    0x6c, 0x0, 0x3, 0x6d, 0x20,

    /* U+003C "<" */
    0x4, 0x77, 0x30, 0xe0, 0xe0, 0xc0,

    /* U+003D "=" */
    0xf8, 0x0, 0xf, 0x80,

    /* U+003E ">" */
    0x83, 0x83, 0x3, 0x19, 0x8c, 0x0,

    /* U+003F "?" */
    0x74, 0x62, 0x10, 0x88, 0x44, 0x20, 0x0, 0x40,

    /* U+0040 "@" */
    0xf, 0x6, 0x11, 0x1, 0x23, 0x28, 0xb3, 0x26,
    0x64, 0xcc, 0x99, 0x92, 0x32, 0x66, 0x4d, 0x26,
    0xe4, 0x0, 0x42, 0x7, 0x80,

    /* U+0041 "A" */
    0x8, 0x6, 0x7, 0x2, 0x81, 0x61, 0xb0, 0x88,
    0x46, 0x7f, 0x30, 0x90, 0x58, 0x30,

    /* U+0042 "B" */
    0xfa, 0x18, 0x61, 0x8f, 0xe8, 0xe1, 0x86, 0x18,
    0xfe,

    /* U+0043 "C" */
    0x3c, 0x8a, 0xc, 0x18, 0x10, 0x20, 0x40, 0x83,
    0x5, 0x11, 0xe0,

    /* U+0044 "D" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0xe, 0x17, 0xc0,

    /* U+0045 "E" */
    0xfe, 0x8, 0x20, 0x83, 0xe8, 0x20, 0x82, 0x8,
    0x3f,

    /* U+0046 "F" */
    0xfe, 0x8, 0x20, 0x82, 0xf, 0xa0, 0x82, 0x8,
    0x20,

    /* U+0047 "G" */
    0x3c, 0x8e, 0xc, 0x8, 0x10, 0x23, 0xc1, 0x83,
    0x5, 0x9, 0xe0,

    /* U+0048 "H" */
    0x83, 0x6, 0xc, 0x18, 0x3f, 0xe0, 0xc1, 0x83,
    0x6, 0xc, 0x10,

    /* U+0049 "I" */
    0xff, 0xf0,

    /* U+004A "J" */
    0x4, 0x10, 0x41, 0x4, 0x10, 0x41, 0x6, 0x1c,
    0xde,

    /* U+004B "K" */
    0x85, 0x1a, 0x64, 0x8b, 0x1c, 0x3c, 0x48, 0x99,
    0x1a, 0x34, 0x30,

    /* U+004C "L" */
    0x84, 0x21, 0x8, 0x42, 0x10, 0x84, 0x21, 0xf0,

    /* U+004D "M" */
    0xc1, 0xe0, 0xf0, 0x7c, 0x7a, 0x2d, 0x16, 0x8b,
    0x29, 0x94, 0xca, 0x67, 0x31, 0x10,

    /* U+004E "N" */
    0x83, 0x87, 0xf, 0x1a, 0x36, 0x64, 0xc5, 0x8f,
    0xe, 0x1c, 0x10,

    /* U+004F "O" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+0050 "P" */
    0xfd, 0xe, 0xc, 0x18, 0x30, 0xff, 0x40, 0x81,
    0x2, 0x4, 0x0,

    /* U+0051 "Q" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x5, 0x11, 0xe0, 0x60, 0x40,

    /* U+0052 "R" */
    0xfa, 0x38, 0x61, 0x86, 0x3f, 0xa6, 0x8a, 0x38,
    0x61,

    /* U+0053 "S" */
    0x3c, 0xcd, 0xa, 0x6, 0x7, 0x7, 0x3, 0x3,
    0x5, 0x19, 0xe0,

    /* U+0054 "T" */
    0xfe, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x80,

    /* U+0055 "U" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x7, 0x13, 0xe0,

    /* U+0056 "V" */
    0xc1, 0xa0, 0x98, 0xcc, 0x62, 0x21, 0x10, 0xd8,
    0x28, 0x14, 0xa, 0x7, 0x1, 0x0,

    /* U+0057 "W" */
    0x8c, 0x71, 0x8a, 0x31, 0x67, 0x2c, 0xa4, 0xb5,
    0x94, 0xb2, 0x94, 0x53, 0x8e, 0x30, 0x86, 0x10,
    0xc0,

    /* U+0058 "X" */
    0xc7, 0x8d, 0x33, 0x63, 0x87, 0xe, 0x14, 0x6c,
    0x9b, 0x1c, 0x30,

    /* U+0059 "Y" */
    0xc7, 0x8d, 0x13, 0x62, 0x85, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x80,

    /* U+005A "Z" */
    0xfe, 0xc, 0x30, 0x61, 0x83, 0xc, 0x10, 0x60,
    0x83, 0x7, 0xf0,

    /* U+005B "[" */
    0xea, 0xaa, 0xaa, 0xab,

    /* U+005C "\\" */
    0x41, 0x4, 0x18, 0x20, 0x83, 0x4, 0x10, 0x60,
    0x82, 0xc,

    /* U+005D "]" */
    0xd5, 0x55, 0x55, 0x57,

    /* U+005E "^" */
    0x66, 0x6f, 0x99,

    /* U+005F "_" */
    0xfc,

    /* U+0061 "a" */
    0x76, 0x62, 0x17, 0xe6, 0x31, 0x78,

    /* U+0062 "b" */
    0x82, 0x8, 0x3e, 0xce, 0x18, 0x61, 0x86, 0x1c,
    0xfe,

    /* U+0063 "c" */
    0x7b, 0x38, 0x60, 0x82, 0x8, 0x71, 0x78,

    /* U+0064 "d" */
    0x4, 0x10, 0x5f, 0xce, 0x18, 0x61, 0x86, 0x1c,
    0xdf,

    /* U+0065 "e" */
    0x79, 0x38, 0x61, 0xfe, 0x8, 0x31, 0x78,

    /* U+0066 "f" */
    0x3a, 0x10, 0x8f, 0x21, 0x8, 0x42, 0x10, 0x84,
    0x0,

    /* U+0067 "g" */
    0x7f, 0x38, 0x61, 0x86, 0x18, 0x73, 0x7c, 0x10,
    0x5e,

    /* U+0068 "h" */
    0x84, 0x21, 0xec, 0xc6, 0x31, 0x8c, 0x63, 0x10,

    /* U+0069 "i" */
    0x9f, 0xf0,

    /* U+006A "j" */
    0x41, 0x55, 0x55, 0x5c,

    /* U+006B "k" */
    0x82, 0x8, 0x26, 0x92, 0xce, 0x38, 0xb2, 0x49,
    0xa2,

    /* U+006C "l" */
    0xff, 0xf0,

    /* U+006D "m" */
    0xf7, 0x46, 0x62, 0x31, 0x18, 0x8c, 0x46, 0x23,
    0x11, 0x88, 0x80,

    /* U+006E "n" */
    0xf6, 0x63, 0x18, 0xc6, 0x31, 0x88,

    /* U+006F "o" */
    0x79, 0x28, 0x61, 0x86, 0x18, 0x52, 0x78,

    /* U+0070 "p" */
    0xfb, 0x38, 0x61, 0x86, 0x18, 0x63, 0xfa, 0x8,
    0x20,

    /* U+0071 "q" */
    0x7f, 0x18, 0x61, 0x86, 0x18, 0x71, 0x7c, 0x10,
    0x41,

    /* U+0072 "r" */
    0xf2, 0x49, 0x24, 0x80,

    /* U+0073 "s" */
    0x7a, 0x38, 0x70, 0x78, 0x38, 0x71, 0x78,

    /* U+0074 "t" */
    0x44, 0xf4, 0x44, 0x44, 0x44, 0x30,

    /* U+0075 "u" */
    0x8c, 0x63, 0x18, 0xc6, 0x33, 0x78,

    /* U+0076 "v" */
    0xc5, 0x14, 0xda, 0x28, 0xa3, 0x8c, 0x10,

    /* U+0077 "w" */
    0x4c, 0x53, 0x34, 0xc9, 0x3a, 0x7a, 0x8c, 0xe3,
    0x38, 0xcc, 0x31, 0x0,

    /* U+0078 "x" */
    0x44, 0xd8, 0xa1, 0xc1, 0x7, 0xa, 0x36, 0x44,

    /* U+0079 "y" */
    0x8e, 0x2c, 0x96, 0x51, 0x47, 0xc, 0x20, 0x86,
    0x30,

    /* U+007A "z" */
    0xfc, 0x31, 0x84, 0x31, 0x86, 0x30, 0xfc,

    /* U+007B "{" */
    0x12, 0x22, 0x22, 0x2c, 0x22, 0x22, 0x22, 0x10,

    /* U+007C "|" */
    0xff, 0xfc,

    /* U+007D "}" */
    0x84, 0x44, 0x44, 0x43, 0x44, 0x44, 0x44, 0x80,

    /* U+00A0 " " */
    0x0,

    /* U+00A7 "§" */
    0x3c, 0xcd, 0xa, 0x6, 0xf, 0x23, 0x41, 0x82,
    0xc4, 0xf0, 0x70, 0x30, 0x71, 0xbe,

    /* U+00A9 "©" */
    0x1f, 0x6, 0x31, 0x39, 0x68, 0xb9, 0x3, 0x20,
    0x64, 0xc, 0x89, 0xce, 0x68, 0x8, 0xc6, 0xf,
    0x80,

    /* U+00AB "«" */
    0x4a, 0xad, 0x65, 0x24,

    /* U+00BB "»" */
    0xb2, 0x92, 0x95, 0x58,

    /* U+00C4 "Ä" */
    0x22, 0x0, 0x7, 0x3, 0x81, 0xc1, 0xa0, 0xd8,
    0x44, 0x22, 0x31, 0x9f, 0xc8, 0x24, 0x16, 0xc,

    /* U+00D6 "Ö" */
    0x24, 0x0, 0xe3, 0x2c, 0x38, 0x70, 0xe1, 0xc3,
    0x87, 0xe, 0x16, 0x47, 0x0,

    /* U+00DC "Ü" */
    0x44, 0x2, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x6, 0xc, 0x1c, 0x4f, 0x80,

    /* U+00DF "ß" */
    0x72, 0x28, 0xa2, 0x92, 0x49, 0x22, 0x86, 0x18,
    0x6e,

    /* U+00E4 "ä" */
    0x48, 0x0, 0xec, 0xc4, 0x2f, 0xcc, 0x62, 0xf0,

    /* U+00F6 "ö" */
    0x48, 0x0, 0x1e, 0x4a, 0x18, 0x61, 0x86, 0x14,
    0x9e,

    /* U+00FC "ü" */
    0x48, 0x1, 0x18, 0xc6, 0x31, 0x8c, 0x66, 0xf0,

    /* U+2010 "‐" */
    0xe0,

    /* U+2011 "‑" */
    0xe0,

    /* U+2013 "–" */
    0xfe,

    /* U+2014 "—" */
    0xff, 0x80,

    /* U+2018 "‘" */
    0x2a,

    /* U+201A "‚" */
    0x54,

    /* U+201C "“" */
    0x4a, 0xaa,

    /* U+201E "„" */
    0x55, 0x57,

    /* U+2026 "…" */
    0xdb, 0xdb,

    /* U+2030 "‰" */
    0x60, 0x9, 0x20, 0x92, 0x9, 0x40, 0x64, 0x0,
    0x80, 0xb, 0x61, 0x49, 0x14, 0x92, 0x49, 0x24,
    0x90, 0x36,

    /* U+20AC "€" */
    0x1c, 0x84, 0x10, 0xf9, 0xf, 0x90, 0x41, 0x2,
    0x7
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 59, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 63, .box_w = 1, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 82, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 5, .adv_w = 140, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 16, .adv_w = 126, .box_w = 6, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 28, .adv_w = 162, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 42, .adv_w = 139, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 54, .adv_w = 45, .box_w = 1, .box_h = 4, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 55, .adv_w = 80, .box_w = 3, .box_h = 17, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 62, .adv_w = 82, .box_w = 3, .box_h = 17, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 69, .adv_w = 110, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 76, .adv_w = 127, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 83, .adv_w = 50, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 84, .adv_w = 64, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 85, .adv_w = 67, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 95, .box_w = 5, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 95, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 126, .box_w = 4, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 110, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 126, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 148, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 157, .adv_w = 126, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 168, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 60, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 52, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 194, .adv_w = 114, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 200, .adv_w = 123, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 204, .adv_w = 118, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 210, .adv_w = 108, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 218, .adv_w = 197, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 239, .adv_w = 148, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 253, .adv_w = 140, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 262, .adv_w = 145, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 273, .adv_w = 146, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 284, .adv_w = 127, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 293, .adv_w = 123, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 302, .adv_w = 151, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 313, .adv_w = 159, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 324, .adv_w = 64, .box_w = 1, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 124, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 335, .adv_w = 140, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 346, .adv_w = 122, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 193, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 368, .adv_w = 159, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 379, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 390, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 401, .adv_w = 154, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 414, .adv_w = 136, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 423, .adv_w = 133, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 133, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 445, .adv_w = 144, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 456, .adv_w = 143, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 470, .adv_w = 195, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 487, .adv_w = 141, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 498, .adv_w = 134, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 509, .adv_w = 134, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 520, .adv_w = 64, .box_w = 2, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 524, .adv_w = 95, .box_w = 6, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 534, .adv_w = 64, .box_w = 2, .box_h = 16, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 538, .adv_w = 95, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 541, .adv_w = 104, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 542, .adv_w = 123, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 548, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 557, .adv_w = 118, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 564, .adv_w = 127, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 573, .adv_w = 120, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 580, .adv_w = 81, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 589, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 598, .adv_w = 124, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 606, .adv_w = 59, .box_w = 1, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 608, .adv_w = 57, .box_w = 2, .box_h = 15, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 612, .adv_w = 115, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 621, .adv_w = 59, .box_w = 1, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 623, .adv_w = 193, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 634, .adv_w = 124, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 640, .adv_w = 129, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 647, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 656, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 665, .adv_w = 78, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 669, .adv_w = 117, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 676, .adv_w = 76, .box_w = 4, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 682, .adv_w = 124, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 688, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 695, .adv_w = 166, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 707, .adv_w = 112, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 715, .adv_w = 107, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 724, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 731, .adv_w = 78, .box_w = 4, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 739, .adv_w = 62, .box_w = 1, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 741, .adv_w = 78, .box_w = 4, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 749, .adv_w = 59, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 750, .adv_w = 138, .box_w = 7, .box_h = 16, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 764, .adv_w = 201, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 781, .adv_w = 107, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 785, .adv_w = 107, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 789, .adv_w = 148, .box_w = 9, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 805, .adv_w = 154, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 818, .adv_w = 144, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 831, .adv_w = 134, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 840, .adv_w = 123, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 848, .adv_w = 129, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 857, .adv_w = 124, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 865, .adv_w = 64, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 866, .adv_w = 64, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 867, .adv_w = 147, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 868, .adv_w = 174, .box_w = 9, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 870, .adv_w = 50, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 871, .adv_w = 50, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 872, .adv_w = 86, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 874, .adv_w = 84, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 876, .adv_w = 151, .box_w = 8, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 878, .adv_w = 210, .box_w = 12, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 896, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_2[] = {
    0x0, 0x7, 0x9, 0xb, 0x1b, 0x24, 0x36, 0x3c,
    0x3f, 0x44, 0x56, 0x5c, 0x1f70, 0x1f71, 0x1f73, 0x1f74,
    0x1f78, 0x1f7a, 0x1f7c, 0x1f7e, 0x1f86, 0x1f90, 0x200c
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 64, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 29, .glyph_id_start = 65,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 160, .range_length = 8205, .glyph_id_start = 94,
        .unicode_list = unicode_list_2, .glyph_id_ofs_list = NULL, .list_length = 23, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 1, 0, 2, 0, 0, 0, 0,
    2, 3, 0, 0, 0, 4, 0, 4,
    5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 6, 7, 8, 9, 10, 11,
    0, 12, 12, 13, 14, 15, 12, 12,
    9, 16, 17, 18, 0, 19, 13, 20,
    21, 22, 23, 24, 25, 0, 0, 0,
    0, 26, 27, 28, 0, 29, 30, 0,
    31, 0, 0, 32, 0, 31, 31, 33,
    27, 0, 34, 0, 35, 0, 36, 37,
    38, 36, 39, 40, 0, 0, 0, 0,
    0, 0, 0, 6, 9, 13, 0, 26,
    33, 0, 0, 0, 0, 0, 2, 4,
    2, 4, 4, 0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 1, 0, 2, 0, 0, 0, 3,
    2, 0, 4, 5, 0, 6, 7, 6,
    8, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    9, 0, 10, 0, 11, 0, 0, 0,
    11, 0, 0, 12, 0, 0, 0, 0,
    11, 0, 11, 0, 13, 14, 15, 16,
    17, 18, 19, 20, 0, 0, 21, 0,
    0, 22, 0, 23, 23, 23, 24, 23,
    0, 0, 0, 0, 0, 25, 25, 26,
    25, 23, 27, 28, 29, 30, 31, 32,
    33, 31, 34, 0, 0, 35, 0, 0,
    0, 36, 37, 10, 11, 15, 0, 22,
    26, 30, 0, 0, 7, 7, 2, 6,
    2, 6, 6, 0, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -13, 0,
    0, 0, 0, 0, 0, 0, -15, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -6, -7, 0, -2, -8, 0,
    -10, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 2, 0, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -21, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -28, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -15, 0, 0, 0, 0, 0,
    0, -8, 0, -1, 0, 0, -16, -2,
    -11, -9, 0, -12, 0, 0, 0, 0,
    0, 0, -1, 0, 0, -2, -1, -6,
    -4, 0, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -3, 0, -3, 0, 0,
    -7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -4, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    0, 0, 0, 0, 0, -13, 0, 0,
    0, -3, 0, 0, 0, -3, 0, -3,
    0, -3, -5, -3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 3, 0, 0, 0, 0, 0,
    0, 0, 0, -2, -2, 0, -2, 0,
    0, 0, -2, -3, -3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -29,
    0, 0, 0, -21, 0, -33, 0, 3,
    0, 0, 0, 0, 0, 0, 0, -4,
    -3, 0, 0, -3, -3, 0, 0, -3,
    -3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, -4, 0, 0, 0,
    2, -3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -8,
    0, 0, 0, -4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    0, -3, -3, 0, 0, 0, -3, -5,
    -8, 0, 0, 0, 0, 0, 0, -42,
    0, 0, 0, 0, 0, 0, 0, 2,
    -8, 0, 0, -34, -7, -22, -18, 0,
    -30, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -5, -17, -11, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -40, 0, 0, 0, -17, 0, -25, 0,
    0, 0, 0, 0, -4, 0, -3, 0,
    -1, -2, 0, 0, -2, 0, 0, 2,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -5, 0, -3,
    -2, 0, -4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -10, 0, -2, 0, 0, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -5, 0, 0, 0, 0, -27,
    -29, 0, 0, -10, -3, -30, -2, 2,
    0, 2, 2, 0, 2, 0, 0, -14,
    -12, 0, -14, -12, -9, -14, 0, -12,
    -9, -7, -10, -7, 0, -41, -27, 0,
    0, 0, 3, 0, -28, -5, 0, 0,
    -9, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, -6, -5, 0, 0,
    -6, -4, 0, 0, -3, -1, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 2,
    0, -15, -7, 0, 0, -5, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    2, -4, -4, 0, 0, -4, -3, 0,
    0, -2, 0, 0, 0, 0, 2, 0,
    0, 0, 0, 0, 0, 0, 0, -6,
    0, 0, 0, -3, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, -3,
    0, 0, -3, 0, 0, 0, -3, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    -4, 3, -6, -26, -6, 0, 0, -12,
    -4, -12, -2, 2, -12, 2, 2, 2,
    2, 0, 2, -9, -8, -3, -5, -8,
    -5, -7, -3, -5, -2, 0, -3, -4,
    2, -10, -6, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 0, 0, -3, 0, 0, 0,
    -2, -3, -3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -8, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, -4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, -2, -2, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 2, 0, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 0, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 0, 0, 0, -13,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -17, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    -3, -2, 0, 0, 0, 0, 2, 0,
    0, 0, -15, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -5, -2, 2, 0, -2, 0,
    0, 6, 0, 2, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, -13, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, -2, 2, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -15, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 40,
    .right_class_cnt     = 37,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR >= 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 3,
    .bpp = 1,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR >= 8
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t robotocondensedregular16 = {
#else
lv_font_t robotocondensedregular16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 18,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
    .fallback = NULL,
    .user_data = NULL
};



#endif /*#if ROBOTOCONDENSEDREGULAR16*/

