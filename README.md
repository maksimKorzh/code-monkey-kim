# Code Monkey KIM
This is a fork of KIM UNO project by Oscar Vermeulen<br>
with a custom hardware and expanded output capabilities.

# Video demo and tutorials
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/EQc5zbSJ_vo/0.jpg)](https://www.youtube.com/watch?v=EQc5zbSJ_vo&list=PLLfIBXQeu3abUqy2XK2NiMkrRzH2722ht)

# Original project by Oscar Vermeulen
https://obsolescence.wixsite.com/obsolescence/kim-uno-summary-c1uuh<br>

# Ultimate KIM-1 / 6502 CPU guides
    http://users.telenet.be/kim1-6502/

# Memory map
 - 0x1780 - 0x17C0 (64 bytes) MOVIT utility

# System utilities
    MOVIT - moves bytes from one memory location to another, including EEPROM<br>
    ----------------------------------------------------------------------------------<br>
    Enter original start adress of the program to move in $00D0 (LSB) and $00D1 (MSB),<br>
    Enter original end adress of the program to move in $00D2 (LSB) and $00D3 (MSB),<br>
    Enter new start adress of the program to move in $00D4 (LSB) and $00D5 (MSB),<br>
    and press 1780 [GO].
    
    RELOCATE - doesn't move the data, fixes up the addresses before you make the move.<br>
    ----------------------------------------------------------------------------------<br>
    You'll have to give RELOCATE a lot of information about your program:
    1. Where your program starts. This is the first instruction in your whole program (including the part
    that doesn't move). RELOCATE has to look through your whole program, instruction by instruction,
    correcting addresses and branches where necessary. Be sure your program is a continuous series of
    instructions (don't mix data in; RELOCATE will take a data value of 10 as a BEL instruction and try to
    correct the branch address), and place a dud instruction (FF) behind your last program instruction.
    This tells RELOCATE where to stop. Place the program start address in locations EA and EB, low
    order first as usual. Don't forget the FF behind the last instruction; it doesn't matter if you
    temporarily wipe out a byte of data - you can always put it back later.
    2. Where relocation starts, this is the first address in your program that you want to move. If you're
    moving the whole program, it will be the same as the program start address, above. This address is
    called the boundary. Place the boundary address in locations EC and ED, low order first.
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
    Now you're ready to go. Set RELOCATE's start address [0110], hit [GO] - and ZAP!-your addresses are fixed
    up. After the run, it's a good idea to check the address now in 00EA and 00EB - it should point at the FF at
    13the end of your program, confirming that the run went OK. Now you can move the program. If you have lots
    of memory to spare, you can write a general MOVE program and link it in to RELOCATE, so as to do the
    whole job in one shot. [this is why movit is included] Last note: the program terminates with a BRK
    instruction. Be sure your interrupt vector (at l7FE and 17FF) is set to KIM address 1C00 so that you get a valid
    "halt" [note: this is always done in the KIM Uno].

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
