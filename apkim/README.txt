2020 update of the KIM Uno
==========================

This is a KIM-1 and Apple-1 simulator, compatible with both at the same time. Lots of 6502 software gems built in to the memory map. It also is a pretty competent 6502 programmable calculator, see the web site.

Runs on 
- Arduino Pro Mini and (untested) likely, most Arduinos
- STM32 Blue Pill
- ESP32 and TTGO Watch 2020

The Pro Mini does not have enough Flash or RAM to grow the functionality much more (12 bytes of Flash to add code into at last count), so it will roughly stay what it is. The STM32 and esp32 have massive room for adding features; this release is simply the first one to support them.


You can either use any of these microcontrollers 'naked', or plug them into the KIM Uno PCB to have the physical KIM-1 keypad and LEDs. If used without the PCB, the serial terminal will switch between a simulation of the KIM-1 keypad/LEDs and the regular KIM serial terminal mode (using the TAB key as the switch).

A Linux PC version is in the pcuno directory. The code is a mess, it's just my private testing ground. It contains a modestly fancy 6502 debugger. Breakpoints have to be set in the source code (I mentioned modestly...) in pcuno.c-->loop().
Note you WILL need to set the compile target in kimuno/config.h before running the Makefile though!
The PC version uses the ncurses library; this will install that on Linux: sudo apt-get install libcdk5-dev


All of this is work in progress. But the Arduino version should be stable.

More information, including a PDF manual:
https://obsolescence.wixsite.com/obsolescence/kim-uno-summary-c1uuh


Source code
===========

- kimuno directory: ***it's all you need*** to flash an Arduino/esp32/STM32. Edit config.h to choose the platform, and to select which software is included. Only the Arduino is too small to add any beyond the default, but if you sacrifice the Calculator ROM there will be plenty of space. 

- pcuno directory: the PC version. As mentioned, this is not meant to be anything else than my testing ground for new things. But it does a good job of being a KIM-1/Apple-1 emulator.

- kimwatch: a quick hack to get the KIM Uno code running on a $25 smartwatch, the TT-GO 2020. I don't think I will continue with this, it was just a fun sidestep. But as this $25 watch contains an esp32, there's massive scope for added features. Like a swipe-to-right serial terminal. Who knows.

- 6502 sources: here you'll find all the source code with compile instructions. They were used to generate the (human and computer-readable!) assembler lists in kimuno/roms.h


Settings in Arduino IDE for programming:
========================================
- standard Arduino Pro Mini: the regular 5v/16Mhz Arduino Mini/Pro Mini option

- STM32: Board: Generic ST32MF1 Series, Board Part Number: STM32F103C8 (or CB if you have a recent one, does not matter), U(S)ART Support: Enabled (Generic Serial), USB Support (if available): CDC (Generic Serial Supercede). 
You can use different options, it's just my setup. See kimuno.ino, where you can assign a different serial port (Serial3) if you want. I use the ST-Link programmer v2. The USB cable is the serial port by default. 

- esp32: I use the Feather32, but it should not matter. I tried with other boards as well.

KIM Uno PCB
===========
To wire up the STM32 and esp32 to the KIM  Uno PCB, see the pin assignments defined in kimuno.ino. 

But of course, you do not really need the PCB to use the KIM Uno software. And I'll make new PCBs relatively soon if you do not feel like soldering many wires from MCU to PCB.



