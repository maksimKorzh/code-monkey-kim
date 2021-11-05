//-----------------------------------
//------ Targets to compile for -----
//-----------------------------------
#define ARDUINO_UNO         1
#define ARDUINO_MINI        2
#define ARDUINO_MEGA        3
#define ARDUINO_DUE         4
#define BLUEPILL_STM        5
#define ESP32_R             6
#define PERSONAL_COMPUTER   8


//-----------------------------------
// ------ Choose your target --------
//-----------------------------------

//#define _TARGET PERSONAL_COMPUTER


#define _TARGET ESP32_R
#define AVRX

#define WATCH


/*
#define _TARGET ARDUINO_MINI
#define AVRX
#define AVR
*/
// reminder: ------------------------------------------------------------
// #define AVRX   // define on for Arduino (or Blue Pill or ESP32, just to confuse you), 
                        // but off for normal PC C compilers. 
                        // in kimuno.ino, this only relates to using hardware or serial emulated keypad in calculator functions
                        // in cpu.c it determines regular C arrays (PC) or pgmspace arrays (Arduino)
// ----------------------------------------------------------------------


/*
#define _TARGET BLUEPILL_STM
//#define BLUEPILL
*/

//-----------------------------------
// ------ Hardware options ----------
//-----------------------------------
// enable to use OLED; **has** to be disabled if OLED is not connected!
//#define OLED


//-------------------------------------------------------------
//----- Programs to include in ROM, set any combination to TRUE
//----- (as long as target has space: Pro Mini can't have all)
//-------------------------------------------------------------
#define _INCL_DISASM      FALSE
#define _INCL_MINIASM     TRUE
#define _INCL_MCHESS      TRUE
#define _INCL_CALCULATOR  FALSE
#define _INCL_VTL02       TRUE
#define _INCL_A1BASIC     TRUE
#define _INCL_ACI         TRUE
#define _INCL_LIFE        TRUE

//-------------------------------------------------------------
//----- Programs to preload in RAM, choose only ONE of the three
//--------------------------------------------------------------
#define _DEMO_FIRST       TRUE
#define _DEMO_TIMER       FALSE
#define _DEMO_HILO        FALSE
