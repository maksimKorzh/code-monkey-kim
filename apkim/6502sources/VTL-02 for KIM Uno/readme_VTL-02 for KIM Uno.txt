Notes:

Compile with the Kowalski 6502 simulator & assembler.

Recompilation process:
- compile in Kowalski simulator/assember, make sure to set a .lst output file in Options-Assembler
- Convert .lst file to C array using lst2c program (lst2c vtl02.lst kowalski > vtl02.h)
- copy & paste array into KIM UNO's cpu.c


Source changes for KIM UNO:
- set memory to 0200-03FF
- point ACIA TX and RX registers to the ones at CFFx (used by Microchess)
- remove CR/LF filter at the very end of the code

Original at:

https://github.com/Klaus2m5/VTL02

See the forum on 6502.org for a discussion of the program and its various versions

Oscar Vermeulen, 20160709/20200704
