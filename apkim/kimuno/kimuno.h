#include <stdint.h>

// Macros: in C, use regular arrays but in Arduino, use Flash-stored arrays
//
#if _TARGET != PERSONAL_COMPUTER
#define kimArray(a,b)       const unsigned char a[b] PROGMEM
#else
#define kimArray(a,b)       unsigned char a[b] 
#endif
//
#if _TARGET != PERSONAL_COMPUTER
#define readRom(a,b)       pgm_read_byte_near(a + b)
#else
#define readRom(a,b)       a[b]
#endif


// -------- various functions for (simulated or real) serial output
 void serout(uint8_t value);  // main function to send a character out on the serial port
 void errout(uint8_t value);  // same thing, for error messages/warnings like %5
 void serouthex(uint8_t val); // same thing but print as hex character, for debug purposes
// -------- various functions for dealing with keyboards (serial, simulated keypad, physical keypad)
 uint8_t getAkey(void);     // for serial port: get normal ASCII keys
 uint8_t getKIMkey();       // for serial emulation of KIM keypad
 void clearkey(void);       // forget any not-yet-read keypresses
// -------- using the keypad on an Arduino KIM Uno PCB
 void driveLEDs();          // only used for Microchess by now (2020)
 extern void scanKeys();    // for reading Arduino's KIM Uno keypad


// -------- new RIOT emulation 20200809
// 
// this emulation is barely fast enough for the Arduino Pro Mini. 
// But OK except for some flicker when used in some games
//
struct riotRegs {
  uint8_t ioPAD;// = 0;
  uint8_t ioPADD;// = 0xFF;
  uint8_t ioPBD;// = 0;
  uint8_t ioPBDD;// = 0;
};

struct riotRegs riot2regs;
void write1740(void);
void write1741(void);
void write1742(void);
void write1743(void);

void riotTimerWrite(uint16_t address, uint16_t setVal);
uint8_t riotTimerRead(uint16_t address);


// -------- Calculator IO functions (7-digit floating point display)

// where FLTPT (the program at 0x5000) saves its floating point registers:
// (note the flt pt registers are just a block of memory in RAM!)
#define WREG_OFFSET 0x0360
//
uint8_t enterflt(uint8_t reg);      // enter a floating point number (7-digits with dec. point)
uint8_t showflt(uint8_t reg);       // show a floating point number
uint8_t enteroperation(void);       // enter the operation (number, for add/divide/cos/tan/atan etc)
void copyWreg(uint8_t a, uint8_t b);  // copy a flt pt register to another
void swapWreg(uint8_t a, uint8_t b);  // swap to flt pt registers.


// -------- Cassette tape emulation

uint8_t tapeSave(uint8_t ID, uint8_t SAL,  uint8_t SAH,  uint8_t EAL,  uint8_t EAH,  uint8_t AppleMode);
uint8_t tapeLoad(uint8_t ID, uint8_t xSAL, uint8_t xSAH, uint8_t xEAL, uint8_t xEAH, uint8_t AppleMode);
void    tapeDirectory(void);
void    tapeDelete(uint8_t ID);
void    tapeInit(void);
void    tapeSetCounter(void);
void    helpscreen(void);
void    userInput(void);

// -------- cpu.c
uint16_t getpc();
uint8_t getop();
void exec6502(int32_t tickcount);
void reset6502();
void nmi6502();
//
uint8_t read6502(uint16_t address);               // read from somewhere in memory map
void write6502(uint16_t address, uint8_t value);  // write to somewhere in memory map
void initKIM(void);
void loadTestProgram(void);
