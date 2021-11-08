# Code Monkey KIM
This is a fork of KIM UNO project by Oscar Vermeulen<br>
with a custom hardware and expanded output capabilities.

# Video demo and tutorials
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/EQc5zbSJ_vo/0.jpg)](https://www.youtube.com/watch?v=EQc5zbSJ_vo&list=PLLfIBXQeu3abUqy2XK2NiMkrRzH2722ht)

# Original project by Oscar Vermeulen
https://obsolescence.wixsite.com/obsolescence/kim-uno-summary-c1uuh<br>

# Ultimate KIM-1 / 6502 CPU guides
    http://users.telenet.be/kim1-6502/ 

# System utilities
    ----------------------------------------------------------------------------------
    0x1780 MOVIT - moves bytes from one memory location to another, including EEPROM
    ----------------------------------------------------------------------------------
    Enter original start adress of the program to move in $00D0 (LSB) and $00D1 (MSB),
    Enter original end adress of the program to move in $00D2 (LSB) and $00D3 (MSB),
    Enter new start adress of the program to move in $00D4 (LSB) and $00D5 (MSB),
    and press 1780 [GO].

    ----------------------------------------------------------------------------------
    0x0110 RELOCATE - doesn't move the data, fixes up the addresses before you make the move.
    ----------------------------------------------------------------------------------
    You'll have to give RELOCATE a lot of information about your program:
    
    1. Where your program starts. This is the first instruction in your whole program 
    (including the part that doesn't move). RELOCATE has to look through your whole program,
    instruction by instruction, correcting addresses and branches where necessary.
    Be sure your program is a continuous series of instructions (don't mix data in;
    RELOCATE will take a data value of 10 as a BEL instruction and try to correct the branch address),
    and place a dud instruction (FF) behind your last program instruction.
    This tells RELOCATE where to stop. Place the program start address in locations EA and EB, low
    order first as usual. Don't forget the FF behind the last instruction; it doesn't matter if you
    temporarily wipe out a byte of data - you can always put it back later.
    
    2. Where relocation starts, this is the first address in your program that you want to move.
    If you're moving the whole program, it will be the same as the program start address, above.
    This address is called the boundary. Place the boundary address in locations EC and ED,
    low order first.
    
    3. How far you will want to relocate information above the boundary. This value is called the
    increment. For example, if you want to open up three more locations in your program, the
    increment will be 0003. If you want to close up four addresses, the increment will be FFFC
    (effectively, a negative number). Place the increment value in locations E8 and E9, low order first.
    
    4. A page limit, above which relocation should be disabled. For example, if you're working on a
    program in the 0200 to 03FF range, your program might also address a timer or I/O registers, and
    might call subroutines in the monitor. You don't want these addresses relocated, even though they
    are above the boundary! So your page limit would be 17, since these addresses are all over 1700. On
    the other hand, if you have memory expansion and your program is at address 2000 and up, your
    page limit will need to be much higher. You'd normally set the page limit to FF, the highest page in
    memory. Place the page limit in location E7.
    
    Now you're ready to go. Set RELOCATE's start address [0110], hit [GO] - and ZAP!-your addresses are
    fixed up. After the run, it's a good idea to check the address now in 00EA and 00EB - it should point
    at the FF at 13the end of your program, confirming that the run went OK. Now you can move the program.
    If you have lots of memory to spare, you can write a general MOVE program and link it in to RELOCATE,
    so as to do the whole job in one shot. [this is why movit is included] Last note: the program
    terminates with a BRK instruction. Be sure your interrupt vector (at l7FE and 17FF) is set to KIM
    address 1C00 so that you get a valid "halt" [note: this is always done in the KIM Uno].

    ----------------------------------------------------------------------------------
    0x01A5 BRANCH - a small tool to calculate relative jumps
    ----------------------------------------------------------------------------------
    To calculate the branch value for a BEQ instruction located at $0226, needing to jump back to $0220:
    hit 26 (from 0226); 20 (from 0220) and the right offset, F8 shows up on the the display.

    ----------------------------------------------------------------------------------
    0x2000 DISASSEMBLER - lists your program to serial port
    ----------------------------------------------------------------------------------
    Store the address from which you want to start disassembling in $00 (high byte, ie 02) and $01
    (low byte, ie 00). Obviously, given that this outputs text, you will need the serial terminal
    hooked up. Then enter 2000 [GO] or, in serial TTY mode, type 2000<space>G. The first 13 lines
    of disassembly roll by. Hit G or [GO] again to continue.

    ----------------------------------------------------------------------------------
    0xFDDD MINIASM (assumes TTY mode) - 6502 assembler 
    ----------------------------------------------------------------------------------
    Type address followed by 6502 assembly e.g. 0000: LDA #$61
    
    ----------------------------------------------------------------------------------
    0xFF00 WOZMON (assumes TTY mode) - HEX monitor by Steve Wozniak (APPLE 1 ROM)
    ----------------------------------------------------------------------------------
    See Apple 1 user manual:
    https://archive.computerhistory.org/resources/text/Apple/Apple.AppleI.1976.102646518.pdf

    ----------------------------------------------------------------------------------
    0xE000 A1BASIC (assumes TTY mode) - Apple 1 BASIC interpreter
    ----------------------------------------------------------------------------------
    See Apple 1 BASIC user manual:
    https://archive.org/details/apple1_basic_manual/mode/2up?view=theater

# Controlling LCD display
    ----------------------------------------------------------------------------------
    0xD012  Echo character to LCD
    ----------------------------------------------------------------------------------
    A9 61       LDA #$61   ;  load 'a' to A register
    8D 12 D0    STA $D012  ; print 'a' to LCD

    ----------------------------------------------------------------------------------
    0xD014  Clear LCD screen
    ----------------------------------------------------------------------------------
    8D 14 D0    STA $D014  ; clear LCD screen, no matter what is in A register

    ----------------------------------------------------------------------------------
    0xD015  Show/hide LCD blinking cursor
    ----------------------------------------------------------------------------------
    A9 00       LDS #$01   ; load 0x00 to A register (use 0x00 to hide cursor)
    8D 14 D0    STA $D014  ; set 0x00 to 0xD014 to enable blinking cursor

    ----------------------------------------------------------------------------------
    0xD016  Set cursor at position (register X = col, register Y = row)
    ----------------------------------------------------------------------------------
    A2 03       LDX #$03   ; register X holds LCD cursor col = 3
    A0 00       LDY #$00   ; register Y holds LCD cursor row = 0
    8D 16 D0    STA $D016  ; or STX/STY to set cursor ar (3, 0)

    ----------------------------------------------------------------------------------
    0xD017  Create character (register X = index (0-7), register Y = 1st byte pointer)
    ----------------------------------------------------------------------------------
    A2 00       LDX #$00   ; set custom char index to 0
    A0 00       LDY #$00   ; first byte of 8 bytes pointer (RAM + Y register)
    8D 17 D0    STA $D017  ; create user character, LCD resets
    A9 00       LDA #$00   ; load custom character 0 to A register
    8D 12 D0    STA #D012  ; output custom character to screen

    ----------------------------------------------------------------------------------
    0xD018  Scroll LCD display left/right
    ----------------------------------------------------------------------------------
    A9 01       LDA #$01   ; A = 0x01 to scroll left, if A = 0x00 then scroll right
    8D 18 D0    STA $D018  ; scroll display

# TTY mode commands
    Select an Address

    Type four hex keys (0 to F) to define the desired address.
    Next, press the [SPACE] bar.
    
    The printer will respond showing the address code selected
    followed by a two digit hex code for data stored at the selected
    address location:
    
    Type:                 1234        [SPACE]
    Printer Responds:     1234        AF

    showing that the data AF is stored at location 1234.

    Modify Data

    Select an address as in the previous section.  Now type two hex
    characters to define the data to be stored at that address.  Next type
    the [.] key to authorize the modification of data at the selected address:

    Type:                  1234  [SPACE]
    Printer Responds:      1234  AF
    Type:                              6D    [.]
    Printer Responds:      1235  B7

    Note that the selected address (1234) has been modified and the system
    increments automatically to the next address (1235).

    Note:  Leading zero's need not be entered for either address
    or data fields:  For example:

    EF [SPACE] selects address 00EF
    E [SPACE] selects address 000E
    A [.] enters data 0A
    [.] enters data 00 (etc.)

# Loading and saving programs to your PC
    In serial mode (ie, Option 2, press [TAB] to go there), saving goes as follows:
    store the end address in 0x17F7 (upper byte, i.e. 02) and 0x17F8 (lower byte, i.e. FF).
    Now go to the address that is the starting address (0200 most likely).
    
    Press Q and a data file will be sent to the terminal.
    On the PC, you can then copy it, or capture it to a text file.
    For all the KIM knows, it has just created a paper tape...
    
    The command L allows you to upload a file into the KIM-1.
    Just press L, the KIM will wait for paper tape data so let your terminal program send the
    file. In Windows' HyperTerminal: Transfer->Send Text File.
    You may need to lower the speed at which the PC
    is sending bytes, HyperTerminal has a setting for that.
    A utility, kimpaper, exists to convert between binary
    files and the KIM’s ‘paper tape format’.

# Hardware changes
 - 7-segment LEDs are replaced by 16x2 LCD shield
 - custom 4x4 keypad
 - \[RS\] button is hardware arduino reset hence all the data in RAM would be lost

# Button map
 - 4x4 Keypad S16     is KIM-1 [0]
 - 4x4 Keypad S12     is KIM-1 [1]
 - 4x4 Keypad S8      is KIM-1 [2]
 - 4x4 Keypad S4      is KIM-1 [3]
 - 4x4 Keypad S15     is KIM-1 [4]
 - 4x4 Keypad S11     is KIM-1 [5]
 - 4x4 Keypad S7      is KIM-1 [6]
 - 4x4 Keypad S3      is KIM-1 [7]
 - 4x4 Keypad S14     is KIM-1 [8]
 - 4x4 Keypad S10     is KIM-1 [9]
 - 4x4 Keypad S6      is KIM-1 [A]
 - 4x4 Keypad S2      is KIM-1 [B]
 - 4x4 Keypad S13     is KIM-1 [C]
 - 4x4 Keypad S9      is KIM-1 [D]
 - 4x4 Keypad S5      is KIM-1 [E]
 - 4x4 Keypad S1      is KIM-1 [F]
 - LCD shield Select  is KIM-1 [GO]
 - LCD shield Left    is KIM-1 [AD]
 - LCD shield Right   is KIM-1 [DA]
 - LCD shield Up      is KIM-1 [+]
 - LCD shield Down    is KIM-1 [ST]
 - LCD shield Reset   is KIM-1 [RS]

# Limitations
 - No SST switch
 - No PC button
 - No 'long press' buttons to flip the modes (like in KIM UNO)
 - In a non TTY serial mode 6 digits are no longer displayed

# TODO
 - user defined characters support for LCD dispay
 - expand I/O to print to LCD display

# Circuit
Please see <a href="https://github.com/maksimKorzh/cmk-computer">CMK computer</a> project for details
