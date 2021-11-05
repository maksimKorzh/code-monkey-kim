extern "C" {

/*
** Program  : KIMtimer
** Author   : Willem Aandewiel
*    
* Coding style
*       - One True Brace Style formatting/indenting.
*       - Linux brackets, add brackets to all conditionals.
*
* Volgens The Book of KIM-1 geeft:
*   1704 := 0xFF een delay van 0.000255 seconden,   0.255 mSec,   255 uSec
*   1705 := 0xFF een delay van 0.00204  seconden,   2.04 mSec,   2040 uSec
*   1706 := 0xFF een delay van 0.01632  seconden,  16.32 mSec,  16320 uSec
*   1707 := 0xFF een delay van 0.26112  seconden, 261.12 mSec, 261120 uSec
*/
#if _TARGET == ARDUINO_UNO || _TARGET == ARDUINO_MEGA || _TARGET == ARDUINO_MINI
    #define CORRECTION  200
#elif _TARGET == ARDUINO_DUE || _TARGET == CORE_TEENSY
    #define CORRECTION   50
#elif _TARGET == ESP32_R
    #define CORRECTION   50
#elif _TARGET == BLUEPILL_STM
    #define CORRECTION   50
#endif

#define DEBUG false
#if DEBUG == false 
    #define Sbegin(...)   
    #define Sprint(...)   
    #define Sprintln(...) 
    #define Sflush(...)    
#else
    #define Sbegin(...)     {SerialX.begin(__VA_ARGS__); }
    #define Sprint(...)     {SerialX.print(__VA_ARGS__); }
    #define Sprintln(...)   {SerialX.println(__VA_ARGS__); }
    #define Sflush(...)     {SerialX.flush(__VA_ARGS__); }
#endif



void timerCheck();
void riotTimerWrite(uint16_t address, uint16_t setVal);
uint8_t riotTimerRead(uint16_t address);


//uint16_t    tim1706, tim1707;
uint16_t    tim1707;
uint32_t    tim1706, timerDiv;




  
// --- timerCheck() ---
void timerCheck()
{
    Sprint("timerCheck(): tim1706: ");
    Sprint(tim1706);
    if (tim1706 <= micros()) { 
        Sprint(", micros(): [");
        Sprint(micros());
        Sprintln("] .. Done");
        tim1707  = 0x80;
        timerDiv  = 1;
        return;
    }
    Sprint(", micros(): [");
    Sprint(micros());
    Sprintln("] .. continue");

} // timerCheck()


// --- riotTimerWrite() ---
void riotTimerWrite(uint16_t address, uint16_t setVal)
{
    if ((setVal <= 0) || (setVal > 0xFF)) {
        Sprintln("riotTimerWrite(): Timer set to zero! Abort");
        tim1707 = 0x80;
        tim1706 = 0;
        timerDiv = 1;
        return;
    }
    switch(address) {
    case 0x1704:
        timerDiv = 1 ; //1
        break;
    case 0x1705:
        timerDiv = 8; //8
        break;
    case 0x1706:
        timerDiv = 64; //64
        break;
    case 0x1707:
        timerDiv = 1024; // 1024
        break;
    default:
        timerDiv = 1024;
    }
    Sprint("riotTimerWrite(): timerDiv: "); Sprint(timerDiv);
    tim1706 = (setVal * timerDiv) + micros() - CORRECTION;
    Sprint(", set tim1706 to ["); Sprint(setVal);
    Sprint(" / "); Sprint(tim1706); Sprintln("]");
    tim1707 = 0;

} // riotTimerWrite()

// --- riotTimerRead ---
uint8_t riotTimerRead(uint16_t address)
{
    uint8_t rVal;

    Sprint("riotTimerRead(): tim1707[");
    Sprint(tim1707); Sprint("]");
    if (tim1707 != 0x80) timerCheck();

    switch (address) {
    case 0x1707:
        Sprint(", return tim1707["); Sprint(tim1707); Sprintln("]");
        return tim1707;
        break;
    default:
        rVal  = random(0,0xFF);
        //rVal |= B10000000;
        Sprintln(", return [0]");
        return rVal;
    }

} // riotTimerRead()



} // C
