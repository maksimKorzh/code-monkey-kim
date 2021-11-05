; Added:
CR      =   $8D
ESC     =   $9B
BS      =   $DF   ; _
PROMPT  =   $DC
; -----
;======================================
;    ** ENHANCED WOZMON EXTENSION **
;    MINI-ASSEMBLER AND DISASSEMBLER
;       FOR THE APPLE 1 COMPUTER
;  BASED ON THE CODE OF STEVE WOZNIAK
;       AND ALLEN BAUM, 1976/1977
; MANUALLY AND LOVINGLY ADAPTED BY THE
;    HAND OF MICHAEL T. BARRY, 2020
;    MODIFICATIONS MADE IN 2020 ARE
;     FREE TO USE WITH ATTRIBUTION
;        <<< 6502 FOREVER!! >>>
;======================================
XAML     =   $14
XAMH     =   $15
STL      =   $16
STH      =   $17
L        =   $18   ;($35 ON A2)
H        =   $19
YSAV     =   $1A   ;($34 ON A2)
MODE     =   $1B   ;($31 ON A2)
;
FORMAT   =   $05
LENGTH   =   $06
FMT      =   $07
LMNEM    =   $08
RMNEM    =   $09
PCL      =   $0A
PCH      =   $0B
A1L      =   $0C
A1H      =   $0D
A2L      =   $0E
A2H      =   $0F
A4L      =   $12
A4H      =   $13
;
IN       =   $0200
;
KBD      =   $D010
KBDCR    =   $D011
DSP      =   $D012
DSPCR    =   $D013

;ESCAPE   =   $3F1A
;
         .ORG   $FBAE
;======================================
LIST:             ; ($FE5E ON A2)
   LDA   #20      ; DISASSEM 20 INSTR
LIST1:
   PHA            ; STACK LOOP COUNTER
   JSR   INSTDSP  ; DISASSEM ONE INSTR
   JSR   PCADJ    ; UPDATE VIRTUAL PC
   STA   PCL      ;
   STY   PCH      ;
   PLA            ; UPDATE LOOP COUNTER
   SEC            ;
   SBC   #1       ;
   BNE   LIST1    ; LOOP FOR 20 INSTRS
CMDLINE:
   JSR   GETLNZ   ; INPUT A FRESH LINE
   TXA            ; NULL INPUT?
   BNE   CMDLIN2  ;
   JMP   ESCAPE   ; YES: BACK TO WOZMON
CMDLIN2:
   LDY   #0       ; START AT 1ST CHAR
   JSR   GETNUM   ; PARSE OPTIONAL ADDR
   PHA            ;
   TXA            ; EMPTY ADDR FIELD?
   BEQ   CMDLIN3  ; YES: USE DEFAULT PC
A1PCLP:
   LDA   A1L,X    ; ASSUMES X=1
   STA   PCL,X    ; COPY FROM A1 TO PC
   DEX            ;
   BPL   A1PCLP   ;
CMDLIN3:
   PLA            ;
   CMP   #$05     ; 'L' COMMAND?
   BEQ   LIST     ; YES: DISASSEMBLE
   CMP   #$99     ; ' ' COMMAND?
   BEQ   ASSEM    ; YES: ASM AT CURR PC
   CMP   #$93     ; ':' COMMAND?
   BNE   FORM8A   ; NO: ERROR
ASSEM:
   LDA   #3       ; # OF CHARS IN MNEM
   STA   A1H      ;
NXTMN:
   JSR   GETNSP   ; GET A MNEM CHAR
NXTM:
   ASL            ;
   SBC   #$BE     ; SUBTRACT OFFSET
   CMP   #$C2     ; LEGAL CHAR?
   BCC   FORM8A   ; NO
   ASL            ; COMPRESS/LT-JUSTIFY
   ASL            ;
   LDX   #4       ;
NXTM2:
   ASL            ; DO 5 24-BIT SHIFTS
   ROL   A4L      ;
   ROL   A4H      ;
   DEX            ;
   BPL   NXTM2    ;
   DEC   A1H      ; DONE WITH 3 CHARS?
   BEQ   NXTM2    ; YES: ONE LAST SHIFT
   BPL   NXTMN    ; NO
FORM1:
   LDX   #5       ; 5 CHARS IN ADR MODE
FORM2:
   JSR   GETNSP   ; GET 1ST CHR OF ADDR
   STY   YSAV     ;
   CMP   CHR1,X   ; 1ST CHR MATCH PATT?
   BNE   FORM3    ; NO
   JSR   GETNSP   ; YES: GET 2ND CHAR
   CMP   CHR2,X   ; MATCHES 2ND HALF?
   BEQ   FORM5    ; YES
   LDA   CHR2,X   ; NO: IS 2ND HALF 0?
   BEQ   FORM4    ; YES
   CMP   #$A4     ; NO: 2ND HALF OPT?
   BEQ   FORM4    ; YES
   LDY   YSAV     ;
FORM3:
   CLC            ; CLEAR BIT-NO MATCH
FORM4:
   DEY            ; BACK UP 1 CHAR
FORM5:
   ROL   FMT      ; FORM FORMAT BYTE
   CPX   #3       ; TIME TO CHECK ADDR?
   BNE   FORM7    ; NO
   JSR   GETNUM   ; YES: PARSE IT
   LDA   A2H      ;
   BEQ   FORM6    ; HIGH-ORDER BYTE 0?
   INX            ; NO: INCR FOR 2-BYTE
FORM6:
   STX   L        ; STORE LENGTH
   LDX   #3       ; RELOAD FMT INDEX
   DEY            ; BACK UP 1 CHAR
FORM7:
   STX   A1H      ; SAVE INDEX
   DEX            ; DONE W/FMT CHECK?
   BPL   FORM2    ; NO
   LDA   FMT      ; YES: PUT LENGTH
   ASL            ;   IN LOW BITS
   ASL            ;
   ORA   L        ;
   CMP   #$20     ;
   BCS   FORM8    ;
   LDX   L        ;
   BEQ   FORM8    ;
   ORA   #$80     ;
FORM8:
   STA   FMT      ;
   STY   YSAV     ;
   LDA   IN,Y     ; GET NEXT NONBLANK
   CMP   #$BB     ; START OF COMMENT?{;}
   BEQ   TRYNEXT  ; YES
   CMP   #CR      ; CR?
FORM8A:
   BNE   ERR2     ; NO: ERROR
TRYNEXT:          ; ($F55C ON A2)
   LDA   A1H      ; GET TRIAL OPCODE
   JSR   INSDS2   ; GET FMT+LEN FOR IT
   TAX            ;
   LDA   MNEMR,X  ; GET LOWER MNEM BYTE
   CMP   A4L      ; MATCH?
   BNE   NEXTOP   ; NO: TRY NEXT OPCODE
   LDA   MNEML,X  ; GET UPPER MNEM BYTE
   CMP   A4H      ; MATCH?
   BNE   NEXTOP   ; NO: TRY NEXT OPCODE
   LDA   FMT      ;
   LDY   FORMAT   ; GET TRIAL FORMAT
   CPY   #$9D     ; IS IT RELATIVE?
   BEQ   REL      ; YES
   CMP   FORMAT   ; SAME FORMAT?
   BEQ   FINDOP   ; YES
NEXTOP:
   DEC   A1H      ; NO: TRY NEXT OPCODE
   BNE   TRYNEXT  ;
   INC   FMT      ; NO MORE: TRY LEN=2
   DEC   L        ; WAS L=2 ALREADY?
   BEQ   TRYNEXT  ; NO: KEEP TRYING
ERR:
   LDY   YSAV     ; YES: UNREC. INST
ERR2:
   TYA            ;
   TAX            ;
   JSR   CROUT    ;
   JSR   PRXBL    ; PRINT '^' BELOW THE
   LDA   #$DE     ;   LAST PARSED CHAR{^}
   JSR   ECHO     ;   TO SHOW ERR POSN
   JMP   CMDLINE  ; NEXT COMMAND LINE
REL:
   SBC   #$81     ; YES: IS FMT COMPAT
   LSR            ;   W/RELATIVE MODE?
   BNE   ERR      ; NO: ERROR OUT
   LDY   A2H      ;
   LDX   A2L      ; DOUBLE DECREMENT
   BNE   REL2     ;
   DEY            ;
REL2:
   DEX            ;
   TXA            ;
   CLC            ;
   SBC   PCL      ; CALC ADDR-PC-2
   STA   A2L      ;
   BPL   REL3     ;
   INY            ;
REL3:
   TYA            ;
   SBC   PCH      ;
   BNE   ERR      ; ERROR IF TOO FAR
FINDOP:
   LDY   LENGTH   ;
FNDOP2:
   LDA   A1H,Y    ; MOVE INST TO (PC)
   STA   (PCL),Y  ;
   DEY            ;
   BPL   FNDOP2   ;
   LDA   #1       ; DISASSEMBLE RESULT
   JMP   LIST1    ; BACK TO CMD LINE
;=  92 ================================
INSDS1:           ; ($F882 ON A2)
   LDX   PCL      ; PRINT PCL,H
   LDY   PCH      ;
   JSR   CROUT    ;
   JSR   PRNTYX   ;
   LDY   #0       ;
   LDA   #$AD     ; PRINT '-'
   JSR   ECHO     ;
   JSR   PR3BL    ; PRINT 3 BLANKS
   LDA   (PCL,X)  ; GET OPCODE
INSDS2:           ; ($F88E ON A2)
   TAY            ;
   LSR            ; EVEN/ODD TEST
   BCC   IEVEN    ;
   LSR            ; BIT 1 TEST
   ORA   #$80     ;   (BECAUSE ROR BUG)
   BCS   INVLD    ; XXXXXX11 INVALID OP
   CMP   #$A2     ;
   BEQ   INVLD    ; OPCODE $89 INVALID
   AND   #$87     ; MASK BITS
IEVEN:            ; ($F89B ON A2)
   LSR            ; LSB->C FOR L/R TEST
   TAX            ;
   LDA   FMT1,X   ; GET FMT INDEX BYTE
   BCC   RTMSKZ   ; R/L H-BYTE ON CARRY
   LSR            ;
   LSR            ;
   LSR            ;
   LSR            ;
RTMSKZ:
   AND   #$0F     ;
   BNE   GETFMT   ;
INVLD:
   LDY   #$80     ; SUB $80 FOR INVALID
   LDA   #0       ; SET PRINT FMT INDEX
GETFMT:           ; ($F8A9 ON A2)
   TAX            ;
   LDA   FMT2,X   ; INDX INTO FMT TABLE
   STA   FORMAT   ; SAVE FOR ADR FLD
   AND   #$03     ; MASK FOR 2-BIT LEN
   STA   LENGTH   ;
   TYA            ; OPCODE
   AND   #$8F     ; MASK: 1XXX1010 TEST
   TAX            ; SAVE IT
   TYA            ; OPCODE TO A AGAIN
   LDY   #$03     ;
   CPX   #$8A     ;
   BEQ   MNNDX3   ;
MNNDX1:
   LSR            ;
   BCC   MNNDX3   ; FORM IDX INTO MNEM
   LSR            ;
MNNDX2:
   LSR            ; 1 1XXX1010>00101XXX
   ORA   #$20     ; 2 XXXYYY01>00111XXX
   DEY            ; 3 XXXYYY10>00110XXX
   BNE   MNNDX2   ; 4 XXXYY100>00100XXX
   INY            ; 5 XXXXX000>000XXXXX
MNNDX3:
   DEY            ;
   BNE   MNNDX1   ;
   RTS            ;
;= 120 ================================
INSTDSP:          ; ($F8D0 ON A2)
   JSR   INSDS1   ; GEN FMT, LEN BYTES
   PHA            ; SAVE MNEM TABLE IDX
PRNTOP:
   LDA   (PCL),Y  ;
   JSR   PRBYTE   ;
   LDX   #1       ; PRINT 2 BLANKS
PRNTBL:
   JSR   PRXBL    ;
   CPY   LENGTH   ; PRINT INSTRUCTION
   INY            ; IN A 12 CHAR FIELD
   BCC   PRNTOP   ;
   LDX   #3       ; MNEM CHAR COUNT
   CPY   #4       ;
   BCC   PRNTBL   ;
   PLA            ; RECOVER MNEM INDEX
   TAY            ;
   LDA   MNEML,Y  ;
   STA   LMNEM    ; FETCH 3-CHAR MNEM
   LDA   MNEMR,Y  ;   PACKED IN 2-BYTES
   STA   RMNEM    ;
PRMN1:
   LDA   #0       ;
   LDY   #5       ;
PRMN2:
   ASL   RMNEM    ; SHIFT 5 BITS OF
   ROL   LMNEM    ;   CHARACTER INTO A
   ROL            ;     (CLEARS CARRY)
   DEY            ;
   BNE   PRMN2    ;
   ADC   #$BF     ; ADD "?" OFFSET {?}
   JSR   ECHO     ; SEND A CHAR OF MNEM
   DEX            ;
   BNE   PRMN1    ;
   JSR   PR3BL    ; PRINT 3 BLANKS
   LDY   LENGTH   ;
   LDX   #6       ; CNT FOR 6 FMT BITS
PRADR1:
   CPX   #3       ;
   BEQ   PRADR5   ; IF X=3 THEN ADDR
PRADR2:
   ASL   FORMAT   ;
   BCC   PRADR3   ;
   LDA   CHR1-1,X ;
   JSR   ECHO     ;
   LDA   CHR2-1,X ;
   BEQ   PRADR3   ;
   JSR   ECHO     ;
PRADR3:
   DEX            ;
   BNE   PRADR1   ;
   RTS            ;
PRADR4:
   DEY            ;
   BMI   PRADR2   ;
   JSR   PRBYTE   ;
PRADR5:
   LDA   FORMAT   ;
   CMP   #$E8     ; HANDLE REL ADR MODE
   LDA   (PCL),Y  ; SPECIAL (SND TARGET
   BCC   PRADR4   ;   ADDR, NOT OFFSET)
   JSR   PCADJ3   ;
   TAX            ; PCL,H+OFFSET+1
   INX            ;   TO A,Y
   BNE   PRNTYX   ; +1 TO Y,X
   INY            ;
PRNTYX:
   TYA            ;
PRNTAX:
   JSR   PRBYTE   ; PRINT BRANCH TARGET
   TXA            ;
   JMP   PRBYTE   ;
;=  48 ================================
NOTACR:
   CMP   #ESC     ; ESC?
   BEQ   CANCEL   ; YES: START OVER
   JSR   ECHO     ; PRINT CHAR
   CMP   #$DF     ; {_}
   BEQ   BCKSPC   ;
   INX            ;
   BPL   NXTCHAR  ; 128 CHARS MAX
CANCEL:
   LDA   #$DC     ; {\}
   JSR   ECHO     ;
GETLNZ:           ; ($FD67 ON A2)
   JSR   CROUT    ; FRESH LINE
GETLINE:
   LDA   #$A1     ; PROMPT CHAR {!}
   JSR   ECHO     ;
   LDX   #1       ;
BCKSPC:
   DEX            ;
   BMI   CANCEL   ;
NXTCHAR:
   BIT   KBDCR    ; WAIT FOR KEY PRESS
   BPL   NXTCHAR  ; NO KEY YET
   LDA   KBD      ; LOAD CHAR
   STA   IN,X     ; STORE IN BUFFER
   CMP   #CR      ; CR?
   BNE   NOTACR   ; NO: KEEP GOING
   RTS            ; YES: X=BUFLEN
;=  11 ================================
PR3BL:            ; ($F948 ON A2)
   LDX   #3       ; BLANK COUNT
PRXBL:
   LDA   #$A0     ; LOAD A SPACE CHAR { }
   JSR   ECHO     ; PRINT IT
   DEX            ;
   BNE   PRXBL    ; LOOP UNTIL COUNT=0
   RTS            ;
;=   9 ================================
GETNSP:           ; ($F634 ON A2)
   LDA   IN,Y     ;
   INY            ;
   CMP   #$A0     ; SKIP OVER SPACES { }
   BEQ   GETNSP   ;
   RTS            ;
;=   3 ================================
XMON:             ;
   JMP   CMDLINE  ;
;=   5 ================================
CROUT:            ; ($FD8E ON A2)
   LDA   #CR      ;
   JMP   ECHO     ;
;=  15 ================================
PCADJ:            ; ($F953 ON A2)
   SEC            ; 0=1-BYTE, 1=2-BYTE,
   LDA   LENGTH   ;   2=3-BYTE INST
PCADJ3:
   LDY   PCH      ;
   TAX            ; TEST DISPLCMNT SIGN
   BPL   PCADJ4   ;   (FOR REL BRANCH)
   DEY            ; EXT NEG BY DEC PCH
PCADJ4:
   ADC   PCL      ; PCL + LEN(DSPL) + 1
   BCC   PCADJ5   ;   TO A (PCL),
   INY            ;   CARRY TO Y (PCH)
PCADJ5:
   RTS            ;
;=  46 ================================
GETNUM:           ; ($FFA7 ON A2)
   LDX   #0       ; CLEAR VALID # FLAG
   STX   A2L      ; CLEAR A2
   STX   A2H      ;
   BEQ   NXTCXAR  ;
DIG:              ; ($FF8A ON A2)
   LDX   #3       ;
   ASL            ;
   ASL            ; GOT HEX DIGIT,
   ASL            ;   SHIFT INTO A2
   ASL            ;
NXTBIT:
   ASL            ;
   ROL   A2L      ;
   ROL   A2H      ;
   DEX            ; LEAVE X=$FF IF DIG
   BPL   NXTBIT   ;
NXTBAS:
   LDA   A2H,X    ; COPY A2 TO A1
   STA   A1H,X    ;
NXTBS2:
   INX            ;
   BEQ   NXTBAS   ;
NXTCXAR:          ; ($FFAD ON A2)
   LDA   IN,Y     ; GET CHAR
   INY            ; ADVANCE INDEX
   EOR   #$B0     ;
   CMP   #10      ; DECIMAL DIGIT?
   BCC   DIG      ; YES: ACCUMULATE
   ADC   #$88     ;
   CMP   #$FA     ; 'A'-'F'
   BCS   DIG      ; YES: ACCUMULATE
   RTS            ; NO: RETURN WITH
                  ;   X=0: NO NUMBER
                  ;   X=1: NUMBER
;= 222 ================================
FMT1:             ; ($F962 ON A2)
; FMT1 BYTES: XXXXXXY0 INSTRS
; IF Y=0 THEN LEFT HALF BYTE
; IF Y=1 THEN RIGHT HALF BYTE
; (X=INDEX)
.byte $04, $20 ,$54 ,$30 ,$0D ,$80 ,$04 ,$90

.byte $03 ,$22 ,$54 ,$33 ,$0D ,$80 ,$04 ,$90

.byte $04 ,$20 ,$54 ,$33 ,$0D ,$80 ,$04 ,$90

.byte $04 ,$20 ,$54 ,$3B ,$0D ,$80 ,$04 ,$90

.byte $00 ,$22 ,$44 ,$33 ,$0D ,$C8 ,$44 ,$00

.byte $11 ,$22 ,$44 ,$33 ,$0D ,$C8 ,$44 ,$A9

.byte $01 ,$22 ,$44 ,$33 ,$0D ,$80 ,$04 ,$90

.byte $01 ,$22 ,$44 ,$33 ,$0D ,$80 ,$04 ,$90

; $ZZXXXY01 INSTRS
.byte $26 ,$31 ,$87 ,$9A
FMT2:             ; ($F9A6 ON A2)
; ERR IMM ZP ABS IMP ACC (ZP,X) (ZP),Y
; ZP,X ABS,X ABS,Y (ABS) ZP,Y REL
.byte $00 ,$21 ,$81 ,$82 ,$00 ,$00 ,$59 ,$4D

.byte $91 ,$92 ,$86 ,$4A ,$85 ,$9D

CHR1:             ; ($F9B4 ON A2)
; ",),#($"
.byte $AC ,$A9 ,$AC ,$A3 ,$A8 ,$A4

CHR2:             ; ($F9BA ON A2)
; "Y",0,"X$$",0
.byte $D9 ,$00 ,$D8 ,$A4 ,$A4 ,$00

MNEML:            ; ($F9C0 ON A2)
; MNEML IS OF FORM:
; (A) XXXXX000
; (B) XXXYY100
; (C) 1XXX1010
; (D) XXXYYY10
; (E) XXXYYY01
; (X=INDEX)
.byte $1C ,$8A ,$1C ,$23 ,$5D ,$8B ,$1B ,$A1

.byte $9D ,$8A ,$1D ,$23 ,$9D ,$8B ,$1D ,$A1

.byte $00 ,$29 ,$19 ,$AE ,$69 ,$A8 ,$19 ,$23

.byte $24 ,$53 ,$1B ,$23 ,$24 ,$53 ,$19 ,$A1

.byte $00 ,$1A ,$5B ,$5B ,$A5 ,$69 ,$24 ,$24

.byte $AE ,$AE ,$A8 ,$AD ,$29 ,$00 ,$7C ,$00

.byte $15 ,$9C ,$6D ,$9C ,$A5 ,$69 ,$29 ,$53

.byte $84 ,$13 ,$34 ,$11 ,$A5 ,$69 ,$23 ,$A0

MNEMR:
.byte $D8 ,$62 ,$5A ,$48 ,$26 ,$62 ,$94 ,$88

.byte $54 ,$44 ,$C8 ,$54 ,$68 ,$44 ,$E8 ,$94

.byte $00 ,$B4 ,$08 ,$84 ,$74 ,$B4 ,$28 ,$6E

.byte $74 ,$F4 ,$CC ,$4A ,$72 ,$F2 ,$A4 ,$8A

.byte $00 ,$AA ,$A2 ,$A2 ,$74 ,$74 ,$74 ,$72

.byte $44 ,$68 ,$B2 ,$32 ,$B2 ,$00 ,$22 ,$00

.byte $1A ,$1A ,$26 ,$26 ,$72 ,$72 ,$88 ,$C8

.byte $C4 ,$CA ,$26 ,$48 ,$44 ,$44 ,$A2 ,$C8

;======================================
; WOZMON
RESET:
   CLD            ; CLEAR DECIMAL MODE
   CLI            ; ENABLE INTERRUPTS
   LDY   #$7F     ; DSP DIRECTION MASK
   STY   DSP      ; (DDR MODE ASSUMED)
   LDA   #$A7     ; KBD & DSP CTRL MASK
   STA   KBDCR    ; ENABLE IRQS, SET
   STA   DSPCR    ;  CA1,CB1 FOR POS
                  ; EDGE SENSE/OUT MODE
NOTCR:
   CMP   #BS      ; BACKSPACE KEY?
   BEQ   BACKSP   ;
   CMP   #ESC     ; ESC KEY?
   BEQ   ESCAPE   ;
   INY            ; ADVANCE TEXT INDEX
   BPL   NXTCHR   ; ESC IF LINE TOO BIG
ESCAPE:
   LDA   #PROMPT  ; PRINT PROMPT CHAR
   JSR   ECHO     ;
GETLIN:
   LDA   #CR      ; PRINT CR
   JSR   ECHO     ;
   LDY   #1       ; START NEW LINE
BACKSP:
   DEY            ; BACK UP INDEX
   BMI   GETLIN   ; REINIT IF EMPTY
NXTCHR:
   LDA   KBDCR    ; WAIT FOR KEY PRESS
   BPL   NXTCHR   ; NO KEY YET
   LDA   KBD      ; LOAD CHAR
   STA   IN,Y     ; ADD TO TEXT BUFFER
   JSR   ECHO     ; PRINT CHAR
   CMP   #CR      ;
   BNE   NOTCR    ;
   LDY   #$FF     ;
   LDA   #0       ; DEFAULT MODE IS XAM
   TAX            ;
STSTOR:
   ASL            ;
STMODE:
   STA   MODE     ; SET MODE FLAGS
BLSKIP:
   INY            ; ADVANCE TEXT INDEX
NXTITM:
   LDA   IN,Y     ; GET CHAR
   CMP   #CR      ;
   BEQ   GETLIN   ; DONE IF CR
   CMP   #$AE     ; {.}
   BCC   BLSKIP   ; IGNORE CHAR < "."
   BEQ   STMODE   ; SET BLOCK XAM MODE
   CMP   #$BA     ; {:}
   BEQ   STSTOR   ; SET STOR MODE
   CMP   #$D2     ;{R}
   BEQ   RUN      ; RUN THE PROGRAM
   STX   L        ; CLEAR INPUT VALUE
   STX   H        ;
   STY   YSAV     ; SAVE Y FOR COMPARE
NXTHEX:           ; (OV ADDED)
   LDA   IN,Y     ; GET CHR FOR HEX TST
   EOR   #$B0     ; MAP DIGITS TO 0-9
   CMP   #10      ; DECIMAL DIGIT?
   BCC   DIGX
   ADC   #$88     ; "A"-"F" -> $FA-FF
   CMP   #$FA     ; HEX LETTER?
   BCC   NOTHEX   ;
DIGX:
   ASL            ;
   ASL            ;
   ASL            ;
   ASL            ;
   LDX   #4       ;
HXSHIF:
   ASL            ;
   ROL   L        ;
   ROL   H        ;
   DEX            ;
   BNE   HXSHIF   ;
   INY            ;
   BNE   NXTHEX   ; NXTHEX, D0 E0
NOTHEX:
   CPY   YSAV     ;
   BEQ   ESCAPE   ;
   BIT   MODE     ;
   BVC   NOTSTO   ;
   LDA   L        ;
   STA   (STL,X)  ;
   INC   STL      ;
   BNE   NXTITM   ;
   INC   STH      ;
TONEXT:
   JMP   NXTITM   ;
RUN:              ; RUN USER'S PROGRAM
   JMP   (XAML)
NOTSTO:
   BMI   XAMNXT   ; XAMNXT, 30 2B
   LDX   #2       ; COPY 2 BYTES
SETADR:
   LDA   L-1,X
   STA   STL-1,X
   STA   XAML-1,X
   DEX
   BNE   SETADR
NXTPRN:           ; PRINT ADDR AND DATA
   BNE   PRDATA
   LDA   #$8D
   JSR   ECHO
   LDA   XAMH
   JSR   PRBYTE
   LDA   XAML
   JSR   PRBYTE
   LDA   #$BA     ;':'
   JSR   ECHO
PRDATA:
   LDA   #$A0     ;' '
   JSR   ECHO
   LDA   (XAML,X)
   JSR   PRBYTE
XAMNXT:
   STX   MODE
   LDA   XAML
   CMP   L
   LDA   XAMH
   SBC   H
   BCS   TONEXT
   INC   XAML
   BNE   MD8CHK
   INC   XAMH
MD8CHK:
   LDA   XAML
   AND   #$07
   BPL   NXTPRN
PRBYTE:
   PHA            ; PRINT A AS HEX BYTE
   LSR            ; (DESTRUCTIVE)
   LSR
   LSR
   LSR
   JSR   PRHEX
   PLA
PRHEX:
   AND   #$0F     ; PRINT A AS HEX DIG
   ORA   #$B0     ; '0'
   CMP   #$BA     ; '9'+1
   BCC   ECHO
   ADC   #6
ECHO:
   BIT   DSP      ; PRINT A AS ASCII
   BMI   ECHO
   STA   DSP
   RTS
;
.byte $FF ,$FF ,$00 ,$0F ,$00 ,$FF ,$00 ,$00


