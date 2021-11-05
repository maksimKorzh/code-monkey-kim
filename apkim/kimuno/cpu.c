/* KIM-I/Apple-1 emulator for Arduino, STM32, ESP32 and Linux

   Uses the 6502 emulator from Mike Chambers as its brains,
   http://forum.arduino.cc/index.php?topic=193216.0

     KIM-I/Apple-1 hacked in by Oscar
     http://obsolescenceguaranteed.blogspot.ch/
*/

#include "config.h"

// EEPROM pgmspace or regular C array:
#if _TARGET == ARDUINO_MINI
  #include <avr/pgmspace.h>
#elif _TARGET == BLUEPILL_STM || _TARGET == ESP32_R
  #include <pgmspace.h>
#endif

#if _TARGET == ARDUINO_MINI || _TARGET == BLUEPILL_STM
// not just for ARDUINO_MINI; BLUEPILL_STM also uses eeprom for tape.
  extern uint8_t eepromread(uint16_t eepromaddress);
  extern void eepromwrite(uint16_t eepromaddress, uint8_t bytevalue);
#endif

#if _TARGET != ARDUINO_MINI
  #include <stdio.h>
  #include <stdint.h>
  #include <time.h>
  #include <stdio.h>
  #pragma warning(disable : 4996) // MS VC2008 does not like unsigned char -> signed char converts.
#endif

#include "kimuno.h"
#include "roms.h"


#define FLAG_CARRY     0x01
#define FLAG_ZERO      0x02
#define FLAG_INTERRUPT 0x04
#define FLAG_DECIMAL   0x08
#define FLAG_BREAK     0x10
#define FLAG_CONSTANT  0x20
#define FLAG_OVERFLOW  0x40
#define FLAG_SIGN      0x80
#define BASE_STACK     0x100

#define saveaccum(n) a = (uint8_t)((n) & 0x00FF)

//flag modifier macros
#define setcarry() cpustatus |= FLAG_CARRY
#define clearcarry() cpustatus &= (~FLAG_CARRY)
#define setzero() cpustatus |= FLAG_ZERO
#define clearzero() cpustatus &= (~FLAG_ZERO)
#define setinterrupt() cpustatus |= FLAG_INTERRUPT
#define clearinterrupt() cpustatus &= (~FLAG_INTERRUPT)
#define setdecimal() cpustatus |= FLAG_DECIMAL
#define cleardecimal() cpustatus &= (~FLAG_DECIMAL)
#define setoverflow() cpustatus |= FLAG_OVERFLOW
#define clearoverflow() cpustatus &= (~FLAG_OVERFLOW)
#define setsign() cpustatus |= FLAG_SIGN
#define clearsign() cpustatus &= (~FLAG_SIGN)

//flag calculation macros
#define zerocalc(n) { if ((n) & 0x00FF) clearzero(); else setzero(); }
#define signcalc(n) { if ((n) & 0x0080) setsign(); else clearsign(); }
#define carrycalc(n) { if ((n) & 0xFF00) setcarry(); else clearcarry(); }
#define overflowcalc(n, m, o) { if (((n) ^ (uint16_t)(m)) & ((n) ^ (o)) & 0x0080) setoverflow(); else clearoverflow(); }


//6502 CPU registers
uint16_t pc;
uint8_t sp, a, x, y, cpustatus;
uint16_t lxx,hxx; // for bcd fix oscar v

//helper variables
uint32_t instructions = 0; //keep track of total instructions executed
int32_t clockticks6502 = 0, clockgoal6502 = 0;
uint16_t oldpc, ea, reladdr, value, result;
uint8_t opcode, oldcpustatus, useaccum;

// some interlinkage with cpu.c and the calling Arduino .ino file:
extern int blitzMode;   // status variable only for microchess
extern  char threeHex[3][2];   // buffer for 3 hex digits
//
uint8_t useKeyboardLed=0x01;   // 0 to use Serial port, 1 to use keyboard/LED display.
void nmi6502(void);
uint8_t nmiFlag = 0;    // added to aid single-stepping SST mode on KIM-I
uint8_t SSTmode = 0;    // SST switch in KIM-I: 1 = on.
uint8_t iii;        // counter for various purposes, 
uint8_t Apple1ColCounter=0;  // max 40 columns for the Apple-1


// ======== Base 1K of KIM-1 RAM =================================================
uint8_t RAM[1024]; 	// main 1KB RAM	     0x0000-0x04FF

// ======== Two possible expansion RAMs ==========================================
#if _TARGET != ARDUINO_MINI     // then 0x0400 up to RAM_LIMIT1 is indeed RAM
uint8_t RAM2[RAM_LIMIT1-0x400];   // RAM #2 from 0x0400 instead of Arduino EEPROM
uint8_t RAM3[RAM_LIMIT2-0x2000];  // expansion RAM #3, 12K
#endif

// ======== RIOT chips each contain 64 bytes of RAM ===============================
uint8_t RAM003[64];    // RAM from 6530-003  0x1780-0x17BF, free for user applications
uint8_t RAM002[64];    // RAM from 6530-002  0x17C0-0x17FF, free for user except 0x17E7-0x17FF

// technical note: the above 8K map is not replicated 8 times to fill 64K, 
// but INSTEAD, the emulator mirrors last 6 bytes of ROM 002 to FFFB-FFFF:
//               FFFA, FFFB - NMI Vector
//               FFFC, FFFD - RST Vector
//               FFFE, FFFF - IRQ Vector
//
// And then just so you know: CRBond calculator uses RAM workspace if it is called upon:
// - floating pt registers (8 bytes each) W1 starts at 3Df, W2 starts at 3E7, W3 starts at 3EF
// so once you start using the math lib, those addresses are overwritten.


// ============ MEMORY READ ROUTINE =================================================
//
// slot in your ROM, RAM, or I/O devices for reading
//
uint8_t read6502(uint16_t address) 
{
  uint8_t tempval = 0;

  // ======= First 1K of RAM ==========
  if (address < 0x0400) { return(RAM[address]);
  }

  // ======= RAM above 1K (can be real RAM or EEPROM!) ==========
  #if _TARGET != ARDUINO_MINI             // then 0x0400 up to RAM_LIMIT1 is indeed RAM
    if (address < RAM_LIMIT1)
      return(RAM2[address-0x0400]);
  #else                                   // on the Pro Mini (atMega328) it's EEPROM,
    if (address < RAM_LIMIT1)             // acting as EEPRAM if you want...
      return(eepromread(address-0x0400));
  #endif

  // =================================================================================
  // I really believe this is an undiscovered but innocent bug in Apple Integer Basic:
  // the thing reads from himem+1. But it's harmless, don't generate an error
  // =================================================================================
  //
  if (address == 0x1000) return(0);         //                          <----- REMOVE THIS IF YOU ADD RAM >$1000!


  // ====== read into empty space ======
  if (address < 0x1700)              		    // read in empty space
  { errout('%'); errout('5'); return(0); }  // error code 5 - read in empty space


  // ====== read into IO space inside RIOTs ======

  if (address < 0x1740)          		        // 0x1700-0x1740 is IO space of RIOT 003
  {
    #if _TARGET != PERSONAL_COMPUTER
    if ((address >= 0x1704) && (address <= 0x1707)) // timer, thanks to Willem Aandewiel
    {
      if (SSTmode == 1) 
      {
          if (address == 0x1707)
            return(0X80);                   // always bailout in SST mode
          else 
          { value = riotTimerRead(address);  return(value); }
      } 
      value = riotTimerRead(address);       // status timer (AaW)
      return(value);
    }
    #endif    
    errout('%');  errout('7');              // warn of unimplemented read in I/O 003
    return(0);
  }
    
  if (address < 0x1780)                     // 0x1740-0x1780 is IO space of RIOT 002
  {
    if (address == 0x1740) 
      return (useKeyboardLed);              // returns 1 for Keypad/LED or 0 for Serial terminal
  	if (address == 0x1747) 
  	  return (0xFF);                // CLKRDI  =$1747,READ TIME OUT BIT,count is always complete...
    errout('%');  errout('6');              // warn of unimplemented read in I/O 002
    return(0);
  }
    
  // ====== read in RAM inside RIOTs ======

  if (address < 0x17C0)                     // 0x1780-0x17C0 is RAM from RIOT 003
    return(RAM003[address-0x1780]);
  
  if (address < 0x1800)          	          // 0x17C0-0x1800 is RAM from RIOT 002
    return(RAM002[address-0x17C0]);

  
  // ====== read in ROM inside RIOTs ======

  if (address < 0x1C00) {              	    // 0x1800-0x1C00 is ROM 003

    if (address==0x1800)
        if (pc==0x1800+1)                   // ========= tape save ===============
        {
        tapeSave(RAM002[0x17F9-0x17C0],                             //ID 
            RAM002[0x17F5-0x17C0], RAM002[0x17F6-0x17C0],           //SAL,SAH 
            RAM002[0x17F7-0x17C0], RAM002[0x17F8-0x17C0],0);          //EAL,EAH
        pc = 0x185C;	                      // skip subroutine, display '0000'
	    return (0xEA);                        // return with a fake NOP instruction 
        }

    if (address==0x1873)                    // ========= tape load ===============
        if (pc==0x1873+1)
        {   
            if(tapeLoad(RAM002[0x17F9-0x17C0], RAM002[0x17F5-0x17C0], RAM002[0x17F6-0x17C0], 0xFF, 0xFF,0)==1) // ID, saved location
                pc = 0x1925;                // normal exit, '0000' display
            else
                pc = 0x1929;	              // load error, display 'FFFF'
		    return (0xEA);                      // return with a fake NOP instruction
        }
    if (address==0x1900)                    // ========= tape directory ==========
        if (pc==0x1900+1)
        {   tapeDirectory();    pc = 0x1C00;    return(0xEA);   }
    if (address=0x1901)                     // ========= tape delete =============
        if (pc==0x1901+1)                   // reading 1901 is not enough, you need to be executing it
        {  tapeDelete(RAM002[0x17F9-0x17C0]); //ID
           pc = 0x1C00;    return(0xEA);
        }
    if (address=0x1902)                     // ========= tape initialise =========
        if (pc==0x1902+1)
    {   tapeInit();    pc = 0x1C00;    return(0xEA);  }

    return(readRom(rom003,address - 0x1800));                           // <-------------------- ???????????????????????????????
  }
  
  if (address < 0x2000)			// 0x1C00-0x2000 is ROM 002. Needs some intercepting
  {
	if (address == 0x1EA0) 		// intercept OUTCH (send char to serial)
	{		serout(a);	// print A to serial
			pc = 0x1ED3;	// skip subroutine
			return (0xEA);  // and return from subroutine with a fake NOP instruction
	}
	if (address == 0x1E65)          //intercept GETCH (get char from serial).
	{		a=getAkey();		// get A from main loop's curkey
			if (a==0) {
				pc=0x1E60;	// cycle through GET1 loop for character start,
				return (0xEA);  //  let the 6502 runs through this loop in a fake way
			}
			clearkey();
			x = RAM[0x00FD];	// x saved in TMPX by getch, need to get it in x;
			pc = 0x1E87;   // skip subroutine
			return (0xEA); // and return from subroutine with a fake NOP instruction
	}
	if (address == 0x1C2A) // intercept DETCPS
	{		RAM002[0x17F3-0x17C0] = 1;  // just store some random bps delay on TTY in CNTH30
			RAM002[0x17F2-0x17C0] = 1;	// just store some random bps delay on TTY in CNTL30
			pc = 0x1C4F;    // skip subroutine
			return (0xEA); // and return from subroutine with a fake NOP instruction
	}

  // the ROM patch below shortens the timing loop in the KIM-1 ROM. Reason: the KIM Uno takes time to emulate the RIOT
  // hardware, whereas the real KIM-1 just bangs a byte or two in RIOT registers and that's it.
  // we shorten the timing loop to compensate for this. If you don't, you'll see the display flicker as the emulation
  // does not achieve enough 'frames per second' in the display updates. 0x2F instead of 0x7F seems to compensate appropriately.
  //
  #if _TARGET == ARDUINO_MINI
  if (address==0x1F5A) return(0x1f); // shorten wait between LED updates as atmega328 is slower! 0x7F originally.  	
  #elif _TARGET == ESP32_R
  if (address==0x1F5A) return(0xff); // lengthen wait between LED updates as esp32 is faster! 0x7F originally.    
  #endif
    
  if (address == 0x1F1F) // intercept SCANDS (display F9,FA,FB)
	{
		// light LEDs ---------------------------------------------------------
		threeHex[0][0]= (RAM[0x00FB] & 0xF0) >> 4;
		threeHex[0][1]= RAM[0x00FB] & 0xF;
		threeHex[1][0]= (RAM[0x00FA] & 0xF0) >> 4;
		threeHex[1][1]= RAM[0x00FA] & 0xF;
		threeHex[2][0]= (RAM[0x00F9] & 0xF0) >> 4;
		threeHex[2][1]= RAM[0x00F9] & 0xF;
                        
	//#ifdef AVRX      // remove this block to get led digits on serial for AVR too
  #if _TARGET != PERSONAL_COMPUTER
		serout(13); serout('>');
		for (iii=0;iii<3;iii++)
		{ serouthex(threeHex[iii][0]); serouthex(threeHex[iii][1]); serout(' ');
		  if (iii==1) serout (' ');
		}
		serout('<'); serout(13); 
//		driveLEDs(); // this was where LEDs were lit up before emulation of 1740-43 was implemented.

	#else          // ------------------ end of block for serial-port led digits
		serouthex(0);  // the PC version does things differently elsewhere...
	#endif

//		pc = 0x1F45;    // skip subroutine part that deals with LEDs (used with driveLEDs above)
//		return (0xEA); // return fake NOP for this first read in subroutine, it'll now go to AK (used with driveLEDs above)
	}

  if (address == 0x1EFE) // intercept AK (check for any key pressed)
	{

// future option: use scankeys here, rather than in the loop() of kimuno.ino.
// But the problem: it will perfectly handle the software keys of the KIM-1,
// however, the hardware switches ST RS SST are then only looked at when called from the KIM ROM,
// so you can't reset anymore if things run outside the ROM. 
//scanKeys();                                   // read input from physical keypad

			a=getAkey();		 // 0 = no key pressed
			if (a==0)	a=0xFF; // that's how AK wants to see 'no key'
			pc = 0x1F14;            // skip subroutine 
			return (0xEA);          // return fake NOP for this first read
	}                                       //  in the subroutine, it'll now RTS at its end
	if (address == 0x1F6A) // intercept GETKEY (get key from keyboard)
	{
			a=getKIMkey();		 // curkey = key code in emulator's keyboard buffer
			clearkey();
			pc = 0x1F90;    // skip subroutine part that deals with LEDs
			return (0xEA); // return fake NOP for this first read in subroutine,
	}                              // it'll now RTS at its end

	// if we're still here, it's normal reading from the highest ROM 002.
	return(readRom(rom002, address - 0x1C00));
}
 

  // ====== read in any potential extra ROMs ======

  #if _INCL_DISASM
  // ==============================================
  // Baum/Wozniak original disasm at 2000
  // ==============================================
	// this now conflicts with RAM expansion in anything except Arduino Mini
  // so disabled for now anywhere else
  #if _TARGET == ARDUINO_MINI  
	if (address < 0x21F9)              	// 0x2000-0x21F8 is disasm              <---------- todo: for now this is Pro Mini only!
    return(readRom(disasmROM, address - 0x2000));
  #endif
  #endif


  // ==============================================
  // KIM-1 expansion RAM from 2000 up to 5000 (could be more w/o Calc ROM!    <---------- todo: would conflict with Baum as ROM
  // ==============================================
  #if _TARGET != ARDUINO_MINI
  if (address < RAM_LIMIT2) 
    return(RAM3[address-0x2000]);
  #endif


  // ==============================================
  // CR Bond's floating point package with support
  // ==============================================
  #if _INCL_CALCULATOR
	if ((address >= 0x5000) && (address <= 0x6FE3)) 	// 0x6FDF, plus 4 bytes for JSR manually added - Read to floating point library between $5000 and $6157
    return(readRom(calcROM,address - 0x5000));

	// 6502 programmable calculator functions. Memory locations in the $7000 range perform
	// special math functions, to make it easy to write programmable calculator programs.
	if ((address>=0x7000) && (address <=0x7200))
        {
          switch (address)
          { // 70Ax SERIES: ASK FOR A VALUE INTO REGISTER x
            case 0x70A1:      // Load W1
              enterflt((uint8_t)0);  return(0x60);  break;
            case 0x70A2:      // Load W2
              enterflt(1);  return(0x60);  break;
            case 0x70A3:      // Load W3
              enterflt(2);  return(0x60);  break;
            case 0x70A4:      // Load W4
              enterflt(3);  return(0x60);  break;
            case 0x70AA:      // Load A
              a = enteroperation();  
              serout(a);
              if (a==0)
                setzero();
              else
                clearzero();
              return(0x60);  
              break;

            //70Dx SERIES: DISPLAY REGISTER x
            case 0x70D1:      // View W1
              showflt(0);  return(0x60);  break;
            case 0x70D2:      // View W2
              showflt(1);  return(0x60);  break;
            case 0x70D3:      // View W3
              showflt(2);  return(0x60);  break;
            case 0x70D4:      // View W4
              showflt(3);  return(0x60);  break;
            
            //71xy SERIES: SWAP REG x AND REG y  
            case 0x7113:      // Swap W1-W3
              swapWreg(1,3);  return(0x60);  break;
            case 0x7123:      // Swap W2-W3
              swapWreg(2,3);  return(0x60);  break;
            case 0x7114:      // Swap W1-W4
              swapWreg(1,4);  return(0x60);  break;
            case 0x7124:      // Swap W2-W4
              swapWreg(2,4);  return(0x60);  break;
            case 0x7112:      // Swap W1-W2
              swapWreg(1,2);  return(0x60);  break;
            case 0x7134:      // Swap W3-W4
              swapWreg(3,4);  return(0x60);  break;

            //70xy SERIES: COPY REG x INTO REG y
            case 0x7013:      // Copy W1-W3
              copyWreg(1,3);  return(0x60);  break;
            case 0x7023:      // Copy W2-W3
              copyWreg(2,3);  return(0x60);  break;

            case 0x7031:      // Copy W3-W1
              copyWreg(3,1);  return(0x60);  break;
            case 0x7032:      // Copy W3-W2
              copyWreg(3,2);  return(0x60);  break;


            case 0x7014:      // Copy W1-W4
              copyWreg(1,4);  return(0x60);  break;
            case 0x7024:      // Copy W2-W4
              copyWreg(2,4);  return(0x60);  break;
            case 0x7034:      // Copy W3-W4
              copyWreg(3,4);  return(0x60);  break;

            case 0x7041:      // Copy W4-W1
              copyWreg(4,1);  return(0x60);  break;
            case 0x7042:      // Copy W4-W2
              copyWreg(4,2);  return(0x60);  break;
            case 0x7043:      // Copy W4-W3
              copyWreg(4,3);  return(0x60);  break;

            default:
	      errout('%'); errout('A');  // error code on serial port to warn of illegal address read
              return (0x00);  // DO A brk TO HALT PROGRAM IF ANY OTHER ADDRESS IS CALLED
          }
        }
#endif

#if _INCL_MCHESS	
	// ===================================================
	// Microchess, with the board print routine at the end
	// ===================================================
	if (address >= 0xC000 && address <0xC571)
	{ 
    	if (address == 0xC202) 	// intercept $C202: Blitz mode should return 0 instead of 8
			if (blitzMode==1)	// This is the Blitz mode hack from the microchess manual.
			 	return((uint8_t) 0x00);

    return(readRom(mchessROM,address - 0xC000));
	}
#endif


	// =====================================================
	// An extra UART for the KIM: Serial port read functions
	// =====================================================
	// for Microchess, VTL-02 and any others but not Apple-1
	// $CFF3: 0 = no key pressed, 1 key pressed
	// Data input: note that this may be a real or simulated serial port depending on setup!
	// $CFF4: input over serial port for mchess. Translates some chars into keypad presses.
	// $CFF5: input over serial port for VTL-02. Pure, no mixing in of keypad simulation. 
	// (see also, in write6502: output services)
	
	if (address == 0xCFF3)		//simulated keyboard input 0=no key press, 1 = key press 
	{ 	
          // update & light LEDs for Mchess
          threeHex[0][0]= (RAM[0x00FB] & 0xF0) >> 4;
          threeHex[0][1]= RAM[0x00FB] & 0xF;
          threeHex[1][0]= (RAM[0x00FA] & 0xF0) >> 4;
          threeHex[1][1]= RAM[0x00FA] & 0xF;
          threeHex[2][0]= (RAM[0x00F9] & 0xF0) >> 4;
          threeHex[2][1]= RAM[0x00F9] & 0xF;
          //#ifdef AVRX
          #if _TARGET != PERSONAL_COMPUTER
          driveLEDs();
          #endif  
	  return(getAkey()==0?(uint8_t)0:(uint8_t)1);
	}

	if (address == 0xCFF4) 					//simulated keyboard input for Microchess only
	{	tempval = getAkey();
		clearkey();
        // translate KIM-1 button codes into ASCII code expected by this version of Microchess
        switch (tempval) 
        {	case 16:  tempval = 'P'; break;    // PC translated to P
    			case 'F': tempval = 13;  break;    // F translated to Return
    			case '+': tempval = 'W'; break;    // + translated to W == Blitz mode toggle 
        }             
		if (tempval==0x57)         // 'W'. If user presses 'W', he wants to toggle Blitz mode
		{	if (blitzMode==1) (blitzMode=0);
			else              (blitzMode=1);
			serout('>'); serout( (blitzMode==1)?'B':'N' );	serout('<');
		}
		return(tempval);
	}

	if (address == 0xCFF5)           // keyboard input, w/o keypad translation for e.g. VTL02
	  { tempval = getAkey();
	    clearkey();
	    return(tempval);
	  }

// ==============================================
// Apple-1 keyboard & display I/O: read functions
// ==============================================
//
	if (address == 0xD010)           // KBD read keyboard
	{ tempval = getAkey();
	    clearkey();
	    
		if (tempval >= 0x61 && tempval <= 0x7A)
			tempval &= 0x5F;	// lower to upper case conversion

		else if (tempval=='\n')
		{
			tempval ='\r'; 
		}
		return(tempval|0x80);	
		// set the bit7 (apple-1 hardware artifact), 
	        //and deal with the 8D (\n) vs 8A (\r) confusion
	}

	if (address == 0xD011)		//KBDCR check for keypress
	{ 	
	  return(getAkey()==0?(uint8_t)0x00:(uint8_t)0x80); // bit 7 = 1 when key pressed
	}
	
	if (address == 0xD012)		//DSP
	{ 	
		return (0x00);		// 0x80 BPL (bit 7!=1 --> DSP not ready)
	}


#if _INCL_VTL02
  // ==============================================
  // VTL-02 at DC00
  // ==============================================
  if ((address >= 0xDC00) && (address <= 0xDFFE))   // VTL-02
    return(readRom(vtl02ROM,address - 0xDC00));
#endif


#if _INCL_A1BASIC
  // ==============================================
  // APPLE-1 BASIC at E0000
  // ==============================================
	if (address >= 0xE000 && address <=0xEFFF) 	// Read from 4K Apple-1 Basic ROM
    return(readRom(apple1ROM,address - 0xE000));
#endif


#if _INCL_ACI
  // ==============================================
  // APPLE-1 ACI (cassette i/f) at F1000
  // ==============================================
	if ((address >= 0xF100) && (address < 0xF200)) 	// 
	{
    // INCOMPATIBILITY: multi-segment writes and loads will not work

    if (address==0xF170)
        if (pc==0xF170+1)                       // ========= tape save ===============
        {
          tapeSave(RAM002[0x17F9-0x17C0],                       //ID (not used unless atMega328!)
            read6502(0x0026), read6502(0x0027),                 //SAL,SAH 
            read6502(0x0024), read6502(0x0025),1);              //EAL,EAH
        pc = 0xF18B;	                  // skip subroutine
	    return (0xEA);                    // return with a fake NOP instruction 
        }

    if (address==0xF18D)                        // ========= tape load ===============
        if (pc==0xF18D+1)
        {   
          if(tapeLoad(RAM002[0x17F9-0x17C0],                  //ID (not used unless atMega328!)
              read6502(0x0026), read6502(0x0027),             //SAL, SAH
              read6502(0x0024), read6502(0x0025),1)==1)       //EAL, EAH, 1 means Apple fmt

              pc = 0xF18B; // incorrect but harmless //0xF1CB; // normal load
          else
              pc = 0xF18B; // incorrect but harmless //0xF1CB; // load error
		      return (0xEA);                        // return with a fake NOP instruction
        }

    return(readRom(aciROM, address - 0xF100));  // ======== APPLE-1 ACI ROM regular read ========
  }
#endif


#if _INCL_MINIASM
  // ==============================================
  // APPLE-1 MINI-ASM at FBAE and WOZMON at FF00
  // ==============================================
	if ((address >= 0xFBAE) && (address < 0xFFFB)) 	// 0xFBAE-0xFFFF is mini-assembler & wozmon
    return(readRom(asmROM, address - 0xFBAE));    // but last 4 bytes are handled separately
#endif

  // ==============================================
  // 6502 reset vectors at top of memory
  // ==============================================
  // You could make these switch between KIM-1 and Apple-1 reset vectors. Too cute for me though.
  // As it is now, this just returns the top of the KIM-1 ROM with its reset vectors.
  // Remember, unexpanded KIM-1's mirrored the bottom 8K 8 times, so that ROM came out here on top too.
  //
	if (address >= 0xFFFA)    // Reroute to top of KIM-1's ROM002.
    return(readRom(rom002, address - 0xFC00));
	
  // funny fix this one...
  if (address == 0x20A9)    // microchess' board print routine uses a BIT (0x2c) test/skip
    return(0);              // it randomly reads 0x20A9; if that's not RAM, don't report the error below
	else
	  errout('%'); errout('9');
	return (0);	// This should never be reached unless some addressing bug, so return 6502 BRK
}


// ============ MEMORY WRITE ROUTINE ================================================
//
// here, you can slot in your RAM or I/O devices for writing
//
void write6502(uint16_t address, uint8_t value) 
{
  // ==========================================
  // First 1 K of RAM:
  // ==========================================
  if (address < 0x0400) {
    RAM[address]=value;
    return;
  }

  // ===========================================
  // RAM above 1K (can be either RAM or EEPROM!)
  // size depends on platform: either 1K or more
  // ===========================================
  #if _TARGET != ARDUINO_MINI     // then 0x0400 up to RAM_LIMIT1 is indeed RAM
    if (address < RAM_LIMIT1)
      { RAM2[address-0x0400]=value; return; }
  #else                           // on the Pro Mini (atMega328) it's EEPROM,
    if (address < RAM_LIMIT1)     // acting as EEPRAM if you want...
      { eepromwrite(address-0x0400, value); return; }
  #endif


  // ===========================================
  // warn for writing into empty space in memmap;
  // ===========================================  
//  if (address < 0x1700) {                          // illegal access
//    errout('%');  errout('1');      // error code 1 - write in empty space
//    return;
//  }


  // ===========================================
  // RIOT I/O space, first RIOT003, then RIOT002
  // ===========================================
  //
  if (address < 0x1740) {                          // ======== I/O 003 ========
    #if _TARGET != PERSONAL_COMPUTER
    // timer, thanks to Willem Aandewiel
    if ((address >= 0x1704) && (address <= 0x1707)) {   // set timer
        riotTimerWrite((address), value);                   // AaW
        return;
    }
    #endif
    // errout('%');  errout('3');      // trap code 3 - io3 access
    return;
  }
  
  if (address < 0x1780) {                          // ======== I/O 002 ========
    // direct addressing of LED display, as per Jeremy Starcher:

    if (address == 0x1740) {    // Set Segment 1 t/m 7      
        riot2regs.ioPAD = value;        write1740();
    }
    if (address == 0x1741) {    // Set DDR for segment 1 t/m 7
        riot2regs.ioPADD = value;        write1741();
    }
    if (address == 0x1742) {    // Select row0 t/m row2 and led1 t/m led6 
        riot2regs.ioPBD = value;// & riot2regs.ioPBDD;  // <--------------- future improvement...
        write1742();
    }
    if (address == 0x1743) {    // set DDR for row0 t/m row2 and led1 t/m led6
        riot2regs.ioPBDD = value;        write1743();
    }
    //errout('%');  errout('2');      // trap code 2 - io2 access
    return;
  }

  // ===========================================
  // the 64 bytes of RAM in each RIOT chip
  // ===========================================    
  //
  if (address < 0x17C0) {                          // RAM 003
    RAM003[address-0x1780]=value;
    return;
  }
  if (address < 0x1800) {                          // RAM 002
    RAM002[address-0x17C0]=value;
    return;
  }

  // ===========================================
  // KIM-1 expansion RAM from $2000
  // ===========================================    
  //
#if _TARGET != ARDUINO_MINI
  if (address < RAM_LIMIT2) {                         // RAM3
    RAM3[address-0x2000]=value;
    return;
  }
#endif

/*
  if ((address >=0x5000) && (address <=0x6FDF)) {                          // illegal write in fltpt65 ROM
    serout('%');  serout('a');
    return;
  }
*/

  // ===========================================
  // Extra UART: Serial port out for microchess
  //  (or anything else that needs it) at $CFF1
  // ===========================================
  //
  if (address == 0xCFF1) {                          // Character out for microchess only
  	serout(value);
	  return;
  }
  
  // ==============================================
  // Apple-1 keyboard and display I/O
  // write functions
  // ==============================================
  //
  if (address == 0xD011) {                          // KBDCR
    return;
  }
  if (address == 0xD012) {                          // DSP
  	// below is misery around CR LF output; KIM and Apple do it differently, PC/ncurses and Arduino do it differently too. Jeez.
    #if _TARGET == PERSONAL_COMPUTER                //            <------- todo: normalise PC version of 40 col back to normal version
  	if (value==0x8d) value=0x8a;  // to deal with ncurses on pc, should be handled elsewhere!
    serout(value & 0x7F);       // strip bit 7
    return;
    #endif

    #if _TARGET != PERSONAL_COMPUTER
/*    if (value==0x8d) serout(0x0a); // serial terminals want both a CR and a LF...
    serout(value & 0x7F);       // strip bit 7
    return;
*/
    value = value & 0x7F;     // strip 7th bit to get to normal ascii

    if (value=='\r')
      Apple1ColCounter=0;

    if(Apple1ColCounter==40)
    {    Apple1ColCounter=0;         serout('\r'); serout('\n');   }

    if (value!=0x00)
      serout(value & 0x7F);       // strip bit 7
    else 
      serout('#');

    if (value=='\r')  
      serout('\n');
    else      
      Apple1ColCounter++;
    return;  
    #endif
  }
  if (address == 0xD013) {                          // DSPCR
  	return;											// nothing to do
  }

  // ==============================================
  // Warn if written anywhere else
  // ==============================================
//  errout('%');  errout('4');      // error code 4 - write to ROM
}


// ---- two functions for fltpt65 support: copy/swap W registers
void copyWreg(uint8_t a, uint8_t b)
{
  uint8_t i;
  for (i=0;i<8;i++)
    RAM[WREG_OFFSET+8*(b-1)+i] = RAM[WREG_OFFSET+8*(a-1)+i];
}
void swapWreg(uint8_t a, uint8_t b)
{
  uint8_t i, buffer;
  for (i=0;i<8;i++)
  { buffer = RAM[WREG_OFFSET+8*(a-1)+i];
    RAM[WREG_OFFSET+8*(a-1)+i] = RAM[WREG_OFFSET+8*(b-1)+i];
    RAM[WREG_OFFSET+8*(b-1)+i] = buffer;
  }
}


void initKIM() {                  
        uint16_t i;
        // this is what user has to enter manually when powering KIM on. Why not do it here.
	RAM002[(0x17FA)-(0x17C0)]=0x00;
	RAM002[(0x17FB)-(0x17C0)]=0x1C;
	RAM002[(0x17FE)-(0x17C0)]=0x00;
	RAM002[(0x17FF)-(0x17C0)]=0x1C;

  // the code below copies movit (a copy routine) to 0x1780 in RAM. 
  for (i=0;i<64;i++)  //64 of 102 program bytes
  	RAM003[i] = readRom(movitROM, i);

	// movit spans into the second 64 byte memory segment...
  for (i=0;i<(95-64);i++)                            
    RAM002[i] = readRom( movitROM, i+64);

  // the code below copies relocate to 0x0110 in RAM. i
	// It can be overwritten by users or by the stack pointer - it's an extra
  for (i=0;i<149;i++) 
    RAM[i+0x0110] = readRom(relocateROM, i);

  // the code below copies branch to 0x01A5 in RAM. 
	// It can be overwritten by users - it's an extra
  // the program can easily be overwritten by the stack, because it ends at 1CF. 
	// Still, the monitor brings the stack down to no worse than 0x1FF-8.
  for (i=0;i<42;i++) 
          RAM[i+0x01A5] = readRom(branchROM, i);

	// setting these two values facilitates the first program in the 1st Book of KIM.
	RAM[0x0010] = 0x10;
	RAM[0x0011] = 0x11;

  // the code below copies Apple-1 life to $2000 
  #if _TARGET != ARDUINO_MINI
  for (i=0;i<440;i++)  //64 of 102 program bytes
  	RAM3[i] = readRom(lifeROM, i);
  #endif
}

void loadTestProgram() {  // Call this from main() if you want a program preloaded. It's the first program from First Book of KIM...
        uint8_t i;

  // Fill RAM with fun software...
  
  // Demo First is the first program from the First Book of KIM. A silly but simple demo. 
  // it can be overwritten if below you load a particularly big other demo program, that's OK.
  #if _DEMO_FIRST
  for (i=0;i<9;i++) 
    	RAM[i+0x0200]=readRom(fbkDemo,i);
  #endif
  
  // The little demo program to show off the 6502 programmable calculator feature.
  // it can be overwritten if below you load a particularly big other demo program, that's OK.
  #if _INCL_CALCULATOR
  for (i=0x0;i<31;i++) 
    	RAM[i+0x0210]=readRom(fltptDemo, i);
  #endif

// the others below are games from First Book of KIM. Mostly here to test the existing
// incompatibilities for a future version. 'DIRECT RIOT ADDRESSING GAMES MOSTLY DO NOT WORK'
// BTW - incompatibilities have to do with our emulation from keypad out of ROM, rather
// than from inside the RIOT hardware that the ROM uses. We do so for a reason:
// we want the Reset key to work at all times. But obviously, we need a better plan...
//

#if _DEMO_TIMER
  for (uint16_t ii=0;ii<100;ii++) 
    	RAM[ii+0x0200]=readRom(timerDemo, ii);
#endif

#if _DEMO_HILO
  for (uint16_t ii=0;ii<110;ii++) 
    	RAM[ii+0x0200]=readRom(hiLoDemo, ii);
#endif

#if _INCL_FARMERBROWN
  for (uint16_t ii=0;ii<206;ii++) 
      RAM[ii+0x0200]=readRom(farmerROM, ii);
#endif

#if _DEMO_ASTEROID
  for (uint16_t ii=0;ii<262;ii++) 
      RAM[ii+0x0200]=readRom(asteroidDemo, ii);
#endif

#if _DEMO_BLACKJACK
  for (uint16_t ii=0;ii<491;ii++) 
      RAM[ii+0x0200]=readRom(blackjackDemo, ii);
#endif
}


// ===========================================================================
// From here, it's mostly Chambers' 6502 simulator with minimal changes
// (BCD bug fixed, some NMI/RST intercepts)
// ===========================================================================

// ---- a few general functions used by various other functions
void push16(uint16_t pushval) {
    write6502(BASE_STACK + sp, (pushval >> 8) & 0xFF);
    write6502(BASE_STACK + ((sp - 1) & 0xFF), pushval & 0xFF);
    sp -= 2;
}

void push8(uint8_t pushval) {
    write6502(BASE_STACK + sp--, pushval);
}

uint16_t pull16() {
    uint16_t temp16;
    temp16 = read6502(BASE_STACK + ((sp + 1) & 0xFF)) | ((uint16_t)read6502(BASE_STACK + ((sp + 2) & 0xFF)) << 8);
    sp += 2;
    return(temp16);
}

uint8_t pull8() {
    return (read6502(BASE_STACK + ++sp));
}

void reset6502() {
    pc = (uint16_t)read6502(0xFFFC) | ((uint16_t)read6502(0xFFFD) << 8);
    a = 0;
    x = 0;
    y = 0;
    sp = 0xFD;
    cpustatus |= FLAG_CONSTANT;
}
//addressing mode functions, calculates effective addresses
void imp() { //implied
}

void acc() { //accumulator
  useaccum = 1;
}

void imm() { //immediate
    ea = pc++;
}

void zp() { //zero-page
    ea = (uint16_t)read6502((uint16_t)pc++);
}

void zpx() { //zero-page,X
    ea = ((uint16_t)read6502((uint16_t)pc++) + (uint16_t)x) & 0xFF; //zero-page wraparound
}

void zpy() { //zero-page,Y
    ea = ((uint16_t)read6502((uint16_t)pc++) + (uint16_t)y) & 0xFF; //zero-page wraparound
}

void rel() { //relative for branch ops (8-bit immediate value, sign-extended)
    reladdr = (uint16_t)read6502(pc++);
    if (reladdr & 0x80) reladdr |= 0xFF00;
}

void abso() { //absolute
    ea = (uint16_t)read6502(pc) | ((uint16_t)read6502(pc+1) << 8);
    pc += 2;
}

void absx() { //absolute,X
    uint16_t startpage;
    ea = ((uint16_t)read6502(pc) | ((uint16_t)read6502(pc+1) << 8));
    startpage = ea & 0xFF00;
    ea += (uint16_t)x;

    pc += 2;
}

void absy() { //absolute,Y
    uint16_t startpage;
    ea = ((uint16_t)read6502(pc) | ((uint16_t)read6502(pc+1) << 8));
    startpage = ea & 0xFF00;
    ea += (uint16_t)y;

    pc += 2;
}

void ind() { //indirect
    uint16_t eahelp, eahelp2;
    eahelp = (uint16_t)read6502(pc) | (uint16_t)((uint16_t)read6502(pc+1) << 8);
    eahelp2 = (eahelp & 0xFF00) | ((eahelp + 1) & 0x00FF); //replicate 6502 page-boundary wraparound bug
    ea = (uint16_t)read6502(eahelp) | ((uint16_t)read6502(eahelp2) << 8);
    pc += 2;
}

void indx() { // (indirect,X)
    uint16_t eahelp;
    eahelp = (uint16_t)(((uint16_t)read6502(pc++) + (uint16_t)x) & 0xFF); //zero-page wraparound for table pointer
    ea = (uint16_t)read6502(eahelp & 0x00FF) | ((uint16_t)read6502((eahelp+1) & 0x00FF) << 8);
}

void indy() { // (indirect),Y
    uint16_t eahelp, eahelp2, startpage;
    eahelp = (uint16_t)read6502(pc++);
    eahelp2 = (eahelp & 0xFF00) | ((eahelp + 1) & 0x00FF); //zero-page wraparound
    ea = (uint16_t)read6502(eahelp) | ((uint16_t)read6502(eahelp2) << 8);
    startpage = ea & 0xFF00;
    ea += (uint16_t)y;

}

static uint16_t getvalue() {
    if (useaccum) return((uint16_t)a);
        else return((uint16_t)read6502(ea));
}

void putvalue(uint16_t saveval) {
    if (useaccum) a = (uint8_t)(saveval & 0x00FF);
        else write6502(ea, (saveval & 0x00FF));
}


//instruction handler functions
void adc() {
    value = getvalue();

// BCD fix OV 20140915 - adc 
	if ((cpustatus & FLAG_DECIMAL)==0) {
    	result = (uint16_t)a + value + (uint16_t)(cpustatus & FLAG_CARRY);
   
	    carrycalc(result);
	    zerocalc(result);
 	 	overflowcalc(result, a, value);
  	 	signcalc(result);
	}    
    else // #ifndef NES_CPU
    {	 // Decimal mode
        lxx = (a & 0x0f) + (value & 0x0f) + (uint16_t)(cpustatus & FLAG_CARRY);
		if ((lxx & 0xFF) > 0x09)
            lxx += 0x06;
        hxx = (a >> 4) + (value >> 4) + (lxx > 15 ? 1 : 0);
        if ((hxx & 0xff) > 9) 
			hxx += 6;
        result = (lxx & 0x0f);
		result += (hxx << 4); 
        result &= 0xff;
		// deal with carry flag:
		if (hxx>15)
			setcarry();
		else
			clearcarry();
	    zerocalc(result);
    	clearsign();	// negative flag never set for decimal mode.
	    clearoverflow();	// overflow never set for decimal mode.
// end of BCD fix PART 2

        clockticks6502++;
    }
//    #endif // of NES_CPU

    saveaccum(result);
}

void op_and() {
    value = getvalue();
    result = (uint16_t)a & value;

    zerocalc(result);
    signcalc(result);

    saveaccum(result);
}

void asl() {
    value = getvalue();
    result = value << 1;

    carrycalc(result);
    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void bcc() {
    if ((cpustatus & FLAG_CARRY) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) clockticks6502 += 2; //check if jump crossed a page boundary
            else clockticks6502++;
    }
}

void bcs() {
    if ((cpustatus & FLAG_CARRY) == FLAG_CARRY) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) clockticks6502 += 2; //check if jump crossed a page boundary
            else clockticks6502++;
    }
}

void beq() {
    if ((cpustatus & FLAG_ZERO) == FLAG_ZERO) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) clockticks6502 += 2; //check if jump crossed a page boundary
            else clockticks6502++;
    }
}

void op_bit() {
    value = getvalue();
    result = (uint16_t)a & value;

    zerocalc(result);
    cpustatus = (cpustatus & 0x3F) | (uint8_t)(value & 0xC0);
}

void bmi() {
    if ((cpustatus & FLAG_SIGN) == FLAG_SIGN) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) clockticks6502 += 2; //check if jump crossed a page boundary
            else clockticks6502++;
    }
}

void bne() {
    if ((cpustatus & FLAG_ZERO) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) clockticks6502 += 2; //check if jump crossed a page boundary
            else clockticks6502++;
    }
}

void bpl() {
    if ((cpustatus & FLAG_SIGN) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) clockticks6502 += 2; //check if jump crossed a page boundary
            else clockticks6502++;
    }
}

void brk() {
    pc++;
    push16(pc); //push next instruction address onto stack
    push8(cpustatus | FLAG_BREAK); //push CPU cpustatus to stack
    setinterrupt(); //set interrupt flag
    pc = (uint16_t)read6502(0xFFFE) | ((uint16_t)read6502(0xFFFF) << 8);
}

void bvc() {
    if ((cpustatus & FLAG_OVERFLOW) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) clockticks6502 += 2; //check if jump crossed a page boundary
            else clockticks6502++;
    }
}

void bvs() {
    if ((cpustatus & FLAG_OVERFLOW) == FLAG_OVERFLOW) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) clockticks6502 += 2; //check if jump crossed a page boundary
            else clockticks6502++;
    }
}

void clc() {
    clearcarry();
}

void cld() {
    cleardecimal();
}

void cli() {
    clearinterrupt();
}

void clv() {
    clearoverflow();
}

void cmp() {
    value = getvalue();
    result = (uint16_t)a - value;

    if (a >= (uint8_t)(value & 0x00FF)) setcarry();
        else clearcarry();
    if (a == (uint8_t)(value & 0x00FF)) setzero();
        else clearzero();
    signcalc(result);
}

void cpx() {
    value = getvalue();
    result = (uint16_t)x - value;

    if (x >= (uint8_t)(value & 0x00FF)) setcarry();
        else clearcarry();
    if (x == (uint8_t)(value & 0x00FF)) setzero();
        else clearzero();
    signcalc(result);
}

void cpy() {
    value = getvalue();
    result = (uint16_t)y - value;

    if (y >= (uint8_t)(value & 0x00FF)) setcarry();
        else clearcarry();
    if (y == (uint8_t)(value & 0x00FF)) setzero();
        else clearzero();
    signcalc(result);
}

void dec() {
    value = getvalue();
    result = value - 1;

    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void dex() {
    x--;

    zerocalc(x);
    signcalc(x);
}

void dey() {
    y--;

    zerocalc(y);
    signcalc(y);
}

void eor() {
    value = getvalue();
    result = (uint16_t)a ^ value;

    zerocalc(result);
    signcalc(result);

    saveaccum(result);
}

void inc() {
    value = getvalue();
    result = value + 1;

    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void inx() {
    x++;

    zerocalc(x);
    signcalc(x);
}

void iny() {
    y++;

    zerocalc(y);
    signcalc(y);
}

void jmp() {
    pc = ea;
}

void jsr() {
    push16(pc - 1);
    pc = ea;
}

void lda() {
    value = getvalue();
    a = (uint8_t)(value & 0x00FF);

    zerocalc(a);
    signcalc(a);
}

void ldx() {
    value = getvalue();
    x = (uint8_t)(value & 0x00FF);

    zerocalc(x);
    signcalc(x);
}

void ldy() {
    value = getvalue();
    y = (uint8_t)(value & 0x00FF);

    zerocalc(y);
    signcalc(y);
}

void lsr() {
    value = getvalue();
    result = value >> 1;

    if (value & 1) setcarry();
        else clearcarry();
    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void nop() {
}

void ora() {
    value = getvalue();
    result = (uint16_t)a | value;

    zerocalc(result);
    signcalc(result);

    saveaccum(result);
}

void pha() {
    push8(a);
}

void php() {
    push8(cpustatus | FLAG_BREAK);
}

void pla() {
    a = pull8();

    zerocalc(a);
    signcalc(a);
}

void plp() {
    cpustatus = pull8() | FLAG_CONSTANT;
}

void rol() {
    value = getvalue();
    result = (value << 1) | (cpustatus & FLAG_CARRY);

    carrycalc(result);
    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void ror() {
    value = getvalue();
    result = (value >> 1) | ((cpustatus & FLAG_CARRY) << 7);

    if (value & 1) setcarry();
        else clearcarry();
    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void rti() {
    cpustatus = pull8();
    value = pull16();
    pc = value;
}

void rts() {
    value = pull16();
    pc = value + 1;
}

void sbc() {
// BCD fix OV 20140915 - adc 
    if ((cpustatus & FLAG_DECIMAL)==0) {
	    value = getvalue() ^ 0x00FF;
	    result = (uint16_t)a + value + (uint16_t)(cpustatus & FLAG_CARRY);
   
	 	carrycalc(result);
	    zerocalc(result);
	    overflowcalc(result, a, value);
	    signcalc(result);
	}
 	else //   #ifndef NES_CPU
	{ 		// decimal mode
	    value = getvalue();
        lxx = (a & 0x0f) - (value & 0x0f) - (uint16_t)((cpustatus & FLAG_CARRY)?0:1);  
        if ((lxx & 0x10) != 0) 
			lxx -= 6;
        hxx = (a >> 4) - (value >> 4) - ((lxx & 0x10) != 0 ? 1 : 0);
        if ((hxx & 0x10) != 0) 
			hxx -= 6;
		result = (lxx & 0x0f);
		result += (hxx << 4);
		result = (lxx & 0x0f) | (hxx << 4);
		// deal with carry
		if ((hxx & 0xff) < 15)
			setcarry();			// right? I think so. Intended is   setCarryFlag((hxx & 0xff) < 15);
		else 
			clearcarry();
	    zerocalc(result); // zero dec is zero hex, no problem?
    	clearsign();	// negative flag never set for decimal mode. That's a simplification, see http://www.6502.org/tutorials/decimal_mode.html
	    clearoverflow();	// overflow never set for decimal mode.
		result = result & 0xff;
// end of BCD fix PART 3 (final part)

        clockticks6502++;
    }
//    #endif // of NES_CPU

    saveaccum(result);
}

void sec() {
    setcarry();
}

void sed() {
    setdecimal();
}

void sei() {
    setinterrupt();
}

void sta() {
    putvalue(a);
}

void stx() {
    putvalue(x);
}

void sty() {
    putvalue(y);
}

void tax() {
    x = a;

    zerocalc(x);
    signcalc(x);
}

void tay() {
    y = a;

    zerocalc(y);
    signcalc(y);
}

void tsx() {
    x = sp;

    zerocalc(x);
    signcalc(x);
}

void txa() {
    a = x;

    zerocalc(a);
    signcalc(a);
}

void txs() {
    sp = x;
}

void tya() {
    a = y;

    zerocalc(a);
    signcalc(a);
}

//undocumented instructions
#ifdef UNDOCUMENTED
    void lax() {
        lda();
        ldx();
    }

    void sax() {
        sta();
        stx();
        putvalue(a & x);
    }

    void dcp() {
        dec();
        cmp();
    }

    void isb() {
        inc();
        sbc();
    }

    void slo() {
        asl();
        ora();
    }

    void rla() {
        rol();
        op_and();
    }

    void sre() {
        lsr();
        eor();
    }

    void rra() {
        ror();
        adc();
    }
#else
    #define lax nop
    #define sax nop
    #define dcp nop
    #define isb nop
    #define slo nop
    #define rla nop
    #define sre nop
    #define rra nop
#endif


void nmi6502() {
    push16(pc);
    push8(cpustatus);
    cpustatus |= FLAG_INTERRUPT;
pc = (uint16_t)read6502(0xFFFA) | ((uint16_t)read6502(0xFFFB) << 8);
pc = 0x1C1C;
}

void irq6502() {
    push16(pc);
    push8(cpustatus);
    cpustatus |= FLAG_INTERRUPT;
    pc = (uint16_t)read6502(0xFFFE) | ((uint16_t)read6502(0xFFFF) << 8);
//	pc = 0x1C1F;
}

#ifdef USE_TIMING
prog_char ticktable[256] PROGMEM = {
/*        |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |     */
/* 0 */      7,    6,    2,    8,    3,    3,    5,    5,    3,    2,    2,    2,    4,    4,    6,    6,  /* 0 */
/* 1 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 1 */
/* 2 */      6,    6,    2,    8,    3,    3,    5,    5,    4,    2,    2,    2,    4,    4,    6,    6,  /* 2 */
/* 3 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 3 */
/* 4 */      6,    6,    2,    8,    3,    3,    5,    5,    3,    2,    2,    2,    3,    4,    6,    6,  /* 4 */
/* 5 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 5 */
/* 6 */      6,    6,    2,    8,    3,    3,    5,    5,    4,    2,    2,    2,    5,    4,    6,    6,  /* 6 */
/* 7 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 7 */
/* 8 */      2,    6,    2,    6,    3,    3,    3,    3,    2,    2,    2,    2,    4,    4,    4,    4,  /* 8 */
/* 9 */      2,    6,    2,    6,    4,    4,    4,    4,    2,    5,    2,    5,    5,    5,    5,    5,  /* 9 */
/* A */      2,    6,    2,    6,    3,    3,    3,    3,    2,    2,    2,    2,    4,    4,    4,    4,  /* A */
/* B */      2,    5,    2,    5,    4,    4,    4,    4,    2,    4,    2,    4,    4,    4,    4,    4,  /* B */
/* C */      2,    6,    2,    8,    3,    3,    5,    5,    2,    2,    2,    2,    4,    4,    6,    6,  /* C */
/* D */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* D */
/* E */      2,    6,    2,    8,    3,    3,    5,    5,    2,    2,    2,    2,    4,    4,    6,    6,  /* E */
/* F */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7   /* F */
};
#endif

void exec6502(int32_t tickcount) {
#ifdef USE_TIMING
  clockgoal6502 += tickcount;

  while (clockgoal6502 > 0) {
#else
  while (tickcount--) {
#endif

#ifdef DEBUGUNO
	debugPC = pc;
	int ii=0;

#endif

// part 1 of single stepping using NMI
	if ((SSTmode==1) & (pc<0x1C00)) // no mni if running ROM code (K7), that would also single-step the monitor code!
			nmiFlag=1; // handled after this instruction has completed.
// -------------

    opcode = read6502(pc++);
    cpustatus |= FLAG_CONSTANT;
    useaccum = 0;

		switch (opcode) {
		case 0x0:
			imp();
			brk();
			break;
		case 0x1:
			indx();
			ora();
			break;
		case 0x5:
			zp();
			ora();
			break;
		case 0x6:
			zp();
			asl();
			break;
		case 0x8:
			imp();
			php();
			break;
		case 0x9:
			imm();
			ora();
			break;
		case 0xA:
			acc();
			asl();
			break;
		case 0xD:
			abso();
			ora();
			break;
		case 0xE:
			abso();
			asl();
			break;
		case 0x10:
			rel();
			bpl();
			break;
		case 0x11:
			indy();
			ora();
			break;
		case 0x15:
			zpx();
			ora();
			break;
		case 0x16:
			zpx();
			asl();
			break;
		case 0x18:
			imp();
			clc();
			break;
		case 0x19:
			absy();
			ora();
			break;
		case 0x1D:
			absx();
			ora();
			break;
		case 0x1E:
			absx();
			asl();
			break;
		case 0x20:
			abso();
			jsr();
			break;
		case 0x21:
			indx();
			op_and();
			break;
		case 0x24:
			zp();
			op_bit();
			break;
		case 0x25:
			zp();
			op_and();
			break;
		case 0x26:
			zp();
			rol();
			break;
		case 0x28:
			imp();
			plp();
			break;
		case 0x29:
			imm();
			op_and();
			break;
		case 0x2A:
			acc();
			rol();
			break;
		case 0x2C:
			abso();
			op_bit();
			break;
		case 0x2D:
			abso();
			op_and();
			break;
		case 0x2E:
			abso();
			rol();
			break;
		case 0x30:
			rel();
			bmi();
			break;
		case 0x31:
			indy();
			op_and();
			break;
		case 0x35:
			zpx();
			op_and();
			break;
		case 0x36:
			zpx();
			rol();
			break;
		case 0x38:
			imp();
			sec();
			break;
		case 0x39:
			absy();
			op_and();
			break;
		case 0x3D:
			absx();
			op_and();
			break;
		case 0x3E:
			absx();
			rol();
			break;
		case 0x40:
			imp();
			rti();
			break;
		case 0x41:
			indx();
			eor();
			break;
		case 0x45:
			zp();
			eor();
			break;
		case 0x46:
			zp();
			lsr();
			break;
		case 0x48:
			imp();
			pha();
			break;
		case 0x49:
			imm();
			eor();
			break;
		case 0x4A:
			acc();
			lsr();
			break;
		case 0x4C:
			abso();
			jmp();
			break;
		case 0x4D:
			abso();
			eor();
			break;
		case 0x4E:
			abso();
			lsr();
			break;
		case 0x50:
			rel();
			bvc();
			break;
		case 0x51:
			indy();
			eor();
			break;
		case 0x55:
			zpx();
			eor();
			break;
		case 0x56:
			zpx();
			lsr();
			break;
		case 0x58:
			imp();
			cli();
			break;
		case 0x59:
			absy();
			eor();
			break;
		case 0x5D:
			absx();
			eor();
			break;
		case 0x5E:
			absx();
			lsr();
			break;
		case 0x60:
			imp();
			rts();
			break;
		case 0x61:
			indx();
			adc();
			break;
		case 0x65:
			zp();
			adc();
			break;
		case 0x66:
			zp();
			ror();
			break;
		case 0x68:
			imp();
			pla();
			break;
		case 0x69:
			imm();
			adc();
			break;
		case 0x6A:
			acc();
			ror();
			break;
		case 0x6C:
			ind();
			jmp();
			break;
		case 0x6D:
			abso();
			adc();
			break;
		case 0x6E:
			abso();
			ror();
			break;
		case 0x70:
			rel();
			bvs();
			break;
		case 0x71:
			indy();
			adc();
			break;
		case 0x75:
			zpx();
			adc();
			break;
		case 0x76:
			zpx();
			ror();
			break;
		case 0x78:
			imp();
			sei();
			break;
		case 0x79:
			absy();
			adc();
			break;
		case 0x7D:
			absx();
			adc();
			break;
		case 0x7E:
			absx();
			ror();
			break;
		case 0x81:
			indx();
			sta();
			break;
		case 0x84:
			zp();
			sty();
			break;
		case 0x85:
			zp();
			sta();
			break;
		case 0x86:
			zp();
			stx();
			break;
		case 0x88:
			imp();
			dey();
			break;
		case 0x8A:
			imp();
			txa();
			break;
		case 0x8C:
			abso();
			sty();
			break;
		case 0x8D:
			abso();
			sta();
			break;
		case 0x8E:
			abso();
			stx();
			break;
		case 0x90:
			rel();
			bcc();
			break;
		case 0x91:
			indy();
			sta();
			break;
		case 0x94:
			zpx();
			sty();
			break;
		case 0x95:
			zpx();
			sta();
			break;
		case 0x96:
			zpy();
			stx();
			break;
		case 0x98:
			imp();
			tya();
			break;
		case 0x99:
			absy();
			sta();
			break;
		case 0x9A:
			imp();
			txs();
			break;
		case 0x9D:
			absx();
			sta();
			break;
		case 0xA0:
			imm();
			ldy();
			break;
		case 0xA1:
			indx();
			lda();
			break;
		case 0xA2:
			imm();
			ldx();
			break;
		case 0xA4:
			zp();
			ldy();
			break;
		case 0xA5:
			zp();
			lda();
			break;
		case 0xA6:
			zp();
			ldx();
			break;
		case 0xA8:
			imp();
			tay();
			break;
		case 0xA9:
			imm();
			lda();
			break;
		case 0xAA:
			imp();
			tax();
			break;
		case 0xAC:
			abso();
			ldy();
			break;
		case 0xAD:
			abso();
			lda();
			break;
		case 0xAE:
			abso();
			ldx();
			break;
		case 0xB0:
			rel();
			bcs();
			break;
		case 0xB1:
			indy();
			lda();
			break;
		case 0xB4:
			zpx();
			ldy();
			break;
		case 0xB5:
			zpx();
			lda();
			break;
		case 0xB6:
			zpy();
			ldx();
			break;
		case 0xB8:
			imp();
			clv();
			break;
		case 0xB9:
			absy();
			lda();
			break;
		case 0xBA:
			imp();
			tsx();
			break;
		case 0xBC:
			absx();
			ldy();
			break;
		case 0xBD:
			absx();
			lda();
			break;
		case 0xBE:
			absy();
			ldx();
			break;
		case 0xC0:
			imm();
			cpy();
			break;
		case 0xC1:
			indx();
			cmp();
			break;
		case 0xC4:
			zp();
			cpy();
			break;
		case 0xC5:
			zp();
			cmp();
			break;
		case 0xC6:
			zp();
			dec();
			break;
		case 0xC8:
			imp();
			iny();
			break;
		case 0xC9:
			imm();
			cmp();
			break;
		case 0xCA:
			imp();
			dex();
			break;
		case 0xCC:
			abso();
			cpy();
			break;
		case 0xCD:
			abso();
			cmp();
			break;
		case 0xCE:
			abso();
			dec();
			break;
		case 0xD0:
			rel();
			bne();
			break;
		case 0xD1:
			indy();
			cmp();
			break;
		case 0xD5:
			zpx();
			cmp();
			break;
		case 0xD6:
			zpx();
			dec();
			break;
		case 0xD8:
			imp();
			cld();
			break;
		case 0xD9:
			absy();
			cmp();
			break;
		case 0xDD:
			absx();
			cmp();
			break;
		case 0xDE:
			absx();
			dec();
			break;
		case 0xE0:
			imm();
			cpx();
			break;
		case 0xE1:
			indx();
			sbc();
			break;
		case 0xE4:
			zp();
			cpx();
			break;
		case 0xE5:
			zp();
			sbc();
			break;
		case 0xE6:
			zp();
			inc();
			break;
		case 0xE8:
			imp();
			inx();
			break;
		case 0xE9:
			imm();
			sbc();
			break;
		case 0xEB:
			imm();
			sbc();
			break;
		case 0xEC:
			abso();
			cpx();
			break;
		case 0xED:
			abso();
			sbc();
			break;
		case 0xEE:
			abso();
			inc();
			break;
		case 0xF0:
			rel();
			beq();
			break;
		case 0xF1:
			indy();
			sbc();
			break;
		case 0xF5:
			zpx();
			sbc();
			break;
		case 0xF6:
			zpx();
			inc();
			break;
		case 0xF8:
			imp();
			sed();
			break;
		case 0xF9:
			absy();
			sbc();
			break;
		case 0xFD:
			absx();
			sbc();
			break;
		case 0xFE:
			absx();
			inc();
			break;
		}

#ifdef USE_TIMING
      clockgoal6502 -= (int32_t)pgm_read_byte_near(ticktable + opcode);
#endif
      instructions++;

	#ifdef DEBUGUNO
	// ----------------------------- debug trace file
	if (fpx!=NULL)
	{
		char flagName[]="CZIDB-VN";
				
		fprintf(fpx, "%4x  ", debugPC);
		for (ii=0;ii<3;ii++)
			if ((int)(pc-debugPC-ii)<0)
				fprintf(fpx, "   ");
			else
				fprintf(fpx, "%2x ", read6502(debugPC+ii));
		fprintf(fpx, "               ");
		fprintf(fpx, "A:%2x X:%2x Y:%2x F:%2x S:1%2x [", a, x, y, cpustatus, sp);
		for (ii=7;ii>=6;ii--)
			if (cpustatus & 1<<ii)
				fprintf(fpx, "%c", flagName[ii]);
			else
				fprintf(fpx, ".");
		fprintf(fpx, "-");
		for (ii=4;ii>=0;ii--)
			if (cpustatus & 1<<ii)
				fprintf(fpx, "%c", flagName[ii]);
			else
				fprintf(fpx, ".");
		fprintf(fpx, "]\n");
	}
	// ----------------------------- debug
	#endif


// part 2 of NMI single-step handling for KIM-I
	if (nmiFlag==1) //SST switch on and not in K7 area (ie, ROM002), so single step
	{	nmi6502(); // set up for NMI
		nmiFlag=0;
	}
// ----------------------------------
  }
}

uint16_t getpc() {
  return(pc);
}

uint8_t getop() {
  return(opcode);
}

/*
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}
*/

// little function to dump a ROM into a binary file
/*
void dumprom(void)
{
	FILE *ofp;

	ofp = fopen("bwoz.bin","w");

	for (int i=0;i<1106;i++)
	     fwrite(asmROM, (size_t) 1, (size_t) 1106, ofp);
		
	fclose(ofp);
}
*/
