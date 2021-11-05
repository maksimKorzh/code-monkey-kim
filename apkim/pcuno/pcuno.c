//version date: 20200608
#include <stdint.h>
#include <stdio.h>
#include <ncurses.h>
#include "dcc6502.h"
#include "../kimuno/config.h"	// calculator functions (not on Linux)
#include "../kimuno/kimuno.h"   // RIOT simulation (not on Linux - no LEDs...)

// ---------- in kimuno.c ------------------------
void main(void);
void setup(void);
void interpretkeys(void);
void initKIM(void);
void dbgout(void);
void dbgtxt(uint8_t val);
void serout(uint8_t val);
void errout(uint8_t val); 
void serouthex(uint8_t val);
uint8_t getAkey(void);
void clearkey(void); 
void helpscreen(void);
/*
uint8_t tapeSave(uint8_t ID, uint8_t SAL,  uint8_t SAH,  uint8_t EAL,  uint8_t EAH,  uint8_t AppleMode);
uint8_t tapeLoad(uint8_t ID, uint8_t xSAL, uint8_t xSAH, uint8_t xEAL, uint8_t xEAH, uint8_t AppleMode);
void    tapeDirectory(void);
void    tapeDelete(uint8_t ID);
void    tapeInit(void);
*/
int fileSelect(char *papertapeFile);

// ---------- referenced from cpu.c ------------------------------
extern void exec6502(int32_t tickcount);
extern void reset6502();
extern void nmi6502();
extern int8_t read6502(uint16_t address);
extern void write6502(uint16_t address, uint8_t value);

extern void loadTestProgram(void);
//  extern void dumprom(void);	// just a quick hack, delete

extern uint8_t RAM[1024]; 		// main 1KB RAM		 0x000-0x3FF
extern uint16_t pc;  			// 6502 pc
extern uint8_t sp, a, x, y, cpustatus;
extern uint8_t SSTmode;
extern uint8_t useKeyboardLed; // 0 to use Serial port or 1 for HEX digits.

// - status variables etc ----------------------------------------
int AppleTapeCounter=0; // KIM-1 uses an ID, Apple-1 does not.
                        // This acts to give Apple-1 saves a file ID
int AppleColCounter=0;  // max 40 columns for the Apple-1

uint8_t curkey = 0;
uint8_t eepromProtect=1;  // default is to write-protect EEPROM
int blitzMode=1;  // microchess variable. 1 speeds up moves (and dumbs down play)
uint8_t keyboardMode=0;  // start with keyboard in KIM-1 mode. 1: calculator mode
char threeHex[3][2];        // LED display
int dbgmode=0;
char disasmstr[512];		// text buffer for disassembler output
uint8_t disasmbuffer[4];	// buffer for binary code going into disassembler
WINDOW *hex_win, *tty_win, *dbg_win;
char papertapeFile[256];    // filename for papertape reader/punch
FILE    *ptrFP=NULL;        // paper tape reader file pointer

void setup (void) {
  reset6502();
  initKIM(); 		// Enters 1c00 in KIM vectors 17FA and 17FE. 
  			// Might consider doing 17FC as well????????
  loadTestProgram();
}

void main (void) 
{
	setup();
	int c;
	int n=1,cx;

	// ncurses stuff
        initscr();
        start_color();
	scrollok(stdscr,TRUE);
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        init_pair(4, COLOR_BLACK, COLOR_GREEN);
        bkgd(COLOR_PAIR(1));
        //printw("Kim Uno on Linux/ncurses");
        refresh();
        noecho();
        nodelay(stdscr, TRUE);
        cbreak();       /* Line buffering disabled. pass on everything */
	
	hex_win = newwin(3,25,21,81);
	//?wattrset(hex_win, A_REVERSE);
	wbkgd(hex_win, COLOR_PAIR(3)); 
	wrefresh(hex_win);

	tty_win = newwin(24,80,0,0);
	scrollok(tty_win,TRUE);
	wbkgd(tty_win, COLOR_PAIR(2)); 
	wrefresh(tty_win);

	dbg_win = newwin(20,25,1,81);
	scrollok(dbg_win,TRUE);
	wbkgd(dbg_win, COLOR_PAIR(4)); 
	wrefresh(dbg_win);

	// hardcoded breakpoint option:
	uint16_t breakpoint = 0xC3A8;
	mvprintw(0,81, "breakpoint @ %0X", breakpoint);
	wrefresh(hex_win);

	do {
		c = getch();

		if (c!=ERR) {
			if (c==10)  c=13;	// why does ncurses not do 13 for Return?!

			if (c==KEY_F(3)) 
            {   dbgout();   dbgmode=1;  }
			else if (c==KEY_F(4))
				dbgmode=0;
			else if (c==KEY_F(9))
				keyboardMode=0;		// regular serial+keypad emu
			else if (c==KEY_F(10))
				keyboardMode=1;		// for VTL-02: ASCII w/o special funcs
            else if (c==KEY_F(7))
            {
                do
                {   wprintw(dbg_win, "\nSet tape # for Apple-1:");
	                wrefresh(dbg_win);
                    wscanw(dbg_win, "%d", &AppleTapeCounter);
                } while (AppleTapeCounter<1 || AppleTapeCounter>255);
                wprintw(dbg_win, "Tape counter set to %02x\n", AppleTapeCounter);
	            wrefresh(dbg_win);
            }
            else if (c==KEY_F(8))
            {
                wprintw(dbg_win, "\nDelete %02x, sure?\n", read6502(0x17F9));
	            wrefresh(dbg_win);
                while((c=getch())==ERR)
                {}
                waddch(dbg_win, c);
	            wrefresh(dbg_win);
                if (c=='Y' || c=='y')
                    tapeDelete(read6502(0x17F9)); //ID
            }
            else if (c==KEY_F(5))
                tapeDirectory();
			else if (c==KEY_F(1))
			{   helpscreen();
                redrawwin(tty_win);
                wnoutrefresh(tty_win);
                redrawwin(dbg_win);
                wnoutrefresh(dbg_win);
                redrawwin(hex_win);
                wnoutrefresh(hex_win);
                refresh();
                c=ERR;  
            }
			else if (c==(KEY_F(6)+12)) // Shift-F6
			{
                //char **argv=NULL;	
                fileSelect(papertapeFile);
                if(papertapeFile[0]!='\0')
                {
                    ptrFP=fopen(papertapeFile,"r");
                    if (ptrFP==NULL) //{ printf("papertape read open error\r\n");  exit(1); }
                    {   errout('P');    errout('R');    errout('E');    }
                }    
                redrawwin(tty_win);                wnoutrefresh(tty_win);
                redrawwin(dbg_win);                wnoutrefresh(dbg_win);
                redrawwin(hex_win);                wnoutrefresh(hex_win);
                refresh();
                c=ERR;  
    }
/*			else if (c==(KEY_F(7)+12)) // Shift-F7
			{   helpscreen();
    char **argv=NULL;	
    fileSelect(0, argv);
    redrawwin(tty_win);
    wnoutrefresh(tty_win);
    redrawwin(dbg_win);
    wnoutrefresh(dbg_win);
    redrawwin(hex_win);
    wnoutrefresh(hex_win);
    refresh();
                c=ERR;  
            }
*/
            else 
            {   curkey = c & 0x7F;
                interpretkeys();
            }
        }
        else // c==ERR
        {
            if ((ptrFP!=NULL) && (curkey==0))
            {   c = fgetc(ptrFP); // simply insert character from file into input
                curkey = c & 0x7F;
                interpretkeys();
                if (feof(ptrFP))  { fclose(ptrFP); ptrFP=NULL;  }
            }
        }

		if (pc==breakpoint) {
			dbgout();
			dbgmode=1;
		}

		if ((dbgmode==0) || (c==KEY_F(3)))
			exec6502(n); //do 1 6502 instruction

	  } while (c!=KEY_F(2));
	
	printw ("Exiting...");
	endwin();
}


// ======== I/O, called from cpu.c ==============================================
//
  void serout(uint8_t val)    { 
    
    if (val=='\r')  val = '\n'; 
    if (val=='\n')
    {    AppleColCounter=0;   }
    if(AppleColCounter++>39)
    {    AppleColCounter=0;         serout('\n');   }
    
if ((val!='\r')&& (val!=0x00)) {
	 waddch(tty_win, val);

	 wrefresh(tty_win);
		}
	}
  void errout(uint8_t val)    { 
	  waddch(dbg_win, val);		// useful for chasing illegal memory access
	  wrefresh(dbg_win);		// see errout in cpu.c
	  refresh();
	}
  void dbgout(void) {
	  wprintw(dbg_win, "P%0004x S%02x A%02x X%02x Y%02x C%02x\n", pc,sp,a,x,y,cpustatus);

	  uint16_t pcx = pc;
	  for (int i=0;i<3;i++) disasmbuffer[i]=read6502(pc+i); 
	  disassemble (disasmstr, disasmbuffer, &pcx);
	  wprintw(dbg_win, "%s\n\n", disasmstr);
	  wrefresh(dbg_win);
	  refresh();
	}

  void dbgtxt(uint8_t val) {
	  wprintw(dbg_win, "[[[%0x]]]\n\n", val);
	  wrefresh(dbg_win);
	  refresh();
	}

  void serouthex(uint8_t val) { 
	  mvwprintw(hex_win, 1,1, "|%02x%02x| |%02x|", RAM[0x00FB], RAM[0x00FA], RAM[0x00F9]); 
	  wrefresh(hex_win);
	  refresh();
  }
  uint8_t getAkey(void)            
{ 
	  return(curkey);  }
  void clearkey(void)             { curkey = 0; }


// ==== interpretkeys =============================================================
// intercept some special keys to emulator actions
//   keyboardMode 0: normal. 1: calculator mode. 2: VTL-02 mode. 
//
void interpretkeys(void)
{    
  // 1. keys that always have the same meaning
  switch (curkey) {
    case 18:  // CtrlR = RS key = hardware reset (RST)
      reset6502(); clearkey(); mvprintw(0,81,"RSet                "); break;
    case 20: // CtrlT = ST key = throw an NMI to stop execution of user program
      nmi6502(); clearkey(); mvprintw(0,81,"STop                  "); break;
  }

  // 2. keys that always have the same meaning but are disabled in VTL-02
  if(keyboardMode!=2)
  {
    switch(curkey) {
      case '[': // SST off
        SSTmode = 0; clearkey();
        mvprintw(0,81,"SST OFF         ");
        break;
      case ']': // SST on
        SSTmode = 1; clearkey();
        mvprintw(0,81,"SST ON          "); 
        break;
      case 9: // TAB pressed, toggle between serial port and onboard keyboard/display
        if (useKeyboardLed==0) 
        { useKeyboardLed=1;    mvprintw(0,81,"Keypad/LED Mode      "); }
        else 
        { useKeyboardLed=0;    mvprintw(0,81,"Serial Terminal Mode ");}
        reset6502();  clearkey();  break;
      case '>': // Toggle write protect on eeprom
        if (eepromProtect==0) {
          eepromProtect = 1; 
          mvprintw(0,81,"Eeprom R/O     "); 
        } else {
          eepromProtect = 0;
          mvprintw(0,81,"Eeprom R/W     "); 
        }
        clearkey(); break;
    }
  }
/*  
  // 3. keys in Calculator Mode
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
*/
}
  

// ============ getKIMkey() =======================================
// translates ASCII keypresses to codes the KIM ROM expects.
// Used in cpu.c (but here due to Arduino heritage)
//
uint8_t getKIMkey(void) 
{
    if (curkey==0)
      return (0xFF);	//0xFF: illegal keycode 

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
 

// ========= functions to handle Calculator mode. ==============
// Not in PC version, only Arduino version...
//
uint8_t enterflt(uint8_t reg)		// result code -1 = cancel, 0 = good
{	printf("you should not be in enterflt!\r\n");
	return (curkey); 
}
uint8_t showflt(uint8_t reg)	// returns location of decimal point in string
{	printf("you should not be in showflt!\r\n");
	return 0;				
}
uint8_t enteroperation(void)		// result code -1 = cancel, 0 = good
{	printf("you should not be in enteroperation!\r\n");
      return(RAM[0x00F3]);
}

// ==============================================================
// TAPE STORAGE
// ==============================================================
#include <dirent.h>
#include <string.h>
// KIM-1 addresses:
// SAL 17F5   SAH 17F6
// EAL 17F7   EAH 17F8
// ID  17F9

uint8_t tapeSave(uint8_t ID, uint8_t SAL, uint8_t SAH, uint8_t EAL, uint8_t EAH, uint8_t AppleMode)
{
    FILE    *fp;
    char    filnam[18];
    uint8_t buffer; 

    if(AppleMode==1)
        ID=(uint8_t) AppleTapeCounter;    // for Apple-1 tapes which are raw binaries without header

    if (ID<0x01 || ID>0xFE)          // not allowed on KIM-1
    {   errout('T');    errout('W');    errout('E');    return(0);  }

    sprintf(filnam, "tape/%02x", ID);
    fp=fopen(filnam,"w");
    if (fp==NULL)
    {   errout('T');    errout('W');    errout('e');    return(0);  }
    // write file header, 5 bytes
    if (AppleMode==0)       // write file header, 5 bytes
    {
        fwrite(&ID, sizeof(uint8_t),1,fp);
        fwrite(&SAL,sizeof(uint8_t),1,fp);        fwrite(&SAH,sizeof(uint8_t),1,fp);
        fwrite(&EAL,sizeof(uint8_t),1,fp);        fwrite(&EAH,sizeof(uint8_t),1,fp);
    }
    // write data, note saving ends with the byte BEFORE EndAddress!
    for( uint16_t i=((SAH<<8)+SAL); i<((EAH<<8)+EAL)+(AppleMode?1:0); i++)
    {   buffer = read6502(i);
        fwrite(&buffer,sizeof(uint8_t),1,fp);
    }
    fclose(fp);
    return(1);
}
uint8_t tapeLoad(uint8_t ID, uint8_t xSAL, uint8_t xSAH, uint8_t xEAL, uint8_t xEAH, uint8_t AppleMode)
{
    // NOTE! At present, SAL and SAH are loaded from file, so the given SAL/SAH are ignored
    // but might be useful if you want a more flexible 'load at stated address' version.

    FILE    *fp;
    char    filnam[18];
    int     x;
    uint8_t buffer; 
    uint8_t SAL, SAH, EAL, EAH;

    if(AppleMode==1)
        ID=(uint8_t) AppleTapeCounter;    // for Apple-1 tapes which are raw binaries without header
    
    sprintf(filnam, "tape/%02x", ID);
    fp=fopen(filnam,"r");
    if (fp==NULL) //{ printf("save file open error\r\n");  exit(1); }
    {   errout('T');    errout('R');    errout('E');    return(0);  }
    if (AppleMode==0)       // read file header, 5 bytes
    {
        x=fread(&ID,sizeof(uint8_t),1,fp);
        x+=fread(&SAL,sizeof(uint8_t),1,fp);        x+=fread(&SAH,sizeof(uint8_t),1,fp);
        x+=fread(&EAL,sizeof(uint8_t),1,fp);        x+=fread(&EAH,sizeof(uint8_t),1,fp);

        for( uint16_t i=((SAH<<8)+SAL); i<((EAH<<8)+EAL); i++)
        {   x=fread(&buffer,sizeof(uint8_t),1,fp);
            write6502(i, buffer);
        }
    }
    else        // Apple-1, raw binary file without header
    {
        SAL=xSAL;   SAH=xSAH;   EAL=xEAL;   EAH=xEAH;  

        for( uint16_t i=((SAH<<8)+SAL); i<=((EAH<<8)+EAL); i++) // note <=, not <!
        {   x=fread(&buffer,sizeof(uint8_t),1,fp);
            if (x==1)
                write6502(i, buffer);
            else
                break;      // so read till end of tape or till EndAddress, whichever comes 1st
        }
    }
    // read data, note loading ends with the byte BEFORE EndAddress!
    fclose(fp);
    return(1);
}
void    tapeDirectory(void)
{
    DIR     *d;
    struct dirent *dir;
    uint8_t i;

    d = opendir("tape/.");
    errout('\n');
    if (d)
    {   while ((dir = readdir(d)) != NULL)
        {   if((strcmp(dir->d_name,".")==0||strcmp(dir->d_name,"..")==0||(*dir->d_name) == '.' ))
            {   }
            else
            {    //printf("%s\n", dir->d_name);
                errout(' ');
                for (i=0;i<strlen(dir->d_name);i++)
                    errout(dir->d_name[i]);
                errout(' ');
            }
        }
        /*serout('\r');*/   errout ('\n');
        closedir(d);
    }
}
void    tapeDelete(uint8_t ID)
{
    char    filnam[18];

    sprintf(filnam, "tape/%02x", ID);
    errout('T'); errout('D'); // printf("deleting file %s\r\n", filnam);
    remove(filnam);

}
void    tapeInit(void)
{   // no need on PC version.
}

void    helpscreen(void)
{
    char c;
    FILE    *fp;


	WINDOW *hlp_win = newwin(20,76,2,2);
	scrollok(hlp_win,TRUE);
	wbkgd(hlp_win, COLOR_PAIR(4)); 
	wrefresh(hlp_win);
	
    fp=fopen("help.txt","r");
    c = fgetc(fp);
    while (c != EOF)
    {
	    waddch(hlp_win, c);		// useful for chasing illegal memory access
        c = fgetc(fp);
    }
	wrefresh(hlp_win);

    while((c=wgetch(hlp_win))==ERR)
    {}
	delwin(hlp_win);
    redrawwin(tty_win);
    wnoutrefresh(tty_win);
    refresh();
} 


void write1740(void){}
void write1741(void){}
void write1742(void){}
void write1743(void){}

