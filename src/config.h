#define FALSE          0
#define TRUE           !(FALSE)

//-----------------------------------
//-- list of targets to compile for -
//-----------------------------------
#define ARDUINO_MINI        2
#define BLUEPILL_STM        5
#define ESP32_R             6
#define PERSONAL_COMPUTER   8


//-----------------------------------
// ------ Choose your target --------
// ------ (uncomment out one) -------
//-----------------------------------

#define _TARGET ARDUINO_MINI
//#define _TARGET BLUEPILL_STM
//#define _TARGET ESP32_R
//#define _TARGET PERSONAL_COMPUTER


//-------------------------------------------------------------
//----- Programs to include in ROM, set any combination to TRUE
//----- (as long as target has space: Pro Mini can't have all)
//-------------------------------------------------------------
#define _INCL_DISASM      TRUE
#define _INCL_MINIASM     TRUE
#define _INCL_MCHESS      FALSE
#define _INCL_CALCULATOR  FALSE
#define _INCL_VTL02       TRUE
#define _INCL_A1BASIC     TRUE
#define _INCL_ACI         TRUE
#define _INCL_LIFE        FALSE


//--------------------------------------------------------------
//----- Programs to preload in RAM, choose only ONE of these
//--------------------------------------------------------------
#define _DEMO_FIRST       FALSE
#define _DEMO_TIMER       FALSE
#define _DEMO_HILO        FALSE
#define _INCL_FARMERBROWN FALSE
#define _DEMO_CHESSCLK    FALSE
#define _DEMO_ASTEROID    FALSE
#define _DEMO_BLACKJACK   FALSE


//-----------------------------------
// ------ Hardware options ----------
//-----------------------------------
// enable to use OLED; **has** to be disabled if OLED is not connected!
// untested for quite a long time by now, 'should work' on Arduino/STM32
//#define OLED


//-----------------------------------
// ----- Memory size, per target
//-----------------------------------

#if _TARGET != ARDUINO_MINI
#define RAM_LIMIT1 0x1000
#else
#define RAM_LIMIT1 0x0800
#endif

#if _TARGET == PERSONAL_COMPUTER || _TARGET == ESP32_R
#define RAM_LIMIT2 0x5000
#elif _TARGET == BLUEPILL_STM
#define RAM_LIMIT2 0x3000
#else
#define RAM_LIMIT2 0
#endif
