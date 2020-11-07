#define mcufriend480
//#define adafruit320

#ifdef mcufriend480
 //(MCUFRIEND screen 480x320)
 #include <UTFTGLUE.h>
 MCUFRIEND_kbv tft;
 #define screen_width 480
 #define screen_height 320
 #define mapx1 980
 #define mapx2 110
//  #define mapy1 145
//  #define mapy2 902
 #define mapy2 145
 #define mapy1 902
#endif

#ifdef adafruit320
 //(screen 320x240)
 #include <Adafruit_TFTLCD.h>
 Adafruit_TFTLCD tft;
 #define screen_width 320
 #define screen_height 240
 #define mapx1 930
 #define mapx2 170
 #define mapy1 145
 #define mapy2 914
#endif

#include "TouchScreen.h"
// int XP = 6, YP = A1, XM = A2, YM = 7;
int XP = 8, YP = A3, XM = A2, YM = 9;
TouchScreen ts(XP, YP, XM, YM, 300);
TSPoint tp;

int touchx,touchy,touched;
#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define PinBuzz 44

boolean rotate=0;
boolean sound=0;
boolean menu=0;
boolean pause=0;
unsigned long guitime=0;
unsigned long statustime=0;

char poledisp[64]; //
char statusbuf1[11],statusbuf2[11],statusbuf3[11];
char status_c1, status_c2;

const uint8_t fig24[6][72] PROGMEM={
{0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1E, 0x0,  0x0,  0x3F, 0x0,  0x80, 0x7F, 0x0,  0x80, 0x7F, 0x0,
0x80, 0x7F, 0x0,  0x0,  0x3F, 0x0,  0x0,  0x1C, 0x0,  0x0,  0x1C, 0x0,  0x80, 0xFF, 0x0,  0x80, 0xFF, 0x0,  0x0,  0x1C, 0x0,  0x0,  0x1C, 0x0,
0x0,  0x3E, 0x0,  0x0,  0x7F, 0x0,  0x80, 0xFF, 0x0,  0xC0, 0xFF, 0x1,  0xE0, 0xFF, 0x3,  0xE0, 0xFF, 0x3,  0xE0, 0xFF, 0x3,  0x0,  0x0,  0x0
}, //pawn
{0x0, 0x0,  0x0,  0x0,  0x6E, 0x0,  0x0,  0xFF, 0x0,  0x80, 0xFF, 0x1,  0x80, 0xFF, 0x3,  0xC0, 0xFF, 0x7,  0xE0, 0xFF, 0xF,  0xE0, 0xF9, 0xF,
0xF0, 0xF8, 0xF,  0xF0, 0xF8, 0xF,  0x60, 0xFC, 0xF,  0x0,  0xFE, 0xF,  0x0,  0xFF, 0xF,  0x0,  0xFF, 0x7,  0x0,  0xFF, 0x3,  0x0,  0xFF, 0x1,
0x0,  0xFE, 0x0,  0x0,  0x7C, 0x0,  0x0,  0x38, 0x0,  0x0,  0x38, 0x0,  0x0,  0x7C, 0x0,  0xF0, 0xFF, 0xF,  0xF0, 0xFF, 0xF,  0x0,  0x0,  0x0
}, //knight
{0x0, 0x0,  0x0,  0x0,  0x3C, 0x0,  0x0,  0x7E, 0x0,  0x0,  0x7E, 0x0,  0x0,  0x3C, 0x0,  0x0,  0x18, 0x0,  0x80, 0xFF, 0x1,  0xC0, 0xFF, 0x3,
0xE0, 0xFF, 0x7,  0xE0, 0xFF, 0x7,  0xE0, 0xFF, 0x7,  0xE0, 0xFF, 0x7,  0xE0, 0xFF, 0x7,  0xE0, 0xFF, 0x7,  0xE0, 0xFF, 0x7,  0xE0, 0xFF, 0x7,
0xE0, 0xFF, 0x7,  0xC0, 0xFF, 0x3,  0x80, 0xFF, 0x1,  0x0,  0xFF, 0x0,  0x8,  0x7E, 0x10, 0x1C, 0x3C, 0x38, 0xFE, 0xFF, 0x7F, 0x0,  0x0,  0x0
}, //bishop
{0x0, 0x0,  0x0,  0xF0, 0x3C, 0xF,  0xF0, 0x3C, 0xF,  0xF0, 0xFF, 0xF,  0xE0, 0xFF, 0x7,  0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,
0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,
0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,  0xC0, 0xFF, 0x3,  0xE0, 0xFF, 0x7,  0xF0, 0xFF, 0xF,  0xF0, 0xFF, 0xF,  0x0,  0x0,  0x0
}, //rook
{0x0, 0x0,  0x0,  0xEE, 0xBD, 0x77, 0xEE, 0xBD, 0x77, 0xC6, 0x18, 0x63, 0xC6, 0x18, 0x63, 0xC6, 0x18, 0x63, 0xCC, 0x18, 0x33, 0xCC, 0x18, 0x33,
0xCC, 0x18, 0x33, 0xCC, 0x18, 0x33, 0xCC, 0x18, 0x33, 0x98, 0x99, 0x19, 0x98, 0x99, 0x19, 0x98, 0x99, 0x19, 0x98, 0x99, 0x19, 0x98, 0x99, 0x19,
0x30, 0xFF, 0xC,  0xB0, 0xFF, 0xD,  0xF0, 0xFF, 0xF,  0xE0, 0xFF, 0x7,  0xE0, 0xFF, 0x7,  0xF0, 0xFF, 0xF,  0xF0, 0xFF, 0xF,  0x0,  0x0,  0x0
}, //queen
{0x0, 0x0,  0x0,  0x0,  0x18, 0x0,  0xF0, 0x18, 0xF,  0xF8, 0x99, 0x1F, 0xFC, 0xDB, 0x3F, 0xFE, 0xFF, 0x7F, 0xFE, 0xFF, 0x7F, 0xFE, 0xFF, 0x7F,
0xFE, 0xFF, 0x7F, 0xFE, 0xFF, 0x7F, 0xFE, 0xFF, 0x7F, 0xFE, 0xFF, 0x7F, 0xFE, 0xFF, 0x7F, 0xFE, 0xFF, 0x3F, 0xFC, 0xFF, 0x1F, 0xF8, 0xFF, 0x1F,
0xF0, 0xFF, 0xF,  0xE0, 0xFF, 0x7,  0xE0, 0xFF, 0x7,  0xC0, 0xFF, 0x3,  0xE0, 0xFF, 0x7,  0xF0, 0xFF, 0xF,  0xF0, 0xFF, 0xF,  0x0,  0x0,  0x0
} //king
};

const uint8_t fig24_cont[6][72] PROGMEM={
{0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1C, 0x0,  0x0,  0x22, 0x0,  0x0,  0x41, 0x0,  0x80, 0x80, 0x0,  0x80, 0x80, 0x0,
0x80, 0x80, 0x0,  0x0,  0x41, 0x0,  0x0,  0x22, 0x0,  0xC0, 0xE3, 0x1,  0x40, 0x0,  0x1,  0x40, 0x0,  0x1,  0xC0, 0xE3, 0x1,  0x0,  0x22, 0x0,
0x0,  0x41, 0x0,  0x80, 0x80, 0x0,  0x40, 0x0,  0x1,  0x20, 0x0,  0x2,  0x10, 0x0,  0x4,  0x10, 0x0,  0x4,  0x10, 0x0,  0x4,  0xF0, 0xFF, 0x7
}, //pawn
{0x0, 0x6E, 0x0,  0x0,  0x91, 0x0,  0x80, 0x0,  0x1,  0x40, 0x2,  0x2,  0x40, 0x0,  0x4,  0x20, 0x0,  0x8,  0x10, 0x0,  0x10, 0x10, 0x6,  0x10,
0x8,  0x5,  0x10, 0x8,  0x5,  0x10, 0x90, 0x2,  0x10, 0x60, 0x1,  0x10, 0x80, 0x0,  0x10, 0x80, 0x0,  0x8,  0x80, 0x0,  0x4,  0x80, 0x0,  0x2,
0x0,  0x1,  0x1,  0x0,  0x82, 0x0,  0x0,  0x44, 0x0,  0x0,  0x44, 0x0,  0xF8, 0x83, 0x1F, 0x8,  0x0,  0x10, 0x8,  0x0,  0x10, 0xF8, 0xFF, 0x1F
}, //knight
{0x0, 0x3C, 0x0,  0x0,  0x42, 0x0,  0x0,  0x81, 0x0,  0x0,  0x81, 0x0,  0x0,  0x42, 0x0,  0x80, 0xE7, 0x1,  0x40, 0x0,  0x2,  0x20, 0x18, 0x4,
0x10, 0x18, 0x8,  0x10, 0x18, 0x8,  0x10, 0x18, 0x8,  0x90, 0xFF, 0x9,  0x90, 0xFF, 0x9,  0x10, 0x18, 0x8,  0x10, 0x18, 0x8,  0x10, 0x18, 0x8,
0x10, 0x18, 0x8,  0x20, 0x18, 0x4,  0x40, 0x18, 0x2,  0x88, 0x0,  0x11, 0x14, 0x81, 0x28, 0xE2, 0xC3, 0x47, 0x1,  0x0,  0x80, 0xFF, 0xFF, 0xFF
}, //bishop
{0xF8, 0x7E, 0x1F, 0x8,  0xC3, 0x10, 0x8,  0xC3, 0x10, 0x8,  0x0,  0x10, 0x10, 0x0,  0x8,  0xE0, 0xFF, 0x7,  0x20, 0x0,  0x4,  0x20, 0x0,  0x4,
0x20, 0x0,  0x4,  0x20, 0x0,  0x4,  0x20, 0x0,  0x4,  0x20, 0x0,  0x4,  0x20, 0x0,  0x4,  0x20, 0x0,  0x4,  0x20, 0x0,  0x4,  0x20, 0x0,  0x4,
0x20, 0x0,  0x4,  0x20, 0x0,  0x4,  0x20, 0x0,  0x4,  0xE0, 0xFF, 0x7,  0x10, 0x0,  0x8,  0x8,  0x0,  0x10, 0x8,  0x0,  0x10, 0xF8, 0xFF, 0x1F
}, //rook
{0xEF, 0xBD, 0xF7, 0x11, 0x42, 0x88, 0x11, 0x42, 0x88, 0x29, 0xA5, 0x94, 0x29, 0xA5, 0x94, 0x29, 0xA5, 0x94, 0x32, 0xA5, 0x4C, 0x32, 0xA5, 0x4C,
0x32, 0xA5, 0x4C, 0x32, 0xA5, 0x4C, 0x32, 0xA5, 0x4C, 0x64, 0x66, 0x26, 0x64, 0x66, 0x26, 0x64, 0x66, 0x26, 0x64, 0x66, 0x26, 0x64, 0x66, 0x26,
0xC8, 0x0,  0x13, 0x48, 0x0,  0x12, 0x8,  0x0,  0x10, 0x10, 0x0,  0x8,  0xF0, 0xFF, 0xF,  0x8,  0x0,  0x10, 0x8,  0x0,  0x10, 0xF8, 0xFF, 0x1F
}, //queen
{0x0, 0x18, 0x0,  0xF0, 0x24, 0xF,  0x8,  0xA5, 0x10, 0x4,  0x66, 0x20, 0x2,  0x24, 0x40, 0x1,  0x24, 0x80, 0x1,  0x18, 0x80, 0x1,  0x18, 0x80,
0x1,  0x18, 0x80, 0x1,  0x18, 0x80, 0x1,  0x18, 0x80, 0x1,  0x18, 0x80, 0x1,  0x18, 0x80, 0x1,  0x18, 0x40, 0x2,  0x18, 0x20, 0x4,  0x18, 0x20,
0xE8, 0xFF, 0x17, 0x10, 0x0,  0x8,  0x10, 0x0,  0x8,  0x20, 0x0,  0x4,  0xF0, 0xFF, 0xF,  0x8,  0x0,  0x10, 0x8,  0x0,  0x10, 0xF8, 0xFF, 0x1F
} //king
};

const uint8_t fig32[6][128] PROGMEM={
{0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xF0, 0xF,  0x0,  0x0,  0xF8, 0x1F, 0x0,
0x0,  0xFC, 0x3F, 0x0,  0x0,  0xFC, 0x3F, 0x0,  0x0,  0xFC, 0x3F, 0x0,  0x0,  0xF8, 0x1F, 0x0,
0x0,  0xF0, 0xF,  0x0,  0x0,  0xE0, 0x7,  0x0,  0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,
0x0,  0xFF, 0xFF, 0x0,  0x0,  0xE0, 0x7,  0x0,  0x0,  0xF0, 0xF,  0x0,  0x0,  0xF0, 0xF,  0x0,
0x0,  0xF8, 0x1F, 0x0,  0x0,  0xF8, 0x1F, 0x0,  0x0,  0xFC, 0x3F, 0x0,  0x0,  0xFE, 0x7F, 0x0,
0x0,  0xFE, 0x7F, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x80, 0xFF, 0xFF, 0x1,
0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x0,  0x0,  0x0,  0x0
},
{0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xC0, 0xC,  0x0,  0x0,  0xE0, 0x1C, 0x0,
0x0,  0xF8, 0x3F, 0x0,  0x0,  0xFE, 0x7F, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x80, 0xFF, 0xFF, 0x1,
0xC0, 0xFF, 0xFF, 0x3,  0xE0, 0xFF, 0xFF, 0x7,  0xF0, 0xFF, 0xFF, 0xF,  0xF0, 0xBF, 0xFF, 0x1F,
0xF0, 0x81, 0xFF, 0x1F, 0xE0, 0xC0, 0xFF, 0x1F, 0x0,  0xE0, 0xFF, 0x1F, 0x0,  0xF0, 0xFF, 0x1F,
0x0,  0xF8, 0xFF, 0x1F, 0x0,  0xFC, 0xFF, 0x1F, 0x0,  0xFE, 0xFF, 0xF,  0x0,  0xFE, 0xFF, 0x7,
0x0,  0xFE, 0xFF, 0x1,  0x0,  0xFE, 0xFF, 0x0,  0x0,  0xFE, 0x7F, 0x0,  0x0,  0xFE, 0x3F, 0x0,
0x0,  0xFE, 0x1F, 0x0,  0x0,  0xFC, 0x1F, 0x0,  0x0,  0xF8, 0xF,  0x0,  0x0,  0xF0, 0xF,  0x0,
0x0,  0xFF, 0xFF, 0x0,  0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x0,  0x0,  0x0,  0x0
}, {
0x0,  0x0,  0x0,  0x0,  0x0,  0xE0, 0x7,  0x0,  0x0,  0xF0, 0xF,  0x0,  0x0,  0xF8, 0x1F, 0x0,
0x0,  0xFC, 0x3F, 0x0,  0x0,  0xF8, 0x1F, 0x0,  0x0,  0xF0, 0xF,  0x0,  0x0,  0xE0, 0x7,  0x0,
0x0,  0xFE, 0x7F, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,
0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,
0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,
0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x0,  0xFF, 0xFF, 0x0,
0x0,  0xFE, 0x7F, 0x0,  0x0,  0xFC, 0x3F, 0x0,  0x20, 0xF8, 0x1F, 0x4,  0x70, 0xF0, 0xF,  0xE,
0xF8, 0xFF, 0xFF, 0x1F, 0xF8, 0xFF, 0xFF, 0x1F, 0xF8, 0xFF, 0xFF, 0x1F, 0x0,  0x0,  0x0,  0x0
}, {
 0x0, 0x0,  0x0,  0x0,  0xE0, 0xE3, 0xC7, 0x7,  0xE0, 0xE3, 0xC7, 0x7,  0xE0, 0xE3, 0xC7, 0x7,
0xE0, 0xFF, 0xFF, 0x7,  0xE0, 0xFF, 0xFF, 0x7,  0xE0, 0xFF, 0xFF, 0x7,  0xC0, 0xFF, 0xFF, 0x3,
0x80, 0xFF, 0xFF, 0x1,  0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,
0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,
0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,
0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0x0,
0x80, 0xFF, 0xFF, 0x1,  0xC0, 0xFF, 0xFF, 0x3,  0xE0, 0xFF, 0xFF, 0x7,  0xF0, 0xFF, 0xFF, 0xF,
0xF0, 0xFF, 0xFF, 0xF,  0xF0, 0xFF, 0xFF, 0xF,  0xF0, 0xFF, 0xFF, 0xF,  0x0,  0x0,  0x0,  0x0
}, {
0x0,  0x0,  0x0,  0x0,  0x0,  0x80, 0x1,  0x0,  0x0,  0xC6, 0x63, 0x0,  0x0,  0xCF, 0xF3, 0x0,
0xC,  0x8F, 0xF1, 0x30, 0x1E, 0x86, 0x61, 0x78, 0x1E, 0xC6, 0x63, 0x78, 0xC,  0xCF, 0xF3, 0x30,
0xC,  0xCF, 0xF3, 0x30, 0x1E, 0xCF, 0xF3, 0x78, 0x1E, 0xCF, 0xF3, 0x78, 0x1C, 0xCF, 0xF3, 0x38,
0x3C, 0x8E, 0x71, 0x3C, 0x3C, 0xCE, 0x73, 0x3C, 0x38, 0xEE, 0x77, 0x1C, 0x38, 0xFE, 0x7F, 0x1E,
0x38, 0xFF, 0xFF, 0x1E, 0x78, 0xFF, 0xFF, 0x1E, 0xF0, 0xFF, 0xFF, 0xF,  0xF0, 0xFF, 0xFF, 0xF,
0xF0, 0xFF, 0xFF, 0xF,  0xF0, 0xFF, 0xFF, 0xF,  0xF0, 0xFF, 0xFF, 0x7,  0xE0, 0xFF, 0xFF, 0x7,
0xC0, 0xFF, 0xFF, 0x3,  0xC0, 0xFF, 0xFF, 0x3,  0x80, 0xFF, 0xFF, 0x1,  0x0,  0xFF, 0xFF, 0x0,
0x0,  0xFF, 0xFF, 0x0,  0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x0,  0x0,  0x0,  0x0
}, {
0x0,  0x0,  0x0,  0x0,  0x0,  0x80, 0x1,  0x0,  0x0,  0xC0, 0x3,  0x0,  0x0,  0xC0, 0x3,  0x0,
0xE0, 0x81, 0x81, 0x7,  0xF8, 0x87, 0xE1, 0x1F, 0xFC, 0xCF, 0xF3, 0x3F, 0xFE, 0xDF, 0xFB, 0x7F,
0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F,
0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F,
0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFC, 0xFF, 0xFF, 0x3F, 0xF8, 0xFF, 0xFF, 0x1F,
0xF8, 0xFF, 0xFF, 0x1F, 0xF0, 0xFF, 0xFF, 0xF,  0xF0, 0xFF, 0xFF, 0xF,  0xE0, 0xFF, 0xFF, 0x7,
0xC0, 0xFF, 0xFF, 0x3,  0xC0, 0xFF, 0xFF, 0x3,  0x80, 0xFF, 0xFF, 0x1,  0x0,  0xFF, 0xFF, 0x0,
0x0,  0xFF, 0xFF, 0x0,  0x80, 0xFF, 0xFF, 0x1,  0x80, 0xFF, 0xFF, 0x1,  0x0,  0x0,  0x0,  0x0
}
};

const uint8_t fig32_cont[6][128] PROGMEM={
{0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
0x0,  0x0,  0x0,  0x0,  0x0,  0xF0, 0xF,  0x0,  0x0,  0x8,  0x10, 0x0,  0x0,  0x4,  0x20, 0x0,
0x0,  0x2,  0x40, 0x0,  0x0,  0x2,  0x40, 0x0,  0x0,  0x2,  0x40, 0x0,  0x0,  0x4,  0x20, 0x0,
0x0,  0x8,  0x10, 0x0,  0x80, 0x1F, 0xF8, 0x1,  0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,
0x80, 0x0,  0x0,  0x1,  0x80, 0x1F, 0xF8, 0x1,  0x0,  0x8,  0x10, 0x0,  0x0,  0x8,  0x10, 0x0,
0x0,  0x4,  0x20, 0x0,  0x0,  0x4,  0x20, 0x0,  0x0,  0x2,  0x40, 0x0,  0x0,  0x1,  0x80, 0x0,
0x0,  0x1,  0x80, 0x0,  0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,  0x40, 0x0,  0x0,  0x2,
0x40, 0x0,  0x0,  0x2,  0x40, 0xFF, 0xFF, 0x2,  0x40, 0x0,  0x0,  0x2,  0xC0, 0xFF, 0xFF, 0x3
}, {
0x0,  0x0,  0x0,  0x0,  0x0,  0xC0, 0xC,  0x0,  0x0,  0x20, 0x13, 0x0,  0x0,  0x18, 0x23, 0x0,
0x0,  0x6,  0x60, 0x0,  0x0,  0x19, 0x90, 0x0,  0x80, 0x18, 0x88, 0x1,  0x40, 0x0,  0x40, 0x2,
0x20, 0x0,  0x20, 0x6,  0x10, 0x0,  0x0,  0x9,  0x8,  0x0,  0x80, 0x18, 0x8,  0x40, 0x0,  0x24,
0x8,  0x7E, 0x0,  0x22, 0x10, 0x21, 0x0,  0x20, 0xE0, 0x10, 0x0,  0x20, 0x0,  0x8,  0x0,  0x20,
0x0,  0x4,  0x0,  0x20, 0x0,  0x2,  0x0,  0x20, 0x0,  0x1,  0x0,  0x10, 0x0,  0x1,  0x0,  0x8,
0x0,  0x1,  0x0,  0x6,  0x0,  0x1,  0x0,  0x1,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,  0x40, 0x0,
0x0,  0x1,  0x20, 0x0,  0x0,  0x2,  0x20, 0x0,  0x0,  0x4,  0x10, 0x0,  0x0,  0xF,  0xF0, 0x0,
0x80, 0x0,  0x0,  0x1,  0x40, 0xFF, 0xFF, 0x2,  0x40, 0x0,  0x0,  0x2,  0xC0, 0xFF, 0xFF, 0x3
}, {
0x0,  0xE0, 0x7,  0x0,  0x0,  0x10, 0x8,  0x0,  0x0,  0x8,  0x10, 0x0,  0x0,  0x4,  0x20, 0x0,
0x0,  0x2,  0x40, 0x0,  0x0,  0x4,  0x20, 0x0,  0x0,  0x8,  0x10, 0x0,  0x0,  0x1E, 0x78, 0x0,
0x0,  0x1,  0x80, 0x0,  0x80, 0x80, 0x1,  0x1,  0x40, 0x80, 0x1,  0x2,  0x40, 0x80, 0x1,  0x2,
0x40, 0x80, 0x1,  0x2,  0x40, 0x80, 0x1,  0x2,  0x40, 0x80, 0x1,  0x2,  0x40, 0xFE, 0x7F, 0x2,
0x40, 0xFE, 0x7F, 0x2,  0x40, 0xFE, 0x7F, 0x2,  0x40, 0x80, 0x1,  0x2,  0x40, 0x80, 0x1,  0x2,
0x40, 0x80, 0x1,  0x2,  0x40, 0x80, 0x1,  0x2,  0x40, 0x80, 0x1,  0x2,  0x80, 0x80, 0x1,  0x1,
0x0,  0x81, 0x81, 0x0,  0x20, 0x82, 0x41, 0x4,  0x50, 0x4,  0x20, 0xA,  0x88, 0xF,  0xF0, 0x11,
0x4,  0x0,  0x0,  0x20, 0x4,  0x0,  0x0,  0x20, 0x4,  0x0,  0x0,  0x20, 0xFC, 0xFF, 0xFF, 0x3F
}, {
0xF0,  0xF7, 0xEF, 0xF,  0x10, 0x14, 0x28, 0x8,  0x10, 0x14, 0x28, 0x8,  0x10, 0x1C, 0x38, 0x8,
0x10, 0x0,  0x0,  0x8,  0x90, 0x81, 0x81, 0x9,  0x90, 0xFF, 0xFF, 0x9,  0x20, 0x0,  0x0,  0x4,
0x40, 0x0,  0x0,  0x2,  0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,
0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,
0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,
0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,  0x80, 0x0,  0x0,  0x1,
0x40, 0xFE, 0x7F, 0x2,  0x20, 0x0,  0x0,  0x4,  0x10, 0x0,  0x0,  0x8,  0x8,  0x0,  0x0,  0x10,
0xE8, 0xFF, 0xFF, 0x17, 0x8,  0x0,  0x0,  0x10, 0x8,  0x0,  0x0,  0x10, 0xF8, 0xFF, 0xFF, 0x1F
}, {
0x0,  0x80, 0x1,  0x0,  0x0,  0x46, 0x62, 0x0,  0x0,  0x29, 0x94, 0x0,  0x8C, 0x30, 0xC,  0x31,
0x92, 0x50, 0xA,  0x49, 0x21, 0x49, 0x92, 0x84, 0x21, 0x29, 0x94, 0x84, 0x92, 0x30, 0xC,  0x49,
0x92, 0x30, 0xC,  0x49, 0xA1, 0x30, 0xC,  0x85, 0xA1, 0x30, 0xC,  0x85, 0xA2, 0x30, 0xC,  0x45,
0x42, 0x51, 0x8A, 0x42, 0x42, 0x31, 0x8C, 0x42, 0x44, 0x11, 0x88, 0x22, 0x44, 0x1,  0x80, 0x21,
0xC4, 0x0,  0x0,  0x21, 0x84, 0x0,  0x0,  0x21, 0x8,  0xE0, 0x7,  0x10, 0x8,  0x1E, 0x78, 0x10,
0xC8, 0x1,  0x80, 0x13, 0x8,  0x80, 0x1,  0x10, 0x8,  0xC2, 0x43, 0x8,  0x10, 0x87, 0xE1, 0x8,
0x20, 0x2,  0x40, 0x4,  0x20, 0x0,  0x0,  0x4,  0x40, 0x0,  0x0,  0x2,  0x80, 0x0,  0x0,  0x1,
0x80, 0xFE, 0x7F, 0x1,  0x40, 0x0,  0x0,  0x2,  0x40, 0x0,  0x0,  0x2,  0xC0, 0xFF, 0xFF, 0x3
}, {
 0x0, 0x80, 0x1,  0x0,  0x0,  0x40, 0x2,  0x0,  0x0,  0x20, 0x4,  0x0,  0xE0, 0x21, 0x84, 0x7,
0x18, 0x46, 0x62, 0x18, 0x4,  0x48, 0x12, 0x20, 0x2,  0x30, 0xC,  0x40, 0x1,  0x30, 0xC,  0x80,
0x1,  0x60, 0x6,  0x80, 0x1,  0xC0, 0x3,  0x80, 0x1,  0x80, 0x1,  0x80, 0x1,  0x80, 0x1,  0x80,
0x1,  0x80, 0x1,  0x80, 0x1,  0x80, 0x1,  0x80, 0x1,  0x80, 0x1,  0x80, 0x1,  0x80, 0x1,  0x80,
0x1,  0x80, 0x1,  0x80, 0x1,  0x80, 0x1,  0x80, 0x2,  0x80, 0x1,  0x40, 0x4,  0x80, 0x1,  0x20,
0x4,  0x80, 0x1,  0x20, 0x8,  0xE0, 0x7,  0x10, 0x8,  0x1C, 0x38, 0x10, 0xD0, 0x3,  0xC0, 0xB,
0x20, 0x80, 0x1,  0x4,  0x20, 0xC6, 0x63, 0x4,  0x40, 0x80, 0x1,  0x2,  0x80, 0x0,  0x0,  0x1,
0x80, 0xFE, 0x5F, 0x1,  0x40, 0x0,  0x0,  0x2,  0x40, 0x0,  0x0,  0x2,  0xC0, 0xFF, 0xFF, 0x3
}
};

uint16_t BLACK,BLUE,RED,GREEN,CYAN,MAGENTA,YELLOW,WHITE,GRAY,DARK,GRAY2,BLACKF,WHITEF;

//**********************************
class Button {
public:
int bx,by,bw,bh,bshift;
String bs;
const uint8_t* bp;
 Button(int x,int y,int w,int h,int x1,int y1,int w1,int h1,String s,const uint8_t* p, int shift);
 void Show(int shift);
 void Show(String s);
 void Hide();
 boolean IsPressed();
private:
boolean active;
 void ishow(uint16_t textcolor,uint16_t backcolor);
};
Button::Button(int x,int y,int w,int h,int x1,int y1,int w1,int h1,String s,const uint8_t* p=NULL, int shift=0) {
  if (screen_height==320) {
   bx=x; by=y; bw=w; bh=h;
  } else {
   bx=x1; by=y1; bw=w1; bh=h1;
  }
  bs=s; bp=p; bshift=shift;
};
void Button::Show(int shift=0) {
 bshift=shift;
 active=true;
 ishow(WHITE,DARK);
};
void Button::Show(String s) {
 bs=s;
 active=true;
 ishow(WHITE,DARK);
};
void Button::Hide() {
 active=false;
 ishow(GRAY,DARK);
};
boolean Button::IsPressed() {
  if (active&&abs(bx+bw/2-touchx)<bw/2&&abs(by+bh/2-touchy)<bh/2) {
    ishow(WHITE,GRAY);
    touchx=0; touchy=0;
    return true;
  }
  return false;
};
void Button::ishow(uint16_t textcolor,uint16_t backcolor) {
  tft.fillRoundRect(bx+2,by+2,bw,bh,3,GRAY2);
  tft.fillRoundRect(bx,by,bw,bh,3,backcolor);
  tft.setTextColor(textcolor);
  if (screen_height==320) {
   tft.setTextSize(2);
   tft.setCursor(bx+(bw-bs.length()*12)/2,by+(bh-12)/2);
  }  else {
   tft.setTextSize(1);
   tft.setCursor(bx+(bw-bs.length()*6)/2,by+(bh-8)/2);
  }
  if (bp!=NULL) {
   tft.drawBitmap(1+bx+(bw-16)/2, by+(bh-16)/2, bp+bshift*32, 16, 16,textcolor);
  }
  else  tft.print(bs);
}

Button BAction(354, 280, 100, 30, 252, 210, 60, 20, "START");
Button BMenu(354, 20, 100, 30, 252, 20, 60, 20, "MENU");
Button BBack(354, 58, 100, 30, 252, 47, 60, 20, "BACK");
Button BTime(354, 96, 100, 30, 252, 74, 60, 20, "5 s");
Button BNew(354, 134, 100, 30, 252, 101, 60, 20, "NEW");
Button BAuto(354, 172, 100, 30, 252, 128, 60, 20, "AUTO:B");
Button BRotate(354, 210, 100, 30, 252, 155, 60, 20, "ROTATE");
Button BSave(354, 248, 100, 30, 252, 182, 60, 20, "SAVE");
Button BLoad(354, 286, 100, 30, 252, 209, 60, 20, "LOAD");

//**********************************
void drawBitmap(int16_t x, int16_t y,
            const uint8_t *bitmap, int16_t w, int16_t h,
            uint16_t color) {
  int16_t i, j, byteWidth = (w + 7) / 8;
  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
        if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (1 << (i % 8))) {
        tft.drawPixel(x+i, y+j, color);
      }
    }
  }
}
//****************************
uint16_t getColor(uint8_t red, uint8_t green, uint8_t blue)
{
  red   >>= 3;
  green >>= 2;
  blue  >>= 3;
  return (red << 11) | (green << 5) | blue;
}
//****************************
void show_fig(int c) {
uint16_t colorf,color,color_cont;
   colorf=BLACKF;
   int i=column[c]-1;
   int j=8-row[c];
   if ((i+j+2)%2==0) colorf=WHITEF;
   int jj=j, ii=i;
   if (rotate) { jj=7-j; ii=7-i; }
   color=BLACK; color_cont=getColor(128,128,100);
   if (pole[c]>0) { color=WHITE; color_cont=BLACK; }
   if (screen_height==320) {
    tft.fillRect(ii*39+10,jj*39,39,39,colorf);
    tft.drawRect(ii*39+10,jj*39,39,39,GRAY);
    if (pole[c]!=0) {
     drawBitmap(ii*39+14, jj*39+5,&fig32[abs(pole[c])-1][0], 32, 32,color);
     drawBitmap(ii*39+14, jj*39+5,&fig32_cont[abs(pole[c])-1][0], 32, 32,color_cont);
    }
   } else { //height 240
    tft.fillRect(ii*28+12,jj*28,29,29,colorf);
    tft.drawRect(ii*28+12,jj*28,29,29,GRAY);
    if (pole[c]!=0) {
     drawBitmap(ii*28+14, jj*28+3,&fig24[abs(pole[c])-1][0], 24, 24,color);
     drawBitmap(ii*28+14, jj*28+3,&fig24_cont[abs(pole[c])-1][0], 24, 24,color_cont);
    }
   }
}
//****************************
void show_color() {
int posx, posyw, posyb, rad;
if (screen_height==320) {
  posx=334; posyw=290; posyb=24; rad=5;
 } else { //240
  posx=244; posyw=214; posyb=20; rad=4;
 }
 if (rotate) { int r=posyb; posyb=posyw; posyw=r; }
 tft.fillCircle(posx,posyb,rad,BLACK); tft.fillCircle(posx,posyw,rad,BLACK);
 if (pos[0].w) tft.fillCircle(posx,posyw,rad,GRAY); else  tft.drawCircle(posx,posyb,rad,GRAY);
 uint16_t col=BLACK;
 int r2=rad*2;
 if (autow) col=DARK;
 tft.drawRect(posx-rad,posyw+r2,r2,r2,col);
 col=BLACK;
 if (autob) col=DARK;
 tft.drawRect(posx-rad,posyb+r2,r2,r2,col);
}
//****************************
void show_board() {
  for (int i=0;i<64;i++) {
    if (poledisp[i]!=pole[i]) show_fig(i);
    poledisp[i]=pole[i];
  }
  show_color();
}
//****************************
void border() {
  tft.setTextColor(GRAY,BLACK);
  tft.setTextSize(1);
  if (screen_height==320) {
   for (int j=1;j<9;j++) {
    tft.setCursor(1,j*39-22);
    if (rotate) tft.print(j); else tft.print(9-j);
   }
   for (byte i=1;i<9;i++) {
    tft.setCursor(i*39-11,312);
    if (rotate) tft.print(char(96+9-i)); else tft.print(char(96+i));
   }
  } else { //240
   for (int j=1;j<9;j++) {
    tft.setCursor(2,j*28-15);
    if (rotate) tft.print(j); else tft.print(9-j);
   }
   for (byte i=1;i<9;i++) {
    tft.setCursor(i*28-1,228);
    if (rotate) tft.print(char(96+9-i)); else tft.print(char(96+i));
   }
  }
}
//****************************
void cleardisplayboard() {
  for (int i=0;i<64;i++) poledisp[i]=-100; //
}
//****************************
void initboard() {
  cleardisplayboard();
  tft.fillScreen(BLACK);
  border();
  tft.setTextSize(1);
  if (screen_height==320) {
   tft.setCursor(337,1);  tft.setTextColor(WHITE);
   tft.print(F("Arduino Mega Chess II"));
  } else { //240
   tft.setCursor(242,1); tft.setTextColor(WHITE);
   tft.print(F("Arduino Mega"));
   tft.setCursor(252,10);
   tft.print(F("Chess II"));
  }
}
//****************************
void show_steps() {
  if (screen_height==320) {
   tft.fillRect(330,56,135,150,GRAY2);
  }
   else {
   tft.fillRect(240,44,74,114,GRAY2);
  }
  tft.setTextColor(GRAY);
  tft.setTextSize(2);
  int st=0;
  if (ply>8) st=ply-8;
  if (st<0) st=0;
  for (int i=st;i<ply;i++) {
    if (i%2==0) {
     tft.setTextSize(1);
     if (screen_height==320)
      tft.setCursor(332,(i-st)*18+64);
     else
      tft.setCursor(244,(i-st)*14+48);
     tft.print(String(i/2+1)+".");
    }
    if (screen_height==320) {
     tft.setTextSize(2);
     tft.setCursor(360,(i-st)*18+60);
    } else {
     tft.setTextSize(1);
     tft.setCursor(268,(i-st)*14+48);
    }
    tft.print(str_step(steps[i]));
  }



}
//****************************
void definecolors() {
 BLACK  =0x0000;
 BLUE   =0x07FF;  //0x001F;
 RED    =0xF800;
 GREEN  =0x07E0;
 CYAN   =0x07FF;
 MAGENTA=0xF81F;
 YELLOW =0xFFE0;
 WHITE  =0xFFFF;
 GRAY   =0x7BEF;
 DARK   =getColor(32,32,32);
 GRAY2   =getColor(16,16,16);
 BLACKF  =getColor(94,58,0);
 WHITEF  =getColor(180,114,0);
}
//****************************
void beep(int leng) {
  analogWrite(PinBuzz, 20);
  delay(2);
  if (!sound) analogWrite(PinBuzz, 0);
  delay(leng);
  analogWrite(PinBuzz, 0);
}
//****************************
void guistart() {
  tft.reset();
  tft.begin(tft.readID());
  tft.setRotation(1);
  definecolors();
  initboard();
  BAction.Show("START");
  BMenu.Show("MENU");
  show_board();
}
//****************************
String get_time(int tim) {
  char sz[10];
  //sprintf(sz, "%02d:%02d:%02d", tim/3600,(tim%3600)/60,tim%60);
  sprintf(sz, "%d:%02d",tim/60,tim%60);
  String s=sz; s.trim();
  return String(sz);
}
//****************************
void clearstatus() {
  if (screen_height==320) {
    tft.fillRect(330,210,135,60,GRAY2);
  } else {
    tft.fillRect(242,169,70,36,GRAY2);
  }
  for (int j=0;j<11;j++) { statusbuf1[j]=' '; statusbuf2[j]=' '; statusbuf3[j]=' '; }
  guitime=millis();
  statustime=guitime;
  status_c1=-1;
  status_c2=-1;
}
//****************************
void print_changed(String s, char* b, uint16_t c) {
  for (int i=0;i<11;i++) {
    if (b[i]!=s[i]) {
     tft.setTextColor(c,GRAY2);
     tft.print(s[i]);
    } else {
     tft.setTextColor(GRAY2,GRAY2);
     tft.print(" ");
    }
    b[i]=s[i];
  }
}
//****************************
void status_step(char c,byte show,uint16_t col=0) {
uint16_t color,color2;
  if (c<0||c>63) return;
  color=BLACKF;
  int i=column[c]-1;
  int j=8-row[c];
  if ((i+j+2)%2==0) color=WHITEF;
  color2=GRAY;
  if (show) { color=col; color2=color; }
  int jj=j, ii=i;
  if (rotate) { jj=7-j; ii=7-i; }
  if (screen_height==320) {
   tft.drawRect(ii*39+10,jj*39,39,39,color2);
   tft.drawRect(ii*39+11,jj*39+1,37,37,color);
  } else { //height 240
   tft.drawRect(ii*28+12,jj*28,29,29,color2);
   tft.drawRect(ii*28+13,jj*28+1,27,27,color);
  }
}
//****************************
void showstatus(String s1, String s2, String s3, uint16_t c1=GRAY, uint16_t c2=GRAY, uint16_t c3=GRAY) {
  tft.setTextWrap(1);
  tft.setTextColor(GRAY);
  if (screen_height==320) {
   tft.setTextSize(2);
   tft.setCursor(334,213); print_changed(s1,statusbuf1,c1);
   tft.setCursor(334,233); print_changed(s2,statusbuf2,c2);
   tft.setCursor(334,253); print_changed(s3,statusbuf3,c3);
  } else {
   tft.setTextSize(1);
   tft.setCursor(244,172); print_changed(s1,statusbuf1,c1);
   tft.setCursor(244,184); print_changed(s2,statusbuf2,c2);
   tft.setCursor(244,196); print_changed(s3,statusbuf3,c3);
  }
  if (status_c1!=pos[0].cut.c1||status_c2!=pos[0].cut.c2) {
    if (status_c1!=-1) { status_step(status_c1,0); status_step(status_c2,0); }
    status_c1=pos[0].cut.c1;
    status_c2=pos[0].cut.c2;
    status_step(status_c1,1,getColor(220,0,0));
    status_step(status_c2,1,getColor(0,180,0));
   }
}
//****************************
boolean gettouch() {
  //digitalWrite(13, HIGH);
  tp = ts.getPoint();
  //digitalWrite(13, LOW);
  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  int x=map(tp.y,mapx1,mapx2,0,screen_width-1);
  int y=map(tp.x,mapy1,mapy2,0,screen_height-1);
  touched=0;
  if ((abs(x-touchx)>5||abs(y-touchy)>5)&&tp.z>MINPRESSURE&&tp.z<MAXPRESSURE) {
    touched=1;
    touchx=x;
    touchy=y;
    //tft.drawPixel(x,y,GREEN);
    //Serial.println(tp.y);
    //Serial.println(tp.x);
  }
  return touched;
}
//****************************
void erasestatus() {
  if (status_c1>-1) status_step(status_c1,0);
  if (status_c2>-1) status_step(status_c2,0);
  status_c1=-1;
  status_c2=-1;
}
//**********************************
signed char field_pressed() {
 signed char c;
 if (screen_height==320) {
  c=(touchx-10)/39+8*(touchy/39);
 } else {
  c=(touchx-10)/28+8*(touchy/28);
 }
 if (c<0||c>63) c=-1; else if (rotate) c=63-c;
 if (c!=-1&&status_c1==-1)
  if ((pos[0].w&&pole[c]<=0)||(!pos[0].w&&pole[c]>=0)) c=-1;
 if (c!=-1&&status_c1!=-1&&c==status_c1) c=-1;
 //Serial.println(c);
 return c;
}
//****************************
void clearmenu() {
 if (screen_height==320) {
   tft.fillRect(330,56,135,220,BLACK);
   tft.fillRect(350,276,115,43,BLACK);
  } else {
   tft.fillRect(240,40,80,166,BLACK);
   tft.fillRect(250,206,69,33,BLACK);
  }
}
//****************************
