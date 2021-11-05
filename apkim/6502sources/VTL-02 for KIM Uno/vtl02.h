 /* ; */
 /* ; In the Kowalski 6502 simulator some of the options */
 /* ; below must be set manually. */
 /* ; */
 /* ;    .lf  vtl02ca2.lst  (set listfile in menu: */
 /* ;                        Simulator->Options->Assembler) */
 /* ;    .cr  6502 */
 /*     .opt  Proc6502 */
 /* ; */
 /* ; to run with I/O set terminal active: */
 /* ;    Menu                     or   Key */
 /* ;    Simulator->Options->Simulator [Ctrl-E] */
 /* ;    Simulator->Assemble           [F7] */
 /* ;    Simulator->Debugger           [F6] */
 /* ;    Simulator->Run                [F5] */
 /* ;    View->Input/output            [Alt-5] */
 /* ; */
 /* ;    .tf  vtl02ca2.obj,ap1  (optional save output to */
 /* ;                             file: File->Save Code) */
 /* ;-----------------------------------------------------; */
 /* ;             VTL-2 for the 6502 (VTL02C)             ; */
 /* ;           Original Altair 680b version by           ; */
 /* ;          Frank McCoy and Gary Shannon 1977          ; */
 /* ;    2012: Adapted to the 6502 by Michael T. Barry    ; */
 /* ; Thanks to sbprojects.com for a very nice assembler! ; */
 /* ;-----------------------------------------------------; */
 /* ;        Copyright (c) 2012, Michael T. Barry */
 /* ;       Revision B (c) 2015, Michael T. Barry */
 /* ;       Revision C (c) 2015, Michael T. Barry */
 /* ;               All rights reserved. */
 /* ; */
 /* ; Redistribution and use in source and binary forms, */
 /* ;   with or without modification, are permitted, */
 /* ;   provided that the following conditions are met: */
 /* ; */
 /* ; 1. Redistributions of source code must retain the */
 /* ;    above copyright notice, this list of conditions */
 /* ;    and the following disclaimer. */
 /* ; 2. Redistributions in binary form must reproduce the */
 /* ;    above copyright notice, this list of conditions */
 /* ;    and the following disclaimer in the documentation */
 /* ;    and/or other materials provided with the */
 /* ;    distribution. */
 /* ; */
 /* ; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS */
 /* ; AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED */
 /* ; WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE */
 /* ; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS */
 /* ; FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT */
 /* ; SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE */
 /* ; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, */
 /* ; EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT */
 /* ; NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR */
 /* ; SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS */
 /* ; INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF */
 /* ; LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, */
 /* ; OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING */
 /* ; IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF */
 /* ; ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
 /* ;-----------------------------------------------------; */
 /* ; Except for the differences discussed below, VTL02 was */
 /* ;   designed to duplicate the OFFICIALLY DOCUMENTED */
 /* ;   behavior of Frank's 680b version, detailed here: */
 /* ;     http://www.altair680kit.com/manuals/Altair_ */
 /* ;     680-VTL-2%20Manual-05-Beta_1-Searchable.pdf */
 /* ;   These versions ignore all syntax errors and plow */
 /* ;   through VTL-2 programs with the assumption that */
 /* ;   they are "correct", but in their own unique ways, */
 /* ;   so any claims of compatibility are null and void */
 /* ;   for VTL-2 code brave (or stupid) enough to stray */
 /* ;   from the beaten path. */
 /* ; */
 /* ; Differences between the 680b and 6502 versions: */
 /* ; * {&} and {*} are initialized on entry. */
 /* ; * Division by zero returns 65535 for the quotient and */
 /* ;     the dividend for the remainder (the original 6800 */
 /* ;     version froze). */
 /* ; * The 6502 has NO 16-bit registers (other than PC) */
 /* ;     and less overall register space than the 6800, */
 /* ;     so the interpreter reserves some obscure VTL02C */
 /* ;     variables {@ $ ( ) 0 1 2 3 4 5 6 7 8 9 < > : ?} */
 /* ;     for its internal use (the 680b version used a */
 /* ;     similar tactic, but differed in the details). */
 /* ;     The deep nesting of parentheses also puts {; < =} */
 /* ;     in danger of corruption.  For example, executing */
 /* ;     the statement A=((((((((1)))))))) sets both {A} */
 /* ;     and {;} to the value 1. */
 /* ; * Users wishing to call a machine language subroutine */
 /* ;     via the system variable {>} must first set the */
 /* ;     system variable {"} to the proper address vector */
 /* ;     (for example, "=768). */
 /* ; * The x register is used to point to a simple VTL02C */
 /* ;     variable (it can't point explicitly to an array */
 /* ;     element like the 680b version because it's only */
 /* ;     8-bits).  In the comments, var[x] refers to the */
 /* ;     16-bit contents of the zero-page variable pointed */
 /* ;     to by register x (residing at addresses x, x+1). */
 /* ; * The y register is used as a pointer offset inside */
 /* ;     a VTL02C statement (easily handling the maximum */
 /* ;     statement length of about 128 bytes).  In the */
 /* ;     comments, @[y] refers to the 16-bit address */
 /* ;     formed by adding register y to the value in {@}. */
 /* ; * The structure and flow of this interpreter are */
 /* ;     similar to the 680b version, but have been */
 /* ;     reorganized in a more 6502-friendly format (the */
 /* ;     6502 has no 'bsr' instruction, so the 'stuffing' */
 /* ;     of subroutines within 128 bytes of the caller is */
 /* ;     only advantageous for conditional branches). */
 /* ; * This version is based on the original port, which */
 /* ;     was wound rather tightly, in a failed attempt to */
 /* ;     fit it into 768 bytes like the 680b version; many */
 /* ;     structured programming principles were sacrificed */
 /* ;     in that effort.  The 6502 simply requires more */
 /* ;     instructions than the 6800 does to manipulate 16- */
 /* ;     bit quantities, but the overall execution speed */
 /* ;     should be comparable due to the 6502's slightly */
 /* ;     lower average clocks/instruction ratio.  As it is */
 /* ;     now, it fits into 1KB with just a few bytes to */
 /* ;     spare, but is more feature-laden than the 680b */
 /* ;     interpreter whence it came.  Beginning with */
 /* ;     Revision C, I tried to strike a tasteful balance */
 /* ;     between execution speed and code size, but I */
 /* ;     stubbornly kept it under 1024 ROMable bytes and */
 /* ;     used only documented op-codes that were supported */
 /* ;     by the original NMOS 6502 (without the ROR bug). */
 /* ;     I may have missed a few optimizations -- further */
 /* ;     suggestions are welcome. */
 /* ; * VTL02C is my free gift (?) to the world.  It may be */
 /* ;     freely copied, shared, and/or modified by anyone */
 /* ;     interested in doing so, with only the stipulation */
 /* ;     that any liabilities arising from its use are */
 /* ;     limited to the price of VTL02C (nothing). */
 /* ;-----------------------------------------------------; */
 /* ; 2015: Revision B included some space optimizations */
 /* ;         (suggested by dclxvi) and enhancements */
 /* ;         (suggested by mkl0815 and Klaus2m5): */
 /* ; */
 /* ; * Bit-wise operators & | ^ (and, or, xor) */
 /* ;   Example:  A=$|128) Get a char and set hi-bit */
 /* ; */
 /* ; * Absolute addressed 8-bit memory load and store */
 /* ;   via the {< @} facility: */
 /* ;   Example:  <=P) Point to the I/O port at P */
 /* ;             @=@&254^128) Clear low-bit & flip hi-bit */
 /* ; */
 /* ; * Starting with VTL02B, the space character is no */
 /* ;     longer a valid user variable nor a "valid" binary */
 /* ;     operator.  It's now only significant as a numeric */
 /* ;     constant terminator and as a place-holder in */
 /* ;     strings and program listings, where it may be */
 /* ;     used to improve human readability (at a slight */
 /* ;     cost in execution speed and memory consumption). */
 /* ;   Example: */
 /* ;   *              (VTL-2) */
 /* ;       1000 A=1)         Init loop index */
 /* ;       1010 ?=A)           Print index */
 /* ;       1020 ?="")          Newline */
 /* ;       1030 A=A+1)         Update index */
 /* ;       1040 #=A<10*1010) Loop until done */
 /* ; */
 /* ;   *              (VTL02B) */
 /* ;       1000 A = 1             ) Init loop index */
 /* ;       1010     ? = A         )   Print index */
 /* ;       1020     ? = ""        )   Newline */
 /* ;       1030     A = A + 1     )   Update index */
 /* ;       1040 # = A < 10 * 1010 ) Loop until done */
 /* ; */
 /* ; 2015: Revision C includes further enhancements */
 /* ;   (suggested by Klaus2m5): */
 /* ; */
 /* ; * "THEN" and "ELSE" operators [ ] */
 /* ;     A[B returns 0 if A is 0, otherwise returns B. */
 /* ;     A]B returns B if A is 0, otherwise returns 0. */
 /* ; */
 /* ; * Some effort was made to balance interpreter code */
 /* ;     density with interpreter performance, while */
 /* ;     remaining within the 1KB constraint.  Structured */
 /* ;     programming principles remained at low priority. */
 /* ;-----------------------------------------------------; */
 /* ; VTL02C variables occupy RAM addresses $0080 to $00ff, */
 /* ;   and are little-endian, in the 6502 tradition. */
 /* ; The use of lower-case and some control characters for */
 /* ;   variable names is allowed, but not recommended; any */
 /* ;   attempts to do so would likely result in chaos, due */
 /* ;   to aliasing with upper-case and system variables. */
 /* ; Variables tagged with an asterisk are used internally */
 /* ;   by the interpreter and may change without warning. */
 /* ;   {@ $ ( ) 0..9 : > ?} are (usually) intercepted by */
 /* ;   the interpreter, so their internal use by VTL02C is */
 /* ;   "safe".  The same cannot be said for {; < =}, so be */
 /* ;   careful! */
 /*   0080          at       = $80      ; {@}* internal pointer / mem byte */
 /* ; VTL02C standard user variable space */
 /* ;                     {A B C .. X Y Z [ \ ] ^ _} */
 /* ; VTL02C system variable space */
 /*   00C0          space    = $c0      ; { }  Starting with VTL02B:  the */
 /* ;                       space character is no longer a */
 /* ;                       valid user variable nor a */
 /* ;                       "valid" binary operator. */
 /* ;                       It is now only significant as a */
 /* ;                       numeric constant terminator and */
 /* ;                       as a place-holder in strings */
 /* ;                       and program listings. */
 /*   00C2          bang     = $c2      ; {!}  return line number */
 /*   00C4          quote    = $c4      ; {"}  user ml subroutine vector */
 /*   00C6          pound    = $c6      ; {#}  current line number */
 /*   00C8          dolr     = $c8      ; {$}* temp storage / char i/o */
 /*   00CA          remn     = $ca      ; {%}  remainder of last division */
 /*   00CC          ampr     = $cc      ; {&}  pointer to start of array */
 /*   00CE          tick     = $ce      ; {'}  pseudo-random number */
 /*   00D0          lparen   = $d0      ; {(}* old line # / begin sub-exp */
 /*   00D2          rparen   = $d2      ; {)}* temp storage / end sub-exp */
 /*   00D4          star     = $d4      ; {*}  pointer to end of free mem */
 /* ;          $d6      ; {+ , - . /}  valid variables */
 /* ; Interpreter argument stack space */
 /*   00E0          arg      = $e0      ; {0 1 2 3 4 5 6 7 8 9 :}* */
 /* ; Rarely used variables and argument stack overflow */
 /* ;          $f6      ; {;}* valid user variable */
 /*   00F8          lthan    = $f8      ; {<}* user memory byte pointer */
 /* ;        = $fa      ; {=}* valid user variable */
 /*   00FC          gthan    = $fc      ; {>}* temp / call ML subroutine */
 /*   00FE          ques     = $fe      ; {?}* temp / terminal i/o */
 /* ; */
 /*   01FF          nulstk   = $01ff    ; system stack resides in page 1 */
 /* ;-----------------------------------------------------; */
 /* ; Equates for a 48K+ Apple 2 (original, +, e, c, gs) */
 /* ;ESC      = 27       ; "Cancel current input line" key */
 /* ;BS       = 8        ; "Delete last keypress" key */
 /* ;OP_OR    = '!'      ; Bit-wise OR operator */
 /* ;linbuf   = $0200    ; input line buffer */
 /* ;prgm     = $0800    ; VTL02B program grows from here */
 /* ;himem    = $8000    ;   ... up to the top of user RAM */
 /* ;vtl02c   = $8000    ; interpreter cold entry point */
 /* ;                     (warm entry point is startok) */
 /* ;KBD      = $c000    ; 128 + keypress if waiting */
 /* ;KEYIN    = $fd0c    ; apple monitor keyin routine */
 /* ;COUT     = $fded    ; apple monitor charout routine */
 /* ;-----------------------------------------------------; */
 /* ; Equates for the Kowalski 6502 simulator */
 /*   001B          ESC      = 27       ; "Cancel current input line" key */
 /*   0008          BS       = 8        ; "Delete last keypress" key */
 /*   007C          OP_OR    = '|'      ; Bit-wise OR operator */
 /*   0200          linbuf   = $0200    ; input line buffer */
 /*   0220          prgm     = $0220	; 0400    ; VTL02B program grows from here */
 /*   03FF          himem    = $03FF	; F000    ;   ... up to the top of user RAM */
 /*   DC00          vtl02c   = $DC00    ; interpreter cold entry point */
 /* ;                     (warm entry point is startok) */
 /* io_area  = $f000      ;configure simulator terminal I/O */
 /*   CFF1          acia_tx  = $cff1	; KIM UNO, was io_area+1  ;acia tx data register */
 /*   CFF4          acia_rx  = $cff4	; KIM UNO, was io_area+4  ;acia rx data register */
 /* ;=====================================================; */
/* 00dc00 */                          /*  .org vtl02c */
 /* ;-----------------------------------------------------; */
 /* ; Initialize program area pointers and start VTL02C */
 /* ; 17 bytes */
/* 00dc00 */ 0xa9, 0x20,              /*  LDA  #<prgm */
/* 00dc02 */ 0x85, 0xcc,              /*  sta  ampr       ; {&} -> empty program */
/* 00dc04 */ 0xa9, 0x02,              /*  lda  #>prgm */
/* 00dc06 */ 0x85, 0xcd,              /*  sta  ampr+1 */
/* 00dc08 */ 0xa9, 0xff,              /*  lda  #<himem */
/* 00dc0a */ 0x85, 0xd4,              /*  sta  star       ; {*} -> top of user RAM */
/* 00dc0c */ 0xa9, 0x03,              /*  lda  #>himem */
/* 00dc0e */ 0x85, 0xd5,              /*  sta  star+1 */
 /* startok: */
/* 00dc10 */ 0x38,                    /*  sec             ; request "OK" message */
 /* ; - - - - - - - - - - - - - - - - - - - - - - - - - - ; */
 /* ; Start/restart VTL02C command line with program intact */
 /* ; 32 bytes */
 /* start: */
/* 00dc11 */ 0xd8,                    /*  cld             ; a sensible precaution */
/* 00dc12 */ 0xa2, 0xff,              /*  ldx  #<nulstk */
/* 00dc14 */ 0x9a,                    /*  txs             ; drop whatever is on the stack */
/* 00dc15 */ 0x90, 0x10,              /*  bcc  user       ; skip "OK" if carry clear */
/* 00dc17 */ 0x20, 0x39, 0xdd,        /*  jsr  outnl */
/* 00dc1a */ 0xa9, 0x4f,              /*  lda  #'O'       ; output \nOK\n to terminal */
/* 00dc1c */ 0x20, 0xef, 0xdf,        /*  jsr  outch */
/* 00dc1f */ 0xa9, 0x4b,              /*  lda  #'K' */
/* 00dc21 */ 0x20, 0xef, 0xdf,        /*  jsr  outch */
/* 00dc24 */ 0x20, 0x39, 0xdd,        /*  jsr  outnl */
 /* user: */
/* 00dc27 */ 0x20, 0x64, 0xdf,        /*  jsr  inln       ; input a line from the user */
/* 00dc2a */ 0xa2, 0xc6,              /*  ldx  #pound     ; cvbin destination = {#} */
/* 00dc2c */ 0x20, 0x2f, 0xdf,        /*  jsr  cvbin      ; does line start with a number? */
/* 00dc2f */ 0xf0, 0x39,              /*  beq  direct     ;   no: execute direct statement */
 /* ; - - - - - - - - - - - - - - - - - - - - - - - - - - ; */
 /* ; Delete/insert/replace program line or list program */
 /* ; 7 bytes */
 /* stmnt: */
/* 00dc31 */ 0x18,                    /*  clc */
/* 00dc32 */ 0xa5, 0xc6,              /*  lda  pound */
/* 00dc34 */ 0x05, 0xc7,              /*  ora  pound+1    ; {#} = 0? */
/* 00dc36 */ 0xd0, 0x41,              /*  bne  skp2       ;   no: delete/insert/replace line */
 /* ; - - - - - - - - - - - - - - - - - - - - - - - - - - ; */
 /* ; List program to terminal and restart "OK" prompt */
 /* ; entry:  Carry must be clear */
 /* ; uses:   findln:, outch:, prnum:, prstr:, {@ ( )} */
 /* ; exit:   to command line via findln: */
 /* ; 20 bytes */
 /* list_: */
/* 00dc38 */ 0x20, 0x10, 0xdd,        /*  jsr  findln     ; find program line >= {#} */
/* 00dc3b */ 0xa2, 0xd0,              /*  ldx  #lparen    ; line number for prnum */
/* 00dc3d */ 0x20, 0x94, 0xdd,        /*  jsr  prnum      ; print the line number */
/* 00dc40 */ 0xa9, 0x20,              /*  lda  #' '       ; print a space instead of the */
/* 00dc42 */ 0x20, 0xef, 0xdf,        /*  jsr  outch      ;   line length byte */
/* 00dc45 */ 0xa9, 0x00,              /*  lda  #0         ; zero for delimiter */
/* 00dc47 */ 0x20, 0x1a, 0xdd,        /*  jsr  prstr      ; print the rest of the line */
/* 00dc4a */ 0xb0, 0xec,              /*  bcs  list_      ; (always taken) */
 /* ;-----------------------------------------------------; */
 /* ; The main program execution loop */
 /* ; entry:  with (cs) via "beq direct" in user: */
 /* ; exit:   to command line via findln: or "beq start" */
 /* ; 45 bytes */
 /* progr: */
/* 00dc4c */ 0xf0, 0x14,              /*  beq  eloop0     ; if {#} = 0 then ignore and */
/* 00dc4e */ 0xa4, 0xd1,              /*  ldy  lparen+1   ;   continue (false branch) */
/* 00dc50 */ 0xa6, 0xd0,              /*  ldx  lparen     ; else did {#} change? */
/* 00dc52 */ 0xc4, 0xc7,              /*  cpy  pound+1    ;   yes: perform a branch, with */
/* 00dc54 */ 0xd0, 0x04,              /*  bne  branch     ;     carry flag conditioned for */
/* 00dc56 */ 0xe4, 0xc6,              /*  cpx  pound      ;     the appropriate direction. */
/* 00dc58 */ 0xf0, 0x0c,              /*  beq  eloop      ;   no: execute next line (cs) */
 /* branch: */
/* 00dc5a */ 0xe8,                    /*  inx             ;   execute a VTL02B branch */
/* 00dc5b */ 0xd0, 0x01,              /*  bne  branch2 */
/* 00dc5d */ 0xc8,                    /*  iny */
 /* branch2: */
/* 00dc5e */ 0x86, 0xc2,              /*  stx  bang       ;   {!} = {(} + 1 (return ptr) */
/* 00dc60 */ 0x84, 0xc3,              /*  sty  bang+1 */
 /* eloop0: */
/* 00dc62 */ 0x2a,                    /*  rol */
/* 00dc63 */ 0x49, 0x01,              /*  eor  #1         ; complement carry flag */
/* 00dc65 */ 0x6a,                    /*  ror */
 /* eloop: */
/* 00dc66 */ 0x20, 0x10, 0xdd,        /*  jsr  findln     ; find first/next line >= {#} */
/* 00dc69 */ 0xc8,                    /*  iny             ; skip over the length byte */
 /* direct: */
/* 00dc6a */ 0x08,                    /*  php             ; (cc: program, cs: direct) */
/* 00dc6b */ 0x20, 0x3e, 0xdd,        /*  jsr  exec       ; execute one VTL02B statement */
/* 00dc6e */ 0x28,                    /*  plp */
/* 00dc6f */ 0xa5, 0xc6,              /*  lda  pound      ; update Z for {#} */
/* 00dc71 */ 0x05, 0xc7,              /*  ora  pound+1    ; if program mode then continue */
/* 00dc73 */ 0x90, 0xd7,              /*  bcc  progr      ; if direct mode, did {#} change? */
/* 00dc75 */ 0xf0, 0x9a,              /*  beq  start      ;   no: restart "OK" prompt */
/* 00dc77 */ 0xd0, 0xe9,              /*  bne  eloop0     ;   yes: execute program from {#} */
 /* ;-----------------------------------------------------; */
 /* ; Delete/insert/replace program line and restart the */
 /* ;   command prompt (no "OK" means success) */
 /* ; entry:  Carry must be clear */
 /* ; uses:   find:, start:, linbuf, {@ > # & * (} */
 /* ; 151 bytes */
 /* skp2: */
/* 00dc79 */ 0x98,                    /*  tya             ; save linbuf offset pointer */
/* 00dc7a */ 0x48,                    /*  pha */
/* 00dc7b */ 0x20, 0x84, 0xdf,        /*  jsr  find       ; point {@} to first line >= {#} */
/* 00dc7e */ 0xb0, 0x34,              /*  bcs  insrt */
/* 00dc80 */ 0x45, 0xc6,              /*  eor  pound      ; if line doesn't already exist */
/* 00dc82 */ 0xd0, 0x30,              /*  bne  insrt      ; then skip deletion process */
/* 00dc84 */ 0xe4, 0xc7,              /*  cpx  pound+1 */
/* 00dc86 */ 0xd0, 0x2c,              /*  bne  insrt */
/* 00dc88 */ 0xaa,                    /*  tax             ; x = 0 */
/* 00dc89 */ 0xb1, 0x80,              /*  lda  (at),y */
/* 00dc8b */ 0xa8,                    /*  tay             ; y = length of line to delete */
/* 00dc8c */ 0x49, 0xff,              /*  eor  #-1 */
/* 00dc8e */ 0x65, 0xcc,              /*  adc  ampr       ; {&} = {&} - y */
/* 00dc90 */ 0x85, 0xcc,              /*  sta  ampr */
/* 00dc92 */ 0xb0, 0x02,              /*  bcs  delt */
/* 00dc94 */ 0xc6, 0xcd,              /*  dec  ampr+1 */
 /* delt: */
/* 00dc96 */ 0xa5, 0x80,              /*  lda  at */
/* 00dc98 */ 0x85, 0xfc,              /*  sta  gthan      ; {>} = {@} */
/* 00dc9a */ 0xa5, 0x81,              /*  lda  at+1 */
/* 00dc9c */ 0x85, 0xfd,              /*  sta  gthan+1 */
 /* delt2: */
/* 00dc9e */ 0xa5, 0xfc,              /*  lda  gthan */
/* 00dca0 */ 0xc5, 0xcc,              /*  cmp  ampr       ; delete the line */
/* 00dca2 */ 0xa5, 0xfd,              /*  lda  gthan+1 */
/* 00dca4 */ 0xe5, 0xcd,              /*  sbc  ampr+1 */
/* 00dca6 */ 0xb0, 0x0c,              /*  bcs  insrt */
/* 00dca8 */ 0xb1, 0xfc,              /*  lda  (gthan),y */
/* 00dcaa */ 0x81, 0xfc,              /*  sta  (gthan,x) */
/* 00dcac */ 0xe6, 0xfc,              /*  inc  gthan */
/* 00dcae */ 0xd0, 0xee,              /*  bne  delt2 */
/* 00dcb0 */ 0xe6, 0xfd,              /*  inc  gthan+1 */
/* 00dcb2 */ 0x90, 0xea,              /*  bcc  delt2      ; (always taken) */
 /* insrt: */
/* 00dcb4 */ 0x68,                    /*  pla */
/* 00dcb5 */ 0xaa,                    /*  tax             ; x = linbuf offset pointer */
/* 00dcb6 */ 0xa5, 0xc6,              /*  lda  pound */
/* 00dcb8 */ 0x48,                    /*  pha             ; push the new line number on */
/* 00dcb9 */ 0xa5, 0xc7,              /*  lda  pound+1    ;   the system stack */
/* 00dcbb */ 0x48,                    /*  pha */
/* 00dcbc */ 0xa0, 0x02,              /*  ldy  #2 */
 /* cntln: */
/* 00dcbe */ 0xe8,                    /*  inx */
/* 00dcbf */ 0xc8,                    /*  iny             ; determine new line length in y */
/* 00dcc0 */ 0xbd, 0xff, 0x01,        /*  lda  linbuf-1,x ;   and push statement string on */
/* 00dcc3 */ 0x48,                    /*  pha             ;   the system stack */
/* 00dcc4 */ 0xd0, 0xf8,              /*  bne  cntln */
/* 00dcc6 */ 0xc0, 0x04,              /*  cpy  #4         ; if empty line then skip the */
/* 00dcc8 */ 0x90, 0x43,              /*  bcc  jstart     ;   insertion process */
/* 00dcca */ 0xaa,                    /*  tax             ; x = 0 */
/* 00dccb */ 0x98,                    /*  tya */
/* 00dccc */ 0x18,                    /*  clc */
/* 00dccd */ 0x65, 0xcc,              /*  adc  ampr       ; calculate new program end */
/* 00dccf */ 0x85, 0xfc,              /*  sta  gthan      ; {>} = {&} + y */
/* 00dcd1 */ 0x8a,                    /*  txa */
/* 00dcd2 */ 0x65, 0xcd,              /*  adc  ampr+1 */
/* 00dcd4 */ 0x85, 0xfd,              /*  sta  gthan+1 */
/* 00dcd6 */ 0xa5, 0xfc,              /*  lda  gthan */
/* 00dcd8 */ 0xc5, 0xd4,              /*  cmp  star       ; if {>} >= {*} then the program */
/* 00dcda */ 0xa5, 0xfd,              /*  lda  gthan+1    ;   won't fit in available RAM, */
/* 00dcdc */ 0xe5, 0xd5,              /*  sbc  star+1     ;   so drop the stack and abort */
/* 00dcde */ 0xb0, 0x2d,              /*  bcs  jstart     ;   to the "OK" prompt */
 /* slide: */
/* 00dce0 */ 0xa5, 0xcc,              /*  lda  ampr */
/* 00dce2 */ 0xd0, 0x02,              /*  bne  slide2 */
/* 00dce4 */ 0xc6, 0xcd,              /*  dec  ampr+1 */
 /* slide2: */
/* 00dce6 */ 0xc6, 0xcc,              /*  dec  ampr */
/* 00dce8 */ 0xa5, 0xcc,              /*  lda  ampr */
/* 00dcea */ 0xc5, 0x80,              /*  cmp  at */
/* 00dcec */ 0xa5, 0xcd,              /*  lda  ampr+1 */
/* 00dcee */ 0xe5, 0x81,              /*  sbc  at+1 */
/* 00dcf0 */ 0x90, 0x06,              /*  bcc  move       ; slide open a gap inside the */
/* 00dcf2 */ 0xa1, 0xcc,              /*  lda  (ampr,x)   ;   program just big enough to */
/* 00dcf4 */ 0x91, 0xcc,              /*  sta  (ampr),y   ;   hold the new line */
/* 00dcf6 */ 0xb0, 0xe8,              /*  bcs  slide      ; (always taken) */
 /* move: */
/* 00dcf8 */ 0x98,                    /*  tya */
/* 00dcf9 */ 0xaa,                    /*  tax             ; x = new line length */
 /* move2: */
/* 00dcfa */ 0x68,                    /*  pla             ; pull the statement string and */
/* 00dcfb */ 0x88,                    /*  dey             ;   the new line number and store */
/* 00dcfc */ 0x91, 0x80,              /*  sta  (at),y     ;   them in the program gap */
/* 00dcfe */ 0xd0, 0xfa,              /*  bne  move2 */
/* 00dd00 */ 0xa0, 0x02,              /*  ldy  #2 */
/* 00dd02 */ 0x8a,                    /*  txa */
/* 00dd03 */ 0x91, 0x80,              /*  sta  (at),y     ; store length after line number */
/* 00dd05 */ 0xa5, 0xfc,              /*  lda  gthan */
/* 00dd07 */ 0x85, 0xcc,              /*  sta  ampr       ; {&} = {>} */
/* 00dd09 */ 0xa5, 0xfd,              /*  lda  gthan+1 */
/* 00dd0b */ 0x85, 0xcd,              /*  sta  ampr+1 */
 /* jstart: */
/* 00dd0d */ 0x4c, 0x11, 0xdc,        /*  jmp  start      ; drop stack, restart cmd prompt */
 /* ;-----------------------------------------------------; */
 /* ; Point @[y] to the first/next program line >= {#} */
 /* ; entry:   (cc): start search at beginning of program */
 /* ;          (cs): start search at next line */
 /* ;          ({@} -> beginning of current line) */
 /* ; used by: list_:, progr: */
 /* ; uses:    find:, jstart:, prgm, {@ # & (} */
 /* ; exit:    if line not found then abort to "OK" prompt */
 /* ;          else {@} -> found line, x:a = {#} = {(} = */
 /* ;            actual line number, y = 2, (cc) */
 /* ; 10 bytes */
 /* findln: */
/* 00dd10 */ 0x20, 0x84, 0xdf,        /*  jsr  find       ; find first/next line >= {#} */
/* 00dd13 */ 0xb0, 0xf8,              /*  bcs  jstart     ; if end then restart "OK" prompt */
/* 00dd15 */ 0x85, 0xc6,              /*  sta  pound      ; {#} = {(} */
/* 00dd17 */ 0x86, 0xc7,              /*  stx  pound+1 */
/* 00dd19 */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; {?="...} handler; called from exec: */
 /* ; List line handler; called from list_: */
 /* ; 2 bytes */
 /* prstr: */
/* 00dd1a */ 0xc8,                    /*  iny             ; skip over the " or length byte */
/* 00dd1b */ 0xaa,                    /*  tax             ; x = delimiter, fall through */
 /* ; - - - - - - - - - - - - - - - - - - - - - - - - - - ; */
 /* ; Print a string at @[y] */
 /* ; x holds the delimiter char, which is skipped over, */
 /* ;   not printed (a null byte is always a delimiter) */
 /* ; If a key was pressed, it pauses for another keypress */
 /* ;   before returning.  If either of those keys was a */
 /* ;   ctrl-C, it drops the stack and restarts the "OK" */
 /* ;   prompt with the user program intact */
 /* ; entry:  @[y] -> string, x = delimiter char */
 /* ; uses:   inch:, inkey:, jstart:, outch:, execrts: */
 /* ; exit:   (normal) @[y] -> null or byte after delimiter */
 /* ;         (ctrl-C) drop the stack & restart "OK" prompt */
 /* ; 39 bytes */
 /* prmsg: */
/* 00dd1c */ 0x8a,                    /*  txa */
/* 00dd1d */ 0xd1, 0x80,              /*  cmp  (at),y     ; found delimiter or null? */
/* 00dd1f */ 0xf0, 0x0a,              /*  beq  prmsg2     ; yes: finish up */
/* 00dd21 */ 0xb1, 0x80,              /*  lda  (at),y */
/* 00dd23 */ 0xf0, 0x06,              /*  beq  prmsg2 */
/* 00dd25 */ 0x20, 0xef, 0xdf,        /*  jsr  outch      ; no: print char to terminal */
/* 00dd28 */ 0xc8,                    /*  iny             ;   and loop (with safety escape) */
/* 00dd29 */ 0x10, 0xf1,              /*  bpl  prmsg */
 /* prmsg2: */
/* 00dd2b */ 0xaa,                    /*  tax             ; save closing delimiter */
/* 00dd2c */ 0x20, 0xcc, 0xdf,        /*  jsr  inkey      ; any key = pause? */
 /* ; patch - remove garbage output when halting print */
 /* ;    bcc  prout      ;   no: proceed */
 /* ;    jsr  inch       ;   yes: wait for another key */
 /* ;prout: */
/* 00dd2f */ 0x8a,                    /*  txa             ; retrieve closing delimiter */
/* 00dd30 */ 0xf0, 0x07,              /*  beq  outnl      ; always \n after null delimiter */
/* 00dd32 */ 0x20, 0xc2, 0xdf,        /*  jsr  skpbyte    ; skip over the delimiter */
/* 00dd35 */ 0xc9, 0x3b,              /*  cmp  #';'       ; if trailing char is ';' then */
/* 00dd37 */ 0xf0, 0x4f,              /*  beq  execrts    ;   suppress the \n */
 /* outnl: */
/* 00dd39 */ 0xa9, 0x0d,              /*  lda  #$0d       ; \n to terminal */
 /* joutch: */
/* 00dd3b */ 0x4c, 0xef, 0xdf,        /*  jmp  outch */
 /* ;-----------------------------------------------------; */
 /* ; Execute a (hopefully) valid VTL02C statement at @[y] */
 /* ; entry:   @[y] -> left-side of statement */
 /* ; uses:    nearly everything */
 /* ; exit:    note to machine language subroutine {>=...} */
 /* ;            users: no registers or variables are */
 /* ;            required to be preserved except the system */
 /* ;            stack pointer, the text base pointer {@}, */
 /* ;            and the original line number {(} */
 /* ; if there is a {"} directly after the assignment */
 /* ;   operator, the statement will execute as {?="...}, */
 /* ;   regardless of the variable named on the left side */
 /* ; 84 bytes */
 /* exec: */
/* 00dd3e */ 0x20, 0xc3, 0xdf,        /*  jsr  getbyte    ; fetch left-side variable name */
/* 00dd41 */ 0xf0, 0x45,              /*  beq  execrts    ; do nothing with a null statement */
/* 00dd43 */ 0xc9, 0x29,              /*  cmp  #')'       ; same for a full-line comment */
/* 00dd45 */ 0xf0, 0x41,              /*  beq  execrts */
/* 00dd47 */ 0xc8,                    /*  iny */
/* 00dd48 */ 0xa2, 0xe0,              /*  ldx  #arg       ; initialize argument pointer */
/* 00dd4a */ 0x20, 0x31, 0xde,        /*  jsr  convp      ; arg[{0}] -> left-side variable */
/* 00dd4d */ 0x20, 0xc3, 0xdf,        /*  jsr  getbyte    ; skip over assignment operator */
/* 00dd50 */ 0x20, 0xc2, 0xdf,        /*  jsr  skpbyte    ; is right-side a literal string? */
/* 00dd53 */ 0xc9, 0x22,              /*  cmp  #'"'       ;   yes: print the string with */
/* 00dd55 */ 0xf0, 0xc3,              /*  beq  prstr      ;     trailing ';' check & return */
/* 00dd57 */ 0xa2, 0xe2,              /*  ldx  #arg+2     ; point eval to arg[{1}] */
/* 00dd59 */ 0x20, 0xbf, 0xdd,        /*  jsr  eval       ; evaluate right-side in arg[{1}] */
/* 00dd5c */ 0xa5, 0xe2,              /*  lda  arg+2 */
/* 00dd5e */ 0xa0, 0x00,              /*  ldy  #0 */
/* 00dd60 */ 0xa6, 0xe1,              /*  ldx  arg+1      ; was left-side an array element? */
/* 00dd62 */ 0xd0, 0x12,              /*  bne  exec3      ;   yes: skip to default actions */
/* 00dd64 */ 0xa6, 0xe0,              /*  ldx  arg */
/* 00dd66 */ 0xe0, 0x80,              /*  cpx  #at        ; if {@=...} statement then poke */
/* 00dd68 */ 0xf0, 0x25,              /*  beq  poke       ;   low half of arg[{1}] to ({<}) */
/* 00dd6a */ 0xe0, 0xc8,              /*  cpx  #dolr      ; if {$=...} statement then print */
/* 00dd6c */ 0xf0, 0xcd,              /*  beq  joutch     ;   arg[{1}] as ASCII character */
/* 00dd6e */ 0xe0, 0xfe,              /*  cpx  #ques      ; if {?=...} statement then print */
/* 00dd70 */ 0xf0, 0x20,              /*  beq  prnum0     ;   arg[{1}] as unsigned decimal */
/* 00dd72 */ 0xe0, 0xfc,              /*  cpx  #gthan     ; if {>=...} statement then call */
/* 00dd74 */ 0xf0, 0x13,              /*  beq  usr        ;   user-defined ml routine */
 /* exec3: */
/* 00dd76 */ 0x91, 0xe0,              /*  sta  (arg),y */
/* 00dd78 */ 0x65, 0xcf,              /*  adc  tick+1     ; store arg[{1}] in the left-side */
/* 00dd7a */ 0x2a,                    /*  rol             ;   variable */
/* 00dd7b */ 0xaa,                    /*  tax */
/* 00dd7c */ 0xc8,                    /*  iny */
/* 00dd7d */ 0xa5, 0xe3,              /*  lda  arg+3 */
/* 00dd7f */ 0x91, 0xe0,              /*  sta  (arg),y */
/* 00dd81 */ 0x65, 0xce,              /*  adc  tick       ; pseudo-randomize {'} */
/* 00dd83 */ 0x2a,                    /*  rol */
/* 00dd84 */ 0x85, 0xcf,              /*  sta  tick+1 */
/* 00dd86 */ 0x86, 0xce,              /*  stx  tick */
 /* execrts: */
/* 00dd88 */ 0x60,                    /*  rts */
 /* usr: */
/* 00dd89 */ 0xaa,                    /*  tax             ; jump to user ml routine with */
/* 00dd8a */ 0xa5, 0xe3,              /*  lda  arg+3      ;   arg[{1}] in a:x (MSB:LSB) */
/* 00dd8c */ 0x6c, 0xc4, 0x00,        /*  jmp  (quote)    ; {"} must point to valid 6502 code */
 /* poke: */
/* 00dd8f */ 0x91, 0xf8,              /*  sta  (lthan),y */
/* 00dd91 */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; {?=...} handler; called by exec: */
 /* ; 2 bytes */
 /* prnum0: */
/* 00dd92 */ 0xa2, 0xe2,              /*  ldx  #arg+2     ; x -> arg[{1}], fall through */
 /* ; - - - - - - - - - - - - - - - - - - - - - - - - - - ; */
 /* ; Print an unsigned decimal number (0..65535) in var[x] */
 /* ; entry:   var[x] = number to print */
 /* ; uses:    div:, outch:, var[x+2], saves original {%} */
 /* ; exit:    var[x] = 0, var[x+2] = 10 */
 /* ; 43 bytes */
 /* prnum: */
/* 00dd94 */ 0xa5, 0xca,              /*  lda  remn */
/* 00dd96 */ 0x48,                    /*  pha             ; save {%} */
/* 00dd97 */ 0xa5, 0xcb,              /*  lda  remn+1 */
/* 00dd99 */ 0x48,                    /*  pha */
/* 00dd9a */ 0xa9, 0x00,              /*  lda  #0         ; null delimiter for print */
/* 00dd9c */ 0x48,                    /*  pha */
/* 00dd9d */ 0x95, 0x03,              /*  sta  3,x */
/* 00dd9f */ 0xa9, 0x0a,              /*  lda  #10        ; divisor = 10 */
/* 00dda1 */ 0x95, 0x02,              /*  sta  2,x        ; repeat { */
 /* prnum2: */
/* 00dda3 */ 0x20, 0x04, 0xdf,        /*  jsr  div        ;   divide var[x] by 10 */
/* 00dda6 */ 0xa5, 0xca,              /*  lda  remn */
/* 00dda8 */ 0x09, 0x30,              /*  ora  #'0'       ;   convert remainder to ASCII */
/* 00ddaa */ 0x48,                    /*  pha             ;   stack digits in ascending */
/* 00ddab */ 0xb5, 0x00,              /*  lda  0,x        ;     order ('0' for zero) */
/* 00ddad */ 0x15, 0x01,              /*  ora  1,x */
/* 00ddaf */ 0xd0, 0xf2,              /*  bne  prnum2     ; } until var[x] is 0 */
/* 00ddb1 */ 0x68,                    /*  pla */
 /* prnum3: */
/* 00ddb2 */ 0x20, 0xef, 0xdf,        /*  jsr  outch      ; print digits in descending */
/* 00ddb5 */ 0x68,                    /*  pla             ;   order until delimiter is */
/* 00ddb6 */ 0xd0, 0xfa,              /*  bne  prnum3     ;   encountered */
/* 00ddb8 */ 0x68,                    /*  pla */
/* 00ddb9 */ 0x85, 0xcb,              /*  sta  remn+1     ; restore {%} */
/* 00ddbb */ 0x68,                    /*  pla */
/* 00ddbc */ 0x85, 0xca,              /*  sta  remn */
/* 00ddbe */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; Evaluate a (hopefully) valid VTL02C expression at */
 /* ;   @[y] and place its calculated value in arg[x] */
 /* ; A VTL02C expression is defined as a string of one or */
 /* ;   more terms, separated by operators and terminated */
 /* ;   with a null or an unmatched right parenthesis */
 /* ; A term is defined as a variable name, a decimal */
 /* ;   constant, or a parenthesized sub-expression; terms */
 /* ;   are evaluated strictly from left to right */
 /* ; A variable name is defined as a user variable, an */
 /* ;   array element expression enclosed in {: )}, or a */
 /* ;   system variable (which may have side-effects) */
 /* ; entry:   @[y] -> expression text, x -> argument */
 /* ; uses:    getval:, oper:, {@}, argument stack area */
 /* ; exit:    arg[x] = result, @[y] -> next text */
 /* ; 31 bytes */
 /* eval: */
/* 00ddbf */ 0xa9, 0x00,              /*  lda  #0 */
/* 00ddc1 */ 0x95, 0x00,              /*  sta  0,x        ; start evaluation by simulating */
/* 00ddc3 */ 0x95, 0x01,              /*  sta  1,x        ;   {0+expression} */
/* 00ddc5 */ 0xa9, 0x2b,              /*  lda  #'+' */
 /* notdn: */
/* 00ddc7 */ 0x48,                    /*  pha             ; stack alleged operator */
/* 00ddc8 */ 0xe8,                    /*  inx             ; advance the argument stack */
/* 00ddc9 */ 0xe8,                    /*  inx             ;   pointer */
/* 00ddca */ 0x20, 0xde, 0xdd,        /*  jsr  getval     ; arg[x+2] = value of next term */
/* 00ddcd */ 0xca,                    /*  dex */
/* 00ddce */ 0xca,                    /*  dex */
/* 00ddcf */ 0x68,                    /*  pla             ; retrieve and apply the operator */
/* 00ddd0 */ 0x20, 0x81, 0xde,        /*  jsr  oper       ;   to arg[x], arg[x+2] */
/* 00ddd3 */ 0x20, 0xc3, 0xdf,        /*  jsr  getbyte    ; end of expression? */
/* 00ddd6 */ 0xf0, 0x05,              /*  beq  evalrts    ;   (null or right parenthesis) */
/* 00ddd8 */ 0xc8,                    /*  iny */
/* 00ddd9 */ 0xc9, 0x29,              /*  cmp  #')'       ;   no: skip over the operator */
/* 00dddb */ 0xd0, 0xea,              /*  bne  notdn      ;     and continue the evaluation */
 /* evalrts: */
/* 00dddd */ 0x60,                    /*  rts             ;   yes: return with final result */
 /* ;-----------------------------------------------------; */
 /* ; Get numeric value of the term at @[y] into var[x] */
 /* ; Some examples of valid terms:  123, $, H, (15-:J)/?) */
 /* ; 83 bytes */
 /* getval: */
/* 00ddde */ 0x20, 0x2f, 0xdf,        /*  jsr  cvbin      ; decimal number at @[y]? */
/* 00dde1 */ 0xd0, 0x4d,              /*  bne  getrts     ;   yes: return with it in var[x] */
/* 00dde3 */ 0x20, 0xc3, 0xdf,        /*  jsr  getbyte */
/* 00dde6 */ 0xc8,                    /*  iny */
/* 00dde7 */ 0xc9, 0x3f,              /*  cmp  #'?'       ; user line input? */
/* 00dde9 */ 0xd0, 0x17,              /*  bne  getval2 */
/* 00ddeb */ 0x98,                    /*  tya             ;   yes: */
/* 00ddec */ 0x48,                    /*  pha */
/* 00dded */ 0xa5, 0x80,              /*  lda  at         ;     save @[y] */
/* 00ddef */ 0x48,                    /*  pha             ;     (current expression ptr) */
/* 00ddf0 */ 0xa5, 0x81,              /*  lda  at+1 */
/* 00ddf2 */ 0x48,                    /*  pha */
/* 00ddf3 */ 0x20, 0x64, 0xdf,        /*  jsr  inln       ; input expression from user */
/* 00ddf6 */ 0x20, 0xbf, 0xdd,        /*  jsr  eval       ; evaluate, var[x] = result */
/* 00ddf9 */ 0x68,                    /*  pla */
/* 00ddfa */ 0x85, 0x81,              /*  sta  at+1 */
/* 00ddfc */ 0x68,                    /*  pla */
/* 00ddfd */ 0x85, 0x80,              /*  sta  at         ; restore @[y] */
/* 00ddff */ 0x68,                    /*  pla */
/* 00de00 */ 0xa8,                    /*  tay */
/* 00de01 */ 0x60,                    /*  rts             ; skip over "?" and return */
 /* getval2: */
/* 00de02 */ 0xc9, 0x24,              /*  cmp  #'$'       ; user char input? */
/* 00de04 */ 0xd0, 0x05,              /*  bne  getval2a */
/* 00de06 */ 0x20, 0xdf, 0xdf,        /*  jsr  inch       ;   yes: input one char */
/* 00de09 */ 0xb0, 0x23,              /*  bcs  getval5    ;     (always taken) */
 /* getval2a: */
/* 00de0b */ 0xc9, 0x40,              /*  cmp  #'@'       ; memory access? */
/* 00de0d */ 0xd0, 0x0a,              /*  bne  getval3 */
/* 00de0f */ 0x84, 0xc8,              /*  sty  dolr       ;   yes: */
/* 00de11 */ 0xa0, 0x00,              /*  ldy  #0 */
/* 00de13 */ 0xb1, 0xf8,              /*  lda  (lthan),y  ;     access memory byte at ({<}) */
/* 00de15 */ 0xa4, 0xc8,              /*  ldy  dolr */
/* 00de17 */ 0xd0, 0x15,              /*  bne  getval5    ;     (always taken) */
 /* getval3: */
/* 00de19 */ 0xc9, 0x28,              /*  cmp  #'('       ; sub-expression? */
/* 00de1b */ 0xf0, 0xa2,              /*  beq  eval       ;   yes: evaluate it recursively */
/* 00de1d */ 0x20, 0x31, 0xde,        /*  jsr  convp      ;   no: first set var[x] to the */
/* 00de20 */ 0xa1, 0x00,              /*  lda  (0,x)      ;     named variable's address, */
/* 00de22 */ 0x48,                    /*  pha             ;     then replace that address */
/* 00de23 */ 0xf6, 0x00,              /*  inc  0,x        ;     with the variable's actual */
/* 00de25 */ 0xd0, 0x02,              /*  bne  getval4    ;     value before returning */
/* 00de27 */ 0xf6, 0x01,              /*  inc  1,x */
 /* getval4: */
/* 00de29 */ 0xa1, 0x00,              /*  lda  (0,x) */
/* 00de2b */ 0x95, 0x01,              /*  sta  1,x        ; store high-byte of term value */
/* 00de2d */ 0x68,                    /*  pla */
 /* getval5: */
/* 00de2e */ 0x95, 0x00,              /*  sta  0,x        ; store low-byte of term value */
 /* getrts: */
/* 00de30 */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; Set var[x] to the address of the variable named in a */
 /* ; entry:   a holds variable name, @[y] -> text holding */
 /* ;            array index expression (if a = ':') */
 /* ; uses:    plus, eval, oper8d, {@ &} */
 /* ; exit:    (eq): var[x] -> variable, @[y] unchanged */
 /* ;          (ne): var[x] -> array element, */
 /* ;                @[y] -> following text */
 /* ; 26 bytes */
 /* convp: */
/* 00de31 */ 0xc9, 0x3a,              /*  cmp  #':'       ; array element? */
/* 00de33 */ 0xd0, 0x11,              /*  bne  simple     ;   no: var[x] -> simple variable */
/* 00de35 */ 0x20, 0xbf, 0xdd,        /*  jsr  eval       ;   yes: evaluate array index at */
/* 00de38 */ 0x16, 0x00,              /*  asl  0,x        ;     @[y] and advance y */
/* 00de3a */ 0x36, 0x01,              /*  rol  1,x */
/* 00de3c */ 0xa5, 0xcc,              /*  lda  ampr       ;     var[x] -> array element */
/* 00de3e */ 0x95, 0x02,              /*  sta  2,x        ;       at address 2*index+& */
/* 00de40 */ 0xa5, 0xcd,              /*  lda  ampr+1 */
/* 00de42 */ 0x95, 0x03,              /*  sta  3,x */
/* 00de44 */ 0xd0, 0x2d,              /*  bne  plus       ;     (always taken) */
 /* ; The following section is designed to translate the */
 /* ;   named simple variable from its ASCII value to its */
 /* ;   zero-page address.  In this case, 'A' translates */
 /* ;   to $82, '!' translates to $c2, etc.  The method */
 /* ;   employed must correspond to the zero-page equates */
 /* ;   above, or strange and not-so-wonderful bugs will */
 /* ;   befall the weary traveller on his or her porting */
 /* ;   journey. */
 /* simple: */
/* 00de46 */ 0x0a,                    /*  asl             ; form simple variable address */
/* 00de47 */ 0x09, 0x80,              /*  ora  #$80       ; mapping function is (a*2)|128 */
/* 00de49 */ 0x30, 0x72,              /*  bmi  oper8d     ; (always taken) */
 /* ;-----------------------------------------------------; */
 /* ; 16-bit unsigned multiply routine: var[x] *= var[x+2] */
 /* ; exit:    overflow is ignored/discarded, var[x+2] and */
 /* ;          {>} are modified, a = 0 */
 /* ; 40 bytes */
 /* mul: */
/* 00de4b */ 0xb5, 0x00,              /*  lda  0,x */
/* 00de4d */ 0x85, 0xfc,              /*  sta  gthan */
/* 00de4f */ 0xb5, 0x01,              /*  lda  1,x        ; {>} = var[x] */
/* 00de51 */ 0x85, 0xfd,              /*  sta  gthan+1 */
/* 00de53 */ 0xa9, 0x00,              /*  lda  #0 */
/* 00de55 */ 0x95, 0x00,              /*  sta  0,x        ; var[x] = 0 */
/* 00de57 */ 0x95, 0x01,              /*  sta  1,x */
 /* mul2: */
/* 00de59 */ 0xa5, 0xfc,              /*  lda  gthan */
/* 00de5b */ 0x05, 0xfd,              /*  ora  gthan+1 */
/* 00de5d */ 0xf0, 0x13,              /*  beq  mulrts     ; exit early if {>} = 0 */
/* 00de5f */ 0x46, 0xfd,              /*  lsr  gthan+1 */
/* 00de61 */ 0x66, 0xfc,              /*  ror  gthan      ; {>} /= 2 */
/* 00de63 */ 0x90, 0x03,              /*  bcc  mul3 */
/* 00de65 */ 0x20, 0x73, 0xde,        /*  jsr  plus       ; form the product in var[x] */
 /* mul3: */
/* 00de68 */ 0x16, 0x02,              /*  asl  2,x */
/* 00de6a */ 0x36, 0x03,              /*  rol  3,x        ; left-shift var[x+2] */
/* 00de6c */ 0xb5, 0x02,              /*  lda  2,x */
/* 00de6e */ 0x15, 0x03,              /*  ora  3,x        ; loop until var[x+2] = 0 */
/* 00de70 */ 0xd0, 0xe7,              /*  bne  mul2 */
 /* mulrts: */
/* 00de72 */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; var[x] += var[x+2] */
 /* ; 14 bytes */
 /* plus: */
/* 00de73 */ 0x18,                    /*  clc */
/* 00de74 */ 0xb5, 0x00,              /*  lda  0,x */
/* 00de76 */ 0x75, 0x02,              /*  adc  2,x */
/* 00de78 */ 0x95, 0x00,              /*  sta  0,x */
/* 00de7a */ 0xb5, 0x01,              /*  lda  1,x */
/* 00de7c */ 0x75, 0x03,              /*  adc  3,x */
/* 00de7e */ 0x95, 0x01,              /*  sta  1,x */
/* 00de80 */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; Apply the binary operator in a to var[x] and var[x+2] */
 /* ; Valid VTL02C operators are {* + / [ ] - | ^ & < = >} */
 /* ; {>} is defined as greater than _or_equal_ */
 /* ; An undefined operator will be interpreted as one of */
 /* ;   the three comparison operators */
 /* ; 37 bytes */
 /* oper: */
/* 00de81 */ 0xc9, 0x2b,              /*  cmp  #'+'       ; addition operator? */
/* 00de83 */ 0xf0, 0xee,              /*  beq  plus */
/* 00de85 */ 0xc9, 0x2a,              /*  cmp  #'*'       ; multiplication operator? */
/* 00de87 */ 0xf0, 0xc2,              /*  beq  mul */
/* 00de89 */ 0xc9, 0x2f,              /*  cmp  #'/'       ; division operator? */
/* 00de8b */ 0xf0, 0x77,              /*  beq  div */
/* 00de8d */ 0xc9, 0x5b,              /*  cmp  #'['       ; "then" operator? */
/* 00de8f */ 0xf0, 0x32,              /*  beq  then_ */
/* 00de91 */ 0xc9, 0x5d,              /*  cmp  #']'       ; "else" operator? */
/* 00de93 */ 0xf0, 0x3c,              /*  beq  else_ */
/* 00de95 */ 0xca,                    /*  dex             ; (factored from the following ops) */
/* 00de96 */ 0xc9, 0x2d,              /*  cmp  #'-'       ; subtraction operator? */
/* 00de98 */ 0xf0, 0x41,              /*  beq  minus */
/* 00de9a */ 0xc9, 0x7c,              /*  cmp  #OP_OR     ; bit-wise or operator? */
/* 00de9c */ 0xf0, 0x52,              /*  beq  or_ */
/* 00de9e */ 0xc9, 0x5e,              /*  cmp  #'^'       ; bit-wise xor operator? */
/* 00dea0 */ 0xf0, 0x58,              /*  beq  xor_ */
/* 00dea2 */ 0xc9, 0x26,              /*  cmp  #'&'       ; bit-wise and operator? */
/* 00dea4 */ 0xf0, 0x40,              /*  beq  and_ */
 /* ; - - - - - - - - - - - - - - - - - - - - - - - - - - ; */
 /* ; Apply comparison operator in a to var[x] and var[x+2] */
 /* ;   and place result in var[x] (1: true, 0: false) */
 /* ; expects:  (cs), pre-decremented x */
 /* ; 29 bytes */
/* 00dea6 */ 0x49, 0x3c,              /*  eor  #'<'       ; 0: '<'  1: '='  2: '>' */
/* 00dea8 */ 0x85, 0xfc,              /*  sta  gthan      ; other values in a are undefined, */
/* 00deaa */ 0x20, 0xdb, 0xde,        /*  jsr  minus      ;   but _will_ produce some result */
/* 00dead */ 0xc6, 0xfc,              /*  dec  gthan      ; var[x] -= var[x+2] */
/* 00deaf */ 0xd0, 0x05,              /*  bne  oper8b     ; equality test? */
/* 00deb1 */ 0x15, 0x00,              /*  ora  0,x        ;   yes: 'or' high and low bytes */
/* 00deb3 */ 0xf0, 0x04,              /*  beq  oper8c     ;     (cs) if 0 */
/* 00deb5 */ 0x18,                    /*  clc             ;     (cc) if not 0 */
 /* oper8b: */
/* 00deb6 */ 0xa5, 0xfc,              /*  lda  gthan */
/* 00deb8 */ 0x2a,                    /*  rol */
 /* oper8c: */
/* 00deb9 */ 0x69, 0x00,              /*  adc  #0 */
/* 00debb */ 0x29, 0x01,              /*  and  #1         ; var[x] = 1 (true), 0 (false) */
 /* oper8d: */
/* 00debd */ 0x95, 0x00,              /*  sta  0,x */
/* 00debf */ 0xa9, 0x00,              /*  lda  #0 */
/* 00dec1 */ 0xf0, 0x20,              /*  beq  minus3     ; (always taken) */
 /* ;-----------------------------------------------------; */
 /* ; expects:  (cs) */
 /* ; 14 bytes */
 /* then_: */
/* 00dec3 */ 0xb5, 0x00,              /*  lda  0,x */
/* 00dec5 */ 0x15, 0x01,              /*  ora  1,x */
/* 00dec7 */ 0xf0, 0x1c,              /*  beq  minus4 */
/* 00dec9 */ 0xb5, 0x02,              /*  lda  2,x */
/* 00decb */ 0x95, 0x00,              /*  sta  0,x */
/* 00decd */ 0xb5, 0x03,              /*  lda  3,x */
/* 00decf */ 0xb0, 0x12,              /*  bcs  minus3     ; (always taken) */
 /* ;-----------------------------------------------------; */
 /* ; expects:  (cs) */
 /* ; 10 bytes */
 /* else_: */
/* 00ded1 */ 0xb5, 0x00,              /*  lda  0,x */
/* 00ded3 */ 0x15, 0x01,              /*  ora  1,x */
/* 00ded5 */ 0xf0, 0x9c,              /*  beq  plus */
/* 00ded7 */ 0xa9, 0x00,              /*  lda  #0 */
/* 00ded9 */ 0xf0, 0xe2,              /*  beq  oper8d     ; (always taken) */
 /* ;-----------------------------------------------------; */
 /* ; var[x] -= var[x+2] */
 /* ; expects:  (cs), pre-decremented x */
 /* ; 11 bytes */
 /* minus: */
/* 00dedb */ 0x20, 0xdf, 0xde,        /*  jsr  minus2 */
/* 00dede */ 0xe8,                    /*  inx */
 /* minus2: */
/* 00dedf */ 0xb5, 0x01,              /*  lda  1,x */
/* 00dee1 */ 0xf5, 0x03,              /*  sbc  3,x */
 /* minus3: */
/* 00dee3 */ 0x95, 0x01,              /*  sta  1,x */
 /* minus4: */
/* 00dee5 */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; var[x] &= var[x+2] */
 /* ; expects:  (cs), pre-decremented x */
 /* ; 10 bytes */
 /* and_: */
/* 00dee6 */ 0x20, 0xea, 0xde,        /*  jsr  and_2 */
/* 00dee9 */ 0xe8,                    /*  inx */
 /* and_2: */
/* 00deea */ 0xb5, 0x01,              /*  lda  1,x */
/* 00deec */ 0x35, 0x03,              /*  and  3,x */
/* 00deee */ 0xb0, 0xf3,              /*  bcs  minus3     ; (always taken) */
 /* ;-----------------------------------------------------; */
 /* ; var[x] |= var[x+2] */
 /* ; expects:  (cs), pre-decremented x */
 /* ; 10 bytes */
 /* or_: */
/* 00def0 */ 0x20, 0xf4, 0xde,        /*  jsr  or_2 */
/* 00def3 */ 0xe8,                    /*  inx */
 /* or_2: */
/* 00def4 */ 0xb5, 0x01,              /*  lda  1,x */
/* 00def6 */ 0x15, 0x03,              /*  ora  3,x */
/* 00def8 */ 0xb0, 0xe9,              /*  bcs  minus3     ; (always taken) */
 /* ;-----------------------------------------------------; */
 /* ; var[x] ^= var[x+2] */
 /* ; expects:  (cs), pre-decremented x */
 /* ; 10 bytes */
 /* xor_: */
/* 00defa */ 0x20, 0xfe, 0xde,        /*  jsr  xor_2 */
/* 00defd */ 0xe8,                    /*  inx */
 /* xor_2: */
/* 00defe */ 0xb5, 0x01,              /*  lda  1,x */
/* 00df00 */ 0x55, 0x03,              /*  eor  3,x */
/* 00df02 */ 0xb0, 0xdf,              /*  bcs  minus3     ; (always taken) */
 /* ;-----------------------------------------------------; */
 /* ; 16-bit unsigned division routine */
 /* ;   var[x] /= var[x+2], {%} = remainder, {>} modified */
 /* ;   var[x] /= 0 produces {%} = var[x], var[x] = 65535 */
 /* ; 43 bytes */
 /* div: */
/* 00df04 */ 0xa9, 0x00,              /*  lda  #0 */
/* 00df06 */ 0x85, 0xca,              /*  sta  remn       ; {%} = 0 */
/* 00df08 */ 0x85, 0xcb,              /*  sta  remn+1 */
/* 00df0a */ 0xa9, 0x10,              /*  lda  #16 */
/* 00df0c */ 0x85, 0xfc,              /*  sta  gthan      ; {>} = loop counter */
 /* div1: */
/* 00df0e */ 0x16, 0x00,              /*  asl  0,x        ; var[x] is gradually replaced */
/* 00df10 */ 0x36, 0x01,              /*  rol  1,x        ;   with the quotient */
/* 00df12 */ 0x26, 0xca,              /*  rol  remn       ; {%} is gradually replaced */
/* 00df14 */ 0x26, 0xcb,              /*  rol  remn+1     ;   with the remainder */
/* 00df16 */ 0xa5, 0xca,              /*  lda  remn */
/* 00df18 */ 0xd5, 0x02,              /*  cmp  2,x */
/* 00df1a */ 0xa5, 0xcb,              /*  lda  remn+1     ; partial remainder >= var[x+2]? */
/* 00df1c */ 0xf5, 0x03,              /*  sbc  3,x */
/* 00df1e */ 0x90, 0x0a,              /*  bcc  div2 */
/* 00df20 */ 0x85, 0xcb,              /*  sta  remn+1     ;   yes: update the partial */
/* 00df22 */ 0xa5, 0xca,              /*  lda  remn       ;     remainder and set the */
/* 00df24 */ 0xf5, 0x02,              /*  sbc  2,x        ;     low bit in the partial */
/* 00df26 */ 0x85, 0xca,              /*  sta  remn       ;     quotient */
/* 00df28 */ 0xf6, 0x00,              /*  inc  0,x */
 /* div2: */
/* 00df2a */ 0xc6, 0xfc,              /*  dec  gthan */
/* 00df2c */ 0xd0, 0xe0,              /*  bne  div1       ; loop 16 times */
/* 00df2e */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; If text at @[y] is a decimal constant, translate it */
 /* ;   into var[x] (discarding any overflow) and update y */
 /* ; entry:   @[y] -> text containing possible constant; */
 /* ;            leading space characters are skipped, but */
 /* ;            any spaces encountered after a conversion */
 /* ;            has begun will end the conversion. */
 /* ; used by: user:, getval: */
 /* ; uses:    mul:, plus:, var[x], var[x+2], {@ > ?} */
 /* ; exit:    (ne): var[x] = constant, @[y] -> next text */
 /* ;          (eq): var[x] = 0, @[y] unchanged */
 /* ;          (cs): in all but the truly strangest cases */
 /* ; 43 bytes */
 /* cvbin: */
/* 00df2f */ 0xa9, 0x00,              /*  lda  #0 */
/* 00df31 */ 0x95, 0x00,              /*  sta  0,x        ; var[x] = 0 */
/* 00df33 */ 0x95, 0x01,              /*  sta  1,x */
/* 00df35 */ 0x95, 0x03,              /*  sta  3,x */
/* 00df37 */ 0x20, 0xc3, 0xdf,        /*  jsr  getbyte    ; skip any leading spaces */
/* 00df3a */ 0x84, 0xfe,              /*  sty  ques       ; save pointer */
 /* cvbin2: */
/* 00df3c */ 0xb1, 0x80,              /*  lda  (at),y     ; grab a char */
/* 00df3e */ 0x49, 0x30,              /*  eor  #'0'       ; if char at @[y] is not a */
/* 00df40 */ 0xc9, 0x0a,              /*  cmp  #10        ;   decimal digit then stop */
/* 00df42 */ 0xb0, 0x13,              /*  bcs  cvbin3     ;   the conversion */
/* 00df44 */ 0x48,                    /*  pha             ; save decimal digit */
/* 00df45 */ 0xa9, 0x0a,              /*  lda  #10 */
/* 00df47 */ 0x95, 0x02,              /*  sta  2,x */
/* 00df49 */ 0x20, 0x4b, 0xde,        /*  jsr  mul        ; var[x] *= 10 */
/* 00df4c */ 0x95, 0x03,              /*  sta  3,x */
/* 00df4e */ 0x68,                    /*  pla             ; retrieve decimal digit */
/* 00df4f */ 0x95, 0x02,              /*  sta  2,x */
/* 00df51 */ 0x20, 0x73, 0xde,        /*  jsr  plus       ; var[x] += digit */
/* 00df54 */ 0xc8,                    /*  iny             ; loop for more digits */
/* 00df55 */ 0x10, 0xe5,              /*  bpl  cvbin2     ;   (with safety escape) */
 /* cvbin3: */
/* 00df57 */ 0xc4, 0xfe,              /*  cpy  ques       ; (ne) if valid, (eq) if not */
/* 00df59 */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; Accept input line from user and store it in linbuf, */
 /* ;   zero-terminated (allows very primitive edit/cancel) */
 /* ; entry:   (jsr to inln or newln, not inln6) */
 /* ; used by: user:, getval: */
 /* ; uses:    inch:, outnl:, linbuf, {@} */
 /* ; exit:    @[y] -> linbuf */
 /* ; 42 bytes */
 /* inln6: */
/* 00df5a */ 0xc9, 0x1b,              /*  cmp  #ESC       ; escape? */
/* 00df5c */ 0xf0, 0x03,              /*  beq  newln      ;   yes: discard entire line */
/* 00df5e */ 0xc8,                    /*  iny             ; line limit exceeded? */
/* 00df5f */ 0x10, 0x10,              /*  bpl  inln2      ;   no: keep going */
 /* newln: */
/* 00df61 */ 0x20, 0x39, 0xdd,        /*  jsr  outnl      ;   yes: discard entire line */
 /* inln: */
/* 00df64 */ 0xa0, 0x00,              /*  ldy  #<linbuf   ; entry point: start a fresh line */
/* 00df66 */ 0x84, 0x80,              /*  sty  at         ; {@} -> input line buffer */
/* 00df68 */ 0xa0, 0x02,              /*  ldy  #>linbuf */
/* 00df6a */ 0x84, 0x81,              /*  sty  at+1 */
/* 00df6c */ 0xa0, 0x01,              /*  ldy  #1 */
 /* inln5: */
/* 00df6e */ 0x88,                    /*  dey */
/* 00df6f */ 0x30, 0xf0,              /*  bmi  newln */
 /* inln2: */
/* 00df71 */ 0x20, 0xdf, 0xdf,        /*  jsr  inch       ; get (and echo) one key press */
/* 00df74 */ 0xc9, 0x08,              /*  cmp  #BS        ; backspace? */
/* 00df76 */ 0xf0, 0xf6,              /*  beq  inln5      ;   yes: delete previous char */
/* 00df78 */ 0xc9, 0x0d,              /*  cmp  #$0d       ; cr? */
/* 00df7a */ 0xd0, 0x02,              /*  bne  inln3 */
/* 00df7c */ 0xa9, 0x00,              /*  lda  #0         ;   yes: replace with null */
 /* inln3: */
/* 00df7e */ 0x91, 0x80,              /*  sta  (at),y     ; put key in linbuf */
/* 00df80 */ 0xd0, 0xd8,              /*  bne  inln6      ; continue if not null */
/* 00df82 */ 0xa8,                    /*  tay             ; y = 0 */
/* 00df83 */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; Find the first/next stored program line >= {#} */
 /* ; entry:   (cc): start search at program beginning */
 /* ;          (cs): start search at next line */
 /* ;          ({@} -> beginning of current line) */
 /* ; used by: skp2:, findln: */
 /* ; uses:    prgm, {@ # & (} */
 /* ; exit:    (cs): {@}, x:a and {(} undefined, y = 2 */
 /* ;          (cc): {@} -> beginning of found line, y = 2, */
 /* ;                x:a = {(} = actual found line number */
 /* ; 62 bytes */
 /* find: */
/* 00df84 */ 0xa2, 0x02,              /*  ldx  #>prgm */
/* 00df86 */ 0xa9, 0x20,              /*  lda  #<prgm */
/* 00df88 */ 0x90, 0x15,              /*  bcc  find1st    ; cc: search begins at first line */
/* 00df8a */ 0xa6, 0x81,              /*  ldx  at+1 */
/* 00df8c */ 0xa0, 0x02,              /*  ldy  #2 */
 /* findnxt: */
/* 00df8e */ 0xa5, 0x80,              /*  lda  at */
/* 00df90 */ 0xc5, 0xcc,              /*  cmp  ampr */
/* 00df92 */ 0xa5, 0x81,              /*  lda  at+1 */
/* 00df94 */ 0xe5, 0xcd,              /*  sbc  ampr+1     ; {@} >= {&} (end of program)? */
/* 00df96 */ 0xb0, 0x29,              /*  bcs  findrts    ;   yes: search failed (cs) */
 /* find3: */
/* 00df98 */ 0xa5, 0x80,              /*  lda  at */
/* 00df9a */ 0x71, 0x80,              /*  adc  (at),y     ;   no: {@} -> next line */
/* 00df9c */ 0x90, 0x03,              /*  bcc  find5 */
/* 00df9e */ 0xe8,                    /*  inx */
 /* find1st: */
/* 00df9f */ 0x86, 0x81,              /*  stx  at+1 */
 /* find5: */
/* 00dfa1 */ 0x85, 0x80,              /*  sta  at */
/* 00dfa3 */ 0xa0, 0x00,              /*  ldy  #0 */
/* 00dfa5 */ 0xb1, 0x80,              /*  lda  (at),y */
/* 00dfa7 */ 0x85, 0xd0,              /*  sta  lparen     ; {(} = current line number */
/* 00dfa9 */ 0xc5, 0xc6,              /*  cmp  pound      ;   (invalid if {@} >= {&}, but */
/* 00dfab */ 0xc8,                    /*  iny             ;   we'll catch that later...) */
/* 00dfac */ 0xb1, 0x80,              /*  lda  (at),y */
/* 00dfae */ 0x85, 0xd1,              /*  sta  lparen+1 */
/* 00dfb0 */ 0xe5, 0xc7,              /*  sbc  pound+1    ; if {(} < {#} then try the next */
/* 00dfb2 */ 0xc8,                    /*  iny             ;   program line */
/* 00dfb3 */ 0x90, 0xd9,              /*  bcc  findnxt */
/* 00dfb5 */ 0xa5, 0x80,              /*  lda  at         ; {@} >= {&} (end of program)? */
/* 00dfb7 */ 0xc5, 0xcc,              /*  cmp  ampr       ;   yes: search failed (cs) */
/* 00dfb9 */ 0xa5, 0x81,              /*  lda  at+1       ;   no: search succeeded (cc) */
/* 00dfbb */ 0xe5, 0xcd,              /*  sbc  ampr+1 */
/* 00dfbd */ 0xa5, 0xd0,              /*  lda  lparen */
/* 00dfbf */ 0xa6, 0xd1,              /*  ldx  lparen+1 */
 /* findrts: */
/* 00dfc1 */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /* ; Fetch a byte at @[y], ignoring space characters */
 /* ; 10 bytes */
 /* skpbyte: */
/* 00dfc2 */ 0xc8,                    /*  iny             ; skip over current char */
 /* getbyte: */
/* 00dfc3 */ 0xb1, 0x80,              /*  lda  (at),y */
/* 00dfc5 */ 0xf0, 0x04,              /*  beq  getbyt2 */
/* 00dfc7 */ 0xc9, 0x20,              /*  cmp  #' ' */
/* 00dfc9 */ 0xf0, 0xf7,              /*  beq  skpbyte    ; skip over any space char(s) */
 /* getbyt2: */
/* 00dfcb */ 0x60,                    /*  rts */
 /* ;============ Original I/O subroutines ===============; */
 /* ;-----------------------------------------------------; */
 /* ; Check for user keypress and return with (cc) if none */
 /* ;   is pending.  Otherwise, fall through to inch */
 /* ;   and return with (cs). */
 /* ; 6 bytes */
 /* ;inkey: */
 /* ;    lda  KBD        ; is there a keypress waiting? */
 /* ;    asl */
 /* ;    bcc  outrts     ;   no: return with (cc) */
 /* ; - - - - - - - - - - - - - - - - - - - - - - - - - - ; */
 /* ; Read key from stdin into a, echo, (cs) */
 /* ; drop stack and abort to "OK" prompt if ctrl-C */
 /* ; 16 bytes */
 /* ;inch: */
 /* ;    sty  dolr       ; save y reg */
 /* ;    jsr  KEYIN      ; get a char from keyboard */
 /* ;    ldy  dolr       ; restore y reg */
 /* ;    and  #$7f       ; strip apple's hi-bit */
 /* ;    cmp  #$03       ; ctrl-C? */
 /* ;    bne  outch      ;   no: echo to terminal */
 /* ;    jmp  start      ;   yes: abort to "OK" prompt */
 /* ; - - - - - - - - - - - - - - - - - - - - - - - - - - ; */
 /* ; Print ASCII char in a to stdout, (cs) */
 /* ; 9 bytes */
 /* ;outch: */
 /* ;    pha             ; save original char */
 /* ;    ora  #$80       ; apples prefer "high" ASCII */
 /* ;    jsr  COUT       ; emit char via apple monitor */
 /* ;    pla             ; restore original char */
 /* ;    sec             ; (by contract with callers) */
 /* ;outrts: */
 /* ;    rts */
 /* ;-----------------------------------------------------; */
 /* ;========== 2m5 SBC emulator I/O subroutines ============; */
 /* ;-----------------------------------------------------; */
 /* ; Check for user keypress and return if none */
 /* ;   is pending.  Otherwise, check for ctrl-C and */
 /* ;   return after next keypress. */
 /* ; */
 /* inkey: */
/* 00dfcc */ 0xad, 0xf4, 0xcf,        /*  lda  acia_rx    ; Is there a character waiting? */
/* 00dfcf */ 0xf0, 0x0d,              /*  beq  inkeyr     ;   no: return */
/* 00dfd1 */ 0xc9, 0x03,              /*  cmp  #3         ; is ctrl-c */
/* 00dfd3 */ 0xf0, 0x17,              /*  beq  istart     ;   yes: abort to OK prompt */
 /* inkeyp: */
/* 00dfd5 */ 0xad, 0xf4, 0xcf,        /*  lda  acia_rx    ; pause until next key */
/* 00dfd8 */ 0xf0, 0xfb,              /*  beq  inkeyp */
/* 00dfda */ 0xc9, 0x03,              /*  cmp  #3         ; is ctrl-c */
/* 00dfdc */ 0xf0, 0x0e,              /*  beq  istart     ;   yes: abort to OK prompt */
 /* inkeyr: */
/* 00dfde */ 0x60,                    /*  rts */
 /* ; - - - - - - - - - - - - - - - - - - - - - - - - - - ; */
 /* ; Read key from stdin into a, echo, (cs) */
 /* ; Dump stack and abort to "OK" prompt if ctrl-C */
 /* ; 16 bytes */
 /* inch: */
/* 00dfdf */ 0xad, 0xf4, 0xcf,        /*  lda  acia_rx    ; get character from rx register */
/* 00dfe2 */ 0xf0, 0xfb,              /*  beq  inch       ; wait for character !=0 */
/* 00dfe4 */ 0xc9, 0x0a,              /*  cmp  #10        ; remove line feed to allow paste */
/* 00dfe6 */ 0xf0, 0xf7,              /*  beq  inch       ; in the Kowalski I/O window */
 /* inch2: */
/* 00dfe8 */ 0xc9, 0x03,              /*  cmp  #$03       ; ctrl-C? */
/* 00dfea */ 0xd0, 0x03,              /*  bne  outch      ;   no: echo to terminal */
 /* istart: */
/* 00dfec */ 0x4c, 0x11, 0xdc,        /*  jmp  start      ;   yes: abort to "OK" prompt */
 /* ; - - - - - - - - - - - - - - - - - - - - - - - - - - ; */
 /* ; Print ascii char in a to stdout, (cs) */
 /* ; 16 bytes */
 /* outch: */
/* 00dfef */ 0xc9, 0x0d,              /*  cmp  #13        ; add line feed to carriage return */
/* 00dff1 */ 0xd0, 0x07,              /*  bne  skip_cr */
/* 00dff3 */ 0xa9, 0x0a,              /*  lda  #10 */
/* 00dff5 */ 0x8d, 0xf1, 0xcf,        /*  sta  acia_tx */
/* 00dff8 */ 0xa9, 0x0d,              /*  lda  #13 */
 /* skip_cr: */
/* 00dffa */ 0x8d, 0xf1, 0xcf,        /*  sta  acia_tx    ; emit char via transmit register */
/* 00dffd */ 0x38,                    /*  sec             ; (by contract with callers) */
/* 00dffe */ 0x60,                    /*  rts */
 /* ;-----------------------------------------------------; */
 /*     .end vtl02c     ; set start address */
/* Total bytes read (size of array) 1023 dec, 3ff hex */
