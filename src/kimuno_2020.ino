// KIM Uno 2020 version
//version date: 20200815
//
// This is the Arduino code for Arduino Pro Mini, Blue Pill or ESP32
// The PC version (Linux, uses ncurses) lives in ../pcuno directory
//
// Be sure to set config.h to your target platform!
//
#include "config.h"
#include "Arduino.h"
#include <EEPROM.h>
#include <stdint.h>
extern "C" {
  #include "kimuno.h" 
}

#if _TARGET == PERSONAL_COMPUTER
#error Target PERSONAL_COMPUTER does not use kimuno.ino but its own kimuno.c - see ../pcuno directory
#else
// AVRX is now only used in programmable calculator code section
#define AVRX
#endif

#if _TARGET == ESP32_R
#include "FS.h"
#include "SPIFFS.h"
#endif

#if _TARGET == BLUEPILL_STM
// choose what serial port you want to use.
//#define SerialX Serial1: you could use USART1 pins (PA10=Rx, PA9=Tx) but have to change the KIM Uno pin assignments
//#define SerialX Serial3 // This USART can be used without changing KIM Uno pin assignments below.
#define SerialX Serial  // use the USB virtual COM port as a terminal. normally, that's ttyACM0 in Linux
#else
#define SerialX Serial
#endif

// the OLED is a fun little addition. It's not tested for a while, should work on Arduino_Mini and STM32.
// Because of lack of IO pins and lack of Flash storage, this uses some confusing trickery. Email me if questions...
#ifdef OLED
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;
#endif

/****************************************************************\
 ================================================================
                    CIRCUIT for Arduino NANO/UNO
 ================================================================
           ----------------------------------------------
            Arduino LCD Shield 1602A D1 ROBOT (DF ROBOT)
           ----------------------------------------------

              LCD RS pin to arduino digital pin 8
              LCD E pin to arduino digital pin 9
              LCD Backlight LED pin to arduino pin 10
              LCD D4 pin to arduino digital pin 4
              LCD D5 pin to arduino digital pin 5
              LCD D6 pin to arduino digital pin 6
              LCD D7 pin to arduino digital pin 7
              LCD R/W pin to arduino ground pin
              LCD VSS pin to arduino ground pin
              LCD VCC pin to arduino 5V pin
              SHIELD A0 to arduino A0 (shield buttons analog control)
              SHIELD VIN to arduino VIN
              SHIELD GND to arduino GND
              SHIELD VCC to arduino VCC
              SHIELD RST to arduino RST

                      --------------------
                       Arduino 4x4 keypad
                      --------------------
              
              KEYPAD row 1 to arduino digital pin D12
              KEYPAD row 2 to arduino digital pin D11
              KEYPAD row 3 to arduino digital pin D3
              KEYPAD row 4 to arduino digital pin D2
              KEYPAD column 1 to arduino analog pin A4
              KEYPAD column 2 to arduino analog pin A3
              KEYPAD column 3 to arduino analog pin A2
              KEYPAD column 4 to arduino analog pin A1

 ================================================================
\****************************************************************/

// libraries
#include <LiquidCrystal.h>
#include <Keypad.h>

// LCD pins
#define RS 8    // LCD reset pin
#define En 9    // LCD enable pin
#define D4 4    // LCD data pin 4
#define D5 5    // LCD data pin 5
#define D6 6    // LCD data pin 6
#define D7 7    // LCD data pin 7

// keypad size
const byte num_rows= 4;
const byte num_cols= 4;

// map keypad keys
uint8_t keymap[num_rows][num_cols] = {
  { 23, 12, 16,  5},
  { 24, 13,  2,  6},
  { 10, 14,  3,  7},
  { 11, 15,  4,  8}  
};

// map keypad rows and columns
byte row_pins[num_rows] = {12, 11, 3, 2};     // Rows 0 to 3
byte col_pins[num_cols] = {A4, A3, A2, A1};   // Columns 0 to 3

// init kepad
Keypad keypad = Keypad(makeKeymap(keymap), row_pins, col_pins, num_rows, num_cols);

// init LCD
LiquidCrystal lcd(RS, En, D4, D5, D6, D7);  //interfacing pins

// -------- some interlinkage with cpu.c:
extern uint8_t RAM[1024];     // only calculator functions need access
extern uint16_t pc;  // 6502 pc
extern uint8_t SSTmode;
extern uint8_t useKeyboardLed; // 0 to use Serial port or 1 for HEX digits.

char    threeHex[3][2];       // buffer for LED display
uint8_t AppleTapeCounter=0;       // This gives Apple-1 saves a file ID like KIM-1
uint8_t curkey = 0;           // buffer for the last unread keypress
uint8_t eepromProtect=1;      // default is to write-protect EEPROM
int     blitzMode=1;          // microchess: 1 speeds up (and dumbs down play)
uint8_t keyboardMode=0;       // start with keyboard in KIM-1 mode. 1: calculator, 2: VTL02 mode
                              // mode 0: serial display emulates hex digits and keypad
                              // mode 1: ascii terminal with some control (and other) keys redefined
                              // mode 2: plain ascii terminal, only CTRL-R is still special (reset)


// Pin assignments for KIM Uno display/keypad. LedSelect7 is used in calculator mode.
// kCols is almost the same as aCols, we should make aCols go away now...
//
// aCols: the column pins of the matrix, connected to the individual segment LEDs and switches
// aRows: connected to groups of 8 switches
// ledSelect: connected to one of the segment LED blocks each. LedSelect7 drives 7, not the KIM-1's 6 segment LEDs.
//
#if _TARGET == BLUEPILL_STM
byte aCols[8] = { PB6,    PA8, PA9, PA10,   PA15, PB3, PB4, PB5 }; // note col PB6 is the extra one linked to DecPoint
byte kCols[8] = { PA8, PA9, PA10,   PA15, PB3, PB4, PB5, PB6 }; // note col PB6 is the extra one linked to DP
byte aRows[3] = { PB7, PB8, PB9 };
byte ledSelect[8] =  { PA0, PA1, PA2, PA3,   PA4, PA5, PA7, PA6 }; // note that last pin  not used at present. Can delete .
byte ledSelect7[8] = { PA0, PA1, PA2, PA3,   PA6, PA4, PA5, PA7 }; // note that last pin  not used at present. Can delete .
#elif _TARGET == ESP32_R
byte aCols[8] = { 12,13,14,15, 16,17,18,19 }; // note col PB6 is the extra one linked to DP
byte kCols[8] = { 13,14,15, 16,17,18,19, 12 }; // note col PB6 is the extra one linked to DP
byte aRows[3] = { 21,22,23 };
byte ledSelect[8] =  { 25,26,27,32, 33,4,5,33 }; // note that last pin  not used at present. Can delete .
byte ledSelect7[8] = { 25,26,27,32, 33,4,5,33 }; // note that last pin  not used at present. Can delete .
#elif _TARGET == ARDUINO_MINI
// for Pro Mini, probably also for Arduino Uno though (that's just untested in recent years)
byte aCols[8] = { A5, 2,3,4,5,6,7,8 }; // note col A5 is the extra one linked to DP
byte kCols[8] = { 2,3,4,5,6,7,8, A5 }; // note col A5 is the extra one linked to DP
byte aRows[3] = { 9,10,11 };
byte ledSelect[8] =  { 12, 13, A0, A1,   A2, A3, A7, A4 }; // note that A7  not used at present. Can delete .
byte ledSelect7[8] = { 12, 13, A0, A1,   A4, A2, A3, A7 }; // note that A7  not used at present. Can delete .
#else
#error pin assignments are undefined for _TARGET
#endif

// defines GPIO bit patterns for 7 segment LED digits:
// this is for aCol array, new kCol array follows KIM-1 original assignments.
// (so this is to be cleaned up when driveLEDs() is deleted. For now, Microchess still uses it though)
byte dig[19] = {
//--------------------
//         (a=64/$40)
//           --
//    (f=2) |  | (b=32/$20)
//           --        <-- (g=1)
//    (e=4) |  | (c=16/$10)
//           -- 
//         (d=8)  o (DP)
//----------------------------
// bits     6543210
// digits   abcdefg
          0b01111110,//0  
          0b00110000,//1
          0b01101101,//2
          0b01111001,//3
          0b00110011,//4
          0b01011011,//5
          0b01011111,//6
          0b01110000,//7
          0b01111111,//8
          0b01111011,//9
          0b01110111,//a
          0b00011111,//b
          0b01001110,//c
          0b00111101,//d
          0b01001111,//e
          0b01000111,//f
          0b00000001, //g printed as -
          0b00001000, //h printed as _
          0b00000000  //i printed as <space>
};


  
// ---------- terminal IO, called from cpu.c ----------------------
// ----------------------------------------------------------------
// the extern "C" is because Arduino IDE is C++, different function call format.
// whilst this .ino file is thus C++, cpu.c is C. The 'extern' bit just forces the IDE. Yuck.
extern "C" void errout(uint8_t val)    { SerialX.write(val); }
extern "C" void serout(uint8_t val)    { SerialX.write(val); }
extern "C" void serouthex(uint8_t val) { SerialX.print(val, HEX); }
extern "C" void seroutstr(char *s)     { uint8_t i=0; while (s[i]!='\0') serout(s[i++]); }
extern "C" uint8_t getAkey()           { return(curkey);  }
extern "C" void clearkey()             { curkey = 0; }
extern "C" uint8_t xkeyPressed()       { return (curkey==0?0:1); } // any keypress waiting?

  
// getKIMkey() translates ASCII keypresses to codes that the KIM ROM expects.
//
// note that the KIM Uno board's key codes are first translated to ASCII, then routed through
// here just like key presses from the ASCII serial port are. That's inefficient but left like this
// as it makes serial port 'keypad' input and physical keypad input treated together.
//  
// the keyboard flow:  (keypad)  scankey(/parseChar) --> \
//                                                        >---> interpretkeys()---> getKIMkey()
//                     (serial)  Serial.read() ----------/
//
// the serial terminal can thus give both emulated hex/keypad keypresses *and* regular serial input at the same time
//
extern "C" uint8_t getKIMkey() 
{
  if (curkey==0)
    return (0xFF);  //0xFF: illegal keycode 

  if ((curkey>='0') & (curkey <='9'))
    return (curkey-'0');
  if ((curkey>='A') & (curkey <='F'))
    return(curkey-'A'+10);
  if ((curkey>='a') & (curkey <='f'))
    return(curkey-'a'+10);

  if (curkey==1) // ctrlA
    return(0x10); // AD address mode
  if (curkey==4) // ctrlD
    return(0x11); // DA data mode
  if (curkey=='+') // +
    return(0x12); // step
  if (curkey==7) // ctrlG
    return(0x13); // GO
  if (curkey==16) // ctrlP
    return(0x14); // PC mode
  // curkey==ctrlR for hard reset (/RST) (KIM's RS key) is handled in main loop!
  // curkey==ctrlT for ST key (/NMI) is handled in main loop!
  return(curkey); // any other key, should not be hit but ignored by KIM
}


void setup () 
{
  #if _TARGET == BLUEPILL_STM
  // I forgot, too long ago:
  //disableDebugPorts();
    //  afio_cfg_debug_ports(AFIO_DEBUG_NONE); // using ST-Link, you'd have column pins blocked by JTAG
  // maybe better: "Beware that with AFIO_DEBUG_NONE you won't be able to debug using ST-Link, but with AFIO_DEBUG_SW_ONLY it is allowed."
  #endif

  #ifdef OLED
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(font5x7);
  oledRefresh(1);
  #endif

  SerialX.begin(9600);
  SerialX.println();

  setupUno();         // initialise GPIO pins for keypad/LEDs
  reset6502();
  initKIM();          // Enters 0x1C00 in KIM vectors 17FA and 17FE. Might consider doing 17FC as well?
  loadTestProgram();  // Put some fun programs in the KIM-1's RAM.
}


void loop () {
  exec6502(100);                                //do 100 6502 instructions
                                                // varying the number 100 varies keyboard debounce/responsivity ;)                                                

  if (SerialX.available()) {                    // read input from serial 'emulated' keypad
    curkey = SerialX.read() & 0x7F;             // this is normal serial input *and* simulated keypad keypresse
    interpretkeys();
  }

  scanKeys();                                   // ... and read input from physical keypad
                                                // this too calls interpretkeys, but inside the function. Hmm.
}



// translate keyboard commands to KIM-I keycodes or emulator actions
// remember: 2 types of keys. RS and ST are hardware things, the other switches are just read by KIM-1 software.
// also, pressing some keys for >1second toggles a mode
//
void interpretkeys()
{    
  // 1: keys that always have the same meaning
  switch (curkey) {
    case 18:  // CtrlR = RS key = hardware reset (RST)
      reset6502(); clearkey(); SerialX.print("RSet\n"); break;
    case 20: // CtrlT = ST key = throw an NMI to stop execution of user program
      nmi6502(); clearkey(); SerialX.print("STop\n"); break;
/*      #if _TARGET == ESP32_R
    case 0x3B:  helpscreen(); break;      
      #endif
*/  }

  // 1B: keys that always have the same meaning but are disabled in VTL-02
  // KeyboardMode 2 can be regarded as a switch to a normal ASCII keyboard.
  if(keyboardMode!=2)
  {
    switch(curkey) {
      case '[': // SST off
        SSTmode = 0; clearkey();
        //SerialX.print(F("                                      SST OFF         ")); // nice but too many spaces, out of Flash storage ;)
        SerialX.print(F("\r\nSST OFF\r\n"));
        break;
      case ']': // SST on
        SSTmode = 1; clearkey();
        SerialX.print(F("\r\nSST ON\r\n")); 
        break;
      case 9: // TAB pressed, lets KIM-1 ROM toggle between its KIM-1 Teletype mode and onboard LEDkeypad mode
        if (useKeyboardLed==0) 
        { useKeyboardLed=1;    
          //SerialX.print(F("                    Keyboard/Hex Digits Mode ")); 
          SerialX.print(F("\r\nKeypad Mode\r\n")); 
        }
        else 
        { useKeyboardLed=0;    
        //SerialX.print(F("                        Serial Terminal Mode         "));
        SerialX.print(F("\r\nTTY Mode\r\n"));
        }
        reset6502();  clearkey();  break;
      case '>': // Toggle write protect on eeprom
        if (eepromProtect==0) {
          eepromProtect = 1; 
          //SerialX.print(F("                                      Eeprom R/O     ")); 
          SerialX.print(F("\r\nEeprom R/O\r\n")); 
        } else {
          eepromProtect = 0;
          //SerialX.print(F("                                      Eeprom R/W     ")); 
          SerialX.print(F("\r\nEeprom R/W\r\n")); 
          delay(20);
        }
        clearkey(); break;
    }
  }
  
  // 2: keys in Calculator Mode
  // keyboardMode 1 is used for the somewhat lame calculator mode (0=default, 2=VTL or pure ASCII).
  // It's NOT used, or not needed, for the non-lame 6502 programmable calculator feature!!
  //
  if (keyboardMode==1) {  // in case of Calculator Mode
    switch (curkey) {
      case 'C': enterflt((uint8_t)0);  if (!((curkey>='C') && (curkey<='F')))  break;  // if statement allows roll-through to next reg entry
      case 'D': enterflt((uint8_t)1);  if (!((curkey>='C') && (curkey<='F')))  break;
      case 'F': enteroperation();  break;
      case 'E': enterflt((uint8_t)2);  break;

      case   1: showflt(0);  break; // AD
      case   4: showflt(1);  break; // DA
      case  16: showflt(2);  break; // PC
    }
  }
}
  
// =================================================================================================
// KIM Uno Circuit Board functions
// =================================================================================================

void setupUno() {
  // init LCD
  lcd.begin(16, 2);
  lcd.noAutoscroll();
  //lcd.cursor();
  lcd.print("Code Monkey KIM!");
  lcd.setCursor(0, 1);
}


// driveLEDs is a legacy of the 2014 version. It is however still used in MicroChess.
// but nowhere else, the KIM-1 simulation now goes through the hardware simulation of RIOT 2, further below.
//
extern "C" {
void driveLEDs() {  // now actually drives LCD instead (CMK)
  lcd.setCursor(0, 1);
  for (int iii=0;iii<3;iii++)
  { lcd.print(threeHex[iii][0], HEX); lcd.print(threeHex[iii][1], HEX);
    if (iii==1) lcd.print (' ');
  }
  
  //lcd.print("driveLEDs");
  //lcd.setCursor(0, 1);
}
void write1740(void) {}  // not used (CMK)
void write1741(void) {}  // not used (CMK)
void write1742(void) {}  // not used (CMK)
void write1743(void) {}  // not used (CMK)
} // end of C segment


// parse keycode of hardware keypad to return its ASCII code
// remember, KIM Uno keypad delivers KIM Uno keycodes, they're not (not always) the same as KIM-1 keycodes!
// this is fine, as we feed KIM Uno keypresses **and** serial 'keypad emulation' keypresses into the ROM later on
//
uint8_t parseChar(uint8_t n) 
{
  uint8_t c;
    
  // KIM-I keys
  switch (n-1) {              //KIM Uno keyscan codes to ASCII codes used by emulator
    case  7 : c =   '0' ;  break; //        note: these are n-1 numbers!
    case  6 : c =   '1';  break;  // 
    case  5 : c =   '2';  break;  // 
    case  4 : c =   '3';  break;  //
    case  3 : c =   '4';  break;  //
    case  2 : c =   '5';  break;  //
    case  1 : c =   '6';  break;  //
    case  0 : c =   20;  break; // ST
    
    case  15  : c =   '7' ;  break; //
    case  14  : c =   '8';  break;  //
    case  13  : c =   '9';  break;  //
    case  12  : c =   'A';  break;  //
    case  11  : c =   'B';  break;  //
    case  10  : c =   'C';  break;  //
    case  9 : c =   'D';  break;  //
    case  8 : c =   18;  break; // RS
    
    case  23  : c =  'E';  break;     //
    case  22  : c =  'F';  break;     //
    case  21  : c =   1;   break;     // AD
    case  20  : c =   4;   break;     // DA
    case  19  : c =   '+'; break;     // + 
    case  18  : c =   7;   break; // GO
    case  17  : c =   16;  break; // PC
    case  16  : c = (SSTmode==0?']':'[');  break; //  SST toggle
  }
  return c;
}

// get user keypress
char getch() {
  static int key = NO_KEY;
  key = keypad.getKey();

  if (key == NO_KEY) {
    // use LCD Shield buttons as KIM-1 command keys
    int shield_input;
    shield_input = analogRead (0);
    if (shield_input < 60) { delay(300); return 21; }                // [DA] button right
    else if (shield_input < 200) { delay(300); return 20;}           // [+]  button up
    else if (shield_input < 400) { delay(300); return 18;}           // [PC] button down
    else if (shield_input < 600) { delay(300); return 22;}           // [AD] button left
    else if (shield_input < 800) { delay(300); return 19;}           // [GO] button select
  }

  return key;
}

// scankeys: the actual reading of the hardware keypad
//
extern "C" {  // the extern C is to make function accessible from within cpu.c
void scanKeys() 
{
  //int led,row,col, noKeysScanned;
  //static int keyCode = -1, prevKey = 0;
  //static unsigned long timeFirstPressed = 0;
  static int keyCode = -1;
  
  // get 4x4 keypad keypress (CMK)  
  if ((keyCode = getch()) != NO_KEY) curkey = parseChar(keyCode);

  //KIM Uno board input?
  if (xkeyPressed()!=0) interpretkeys();    
  // --end 20200808

} // end of function
} // end C segment



// =========================================================================================
// Some support functions
// =========================================================================================

// EEPROM handling for the Pro Mini (and other Arduino hardware with EEPROM handling, even ESP32)
extern "C" uint8_t eepromread(uint16_t eepromaddress) 
{
  return(EEPROM.read(eepromaddress));  // thanks to Malik Enes Safak for the bug fix 20160907
}
extern "C" void eepromwrite(uint16_t eepromaddress, uint8_t bytevalue) 
{
  if (eepromProtect==0)
    EEPROM.write(eepromaddress, bytevalue);
  else
    {}//SerialX.println(F("EEPROM WRITE-PROTECT, USE '>'"));
}

/*
// check for out of RAM, it's running low on Pro Mini. Too low makes things buggy!
// Nice but not necessary. No space in flash --> removed.
int freeRam () {
#ifdef AVR
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
#endif
  return 1; // for anything other than the Pro Mini
}
*/

#ifdef OLED
void oledRefresh(int newOledMode)
{
//  oled.clear();
  oled.paintscreen();
//  oled.setCursor(3,3);
//  oled.print("HELLO");
  oled.println();
}
#endif



// =========================================================================================
// =========================================================================================
// =========================================================================================
// =========================================================================================
// below is the code for handling floating points in calculator mode
// don't even look unless you have to; the code is a mess but necessary as the Pro Mini
// does not have space to use the stdio functions that would make this trivial.
// So without sprintf and sscanf, this stuff just about fits in the Arduino Pro Mini's flash
// =========================================================================================
// =========================================================================================
// =========================================================================================
// =========================================================================================


void driveCalcLEDs(uint8_t *numberStr, uint8_t decpt)
{ 
  /*uint8_t led, col, ledNo, currentBit, bitOn;
  uint8_t digit,i;
    
  // 1. initialse for driving the 6 segment LEDs
  for (led=0;led<7;led++)
  { pinMode(ledSelect7[led], OUTPUT);  // set led pins to output
    digitalWrite(ledSelect7[led], LOW); // LOW = not lit
  }
  // 2. switch column pins to output mode
  for (digit=0;digit<7;digit++)
  {  for (col=0;col<8;col++)  
    {  pinMode(aCols[col], OUTPUT);           // set pin to output
       currentBit = (1<<(7-col));             // isolate the current bit in loop
       bitOn = (currentBit&dig[numberStr[digit]-48])==0;  
       if (col==0 && digit==decpt) // show decimal point here?
         bitOn=0; // 0 being light led.
       digitalWrite(aCols[col], bitOn);       // set the bit
    }
    digitalWrite(ledSelect7[digit], HIGH); // Light this LED 
    delay(2);
    digitalWrite(ledSelect7[digit], LOW); // unLight this LED
  }*/
} // end of function


extern "C" {  // the extern C is to make function accessible from within cpu.c
uint8_t enterflt(uint8_t reg)   // result code -1 = cancel, 0 = good
{
  uint8_t fltstr[32];         // display string
  uint8_t decpt = 0xFF, expt = 0xFF;  // pointers to start of mantissa & exponent in string
  uint8_t mntsign = 0, expsign = 0; // 1 means negative for mantissa/exponent
  uint8_t strpos = 0, i, j;       // strpos is position counter in string
  uint8_t mntval = 0, expval = 0;   // parsed value of mantissa & exponent
  uint8_t carry = 0, addToExp;
  int digit;
        uint16_t offset;
        uint8_t done=0; 
        
  // init
        offset = WREG_OFFSET + 8*reg;
  for (i=0;i<8;i++)
        #ifdef AVRX
    fltstr[i]=65;
        #else
    fltstr[i]='_';
        #endif

  // input loop ---------------------------------------------------------------
  do {
                #ifdef AVRX
                    curkey=0;
                    driveCalcLEDs(fltstr, decpt);  // xxxxxx decpt may be a problem
                    scanKeys();
                if (curkey!=0) {
                #else
    if (_kbhit()) {//}
      curkey = _getch();
                #endif
                
      if (curkey=='+') {
        if (expt==0xFF) {     // not yet into exponent
          mntsign = 1; strpos=0;
                                        #ifdef AVRX
                fltstr[strpos++] = 64;
                                        #else
          fltstr[strpos++] = '-';
                                        #endif
        } else {            // minus sign relates to exponent
          expsign = 1; strpos=expt;
                                        #ifdef AVRX
                fltstr[strpos++] = 64;
                                        #else
          fltstr[strpos++] = '-';
                                        #endif
        }
      }
      if ((curkey>='0') && (curkey<='9')) {
      // temp protection against entering 0.025, which breaks this code. 0.0x not OK in scient. notation anyway
                        // temp fix: do not allow entering a 0 after 0.
                          if ((curkey=='0') && (decpt==(strpos-1)) && (fltstr[strpos-1]=='0'))
                          {
                            // do nothing, that's the temp fix
                          }
                          else
                          // end of temp bug fix
                            fltstr[strpos++] = curkey;
      }
      if (curkey=='B') {
        expt = strpos;
                                #ifdef AVRX
        fltstr[strpos++] = 62;
                                #else
        fltstr[strpos++] = 'E';
                                #endif
      }
      if (curkey=='A') {
        decpt = strpos-1;
      }

                        #ifdef AVRX
      fltstr[strpos] = 65;
                        #else
      fltstr[strpos]=0x00;    // terminate into nice string
                        #endif
    }
  } while ((strpos<8) && (curkey!=7) && (curkey!=19) && (1!=(curkey>='C') && (curkey<='F')));

  if (curkey==19)
    return(-1);           // cancel

  // parse into 8 byte fltpt65 format -----------------------------------------------------------------
  // Ugly, horrible code. But running out of Arduino memory means C library calls must be avoided.
  
  if (expt==0xFF)   // if no E was entered, let it start at end of string and be 0 length
    expt = strpos;
  if (decpt==0xFF)  // if no dec pt was entered, put it at end of mantissa, just before the E
    decpt = expt-1;
  addToExp = decpt - mntsign; // normalise mantissa: how much to add to exp to have 1.2345 format

  // Exponent 3: parse and adjust exponent value to get normalised 1.23 format using addToExp
  if ((strpos-1)>expt)  // user at least entered 1 exp digit
    digit = (expsign==1?-1:1) * ((int) fltstr[strpos-1]-48) + (int) addToExp; //expsign*-1: deal with negative exps
  else
    digit = (int) addToExp;     // user entered number without exp. So exp is 0.
  if (digit<0)
    digit = -(digit);   // do not want to use abs() function-arduino out of memory space :)
  RAM[offset+1] = (digit<=9?digit:digit-10);  // store adjusted exp digit
  addToExp = (digit<=9?0:1);          // simple carry mechanism: add could overflow to 2nd sigit

  // Exponent 2: same thing.
  if ((strpos-2)>expt)  // user entered a second exp digit
    digit = (expsign==1?-1:1) * ((int) fltstr[strpos-2]-48) + (int) addToExp; //expsign*-1: deal with negative exps
  else
    digit = (int) addToExp;     // user entered number without exp. So exp is 0.
  if (digit<0)
    digit = -(digit);   // do not want to use abs() function-arduino out of memory space :)
  RAM[offset+1] |= (digit<=9?digit:digit-10)<<4;  // store adjusted exp digit in upper nibble
  addToExp = (digit<=9?0:1);          // simple carry mechanism: add could overflow to 2nd sigit
  
  // Exponent 1: same thing.
  if ((strpos-3)>expt)  // user entered a second exp digit
    digit = ((int) fltstr[strpos-3]-48) + (int) addToExp; // there is no carry or add to exp in digit 3
  else
    digit = (int) addToExp;     // user entered number without exp. So exp is 0.
  if (digit<0)
    digit = -(digit);   // do not want to use abs() function-arduino out of memory space :)
  RAM[offset+0] = (digit<=9?digit:digit-10);  // store adjusted exp digit in lower nibble

  // Sign bits
  RAM[offset+0] |= ((mntsign<<7) | (expsign<<6));
//  printf("%u %u ", (RAM[offset+0] & 0xF0)>>4, RAM[offset+0] & 0x0F);
//  printf("%u %u \n", (RAM[offset+1] & 0xF0)>>4, RAM[offset+1] & 0x0F);

  // print mantissa
  j = mntsign;
  for (i=0;i<12;i++)
  {
    if (j<expt)
      RAM[offset+2+i] = (fltstr[j]-48)<<4;
    else
      RAM[offset+2+i] = 0;
    j++;

    if (j<expt)
      RAM[offset+2+i] |= (fltstr[j]-48);
    j++;

//    printf("%u %u ", (RAM[offset+2+i] & 0xF0)>>4, RAM[offset+2+i] & 0x0F);
  }
//  printf("\n");
  return (curkey);  // return value, if not -1, can be used to jump to next register value entry call
} // end function
} // end C segment

extern "C" {  // the extern C is to make function accessible from within cpu.c
uint8_t showflt(uint8_t reg)  // returns location of decimal point in string
{
  uint8_t fltstr[32];         // display string
  uint8_t mntsign = 0, expsign = 0; // 1 means negative for mantissa/exponent
  uint8_t cnt, expt, i; // decpt,
        uint16_t offset;
  int exp, decpt;

  // init
        offset = WREG_OFFSET + 8*reg;
  for (i=0;i<8;i++)
    fltstr[i]='_';  // no longer necessary I think
  // calculate exponent
  exp = (RAM[offset+1] & 0x0F) + 10*((RAM[offset+1] & 0xF0)>>4) + 100*(RAM[offset+0] & 0x0F);
//  printf("\n\nexp = %d\n", exp);

  // determine maximum exponent value we can show as normal number without E
  mntsign = (RAM[offset+0] & 0x80)>>7;  // negative mantissa: 1
  expsign = (RAM[offset+0] & 0x40)>>6;  // negative exponent: 1
  decpt = (mntsign==0?0:1);         // dec point is after digit0 (+ values) or digit1 (- values)
  
  // with pos numbers, any E between 0 and 7 can be polished away. If there's a '-', one less
  if ((exp>0) && (exp<(7 - mntsign)) && (expsign==0))
  { // yes, we can polish E away
    decpt +=exp;
    expt = 0;
  }
/*    else // negative exponent
    { decpt -=exp;
      // need to ROR the string's digits or decpt gets to be negative!

      expt = 0;
    }
  }
*/  else            // we need to show exponent, how many digits?
  { expt = 0;
    if (exp>0) 
    { expt = 2; //1;
      fltstr[6] = (RAM[offset+1] & 0x0F) + 48;
    }
    if (exp>9) 
    { expt = 3; //2;
      fltstr[5] = (uint8_t) ((RAM[offset+1] & 0xF0)>>4) + (uint8_t) 48;
    }
    if (exp>90) 
    { expt = 4; //3;
      fltstr[4] = (RAM[offset+0] & 0x0F) + 48;
    }
                #ifdef AVRX
    fltstr[7-expt] = (expsign==1?64:62);
                #else
    fltstr[7-expt] = (expsign==1?'-':'E');
                #endif
  }

  // fill string with mantissa
  cnt=0;
  if (mntsign==1)
                #ifdef AVRX
    fltstr[0] = 64;
                #else
    fltstr[0]='-';
                #endif

  for (i=mntsign;i<(7-expt);i=i+2)
  {
    fltstr[i] = (uint8_t) ((RAM[offset + 2 + cnt ] & 0xF0)>>4) + (uint8_t) 48;
//    printf(" %c ", (uint8_t) ((RAM[offset + 2 + cnt ] & 0xF0)>>4) + (uint8_t) 48);
    if ((i+1)<(7-expt)) // bug fix 20141007
                { fltstr[i+1] = (RAM[offset + 2 + cnt ] & 0x0F) + 48;
//        printf(" %c   ", (RAM[offset + 2 + cnt ] & 0x0F) + 48);
                }
    cnt++;
  }
  fltstr[7]=0x00; // string terminator

//  printf("\n\n%s\n", fltstr);

        #ifdef AVRX
        curkey=0;
        do { driveCalcLEDs(fltstr, decpt);          scanKeys();
        } while (curkey==0);
        if ((curkey<'C') || (curkey>'F'))
          curkey=0;  // to clear any keypresses before returning to KIM    
        #else
        // show number with dec point inserted
  for (i=0;i<=decpt;i++)
    printf("%c", fltstr[i]);
  printf(".");
  for (i=decpt+1;i<7;i++)
    printf("%c", fltstr[i]);
  printf("\n");
        #endif
        
  return decpt;           // pointers to start of mantissa & exponent in string
} // end function
} // end C segment


extern "C" {  // the extern C is to make function accessible from within cpu.c
uint8_t enteroperation(void)    // result code -1 = cancel, 0 = good
{
  uint8_t fltstr[8];          // display string
  uint8_t strpos = 4, i;        // strpos is position counter in string

  // init
        #ifdef AVRX
  for (i=0;i<8;i++)  fltstr[i]=66;
  fltstr[4]=65;  fltstr[5]=65;
        #else
  for (i=0;i<8;i++)  fltstr[i]=' ';
  fltstr[4]='_';  fltstr[5]='_';
        #endif

  // input loop ---------------------------------------------------------------
  do {
    #ifdef AVRX
    curkey=0;
    driveCalcLEDs(fltstr, 5);  scanKeys();
    if (curkey!=0) { 
//    #else
//    if (_kbhit()) { // } this curly brace is there so Arduino IDE does not miscount when it does { } highlighting
//      curkey = _getch();
    #endif
                
      if ((curkey>=48) && (curkey<=57))     //only allow dec digits
        fltstr[strpos++] = curkey;

                        #ifdef AVRX
      fltstr[strpos] = 65;
                        #else
      fltstr[strpos]=0x00;    // terminate into nice string
//      printf("> %s  ms%u es%u expt%u decpt%u\r", fltstr, mntsign, expsign, expt, decpt);
                        #endif
    }
  } while ((strpos<6) && (curkey!=7) && (curkey!=19));

  if (curkey==19)
    return(-1);           // cancel

        RAM[0x00F3]= (fltstr[4]-48)*10 + (fltstr[5]-48);  // operation to go into 6502 A register
        RAM[0x00EF] = (uint8_t) 0xE0;  RAM[0x00F0] = (uint8_t) 0x6F;                // set PC register to fltpt65 start address (0x6FE0, not 0x5000, we need to start with a JSR and end with a BRK)
        curkey = 16;  // PC
       
        return(RAM[0x00F3]);
}
} // end C segment



// ================================================================
// ================================================================
// TAPE STORAGE FOR ESP32
// (same for STM32 and Pro Mini is further down, totally different)
// ================================================================
// ================================================================

#if _TARGET == ESP32_R
extern "C" {  // the extern C is to make function accessible from within cpu.c

uint8_t tapeSave(uint8_t ID, uint8_t SAL, uint8_t SAH, uint8_t EAL, uint8_t EAH, uint8_t AppleMode)
{
    char  filnam[4]="/00";
    uint8_t  buffer; 

    
// not using this tape counter thing for now, commented out    
//    if(AppleMode==1)
//        ID=(uint8_t) AppleTapeCounter;    // for Apple-1 tapes which are raw binaries without header

    filnam[1]=( ((ID & 0xF0) >> 8)<0x0A  ?  ((ID & 0xF0) >> 8)+0x30  :  ((ID & 0xF0) >> 8)+0x40-0x09);
    filnam[2]=( ((ID & 0x0F)     )<0x0A  ?  ((ID & 0x0F)     )+0x30  :  ((ID & 0x0F)     )+0x40-0x09);
    if (ID<0x01 || ID>0xFE)          // not allowed on KIM-1
    {   /*seroutstr("Bad file name\r\n");*/    return(0);  }

//sprintf(filnam, "tape/%02x", ID);
//seroutstr(filnam); seroutstr("\r\n");
    File file = SPIFFS.open(filnam, FILE_WRITE);
    if(!file)
    {   seroutstr("Error opening file\r\n");    return(0);  }
    // write file header, 5 bytes
    if (AppleMode==0)       // write file header, 5 bytes
    {
        file.write(&ID, sizeof(uint8_t));
        file.write(&SAL,sizeof(uint8_t));        file.write(&SAH,sizeof(uint8_t));
        file.write(&EAL,sizeof(uint8_t));        file.write(&EAH,sizeof(uint8_t));
    }
    // write data, note saving ends with the byte BEFORE EndAddress!
    for( uint16_t i=((SAH<<8)+SAL); i<((EAH<<8)+EAL)+(AppleMode?1:0); i++)
    {   buffer = read6502(i);
serout(i+'0');serout('-'); serout(buffer+'0');
        file.write(&buffer,sizeof(uint8_t));
    }
    seroutstr("saved.\r\n");
    file.close();
    return(1);


}
uint8_t tapeLoad(uint8_t ID, uint8_t xSAL, uint8_t xSAH, uint8_t xEAL, uint8_t xEAH, uint8_t AppleMode)
{
    char  filnam[4]="/00";
    uint8_t  buffer; 
    int   x;
    uint8_t SAL, SAH, EAL, EAH;
    
//    if(AppleMode==1)
//        ID=(uint8_t) AppleTapeCounter;    // for Apple-1 tapes which are raw binaries without header
    
//    seroutstr("tapeLoad\r\n");
    filnam[1]=( ((ID & 0xF0) >> 8)<0x0A  ?  ((ID & 0xF0) >> 8)+0x30  :  ((ID & 0xF0) >> 8)+0x40-0x09);
    filnam[2]=( ((ID & 0x0F)     )<0x0A  ?  ((ID & 0x0F)     )+0x30  :  ((ID & 0x0F)     )+0x40-0x09);
    File file = SPIFFS.open(filnam);

    if(!file)
    {   errout('T');    errout('R');    errout('E');    return(0);  }

    if (AppleMode==0)       // read file header, 5 bytes
    {
        x=file.read(&ID,sizeof(uint8_t));
        x+=file.read(&SAL,sizeof(uint8_t));        x+=file.read(&SAH,sizeof(uint8_t));
        x+=file.read(&EAL,sizeof(uint8_t));        x+=file.read(&EAH,sizeof(uint8_t));

        for( uint16_t i=((SAH<<8)+SAL); i<((EAH<<8)+EAL); i++)
        {   x=file.read(&buffer,sizeof(uint8_t));
serout(i+'0');serout('-'); serout(buffer+'0');
            write6502(i, buffer);
        }
    }
    else        // Apple-1, raw binary file without header
    {
        SAL=xSAL;   SAH=xSAH;   EAL=xEAL;   EAH=xEAH;  

        for( uint16_t i=((SAH<<8)+SAL); i<=((EAH<<8)+EAL); i++) // note <=, not <!
        {   x=file.read(&buffer,sizeof(uint8_t));
            if (x==1)
                write6502(i, buffer);
            else
                break;      // so read till end of tape or till EndAddress, whichever comes 1st
        }
    }
    // read data, note loading ends with the byte BEFORE EndAddress!
    file.close();
    seroutstr("Load OK.\r\n");
    return(1);
}

void    tapeDirectory(void)
{
    seroutstr("tapeDirectory\r\n");

    // handle unformatted file system ---------------------------------------------------------
    if(!SPIFFS.begin(false))
    { /*seroutstr("SPIFFS Mount Failed, retry after formatting?\r\n"*/);

      userInput();
      
      if (curkey='y' || curkey == 'Y')     
      { curkey=0; 
        if(!SPIFFS.begin(true))
        { /*seroutstr("SPIFFS Mount Failed after formatting attempt. Aborting tape operation.");*/
          return;
        }
      } else
      { curkey=0;
        seroutstr("Aborting tape operation.\r\n");
        return;
      }
    }

    // print directory ----------------------------------------------------------------------
    File root = SPIFFS.open("/");
    if(!root)
    {   seroutstr("- failed to open directory\r\n");
        return;
    }
    if(!root.isDirectory())
    {   seroutstr(" - not a directory\r\n");
        return;
    }

    File file = root.openNextFile();
    while(file)
    {   if(file.isDirectory())
        {} else 
        {   seroutstr((char *)file.name());
            seroutstr(" ");
        }
        file = root.openNextFile();
    }
    seroutstr("\r\n");
}
void    tapeDelete(uint8_t ID)
{
  char  filnam[4]="/00";
  seroutstr("tapeDelete\r\n");
  filnam[1]=( ((ID & 0xF0) >> 8)<0x0A  ?  ((ID & 0xF0) >> 8)+0x30  :  ((ID & 0xF0) >> 8)+0x40-0x09);
  filnam[2]=( ((ID & 0x0F)     )<0x0A  ?  ((ID & 0x0F)     )+0x30  :  ((ID & 0x0F)     )+0x40-0x09);
  SPIFFS.remove(filnam);
  seroutstr("Delete OK\r\n");
}
void    tapeInit(void)
{   
  seroutstr("Formatting...\r\n");
  SPIFFS.format();
  seroutstr("Done.\r\n");
}
/* 
// tapeSetCounter was intended to let you enter a tape counter number. We just use the KIM's $17f9 ID instead
//
void    tapeSetCounter(void)
{ 
  do
  {
    seroutstr("tapeSetCounter\r\n(enter two-digit hex number)\r\n");
    userInput();  
    if (curkey>='0' && curkey <='9')
      AppleTapeCounter=(curkey-0x30)<<8;
    else if (curkey>='a' && curkey <='f')
      AppleTapeCounter=(curkey-0x60+0x09)<<8;
    else if (curkey>='A' && curkey <='F')
      AppleTapeCounter=(curkey-0x40+0x09)<<8;  
    serout(curkey);  curkey=0;
    
    userInput();  
    if (curkey>='0' && curkey <='9')
      AppleTapeCounter=(curkey-0x30);
    else if (curkey>='a' && curkey <='f')
      AppleTapeCounter=(curkey-0x60)+0x09;
    else if (curkey>='A' && curkey <='F')
      AppleTapeCounter=(curkey-0x40)+0x09;  
    serout(curkey);  curkey=0;
    
    serout('\r'); serout('\n');
    serout('-');  serouthex(AppleTapeCounter);
    serout('\r'); serout('\n');
    
    // show on display as ---- xx
    // ...
  } while (AppleTapeCounter<1 || AppleTapeCounter>255);
}
*/
void    helpscreen(void)
{
  seroutstr("helpscreen\r\nHit any key");
  userInput();  curkey=0;

}

void userInput(void)
{
      curkey=0;

      do {
      if (SerialX.available())                      // read input from serial 'emulated' keypad
      { curkey = SerialX.read() & 0x7F;
        interpretkeys();
      }
      scanKeys();                                   // read input from physical keypad
      if (xkeyPressed()!=0) //KIM Uno board input?
        interpretkeys();
      } while (curkey==0);
}

} // end C segment
#endif // enf of ESP32 tape storage simulation



// ==============================================================
// ==============================================================
// TAPE STORAGE FOR ARDUINO_MINI (And Blue Pill)
// ==============================================================
// ==============================================================
//
// Very primitive concept: use the 1K EEPROM, the top 5 bytes are a 'directory' showing where the 'up to 5' files start.
// if a file gets deleted, the ones stored above it are just copied down to eliminate the gap created by the deletion.
// each file is saved with a 5-byte header containing its ID, Start Address (High/Low) and End Address (High/Low).
// -- the Apple-1 does not use a File ID. For Apple-1 files, think of the ID as the tape counter. You have to set it before starting to save.
//
// files are saved on 4-byte page boundaries. This way, the 'directory' only needs a single byte to contain the file's location.
//
// note that the EEPROM can be used in two ways (concurrently, meaning they can mess each other up): 
//   Either the EEPROM is used as a second 1K of RAM (after disabling write protect), or as cassette tape.
// this means you can inspect the tape contents from the KIM-1 monitor: it's simply addresses x0400 to x7FF.
//

#if _TARGET == ARDUINO_MINI || _TARGET == BLUEPILL_STM
extern "C" {  // the extern C is to make function accessible from within cpu.c

uint8_t tapeSave(uint8_t ID, uint8_t SAL,  uint8_t SAH,  uint8_t EAL,  uint8_t EAH,  uint8_t AppleMode)
{
  uint16_t   i=0;
  uint16_t  romstart, lastromstart;
  uint16_t  EA, SA;
  
  SA = (SAH<<8)+SAL;
  EA =  (EAH<<8)+EAL;
  // KIM-1 behaviour: save up to but not including EA!
  
  //
  // find first free slot in 'directory'
  //
  while (EEPROM[1024-5+i]!=255)
  {
    i++;
    if (i>4)
    { SerialX.println(F("Dir full")); return(1);  }
  }
  //
  if (i>4)                    //                                <----- CLEANUP, double
  { SerialX.println(F("Dir full")); return(1);  }
  //
  // So we found a free slot
  // calculate recording's address in ROM
  //  
  if (i==0) // there has to be a first one...
    lastromstart=romstart=0x0000;
  else
  {
    lastromstart=EEPROM[1024-5+i-1]*4; //4: stored was the 4-byte 'page'
    romstart= ((lastromstart + (EEPROM[lastromstart+3] 
        + EEPROM[lastromstart+4]*256 + 4))/4+1)*4;
  }
  //
  // check the 'tape' does not run out of space
  //
  if ( (romstart+6+(EA-SA))>=(1024-5) ) // +6: 5 bytes of header +1 byte 'up-to-and-included' for AppleMode
  {
    SerialX.println("Too Big");
    return(1);
  }
  //
  // save
  //
  EEPROM.update(1024-5+i, (uint8_t) (romstart/4));
  EEPROM.update(romstart, (uint8_t) ID);
  EEPROM.update(romstart+1, (uint8_t) SAL);
  EEPROM.update(romstart+2, (uint8_t) SAH);
  EEPROM.update(romstart+3, (uint8_t) ((EA-SA) & 0xFF));
  EEPROM.update(romstart+4, (uint8_t) ((EA-SA) & 0xFF00)>>8);
  for (i=0;i<((EA-SA)+(AppleMode?1:0));i++) // Apple-1: save up-to-and-including, not up-to like the KIM.
  {
    //SerialX.println((uint8_t) read6502(SA+i), HEX);
    EEPROM.update(romstart+5+i, read6502(SA+i));
  }
  SerialX.print(F("Saved "));  SerialX.println(ID, HEX); 
  return (0); 
}

uint8_t tapeLoad(uint8_t ID, uint8_t xSAL, uint8_t xSAH, uint8_t xEAL, uint8_t xEAH, uint8_t AppleMode)
{ 
  uint16_t  i=0;
  uint16_t  romstart, lastromstart;
  uint16_t  EA, SA;

  // AppleMode differs from KIM: (1) ID=tapecounter. No difference in this version of KIM Uno, but FYI.
  //    (2) save is up-to-and-including EA, not up-to.
  //    (3) save is to the user-specified indicated address up to the user-indicated length.
  
  //
  // find index number (max 5 files, which one do we want?)
  //
  do {
    if (EEPROM[ EEPROM[1024-5+i]*4 ] == ID)
      break;
  } while (i++<5);

  if (i>4) {
    SerialX.println(F("Not found"));
    return (1);
  }
  //
  // calculate recording's address in ROM
  //
  romstart= EEPROM[1024-5+i]*4;
  SA = EEPROM[romstart+1]+(EEPROM[romstart+2]<<8);
  EA = SA + EEPROM[romstart+3]+(EEPROM[romstart+4]<<8);
  //
  //
  if (AppleMode)
  {
    uint16_t fileLen = (EA-SA);
    uint16_t userLen = ((xEAH<<8)+xEAL)- ((xSAH<<8)+xSAL);
    if (userLen<fileLen)  // user wants to load only part of the saved file
      fileLen=userLen;    //  then just load the smaller number of bytes he apparently wants
    
    SA = (xSAH<<8)+xSAL;  // load file where user wants it, overriding stored SA
    EA = SA+fileLen;      // and for as long as he wants to
  }
  // else KIM-1 behaviour: take SA and EA from file.    
  //
  // load
  //
  for (i=0; i<((EA-SA)+(AppleMode?1:0)); i++)  // Apple loads up-to-and-including...
   write6502(SA+i, EEPROM[romstart+5+i]);
  //
  SerialX.println(F("ld OK"));
  return (0);
}

void    tapeDirectory(void)
{ 
  int i;
  
  for (i=0;i<5;i++)
  {
    SerialX.print('_');
    
    if (EEPROM[1024-5+i]==255)
      SerialX.print(F("-"));                            // a still-empty slot...
    else
      SerialX.print(EEPROM[ EEPROM[1024-5+i]*4 ], HEX); // File ID in slot
  }
  SerialX.println();
}

void    tapeDelete(uint8_t ID)
{ 
  uint16_t i=0,j;
  uint8_t deleteIndexNo=255;
  int8_t entryLen;  // SIGNED!
  
  //
  // find index number
  //
  do 
  {
    if (EEPROM[ EEPROM[1024-5+i]*4 ] == (uint8_t) ID)
      deleteIndexNo = i;
    else if (EEPROM[1024-5+i] == 255) // no more entries
      break;
  } while ((i++)<4);

  if (deleteIndexNo>4) {
    SerialX.println(F("Not found"));
    return;
  }
  //
  // adjust directory index by shifting all higher entries one notch down 
  // and adjusting their locations (because deleted file has gone)
  //
  entryLen= (EEPROM[1024-5+deleteIndexNo+1] - EEPROM[1024-5+deleteIndexNo]);    // page length of deleted file
  //                                                                                // = offset factor for entries that will be shifted
  // shift all the entries above the deleted one with the offset entryLen
  //
  for (j=deleteIndexNo+2;j<i;j++) {
      // move first post-delete file to the previous entry by doing nothing: it will start at the same location after all. 
      // But all *following* entries will need adjusting: (1) shift down in index list and (2) adjust starting page.
      EEPROM[1024-5+j-1] = EEPROM[1024-5+j]-entryLen;
  }
  // the last existing entry should now be deleted
  EEPROM[1024-5+i-1]=255;    // wipe out last entry at the end
  //
  //
  // shift memory block (do it simple style for eeprom)
  //
  uint16_t xto = EEPROM[1024-5+deleteIndexNo]*4;
  uint16_t xfrom = xto+entryLen*4; //ROM[1024-5+deleteIndexNo+1]*4;
  uint16_t offset = xfrom-xto;
  //
  if (deleteIndexNo!=(i-1))   // no memory to move if this was the last entry...
      for (j=0;j<(1019-xfrom);j++)
          EEPROM[xto+j] = EEPROM[xfrom+j];
}

void    tapeInit(void)
{ 
  int i;
  
  for (int i=0;i<5;i++) 
    EEPROM.update(1024-5+i,255);
  SerialX.println(F("dir inited"));
}

void    tapeSetCounter(void)
{ } // used for esp32, but Pro Mini version just uses the KIM-1 ID as the tape counter for Apple-1
void    helpscreen(void)
{ // no space left in flash for a helpscreen text!
}

} // end C segment
#endif
