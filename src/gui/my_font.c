// /*******************************************************************************
//  * Size: 16 px
//  * Bpp: 4
//  * Opts: 
//  ******************************************************************************/

// #include "lvgl.h"
// #include "pgmspace.h"

// #ifndef MY_FONT
// #define MY_FONT 1
// #endif

// #if MY_FONT

// /*-----------------
//  *    BITMAPS
//  *----------------*/

// /*Store the image of the glyphs*/

// static const uint8_t glyph_bitmap[] PROGMEM = {
//     /* U+0031 "1" */
//     0x0, 0xc2, 0x48, 0xcf, 0x59, 0xdf, 0xea, 0x40,
//     0xf, 0xf8, 0x5e, 0x65, 0x74, 0x84, 0x20, 0x1f,
//     0xfc, 0x31, 0x6b, 0xdd, 0xb2, 0x4c, 0x3, 0xff,
//     0xfe, 0x1, 0xff, 0xff, 0x0, 0xff, 0xea, 0x9,
//     0x19, 0xf3, 0xf0, 0x80, 0x4, 0xcf, 0xc4, 0x1,
//     0x96, 0xf3, 0x3a, 0x98, 0x80, 0x9, 0x19, 0x9b,
//     0x68, 0xc0,

//     /* U+0032 "2" */
//     0x0, 0xc4, 0xb1, 0x57, 0xbd, 0xfd, 0xb9, 0x2c,
//     0x40, 0x1f, 0xa, 0xde, 0x52, 0x86, 0x45, 0x5c,
//     0x20, 0x34, 0xfc, 0x18, 0x7, 0xa, 0xc5, 0x66,
//     0x29, 0xd5, 0x10, 0xd3, 0xa, 0x4, 0xd4, 0x80,
//     0x1e, 0x34, 0x31, 0x0, 0xf9, 0x74, 0xc0, 0x4c,
//     0x40, 0x3f, 0xf8, 0xab, 0xc4, 0x4, 0x46, 0x0,
//     0xff, 0xe2, 0x26, 0xb8, 0x92, 0xd9, 0x80, 0x7f,
//     0xf0, 0x89, 0xe1, 0x85, 0x2e, 0x4c, 0x3, 0xff,
//     0x82, 0x4d, 0x30, 0x66, 0x89, 0x61, 0x0, 0xff,
//     0xe0, 0x13, 0xf4, 0xa1, 0xc7, 0x39, 0x0, 0x7f,
//     0xc2, 0x93, 0xf0, 0x49, 0x3a, 0xe4, 0x1, 0xff,
//     0x13, 0x55, 0x18, 0x8d, 0xb9, 0x48, 0xcf, 0xf1,
//     0x0, 0x47, 0x70, 0x82, 0x0, 0x37, 0xac, 0xcf,
//     0xed, 0xa3, 0x0,

//     /* U+0033 "3" */
//     0x0, 0xc4, 0xb1, 0x59, 0xbd, 0xfd, 0xcc, 0xa7,
//     0x41, 0x0, 0xf8, 0xe3, 0xe9, 0x42, 0xe2, 0xae,
//     0x1c, 0x12, 0x2e, 0x94, 0x40, 0x38, 0xe3, 0x3b,
//     0x29, 0xd5, 0x10, 0xd3, 0xec, 0x40, 0x98, 0xa0,
//     0x1f, 0x8, 0xc0, 0x1f, 0x8, 0x4, 0x7c, 0xc0,
//     0x1f, 0xfc, 0x11, 0x25, 0x8e, 0x73, 0x4a, 0x52,
//     0x0, 0xff, 0x2d, 0xff, 0x6d, 0x39, 0x35, 0x51,
//     0x88, 0x3, 0xfe, 0x49, 0xcb, 0xa8, 0x62, 0x7b,
//     0xc8, 0x40, 0xf, 0xf8, 0x48, 0xd1, 0x5e, 0x76,
//     0xd0, 0x9e, 0x1c, 0x40, 0x3f, 0xf8, 0x82, 0xf8,
//     0x60, 0x7c, 0xa0, 0x18, 0x95, 0x44, 0x1, 0xfc,
//     0xda, 0x40, 0x42, 0xc0, 0x11, 0xd4, 0x56, 0x62,
//     0x5d, 0x51, 0xa, 0xf7, 0x4a, 0x27, 0x54, 0x20,
//     0x8, 0xe3, 0x2e, 0x54, 0xee, 0x2a, 0xed, 0x4e,
//     0x91, 0x7a, 0xe4, 0x1, 0x0,

//     /* U+0034 "4" */
//     0x0, 0xff, 0x8e, 0x77, 0xfe, 0xc7, 0x20, 0xf,
//     0xfe, 0x8, 0xac, 0x31, 0x88, 0x80, 0x3f, 0xf8,
//     0x85, 0x30, 0x88, 0x77, 0x18, 0x7, 0xff, 0xd,
//     0x21, 0x89, 0x7b, 0x44, 0xc0, 0x3f, 0xf8, 0x22,
//     0xf0, 0xa6, 0xf2, 0xae, 0x1, 0xff, 0xc2, 0x38,
//     0x93, 0x4b, 0x93, 0x0, 0xff, 0xe1, 0xb, 0x43,
//     0x3, 0x43, 0x8, 0x7, 0xff, 0x8, 0xe6, 0xd0,
//     0xe2, 0x6, 0x1, 0xff, 0xc3, 0x2b, 0x52, 0x3,
//     0x9e, 0xff, 0xec, 0x71, 0x3, 0x8d, 0xfb, 0x61,
//     0x29, 0xfd, 0xdf, 0xf5, 0x28, 0x81, 0x3d, 0xec,
//     0xb8, 0x80, 0x91, 0x7f, 0xcc, 0x40, 0x1, 0xf6,
//     0x22, 0x8, 0x7, 0xff, 0x34,

//     /* U+0035 "5" */
//     0x0, 0xcb, 0x5f, 0xff, 0xfc, 0x1b, 0x60, 0xf,
//     0x8c, 0x40, 0x9, 0x17, 0x7f, 0xd0, 0xe0, 0x1e,
//     0x21, 0x70, 0x23, 0x54, 0x4f, 0xe3, 0x10, 0xf,
//     0xf, 0x88, 0xf, 0x98, 0x7, 0xff, 0x15, 0xc4,
//     0xc0, 0xae, 0xb3, 0x7b, 0x9b, 0x70, 0x82, 0x1,
//     0xf9, 0xb1, 0xd5, 0xeb, 0xfd, 0xd5, 0x2c, 0xaf,
//     0x7a, 0xe4, 0x1, 0xe2, 0x59, 0xa9, 0x64, 0x33,
//     0x23, 0xd4, 0x20, 0x1d, 0x50, 0x80, 0x3f, 0xf8,
//     0x82, 0x98, 0x40, 0x44, 0x70, 0xf, 0xfe, 0x28,
//     0xf8, 0x4, 0x62, 0x1, 0x84, 0xcc, 0x20, 0x1f,
//     0x89, 0x10, 0x40, 0x7e, 0xa0, 0x11, 0xcf, 0x67,
//     0xdc, 0x32, 0xa2, 0xb4, 0xfc, 0x18, 0xb4, 0x30,
//     0x80, 0x47, 0x19, 0x72, 0x27, 0x57, 0x54, 0x84,
//     0x67, 0xac, 0x94, 0x0, 0xc0,

//     /* U+0036 "6" */
//     0x0, 0xf8, 0x56, 0x2f, 0x3b, 0xad, 0xca, 0x74,
//     0x10, 0xf, 0xc9, 0x3f, 0x4e, 0xe5, 0x8b, 0xb9,
//     0x5a, 0x61, 0xc8, 0x3, 0x85, 0xa1, 0x85, 0x2b,
//     0x65, 0x95, 0x4d, 0x39, 0xd8, 0xc4, 0x1, 0x89,
//     0xa8, 0xc1, 0x79, 0x44, 0x3, 0xff, 0x86, 0xfa,
//     0x60, 0x7a, 0xe0, 0x1f, 0xfc, 0x41, 0x36, 0x0,
//     0x38, 0xb3, 0xd6, 0x6f, 0x73, 0x6e, 0x10, 0x40,
//     0x3f, 0xc9, 0x10, 0xec, 0x8e, 0xdb, 0x46, 0x7b,
//     0xa6, 0x10, 0x8, 0x4d, 0x80, 0x9, 0x1b, 0x2c,
//     0x86, 0x65, 0x98, 0x50, 0x4d, 0x70, 0xc, 0xc6,
//     0x20, 0xe2, 0x1, 0xf9, 0x8c, 0x41, 0x84, 0x80,
//     0x23, 0xd7, 0x13, 0xc7, 0x10, 0xe, 0x24, 0x51,
//     0x6, 0x22, 0x0, 0x64, 0xb9, 0x34, 0x8a, 0x74,
//     0x34, 0x68, 0xa3, 0x16, 0xf5, 0x0, 0xf1, 0x3e,
//     0xdc, 0x8d, 0xd6, 0xd5, 0xb2, 0x4e, 0x4a, 0x0,
//     0x40,

//     /* U+0061 "a" */
//     0x0, 0xc2, 0x6d, 0x37, 0x9b, 0xdf, 0xdb, 0x92,
//     0xc4, 0x1, 0xf8, 0xe7, 0xec, 0xcd, 0x53, 0x57,
//     0x8, 0x2d, 0x3f, 0x4, 0x1, 0xe3, 0x8c, 0xda,
//     0x86, 0x44, 0x9a, 0x6d, 0x0, 0x96, 0x88, 0x3,
//     0xfe, 0x12, 0x45, 0x67, 0xc7, 0x0, 0x10, 0xb0,
//     0x7, 0x85, 0x22, 0xfb, 0xfe, 0xce, 0x2, 0x61,
//     0x0, 0xfc, 0x4f, 0xb6, 0xc1, 0x1f, 0xb7, 0x30,
//     0xf5, 0xa6, 0x1, 0xf9, 0xf0, 0xc1, 0x75, 0xc4,
//     0x3, 0x87, 0xc0, 0x3f, 0x84, 0x84, 0x12, 0xa1,
//     0x51, 0xb, 0x17, 0xcc, 0x40, 0x1f, 0x92, 0x29,
//     0xcc, 0x9a, 0xa9, 0x3c, 0x1d, 0xf6, 0xc0, 0x1e,

//     /* U+0062 "b" */
//     0x0, 0x1c, 0x77, 0xf4, 0x98, 0x7, 0xff, 0xed,
//     0x5a, 0xb3, 0xba, 0xdb, 0x84, 0x10, 0xf, 0xf0,
//     0xb4, 0x6e, 0xa6, 0xad, 0x85, 0x5e, 0xec, 0xa0,
//     0x1f, 0xc2, 0xb5, 0x6e, 0xa8, 0xb1, 0xcc, 0x40,
//     0xbc, 0xa0, 0x1f, 0xc6, 0xe0, 0x1e, 0x2d, 0x70,
//     0x3, 0x69, 0x0, 0x7f, 0xf1, 0x44, 0xc0, 0x6,
//     0x2, 0x1, 0xff, 0xc5, 0x12, 0x0, 0x17, 0x88,
//     0x7, 0xf3, 0x80, 0x79, 0x7d, 0x0, 0x54, 0xc8,
//     0x3, 0xe1, 0x68, 0x96, 0x55, 0x3d, 0xd9, 0x40,
//     0x9a, 0x90, 0x3, 0xf2, 0xcc, 0x86, 0xae, 0xd2,
//     0xca, 0xd5, 0x90, 0x60, 0x10,

//     /* U+0063 "c" */
//     0x0, 0xf8, 0x96, 0x2f, 0x37, 0xbf, 0xb7, 0x29,
//     0xcc, 0x40, 0x3c, 0x4d, 0x9b, 0x4c, 0xeb, 0x75,
//     0x75, 0x68, 0x3a, 0xa6, 0x1, 0xcb, 0x12, 0x62,
//     0xd5, 0x92, 0xc8, 0x93, 0x45, 0xd3, 0x10, 0x6,
//     0x3c, 0x40, 0x17, 0xa4, 0x0, 0xff, 0xe2, 0x31,
//     0x8, 0x17, 0x8, 0x7, 0xff, 0x14, 0x48, 0x40,
//     0xb8, 0x40, 0x3f, 0xf8, 0xab, 0x8a, 0x2, 0xf4,
//     0x80, 0x1f, 0xfc, 0x41, 0x59, 0x83, 0x16, 0xac,
//     0x86, 0x44, 0x95, 0xeb, 0x29, 0x44, 0x3, 0x89,
//     0xf3, 0x12, 0xee, 0x58, 0xbb, 0x54, 0x58, 0x5f,
//     0x20, 0x80,

//     /* U+0064 "d" */
//     0x0, 0xff, 0xe1, 0x9c, 0xf7, 0xf4, 0x18, 0x7,
//     0xff, 0xac, 0x96, 0x73, 0x7b, 0xac, 0x9b, 0x30,
//     0xf, 0xf1, 0xc7, 0xd3, 0x13, 0xcd, 0xd4, 0x8a,
//     0x20, 0x40, 0x3f, 0x24, 0xb9, 0xb, 0x55, 0x1d,
//     0x51, 0x62, 0xe, 0x20, 0x1f, 0x11, 0xb8, 0x1,
//     0x31, 0x0, 0x3c, 0xe0, 0x1f, 0xc3, 0xc6, 0x2,
//     0x62, 0x1, 0xff, 0xc5, 0x1e, 0x30, 0x13, 0x10,
//     0xf, 0xfe, 0x29, 0x11, 0x80, 0xf, 0xa6, 0x1,
//     0xe6, 0x31, 0x0, 0xfc, 0xbe, 0xa2, 0x4f, 0xf2,
//     0xca, 0xa7, 0xb8, 0x50, 0xf, 0xf2, 0x56, 0xc2,
//     0x8b, 0xc5, 0xce, 0xea, 0x64, 0x80, 0x1e,

//     /* U+0065 "e" */
//     0x0, 0xf1, 0x2c, 0x5e, 0xf7, 0xf6, 0xe5, 0x31,
//     0x0, 0x7f, 0x24, 0xe5, 0x30, 0xd5, 0x33, 0x61,
//     0x5a, 0x72, 0x50, 0x3, 0x89, 0xe5, 0x81, 0x6a,
//     0x8e, 0x86, 0x65, 0x98, 0x60, 0x6e, 0x50, 0xc,
//     0x98, 0x40, 0x9e, 0x80, 0x1f, 0x2f, 0x90, 0x39,
//     0x10, 0x0, 0x42, 0xc0, 0x5, 0xae, 0xef, 0xe9,
//     0x30, 0x3f, 0x10, 0x1, 0xb, 0x0, 0x12, 0x73,
//     0xff, 0xee, 0xed, 0xf2, 0x60, 0x12, 0x19, 0x2,
//     0xfb, 0x8, 0xff, 0xf0, 0x40, 0x31, 0x44, 0xb0,
//     0x35, 0x5c, 0x2a, 0x19, 0x91, 0x5d, 0xca, 0x40,
//     0x1f, 0x24, 0xfd, 0xb0, 0xa4, 0x56, 0x62, 0xa2,
//     0xf4, 0xc4, 0x2,

//     /* U+0066 "f" */
//     0x0, 0xff, 0x12, 0xcd, 0xef, 0x7f, 0xbb, 0x72,
//     0x98, 0xc0, 0x3f, 0xe1, 0x7e, 0xa6, 0x56, 0x9a,
//     0xbd, 0x99, 0x72, 0x18, 0x7, 0xfc, 0xba, 0x60,
//     0x4d, 0xe, 0xa8, 0x68, 0xa8, 0x40, 0x1e, 0x17,
//     0xcf, 0xfa, 0xd4, 0x0, 0x4f, 0x9f, 0xfd, 0x6a,
//     0x1, 0xf0, 0xb7, 0x66, 0x50, 0x80, 0x2, 0x6a,
//     0xcc, 0xe9, 0x40, 0xf, 0xc2, 0x46, 0x78, 0x40,
//     0x2e, 0x73, 0x3e, 0x21, 0x0, 0xff, 0xff, 0x80,
//     0x7f, 0xff, 0xc0, 0x3f, 0xf8, 0xa0,

//     /* U+4E2D "中" */
//     0x0, 0xff, 0xe3, 0xad, 0x7f, 0x49, 0x80, 0x7f,
//     0xfd, 0x84, 0xd5, 0x7f, 0xf8, 0x98, 0xc0, 0x25,
//     0x6a, 0xbf, 0xfc, 0x34, 0x20, 0xc, 0xb5, 0x55,
//     0xc4, 0xcf, 0xfd, 0xe, 0x60, 0x5, 0x54, 0xcf,
//     0xfd, 0x17, 0x54, 0x95, 0x20, 0xf, 0xb, 0xf6,
//     0x67, 0xfd, 0xb6, 0xa0, 0x2f, 0xd9, 0x9f, 0xfc,
//     0x8, 0x40, 0xf, 0xff, 0xf8, 0x7, 0xff, 0x38,
//     0xd8, 0x8b, 0xff, 0x82, 0x20, 0x3, 0x62, 0x2f,
//     0xfe, 0x0, 0xb8, 0x7, 0xf8, 0x56, 0xb7, 0x7f,
//     0xf8, 0x10, 0x80, 0x2b, 0x5b, 0xbf, 0xf6, 0x41,
//     0x80, 0x7f, 0x85, 0xaf, 0xff, 0xff, 0x81, 0x4a,
//     0x2, 0xd7, 0xff, 0xff, 0x74, 0xa0, 0x7, 0xcb,
//     0x79, 0xb6, 0x80, 0x1f, 0xfc, 0x57, 0x0, 0xff,
//     0x85, 0xeb, 0x7d, 0x88, 0x2, 0x12, 0x33, 0x10,
//     0x7, 0xff, 0x4c, 0x44, 0x44, 0x10, 0xf, 0xff,
//     0xf8, 0x7, 0xff, 0x18,

//     /* U+4EBA "人" */
//     0x0, 0xff, 0xe4, 0xb, 0xe7, 0xf4, 0x18, 0x7,
//     0xff, 0x74, 0xc0, 0x23, 0x0, 0xff, 0xef, 0x8f,
//     0x88, 0x4, 0x20, 0x1f, 0xfd, 0xd7, 0x22, 0x1,
//     0x11, 0xc0, 0x3f, 0xfb, 0x84, 0x6c, 0x1, 0x3e,
//     0x98, 0x7, 0xff, 0x6d, 0xf4, 0xc0, 0x22, 0x54,
//     0x20, 0xf, 0xfe, 0xca, 0x62, 0x2, 0x43, 0x99,
//     0x52, 0x90, 0x7, 0xff, 0x5d, 0x79, 0x85, 0x35,
//     0xaa, 0xcc, 0xaa, 0x84, 0x1, 0xff, 0xd4, 0x16,
//     0xe5, 0x5, 0xe7, 0x12, 0x6a, 0x42, 0x68, 0x40,
//     0xf, 0xfe, 0x88, 0xad, 0xc2, 0x13, 0xc2, 0x88,
//     0x0, 0x5a, 0x1c, 0xd6, 0x60, 0xc0, 0x3f, 0xf9,
//     0xa2, 0xb7, 0x65, 0x5, 0x98, 0x20, 0xf, 0x1c,
//     0xc2, 0x9b, 0xf4, 0xa0, 0x7, 0xff, 0x20, 0x52,
//     0x32, 0x94, 0xdf, 0x69, 0x0, 0x3f, 0x85, 0x6b,
//     0x5c, 0x1a, 0xb1, 0xcc, 0x3, 0xff, 0x84, 0x4d,
//     0x39, 0x6e, 0x6b, 0x59, 0x6, 0x1, 0xff, 0xc2,
//     0x38, 0xca, 0x42, 0x8c, 0xc4, 0xb1, 0x0, 0x78,
//     0x56, 0xbb, 0x65, 0xda, 0x33, 0xe9, 0x44, 0x3,
//     0xff, 0x8c, 0x2b, 0x5f, 0x92, 0xc9, 0x3b, 0xd6,
//     0xa2, 0x0, 0x14, 0x98, 0x9c, 0xfb, 0x73, 0x10,
//     0xf, 0xfe, 0x60, 0x9b, 0x56, 0xea, 0xed, 0xa,
//     0x20,

//     /* U+56FD "国" */
//     0x0, 0x14, 0x6f, 0xff, 0xff, 0x66, 0x94, 0x3,
//     0xe4, 0x8d, 0xee, 0xff, 0xe8, 0xd2, 0x88, 0x7,
//     0xfc, 0x44, 0x11, 0x0, 0x7f, 0xf3, 0x4, 0x66,
//     0x30, 0xf, 0xfe, 0x11, 0x3e, 0xf7, 0x7f, 0xf2,
//     0xa9, 0x80, 0x3f, 0xf8, 0xc4, 0xf9, 0xff, 0xfa,
//     0x94, 0xe, 0x3b, 0xff, 0xf5, 0xb0, 0x7, 0xff,
//     0xd5, 0x6b, 0xff, 0xea, 0x50, 0x38, 0xef, 0xff,
//     0xa9, 0x40, 0x3f, 0xf9, 0x2b, 0x5d, 0xdf, 0x4a,
//     0x1, 0x46, 0x76, 0x4b, 0xce, 0x75, 0x28, 0x7,
//     0xff, 0x2c, 0x47, 0xe3, 0x10, 0x10, 0x21, 0x22,
//     0xd, 0xf4, 0x20, 0x80, 0x7f, 0xf2, 0x44, 0x7f,
//     0x8c, 0x40, 0x40, 0x84, 0x16, 0x26, 0x80, 0xd4,
//     0x40, 0x3f, 0xf8, 0xa2, 0xb5, 0xdd, 0xfd, 0x28,
//     0x5, 0x19, 0xdd, 0xae, 0x1a, 0x2e, 0x98, 0x40,
//     0x3f, 0xf8, 0x62, 0xb5, 0xff, 0xff, 0xcc, 0xb6,
//     0x10, 0xf, 0xfe, 0xa, 0x5e, 0xef, 0xff, 0x4d,
//     0x84, 0x3, 0xfc, 0x6d, 0x33, 0xff, 0xd3, 0x40,
//     0xe,

//     /* U+6211 "我" */
//     0x0, 0xff, 0x8, 0x88, 0xd1, 0x5e, 0x26, 0xf3,
//     0x6d, 0xc8, 0x12, 0xbf, 0xd2, 0x80, 0x29, 0x12,
//     0xe6, 0x20, 0x1f, 0xe2, 0x6a, 0xcd, 0xef, 0xfd,
//     0x98, 0xbb, 0x87, 0xfa, 0x3a, 0x9c, 0x80, 0x6,
//     0x20, 0x60, 0x1, 0x5d, 0xa, 0xdc, 0x95, 0x10,
//     0xf, 0x89, 0xe3, 0xb3, 0xfe, 0xee, 0x5a, 0x81,
//     0xa5, 0xb2, 0xa1, 0x88, 0x4, 0xe2, 0x1, 0x8,
//     0x0, 0x56, 0x72, 0x91, 0x15, 0xae, 0x40, 0x1e,
//     0x12, 0x20, 0x88, 0x3, 0xff, 0x8c, 0x20, 0x60,
//     0x26, 0x1, 0xc2, 0x93, 0x9d, 0xae, 0x40, 0x18,
//     0x5a, 0xfb, 0xbf, 0x52, 0x81, 0x3e, 0x77, 0x7e,
//     0xc7, 0x20, 0x49, 0xee, 0xfd, 0xff, 0x75, 0xb1,
//     0x0, 0x5, 0xe7, 0x77, 0xf4, 0x18, 0xb, 0x5e,
//     0xef, 0xf4, 0x20, 0x1, 0x23, 0x77, 0xfe, 0x97,
//     0x20, 0x8, 0x48, 0xcf, 0xf1, 0x30, 0x18, 0xb9,
//     0x9f, 0xf2, 0x99, 0x3, 0x81, 0x19, 0xc2, 0xb1,
//     0xb0, 0xa2, 0x42, 0x1, 0xff, 0xc5, 0x32, 0x55,
//     0x34, 0x4d, 0x5c, 0xa8, 0x96, 0xb8, 0x1f, 0xa8,
//     0x13, 0xf4, 0xaa, 0x73, 0x90, 0x7, 0x9, 0xab,
//     0x3c, 0x4d, 0x5e, 0x75, 0x28, 0xa, 0x54, 0xe0,
//     0x8b, 0x76, 0x71, 0x7, 0xd3, 0x4, 0xea, 0xf8,
//     0x25, 0xab, 0x51, 0x0, 0xf1, 0xaf, 0x5d, 0x88,
//     0xb, 0x2a, 0xdc, 0xc0, 0x9b, 0xf2, 0xa6, 0x1d,
//     0x90, 0x80, 0x4, 0x8a, 0x24, 0xaa, 0x18, 0xca,
//     0x41, 0x0, 0xfc, 0x4d, 0x7b, 0x95, 0x30, 0xec,
//     0xba, 0xc0, 0x1f, 0xfc, 0x2, 0x59, 0xe6, 0x10,
//     0x2, 0x53, 0x10, 0x6, 0x48, 0xb7, 0x30, 0xf,
//     0xfe, 0x50, 0x9b, 0x4e, 0x62, 0xd5, 0x2a, 0x8e,
//     0x49, 0x72, 0x60, 0x18, 0x49, 0x35, 0x80, 0x3c,
//     0x24, 0x5c, 0x9a, 0xc0, 0xf, 0x1, 0x5a, 0xfc,
//     0x82, 0x4a, 0xfc, 0x95, 0x54, 0xc2, 0x9b, 0xf5,
//     0x32, 0x37, 0xa1, 0x6b, 0x80, 0x70, 0xbc, 0xee,
//     0xeb, 0x61, 0x4a, 0x42, 0x15, 0xbc, 0xdf, 0xb8,
//     0x41, 0x0, 0xc2, 0xb5, 0xb0, 0xee, 0x9b, 0x94,
//     0x44, 0x31, 0x0, 0x70, 0xb5, 0xff, 0xf7, 0x6d,
//     0xb9, 0x80, 0x42, 0x44, 0x10, 0xf, 0xf8, 0xde,
//     0xb7, 0xfe, 0xea, 0x51, 0x0, 0x0,

//     /* U+662F "是" */
//     0x0, 0xf9, 0x6f, 0xfe, 0xee, 0xff, 0xe6, 0x7f,
//     0xd4, 0xa0, 0x1f, 0xfc, 0x53, 0x9f, 0xee, 0xff,
//     0xe5, 0x6b, 0x88, 0x7, 0xff, 0x20, 0xe3, 0x33,
//     0xff, 0x97, 0xcc, 0x20, 0x1f, 0xfc, 0x83, 0x9f,
//     0xcc, 0xff, 0xe5, 0x4b, 0x88, 0x7, 0xff, 0x27,
//     0xd8, 0x8b, 0xff, 0x94, 0x26, 0x1, 0xff, 0xc6,
//     0x13, 0x47, 0x3f, 0x99, 0xff, 0xe5, 0x70, 0xaa,
//     0x6b, 0x0, 0x7f, 0x9, 0x16, 0x6a, 0x99, 0xff,
//     0xe9, 0x28, 0x91, 0x61, 0x0, 0xc9, 0x19, 0x9f,
//     0xfc, 0x6d, 0xde, 0xcc, 0xff, 0xe2, 0xcb, 0x88,
//     0x4, 0xb5, 0xdd, 0xed, 0x89, 0x8d, 0xfe, 0xee,
//     0xc8, 0x30, 0x49, 0xee, 0xff, 0xe2, 0xdb, 0x8,
//     0x7, 0xf9, 0x62, 0x63, 0x18, 0x40, 0x3f, 0x9b,
//     0xd5, 0x13, 0xff, 0x82, 0x62, 0x1, 0xff, 0xc0,
//     0x16, 0xe4, 0x0, 0x9, 0x90, 0x7, 0xf1, 0xa4,
//     0xff, 0xff, 0xf0, 0x6d, 0x40, 0x3f, 0xe3, 0x9b,
//     0x42, 0x33, 0x2c, 0xf4, 0x18, 0x7, 0xcb, 0x79,
//     0x77, 0xff, 0xc1, 0x84, 0x0, 0xfc, 0x6f, 0x7a,
//     0xe0, 0xd3, 0x99, 0x4b, 0xb6, 0x7e, 0xdc, 0x61,
//     0x3, 0xf1, 0x91, 0x61, 0x1f, 0xf1, 0x14, 0x20,
//     0x11, 0x47, 0x42, 0xaa, 0x7e, 0x50, 0x0, 0x4d,
//     0x7d, 0xb7, 0x46, 0x0, 0x22, 0x9b, 0x77, 0xbb,
//     0xbf, 0xdb, 0xaf, 0x62, 0x0, 0x14, 0x66, 0xfd,
//     0xb1, 0x0, 0x7e, 0x24, 0x78, 0xab, 0xcd, 0xee,
//     0xbf, 0xff, 0xf8, 0x9a, 0xe4, 0x0
// };


// /*---------------------
//  *  GLYPH DESCRIPTION
//  *--------------------*/

// static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
//     {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
//     {.bitmap_index = 0, .adv_w = 128, .box_w = 24, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 50, .adv_w = 128, .box_w = 24, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 157, .adv_w = 128, .box_w = 24, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 274, .adv_w = 128, .box_w = 24, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 367, .adv_w = 128, .box_w = 24, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 476, .adv_w = 128, .box_w = 24, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 597, .adv_w = 128, .box_w = 24, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 685, .adv_w = 128, .box_w = 24, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 770, .adv_w = 128, .box_w = 24, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 852, .adv_w = 128, .box_w = 24, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 939, .adv_w = 128, .box_w = 24, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 1030, .adv_w = 128, .box_w = 27, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
//     {.bitmap_index = 1100, .adv_w = 256, .box_w = 45, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
//     {.bitmap_index = 1224, .adv_w = 256, .box_w = 48, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
//     {.bitmap_index = 1393, .adv_w = 256, .box_w = 45, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
//     {.bitmap_index = 1530, .adv_w = 256, .box_w = 48, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
//     {.bitmap_index = 1824, .adv_w = 256, .box_w = 48, .box_h = 15, .ofs_x = 0, .ofs_y = -2}
// };

// /*---------------------
//  *  CHARACTER MAPPING
//  *--------------------*/

// static const uint16_t unicode_list_0[] = {
//     0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x30, 0x31,
//     0x32, 0x33, 0x34, 0x35, 0x4dfc, 0x4e89, 0x56cc, 0x61e0,
//     0x65fe
// };

// /*Collect the unicode lists and glyph_id offsets*/
// static const lv_font_fmt_txt_cmap_t cmaps[] =
// {
//     {
//         .range_start = 49, .range_length = 26111, .glyph_id_start = 1,
//         .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 17, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
//     }
// };



// /*--------------------
//  *  ALL CUSTOM DATA
//  *--------------------*/

// #if LV_VERSION_CHECK(8, 0, 0)
// /*Store all the custom data of the font*/
// static  lv_font_fmt_txt_glyph_cache_t cache;
// static const lv_font_fmt_txt_dsc_t font_dsc = {
// #else
// static lv_font_fmt_txt_dsc_t font_dsc = {
// #endif
//     .glyph_bitmap = glyph_bitmap,
//     .glyph_dsc = glyph_dsc,
//     .cmaps = cmaps,
//     .kern_dsc = NULL,
//     .kern_scale = 0,
//     .cmap_num = 1,
//     .bpp = 4,
//     .kern_classes = 0,
//     .bitmap_format = 1,
// #if LV_VERSION_CHECK(8, 0, 0)
//     .cache = &cache
// #endif
// };


// /*-----------------
//  *  PUBLIC FONT
//  *----------------*/

// /*Initialize a public general font descriptor*/
// #if LV_VERSION_CHECK(8, 0, 0)
// const lv_font_t my_font = {
// #else
// lv_font_t my_font = {
// #endif
//     .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
//     .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
//     .line_height = 15,          /*The maximum line height required by the font*/
//     .base_line = 2,             /*Baseline measured from the bottom of the line*/
// #if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
//     .subpx = LV_FONT_SUBPX_HOR,
// #endif
// #if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
//     .underline_position = -2,
//     .underline_thickness = 1,
// #endif
//     .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
// };



// #endif /*#if MY_FONT*/
