reading miniFBAE.lst

assuming ca65 format, skipping leading 4 lines
and assuming max. 4 data bytes per line
/* 000000 */                          /* CR      =   $8D */
/* 000000 */                          /* ESC     =   $9B */
/* 000000 */                          /* BS      =   $DF   ; _ */
/* 000000 */                          /* PROMPT  =   $DC */
/* 000000 */                          /* ; ----- */
/* 000000 */                          /* ;====================================== */
/* 000000 */                          /* ;    ** ENHANCED WOZMON EXTENSION ** */
/* 000000 */                          /* ;    MINI-ASSEMBLER AND DISASSEMBLER */
/* 000000 */                          /* ;       FOR THE APPLE 1 COMPUTER */
/* 000000 */                          /* ;  BASED ON THE CODE OF STEVE WOZNIAK */
/* 000000 */                          /* ;       AND ALLEN BAUM, 1976/1977 */
/* 000000 */                          /* ; MANUALLY AND LOVINGLY ADAPTED BY THE */
/* 000000 */                          /* ;    HAND OF MICHAEL T. BARRY, 2020 */
/* 000000 */                          /* ;    MODIFICATIONS MADE IN 2020 ARE */
/* 000000 */                          /* ;     FREE TO USE WITH ATTRIBUTION */
/* 000000 */                          /* ;        <<< 6502 FOREVER!! >>> */
/* 000000 */                          /* ;====================================== */
/* 000000 */                          /* XAML     =   $14 */
/* 000000 */                          /* XAMH     =   $15 */
/* 000000 */                          /* STL      =   $16 */
/* 000000 */                          /* STH      =   $17 */
/* 000000 */                          /* L        =   $18   ;($35 ON A2) */
/* 000000 */                          /* H        =   $19 */
/* 000000 */                          /* YSAV     =   $1A   ;($34 ON A2) */
/* 000000 */                          /* MODE     =   $1B   ;($31 ON A2) */
/* 000000 */                          /* ; */
/* 000000 */                          /* FORMAT   =   $05 */
/* 000000 */                          /* LENGTH   =   $06 */
/* 000000 */                          /* FMT      =   $07 */
/* 000000 */                          /* LMNEM    =   $08 */
/* 000000 */                          /* RMNEM    =   $09 */
/* 000000 */                          /* PCL      =   $0A */
/* 000000 */                          /* PCH      =   $0B */
/* 000000 */                          /* A1L      =   $0C */
/* 000000 */                          /* A1H      =   $0D */
/* 000000 */                          /* A2L      =   $0E */
/* 000000 */                          /* A2H      =   $0F */
/* 000000 */                          /* A4L      =   $12 */
/* 000000 */                          /* A4H      =   $13 */
/* 000000 */                          /* ; */
/* 000000 */                          /* IN       =   $0200 */
/* 000000 */                          /* ; */
/* 000000 */                          /* KBD      =   $D010 */
/* 000000 */                          /* KBDCR    =   $D011 */
/* 000000 */                          /* DSP      =   $D012 */
/* 000000 */                          /* DSPCR    =   $D013 */
/* 000000 */                          /*  */
/* 000000 */                          /* ;ESCAPE   =   $3F1A */
/* 000000 */                          /* ; */
/* 000000 */                          /*          .ORG   $FBAE */
/* 00fbae */                          /* ;====================================== */
/* 00fbae */                          /* LIST:             ; ($FE5E ON A2) */
/* 00fbae */ 0xa9, 0x14,              /*    LDA   #20      ; DISASSEM 20 INSTR */
/* 00fbb0 */                          /* LIST1: */
/* 00fbb0 */ 0x48,                    /*    PHA            ; STACK LOOP COUNTER */
/* 00fbb1 */ 0x20, 0x21, 0xfd,        /*    JSR   INSTDSP  ; DISASSEM ONE INSTR */
/* 00fbb4 */ 0x20, 0xe5, 0xfd,        /*    JSR   PCADJ    ; UPDATE VIRTUAL PC */
/* 00fbb7 */ 0x85, 0x0a,              /*    STA   PCL      ; */
/* 00fbb9 */ 0x84, 0x0b,              /*    STY   PCH      ; */
/* 00fbbb */ 0x68,                    /*    PLA            ; UPDATE LOOP COUNTER */
/* 00fbbc */ 0x38,                    /*    SEC            ; */
/* 00fbbd */ 0xe9, 0x01,              /*    SBC   #1       ; */
/* 00fbbf */ 0xd0, 0xef,              /*    BNE   LIST1    ; LOOP FOR 20 INSTRS */
/* 00fbc1 */                          /* CMDLINE: */
/* 00fbc1 */ 0x20, 0xac, 0xfd,        /*    JSR   GETLNZ   ; INPUT A FRESH LINE */
/* 00fbc4 */ 0x8a,                    /*    TXA            ; NULL INPUT? */
/* 00fbc5 */ 0xd0, 0x03,              /*    BNE   CMDLIN2  ; */
/* 00fbc7 */ 0x4c, 0x1a, 0xff,        /*    JMP   ESCAPE   ; YES: BACK TO WOZMON */
/* 00fbca */                          /* CMDLIN2: */
/* 00fbca */ 0xa0, 0x00,              /*    LDY   #0       ; START AT 1ST CHAR */
/* 00fbcc */ 0x20, 0xf4, 0xfd,        /*    JSR   GETNUM   ; PARSE OPTIONAL ADDR */
/* 00fbcf */ 0x48,                    /*    PHA            ; */
/* 00fbd0 */ 0x8a,                    /*    TXA            ; EMPTY ADDR FIELD? */
/* 00fbd1 */ 0xf0, 0x07,              /*    BEQ   CMDLIN3  ; YES: USE DEFAULT PC */
/* 00fbd3 */                          /* A1PCLP: */
/* 00fbd3 */ 0xb5, 0x0c,              /*    LDA   A1L,X    ; ASSUMES X=1 */
/* 00fbd5 */ 0x95, 0x0a,              /*    STA   PCL,X    ; COPY FROM A1 TO PC */
/* 00fbd7 */ 0xca,                    /*    DEX            ; */
/* 00fbd8 */ 0x10, 0xf9,              /*    BPL   A1PCLP   ; */
/* 00fbda */                          /* CMDLIN3: */
/* 00fbda */ 0x68,                    /*    PLA            ; */
/* 00fbdb */ 0xc9, 0x05,              /*    CMP   #$05     ; 'L' COMMAND? */
/* 00fbdd */ 0xf0, 0xcf,              /*    BEQ   LIST     ; YES: DISASSEMBLE */
/* 00fbdf */ 0xc9, 0x99,              /*    CMP   #$99     ; ' ' COMMAND? */
/* 00fbe1 */ 0xf0, 0x04,              /*    BEQ   ASSEM    ; YES: ASM AT CURR PC */
/* 00fbe3 */ 0xc9, 0x93,              /*    CMP   #$93     ; ':' COMMAND? */
/* 00fbe5 */ 0xd0, 0x76,              /*    BNE   FORM8A   ; NO: ERROR */
/* 00fbe7 */                          /* ASSEM: */
/* 00fbe7 */ 0xa9, 0x03,              /*    LDA   #3       ; # OF CHARS IN MNEM */
/* 00fbe9 */ 0x85, 0x0d,              /*    STA   A1H      ; */
/* 00fbeb */                          /* NXTMN: */
/* 00fbeb */ 0x20, 0xd4, 0xfd,        /*    JSR   GETNSP   ; GET A MNEM CHAR */
/* 00fbee */                          /* NXTM: */
/* 00fbee */ 0x0a,                    /*    ASL            ; */
/* 00fbef */ 0xe9, 0xbe,              /*    SBC   #$BE     ; SUBTRACT OFFSET */
/* 00fbf1 */ 0xc9, 0xc2,              /*    CMP   #$C2     ; LEGAL CHAR? */
/* 00fbf3 */ 0x90, 0x68,              /*    BCC   FORM8A   ; NO */
/* 00fbf5 */ 0x0a,                    /*    ASL            ; COMPRESS/LT-JUSTIFY */
/* 00fbf6 */ 0x0a,                    /*    ASL            ; */
/* 00fbf7 */ 0xa2, 0x04,              /*    LDX   #4       ; */
/* 00fbf9 */                          /* NXTM2: */
/* 00fbf9 */ 0x0a,                    /*    ASL            ; DO 5 24-BIT SHIFTS */
/* 00fbfa */ 0x26, 0x12,              /*    ROL   A4L      ; */
/* 00fbfc */ 0x26, 0x13,              /*    ROL   A4H      ; */
/* 00fbfe */ 0xca,                    /*    DEX            ; */
/* 00fbff */ 0x10, 0xf8,              /*    BPL   NXTM2    ; */
/* 00fc01 */ 0xc6, 0x0d,              /*    DEC   A1H      ; DONE WITH 3 CHARS? */
/* 00fc03 */ 0xf0, 0xf4,              /*    BEQ   NXTM2    ; YES: ONE LAST SHIFT */
/* 00fc05 */ 0x10, 0xe4,              /*    BPL   NXTMN    ; NO */
/* 00fc07 */                          /* FORM1: */
/* 00fc07 */ 0xa2, 0x05,              /*    LDX   #5       ; 5 CHARS IN ADR MODE */
/* 00fc09 */                          /* FORM2: */
/* 00fc09 */ 0x20, 0xd4, 0xfd,        /*    JSR   GETNSP   ; GET 1ST CHR OF ADDR */
/* 00fc0c */ 0x84, 0x1a,              /*    STY   YSAV     ; */
/* 00fc0e */ 0xdd, 0x74, 0xfe,        /*    CMP   CHR1,X   ; 1ST CHR MATCH PATT? */
/* 00fc11 */ 0xd0, 0x13,              /*    BNE   FORM3    ; NO */
/* 00fc13 */ 0x20, 0xd4, 0xfd,        /*    JSR   GETNSP   ; YES: GET 2ND CHAR */
/* 00fc16 */ 0xdd, 0x7a, 0xfe,        /*    CMP   CHR2,X   ; MATCHES 2ND HALF? */
/* 00fc19 */ 0xf0, 0x0d,              /*    BEQ   FORM5    ; YES */
/* 00fc1b */ 0xbd, 0x7a, 0xfe,        /*    LDA   CHR2,X   ; NO: IS 2ND HALF 0? */
/* 00fc1e */ 0xf0, 0x07,              /*    BEQ   FORM4    ; YES */
/* 00fc20 */ 0xc9, 0xa4,              /*    CMP   #$A4     ; NO: 2ND HALF OPT? */
/* 00fc22 */ 0xf0, 0x03,              /*    BEQ   FORM4    ; YES */
/* 00fc24 */ 0xa4, 0x1a,              /*    LDY   YSAV     ; */
/* 00fc26 */                          /* FORM3: */
/* 00fc26 */ 0x18,                    /*    CLC            ; CLEAR BIT-NO MATCH */
/* 00fc27 */                          /* FORM4: */
/* 00fc27 */ 0x88,                    /*    DEY            ; BACK UP 1 CHAR */
/* 00fc28 */                          /* FORM5: */
/* 00fc28 */ 0x26, 0x07,              /*    ROL   FMT      ; FORM FORMAT BYTE */
/* 00fc2a */ 0xe0, 0x03,              /*    CPX   #3       ; TIME TO CHECK ADDR? */
/* 00fc2c */ 0xd0, 0x0d,              /*    BNE   FORM7    ; NO */
/* 00fc2e */ 0x20, 0xf4, 0xfd,        /*    JSR   GETNUM   ; YES: PARSE IT */
/* 00fc31 */ 0xa5, 0x0f,              /*    LDA   A2H      ; */
/* 00fc33 */ 0xf0, 0x01,              /*    BEQ   FORM6    ; HIGH-ORDER BYTE 0? */
/* 00fc35 */ 0xe8,                    /*    INX            ; NO: INCR FOR 2-BYTE */
/* 00fc36 */                          /* FORM6: */
/* 00fc36 */ 0x86, 0x18,              /*    STX   L        ; STORE LENGTH */
/* 00fc38 */ 0xa2, 0x03,              /*    LDX   #3       ; RELOAD FMT INDEX */
/* 00fc3a */ 0x88,                    /*    DEY            ; BACK UP 1 CHAR */
/* 00fc3b */                          /* FORM7: */
/* 00fc3b */ 0x86, 0x0d,              /*    STX   A1H      ; SAVE INDEX */
/* 00fc3d */ 0xca,                    /*    DEX            ; DONE W/FMT CHECK? */
/* 00fc3e */ 0x10, 0xc9,              /*    BPL   FORM2    ; NO */
/* 00fc40 */ 0xa5, 0x07,              /*    LDA   FMT      ; YES: PUT LENGTH */
/* 00fc42 */ 0x0a,                    /*    ASL            ;   IN LOW BITS */
/* 00fc43 */ 0x0a,                    /*    ASL            ; */
/* 00fc44 */ 0x05, 0x18,              /*    ORA   L        ; */
/* 00fc46 */ 0xc9, 0x20,              /*    CMP   #$20     ; */
/* 00fc48 */ 0xb0, 0x06,              /*    BCS   FORM8    ; */
/* 00fc4a */ 0xa6, 0x18,              /*    LDX   L        ; */
/* 00fc4c */ 0xf0, 0x02,              /*    BEQ   FORM8    ; */
/* 00fc4e */ 0x09, 0x80,              /*    ORA   #$80     ; */
/* 00fc50 */                          /* FORM8: */
/* 00fc50 */ 0x85, 0x07,              /*    STA   FMT      ; */
/* 00fc52 */ 0x84, 0x1a,              /*    STY   YSAV     ; */
/* 00fc54 */ 0xb9, 0x00, 0x02,        /*    LDA   IN,Y     ; GET NEXT NONBLANK */
/* 00fc57 */ 0xc9, 0xbb,              /*    CMP   #$BB     ; START OF COMMENT?{;} */
/* 00fc59 */ 0xf0, 0x04,              /*    BEQ   TRYNEXT  ; YES */
/* 00fc5b */ 0xc9, 0x8d,              /*    CMP   #CR      ; CR? */
/* 00fc5d */                          /* FORM8A: */
/* 00fc5d */ 0xd0, 0x2c,              /*    BNE   ERR2     ; NO: ERROR */
/* 00fc5f */                          /* TRYNEXT:          ; ($F55C ON A2) */
/* 00fc5f */ 0xa5, 0x0d,              /*    LDA   A1H      ; GET TRIAL OPCODE */
/* 00fc61 */ 0x20, 0xdb, 0xfc,        /*    JSR   INSDS2   ; GET FMT+LEN FOR IT */
/* 00fc64 */ 0xaa,                    /*    TAX            ; */
/* 00fc65 */ 0xbd, 0xc0, 0xfe,        /*    LDA   MNEMR,X  ; GET LOWER MNEM BYTE */
/* 00fc68 */ 0xc5, 0x12,              /*    CMP   A4L      ; MATCH? */
/* 00fc6a */ 0xd0, 0x13,              /*    BNE   NEXTOP   ; NO: TRY NEXT OPCODE */
/* 00fc6c */ 0xbd, 0x80, 0xfe,        /*    LDA   MNEML,X  ; GET UPPER MNEM BYTE */
/* 00fc6f */ 0xc5, 0x13,              /*    CMP   A4H      ; MATCH? */
/* 00fc71 */ 0xd0, 0x0c,              /*    BNE   NEXTOP   ; NO: TRY NEXT OPCODE */
/* 00fc73 */ 0xa5, 0x07,              /*    LDA   FMT      ; */
/* 00fc75 */ 0xa4, 0x05,              /*    LDY   FORMAT   ; GET TRIAL FORMAT */
/* 00fc77 */ 0xc0, 0x9d,              /*    CPY   #$9D     ; IS IT RELATIVE? */
/* 00fc79 */ 0xf0, 0x20,              /*    BEQ   REL      ; YES */
/* 00fc7b */ 0xc5, 0x05,              /*    CMP   FORMAT   ; SAME FORMAT? */
/* 00fc7d */ 0xf0, 0x37,              /*    BEQ   FINDOP   ; YES */
/* 00fc7f */                          /* NEXTOP: */
/* 00fc7f */ 0xc6, 0x0d,              /*    DEC   A1H      ; NO: TRY NEXT OPCODE */
/* 00fc81 */ 0xd0, 0xdc,              /*    BNE   TRYNEXT  ; */
/* 00fc83 */ 0xe6, 0x07,              /*    INC   FMT      ; NO MORE: TRY LEN=2 */
/* 00fc85 */ 0xc6, 0x18,              /*    DEC   L        ; WAS L=2 ALREADY? */
/* 00fc87 */ 0xf0, 0xd6,              /*    BEQ   TRYNEXT  ; NO: KEEP TRYING */
/* 00fc89 */                          /* ERR: */
/* 00fc89 */ 0xa4, 0x1a,              /*    LDY   YSAV     ; YES: UNREC. INST */
/* 00fc8b */                          /* ERR2: */
/* 00fc8b */ 0x98,                    /*    TYA            ; */
/* 00fc8c */ 0xaa,                    /*    TAX            ; */
/* 00fc8d */ 0x20, 0xe0, 0xfd,        /*    JSR   CROUT    ; */
/* 00fc90 */ 0x20, 0xcb, 0xfd,        /*    JSR   PRXBL    ; PRINT '^' BELOW THE */
/* 00fc93 */ 0xa9, 0xde,              /*    LDA   #$DE     ;   LAST PARSED CHAR{^} */
/* 00fc95 */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ;   TO SHOW ERR POSN */
/* 00fc98 */ 0x4c, 0xc1, 0xfb,        /*    JMP   CMDLINE  ; NEXT COMMAND LINE */
/* 00fc9b */                          /* REL: */
/* 00fc9b */ 0xe9, 0x81,              /*    SBC   #$81     ; YES: IS FMT COMPAT */
/* 00fc9d */ 0x4a,                    /*    LSR            ;   W/RELATIVE MODE? */
/* 00fc9e */ 0xd0, 0xe9,              /*    BNE   ERR      ; NO: ERROR OUT */
/* 00fca0 */ 0xa4, 0x0f,              /*    LDY   A2H      ; */
/* 00fca2 */ 0xa6, 0x0e,              /*    LDX   A2L      ; DOUBLE DECREMENT */
/* 00fca4 */ 0xd0, 0x01,              /*    BNE   REL2     ; */
/* 00fca6 */ 0x88,                    /*    DEY            ; */
/* 00fca7 */                          /* REL2: */
/* 00fca7 */ 0xca,                    /*    DEX            ; */
/* 00fca8 */ 0x8a,                    /*    TXA            ; */
/* 00fca9 */ 0x18,                    /*    CLC            ; */
/* 00fcaa */ 0xe5, 0x0a,              /*    SBC   PCL      ; CALC ADDR-PC-2 */
/* 00fcac */ 0x85, 0x0e,              /*    STA   A2L      ; */
/* 00fcae */ 0x10, 0x01,              /*    BPL   REL3     ; */
/* 00fcb0 */ 0xc8,                    /*    INY            ; */
/* 00fcb1 */                          /* REL3: */
/* 00fcb1 */ 0x98,                    /*    TYA            ; */
/* 00fcb2 */ 0xe5, 0x0b,              /*    SBC   PCH      ; */
/* 00fcb4 */ 0xd0, 0xd3,              /*    BNE   ERR      ; ERROR IF TOO FAR */
/* 00fcb6 */                          /* FINDOP: */
/* 00fcb6 */ 0xa4, 0x06,              /*    LDY   LENGTH   ; */
/* 00fcb8 */                          /* FNDOP2: */
/* 00fcb8 */ 0xb9, 0x0d, 0x00,        /*    LDA   A1H,Y    ; MOVE INST TO (PC) */
/* 00fcbb */ 0x91, 0x0a,              /*    STA   (PCL),Y  ; */
/* 00fcbd */ 0x88,                    /*    DEY            ; */
/* 00fcbe */ 0x10, 0xf8,              /*    BPL   FNDOP2   ; */
/* 00fcc0 */ 0xa9, 0x01,              /*    LDA   #1       ; DISASSEMBLE RESULT */
/* 00fcc2 */ 0x4c, 0xb0, 0xfb,        /*    JMP   LIST1    ; BACK TO CMD LINE */
/* 00fcc5 */                          /* ;=  92 ================================ */
/* 00fcc5 */                          /* INSDS1:           ; ($F882 ON A2) */
/* 00fcc5 */ 0xa6, 0x0a,              /*    LDX   PCL      ; PRINT PCL,H */
/* 00fcc7 */ 0xa4, 0x0b,              /*    LDY   PCH      ; */
/* 00fcc9 */ 0x20, 0xe0, 0xfd,        /*    JSR   CROUT    ; */
/* 00fccc */ 0x20, 0x91, 0xfd,        /*    JSR   PRNTYX   ; */
/* 00fccf */ 0xa0, 0x00,              /*    LDY   #0       ; */
/* 00fcd1 */ 0xa9, 0xad,              /*    LDA   #$AD     ; PRINT '-' */
/* 00fcd3 */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; */
/* 00fcd6 */ 0x20, 0xc9, 0xfd,        /*    JSR   PR3BL    ; PRINT 3 BLANKS */
/* 00fcd9 */ 0xa1, 0x0a,              /*    LDA   (PCL,X)  ; GET OPCODE */
/* 00fcdb */                          /* INSDS2:           ; ($F88E ON A2) */
/* 00fcdb */ 0xa8,                    /*    TAY            ; */
/* 00fcdc */ 0x4a,                    /*    LSR            ; EVEN/ODD TEST */
/* 00fcdd */ 0x90, 0x0b,              /*    BCC   IEVEN    ; */
/* 00fcdf */ 0x4a,                    /*    LSR            ; BIT 1 TEST */
/* 00fce0 */ 0x09, 0x80,              /*    ORA   #$80     ;   (BECAUSE ROR BUG) */
/* 00fce2 */ 0xb0, 0x15,              /*    BCS   INVLD    ; XXXXXX11 INVALID OP */
/* 00fce4 */ 0xc9, 0xa2,              /*    CMP   #$A2     ; */
/* 00fce6 */ 0xf0, 0x11,              /*    BEQ   INVLD    ; OPCODE $89 INVALID */
/* 00fce8 */ 0x29, 0x87,              /*    AND   #$87     ; MASK BITS */
/* 00fcea */                          /* IEVEN:            ; ($F89B ON A2) */
/* 00fcea */ 0x4a,                    /*    LSR            ; LSB->C FOR L/R TEST */
/* 00fceb */ 0xaa,                    /*    TAX            ; */
/* 00fcec */ 0xbd, 0x22, 0xfe,        /*    LDA   FMT1,X   ; GET FMT INDEX BYTE */
/* 00fcef */ 0x90, 0x04,              /*    BCC   RTMSKZ   ; R/L H-BYTE ON CARRY */
/* 00fcf1 */ 0x4a,                    /*    LSR            ; */
/* 00fcf2 */ 0x4a,                    /*    LSR            ; */
/* 00fcf3 */ 0x4a,                    /*    LSR            ; */
/* 00fcf4 */ 0x4a,                    /*    LSR            ; */
/* 00fcf5 */                          /* RTMSKZ: */
/* 00fcf5 */ 0x29, 0x0f,              /*    AND   #$0F     ; */
/* 00fcf7 */ 0xd0, 0x04,              /*    BNE   GETFMT   ; */
/* 00fcf9 */                          /* INVLD: */
/* 00fcf9 */ 0xa0, 0x80,              /*    LDY   #$80     ; SUB $80 FOR INVALID */
/* 00fcfb */ 0xa9, 0x00,              /*    LDA   #0       ; SET PRINT FMT INDEX */
/* 00fcfd */                          /* GETFMT:           ; ($F8A9 ON A2) */
/* 00fcfd */ 0xaa,                    /*    TAX            ; */
/* 00fcfe */ 0xbd, 0x66, 0xfe,        /*    LDA   FMT2,X   ; INDX INTO FMT TABLE */
/* 00fd01 */ 0x85, 0x05,              /*    STA   FORMAT   ; SAVE FOR ADR FLD */
/* 00fd03 */ 0x29, 0x03,              /*    AND   #$03     ; MASK FOR 2-BIT LEN */
/* 00fd05 */ 0x85, 0x06,              /*    STA   LENGTH   ; */
/* 00fd07 */ 0x98,                    /*    TYA            ; OPCODE */
/* 00fd08 */ 0x29, 0x8f,              /*    AND   #$8F     ; MASK: 1XXX1010 TEST */
/* 00fd0a */ 0xaa,                    /*    TAX            ; SAVE IT */
/* 00fd0b */ 0x98,                    /*    TYA            ; OPCODE TO A AGAIN */
/* 00fd0c */ 0xa0, 0x03,              /*    LDY   #$03     ; */
/* 00fd0e */ 0xe0, 0x8a,              /*    CPX   #$8A     ; */
/* 00fd10 */ 0xf0, 0x0b,              /*    BEQ   MNNDX3   ; */
/* 00fd12 */                          /* MNNDX1: */
/* 00fd12 */ 0x4a,                    /*    LSR            ; */
/* 00fd13 */ 0x90, 0x08,              /*    BCC   MNNDX3   ; FORM IDX INTO MNEM */
/* 00fd15 */ 0x4a,                    /*    LSR            ; */
/* 00fd16 */                          /* MNNDX2: */
/* 00fd16 */ 0x4a,                    /*    LSR            ; 1 1XXX1010>00101XXX */
/* 00fd17 */ 0x09, 0x20,              /*    ORA   #$20     ; 2 XXXYYY01>00111XXX */
/* 00fd19 */ 0x88,                    /*    DEY            ; 3 XXXYYY10>00110XXX */
/* 00fd1a */ 0xd0, 0xfa,              /*    BNE   MNNDX2   ; 4 XXXYY100>00100XXX */
/* 00fd1c */ 0xc8,                    /*    INY            ; 5 XXXXX000>000XXXXX */
/* 00fd1d */                          /* MNNDX3: */
/* 00fd1d */ 0x88,                    /*    DEY            ; */
/* 00fd1e */ 0xd0, 0xf2,              /*    BNE   MNNDX1   ; */
/* 00fd20 */ 0x60,                    /*    RTS            ; */
/* 00fd21 */                          /* ;= 120 ================================ */
/* 00fd21 */                          /* INSTDSP:          ; ($F8D0 ON A2) */
/* 00fd21 */ 0x20, 0xc5, 0xfc,        /*    JSR   INSDS1   ; GEN FMT, LEN BYTES */
/* 00fd24 */ 0x48,                    /*    PHA            ; SAVE MNEM TABLE IDX */
/* 00fd25 */                          /* PRNTOP: */
/* 00fd25 */ 0xb1, 0x0a,              /*    LDA   (PCL),Y  ; */
/* 00fd27 */ 0x20, 0xdc, 0xff,        /*    JSR   PRBYTE   ; */
/* 00fd2a */ 0xa2, 0x01,              /*    LDX   #1       ; PRINT 2 BLANKS */
/* 00fd2c */                          /* PRNTBL: */
/* 00fd2c */ 0x20, 0xcb, 0xfd,        /*    JSR   PRXBL    ; */
/* 00fd2f */ 0xc4, 0x06,              /*    CPY   LENGTH   ; PRINT INSTRUCTION */
/* 00fd31 */ 0xc8,                    /*    INY            ; IN A 12 CHAR FIELD */
/* 00fd32 */ 0x90, 0xf1,              /*    BCC   PRNTOP   ; */
/* 00fd34 */ 0xa2, 0x03,              /*    LDX   #3       ; MNEM CHAR COUNT */
/* 00fd36 */ 0xc0, 0x04,              /*    CPY   #4       ; */
/* 00fd38 */ 0x90, 0xf2,              /*    BCC   PRNTBL   ; */
/* 00fd3a */ 0x68,                    /*    PLA            ; RECOVER MNEM INDEX */
/* 00fd3b */ 0xa8,                    /*    TAY            ; */
/* 00fd3c */ 0xb9, 0x80, 0xfe,        /*    LDA   MNEML,Y  ; */
/* 00fd3f */ 0x85, 0x08,              /*    STA   LMNEM    ; FETCH 3-CHAR MNEM */
/* 00fd41 */ 0xb9, 0xc0, 0xfe,        /*    LDA   MNEMR,Y  ;   PACKED IN 2-BYTES */
/* 00fd44 */ 0x85, 0x09,              /*    STA   RMNEM    ; */
/* 00fd46 */                          /* PRMN1: */
/* 00fd46 */ 0xa9, 0x00,              /*    LDA   #0       ; */
/* 00fd48 */ 0xa0, 0x05,              /*    LDY   #5       ; */
/* 00fd4a */                          /* PRMN2: */
/* 00fd4a */ 0x06, 0x09,              /*    ASL   RMNEM    ; SHIFT 5 BITS OF */
/* 00fd4c */ 0x26, 0x08,              /*    ROL   LMNEM    ;   CHARACTER INTO A */
/* 00fd4e */ 0x2a,                    /*    ROL            ;     (CLEARS CARRY) */
/* 00fd4f */ 0x88,                    /*    DEY            ; */
/* 00fd50 */ 0xd0, 0xf8,              /*    BNE   PRMN2    ; */
/* 00fd52 */ 0x69, 0xbf,              /*    ADC   #$BF     ; ADD "?" OFFSET {?} */
/* 00fd54 */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; SEND A CHAR OF MNEM */
/* 00fd57 */ 0xca,                    /*    DEX            ; */
/* 00fd58 */ 0xd0, 0xec,              /*    BNE   PRMN1    ; */
/* 00fd5a */ 0x20, 0xc9, 0xfd,        /*    JSR   PR3BL    ; PRINT 3 BLANKS */
/* 00fd5d */ 0xa4, 0x06,              /*    LDY   LENGTH   ; */
/* 00fd5f */ 0xa2, 0x06,              /*    LDX   #6       ; CNT FOR 6 FMT BITS */
/* 00fd61 */                          /* PRADR1: */
/* 00fd61 */ 0xe0, 0x03,              /*    CPX   #3       ; */
/* 00fd63 */ 0xf0, 0x1c,              /*    BEQ   PRADR5   ; IF X=3 THEN ADDR */
/* 00fd65 */                          /* PRADR2: */
/* 00fd65 */ 0x06, 0x05,              /*    ASL   FORMAT   ; */
/* 00fd67 */ 0x90, 0x0e,              /*    BCC   PRADR3   ; */
/* 00fd69 */ 0xbd, 0x73, 0xfe,        /*    LDA   CHR1-1,X ; */
/* 00fd6c */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; */
/* 00fd6f */ 0xbd, 0x79, 0xfe,        /*    LDA   CHR2-1,X ; */
/* 00fd72 */ 0xf0, 0x03,              /*    BEQ   PRADR3   ; */
/* 00fd74 */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; */
/* 00fd77 */                          /* PRADR3: */
/* 00fd77 */ 0xca,                    /*    DEX            ; */
/* 00fd78 */ 0xd0, 0xe7,              /*    BNE   PRADR1   ; */
/* 00fd7a */ 0x60,                    /*    RTS            ; */
/* 00fd7b */                          /* PRADR4: */
/* 00fd7b */ 0x88,                    /*    DEY            ; */
/* 00fd7c */ 0x30, 0xe7,              /*    BMI   PRADR2   ; */
/* 00fd7e */ 0x20, 0xdc, 0xff,        /*    JSR   PRBYTE   ; */
/* 00fd81 */                          /* PRADR5: */
/* 00fd81 */ 0xa5, 0x05,              /*    LDA   FORMAT   ; */
/* 00fd83 */ 0xc9, 0xe8,              /*    CMP   #$E8     ; HANDLE REL ADR MODE */
/* 00fd85 */ 0xb1, 0x0a,              /*    LDA   (PCL),Y  ; SPECIAL (SND TARGET */
/* 00fd87 */ 0x90, 0xf2,              /*    BCC   PRADR4   ;   ADDR, NOT OFFSET) */
/* 00fd89 */ 0x20, 0xe8, 0xfd,        /*    JSR   PCADJ3   ; */
/* 00fd8c */ 0xaa,                    /*    TAX            ; PCL,H+OFFSET+1 */
/* 00fd8d */ 0xe8,                    /*    INX            ;   TO A,Y */
/* 00fd8e */ 0xd0, 0x01,              /*    BNE   PRNTYX   ; +1 TO Y,X */
/* 00fd90 */ 0xc8,                    /*    INY            ; */
/* 00fd91 */                          /* PRNTYX: */
/* 00fd91 */ 0x98,                    /*    TYA            ; */
/* 00fd92 */                          /* PRNTAX: */
/* 00fd92 */ 0x20, 0xdc, 0xff,        /*    JSR   PRBYTE   ; PRINT BRANCH TARGET */
/* 00fd95 */ 0x8a,                    /*    TXA            ; */
/* 00fd96 */ 0x4c, 0xdc, 0xff,        /*    JMP   PRBYTE   ; */
/* 00fd99 */                          /* ;=  48 ================================ */
/* 00fd99 */                          /* NOTACR: */
/* 00fd99 */ 0xc9, 0x9b,              /*    CMP   #ESC     ; ESC? */
/* 00fd9b */ 0xf0, 0x0a,              /*    BEQ   CANCEL   ; YES: START OVER */
/* 00fd9d */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; PRINT CHAR */
/* 00fda0 */ 0xc9, 0xdf,              /*    CMP   #$DF     ; {_} */
/* 00fda2 */ 0xf0, 0x12,              /*    BEQ   BCKSPC   ; */
/* 00fda4 */ 0xe8,                    /*    INX            ; */
/* 00fda5 */ 0x10, 0x12,              /*    BPL   NXTCHAR  ; 128 CHARS MAX */
/* 00fda7 */                          /* CANCEL: */
/* 00fda7 */ 0xa9, 0xdc,              /*    LDA   #$DC     ; {\} */
/* 00fda9 */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; */
/* 00fdac */                          /* GETLNZ:           ; ($FD67 ON A2) */
/* 00fdac */ 0x20, 0xe0, 0xfd,        /*    JSR   CROUT    ; FRESH LINE */
/* 00fdaf */                          /* GETLINE: */
/* 00fdaf */ 0xa9, 0xa1,              /*    LDA   #$A1     ; PROMPT CHAR {!} */
/* 00fdb1 */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; */
/* 00fdb4 */ 0xa2, 0x01,              /*    LDX   #1       ; */
/* 00fdb6 */                          /* BCKSPC: */
/* 00fdb6 */ 0xca,                    /*    DEX            ; */
/* 00fdb7 */ 0x30, 0xee,              /*    BMI   CANCEL   ; */
/* 00fdb9 */                          /* NXTCHAR: */
/* 00fdb9 */ 0x2c, 0x11, 0xd0,        /*    BIT   KBDCR    ; WAIT FOR KEY PRESS */
/* 00fdbc */ 0x10, 0xfb,              /*    BPL   NXTCHAR  ; NO KEY YET */
/* 00fdbe */ 0xad, 0x10, 0xd0,        /*    LDA   KBD      ; LOAD CHAR */
/* 00fdc1 */ 0x9d, 0x00, 0x02,        /*    STA   IN,X     ; STORE IN BUFFER */
/* 00fdc4 */ 0xc9, 0x8d,              /*    CMP   #CR      ; CR? */
/* 00fdc6 */ 0xd0, 0xd1,              /*    BNE   NOTACR   ; NO: KEEP GOING */
/* 00fdc8 */ 0x60,                    /*    RTS            ; YES: X=BUFLEN */
/* 00fdc9 */                          /* ;=  11 ================================ */
/* 00fdc9 */                          /* PR3BL:            ; ($F948 ON A2) */
/* 00fdc9 */ 0xa2, 0x03,              /*    LDX   #3       ; BLANK COUNT */
/* 00fdcb */                          /* PRXBL: */
/* 00fdcb */ 0xa9, 0xa0,              /*    LDA   #$A0     ; LOAD A SPACE CHAR { } */
/* 00fdcd */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; PRINT IT */
/* 00fdd0 */ 0xca,                    /*    DEX            ; */
/* 00fdd1 */ 0xd0, 0xf8,              /*    BNE   PRXBL    ; LOOP UNTIL COUNT=0 */
/* 00fdd3 */ 0x60,                    /*    RTS            ; */
/* 00fdd4 */                          /* ;=   9 ================================ */
/* 00fdd4 */                          /* GETNSP:           ; ($F634 ON A2) */
/* 00fdd4 */ 0xb9, 0x00, 0x02,        /*    LDA   IN,Y     ; */
/* 00fdd7 */ 0xc8,                    /*    INY            ; */
/* 00fdd8 */ 0xc9, 0xa0,              /*    CMP   #$A0     ; SKIP OVER SPACES { } */
/* 00fdda */ 0xf0, 0xf8,              /*    BEQ   GETNSP   ; */
/* 00fddc */ 0x60,                    /*    RTS            ; */
/* 00fddd */                          /* ;=   3 ================================ */
/* 00fddd */                          /* XMON:             ; */
/* 00fddd */ 0x4c, 0xc1, 0xfb,        /*    JMP   CMDLINE  ; */
/* 00fde0 */                          /* ;=   5 ================================ */
/* 00fde0 */                          /* CROUT:            ; ($FD8E ON A2) */
/* 00fde0 */ 0xa9, 0x8d,              /*    LDA   #CR      ; */
/* 00fde2 */ 0x4c, 0xef, 0xff,        /*    JMP   ECHO     ; */
/* 00fde5 */                          /* ;=  15 ================================ */
/* 00fde5 */                          /* PCADJ:            ; ($F953 ON A2) */
/* 00fde5 */ 0x38,                    /*    SEC            ; 0=1-BYTE, 1=2-BYTE, */
/* 00fde6 */ 0xa5, 0x06,              /*    LDA   LENGTH   ;   2=3-BYTE INST */
/* 00fde8 */                          /* PCADJ3: */
/* 00fde8 */ 0xa4, 0x0b,              /*    LDY   PCH      ; */
/* 00fdea */ 0xaa,                    /*    TAX            ; TEST DISPLCMNT SIGN */
/* 00fdeb */ 0x10, 0x01,              /*    BPL   PCADJ4   ;   (FOR REL BRANCH) */
/* 00fded */ 0x88,                    /*    DEY            ; EXT NEG BY DEC PCH */
/* 00fdee */                          /* PCADJ4: */
/* 00fdee */ 0x65, 0x0a,              /*    ADC   PCL      ; PCL + LEN(DSPL) + 1 */
/* 00fdf0 */ 0x90, 0x01,              /*    BCC   PCADJ5   ;   TO A (PCL), */
/* 00fdf2 */ 0xc8,                    /*    INY            ;   CARRY TO Y (PCH) */
/* 00fdf3 */                          /* PCADJ5: */
/* 00fdf3 */ 0x60,                    /*    RTS            ; */
/* 00fdf4 */                          /* ;=  46 ================================ */
/* 00fdf4 */                          /* GETNUM:           ; ($FFA7 ON A2) */
/* 00fdf4 */ 0xa2, 0x00,              /*    LDX   #0       ; CLEAR VALID # FLAG */
/* 00fdf6 */ 0x86, 0x0e,              /*    STX   A2L      ; CLEAR A2 */
/* 00fdf8 */ 0x86, 0x0f,              /*    STX   A2H      ; */
/* 00fdfa */ 0xf0, 0x15,              /*    BEQ   NXTCXAR  ; */
/* 00fdfc */                          /* DIG:              ; ($FF8A ON A2) */
/* 00fdfc */ 0xa2, 0x03,              /*    LDX   #3       ; */
/* 00fdfe */ 0x0a,                    /*    ASL            ; */
/* 00fdff */ 0x0a,                    /*    ASL            ; GOT HEX DIGIT, */
/* 00fe00 */ 0x0a,                    /*    ASL            ;   SHIFT INTO A2 */
/* 00fe01 */ 0x0a,                    /*    ASL            ; */
/* 00fe02 */                          /* NXTBIT: */
/* 00fe02 */ 0x0a,                    /*    ASL            ; */
/* 00fe03 */ 0x26, 0x0e,              /*    ROL   A2L      ; */
/* 00fe05 */ 0x26, 0x0f,              /*    ROL   A2H      ; */
/* 00fe07 */ 0xca,                    /*    DEX            ; LEAVE X=$FF IF DIG */
/* 00fe08 */ 0x10, 0xf8,              /*    BPL   NXTBIT   ; */
/* 00fe0a */                          /* NXTBAS: */
/* 00fe0a */ 0xb5, 0x0f,              /*    LDA   A2H,X    ; COPY A2 TO A1 */
/* 00fe0c */ 0x95, 0x0d,              /*    STA   A1H,X    ; */
/* 00fe0e */                          /* NXTBS2: */
/* 00fe0e */ 0xe8,                    /*    INX            ; */
/* 00fe0f */ 0xf0, 0xf9,              /*    BEQ   NXTBAS   ; */
/* 00fe11 */                          /* NXTCXAR:          ; ($FFAD ON A2) */
/* 00fe11 */ 0xb9, 0x00, 0x02,        /*    LDA   IN,Y     ; GET CHAR */
/* 00fe14 */ 0xc8,                    /*    INY            ; ADVANCE INDEX */
/* 00fe15 */ 0x49, 0xb0,              /*    EOR   #$B0     ; */
/* 00fe17 */ 0xc9, 0x0a,              /*    CMP   #10      ; DECIMAL DIGIT? */
/* 00fe19 */ 0x90, 0xe1,              /*    BCC   DIG      ; YES: ACCUMULATE */
/* 00fe1b */ 0x69, 0x88,              /*    ADC   #$88     ; */
/* 00fe1d */ 0xc9, 0xfa,              /*    CMP   #$FA     ; 'A'-'F' */
/* 00fe1f */ 0xb0, 0xdb,              /*    BCS   DIG      ; YES: ACCUMULATE */
/* 00fe21 */ 0x60,                    /*    RTS            ; NO: RETURN WITH */
/* 00fe22 */                          /*                   ;   X=0: NO NUMBER */
/* 00fe22 */                          /*                   ;   X=1: NUMBER */
/* 00fe22 */                          /* ;= 222 ================================ */
/* 00fe22 */                          /* FMT1:             ; ($F962 ON A2) */
/* 00fe22 */                          /* ; FMT1 BYTES: XXXXXXY0 INSTRS */
/* 00fe22 */                          /* ; IF Y=0 THEN LEFT HALF BYTE */
/* 00fe22 */                          /* ; IF Y=1 THEN RIGHT HALF BYTE */
/* 00fe22 */                          /* ; (X=INDEX) */
/* 00fe22 */ 0x04, 0x20, 0x54, 0x30,  /* .byte $04, $20 ,$54 ,$30 ,$0D ,$80 ,$04 ,$90 */
/* 00fe26 */ 0x0d, 0x80, 0x04, 0x90,  /*  */
/* 00fe2a */                          /*  */
/* 00fe2a */ 0x03, 0x22, 0x54, 0x33,  /* .byte $03 ,$22 ,$54 ,$33 ,$0D ,$80 ,$04 ,$90 */
/* 00fe2e */ 0x0d, 0x80, 0x04, 0x90,  /*  */
/* 00fe32 */                          /*  */
/* 00fe32 */ 0x04, 0x20, 0x54, 0x33,  /* .byte $04 ,$20 ,$54 ,$33 ,$0D ,$80 ,$04 ,$90 */
/* 00fe36 */ 0x0d, 0x80, 0x04, 0x90,  /*  */
/* 00fe3a */                          /*  */
/* 00fe3a */ 0x04, 0x20, 0x54, 0x3b,  /* .byte $04 ,$20 ,$54 ,$3B ,$0D ,$80 ,$04 ,$90 */
/* 00fe3e */ 0x0d, 0x80, 0x04, 0x90,  /*  */
/* 00fe42 */                          /*  */
/* 00fe42 */ 0x00, 0x22, 0x44, 0x33,  /* .byte $00 ,$22 ,$44 ,$33 ,$0D ,$C8 ,$44 ,$00 */
/* 00fe46 */ 0x0d, 0xc8, 0x44, 0x00,  /*  */
/* 00fe4a */                          /*  */
/* 00fe4a */ 0x11, 0x22, 0x44, 0x33,  /* .byte $11 ,$22 ,$44 ,$33 ,$0D ,$C8 ,$44 ,$A9 */
/* 00fe4e */ 0x0d, 0xc8, 0x44, 0xa9,  /*  */
/* 00fe52 */                          /*  */
/* 00fe52 */ 0x01, 0x22, 0x44, 0x33,  /* .byte $01 ,$22 ,$44 ,$33 ,$0D ,$80 ,$04 ,$90 */
/* 00fe56 */ 0x0d, 0x80, 0x04, 0x90,  /*  */
/* 00fe5a */                          /*  */
/* 00fe5a */ 0x01, 0x22, 0x44, 0x33,  /* .byte $01 ,$22 ,$44 ,$33 ,$0D ,$80 ,$04 ,$90 */
/* 00fe5e */ 0x0d, 0x80, 0x04, 0x90,  /*  */
/* 00fe62 */                          /*  */
/* 00fe62 */                          /* ; $ZZXXXY01 INSTRS */
/* 00fe62 */ 0x26, 0x31, 0x87, 0x9a,  /* .byte $26 ,$31 ,$87 ,$9A */
/* 00fe66 */                          /* FMT2:             ; ($F9A6 ON A2) */
/* 00fe66 */                          /* ; ERR IMM ZP ABS IMP ACC (ZP,X) (ZP),Y */
/* 00fe66 */                          /* ; ZP,X ABS,X ABS,Y (ABS) ZP,Y REL */
/* 00fe66 */ 0x00, 0x21, 0x81, 0x82,  /* .byte $00 ,$21 ,$81 ,$82 ,$00 ,$00 ,$59 ,$4D */
/* 00fe6a */ 0x00, 0x00, 0x59, 0x4d,  /*  */
/* 00fe6e */                          /*  */
/* 00fe6e */ 0x91, 0x92, 0x86, 0x4a,  /* .byte $91 ,$92 ,$86 ,$4A ,$85 ,$9D */
/* 00fe72 */ 0x85, 0x9d,              /*  */
/* 00fe74 */                          /*  */
/* 00fe74 */                          /* CHR1:             ; ($F9B4 ON A2) */
/* 00fe74 */                          /* ; ",),#($" */
/* 00fe74 */ 0xac, 0xa9, 0xac, 0xa3,  /* .byte $AC ,$A9 ,$AC ,$A3 ,$A8 ,$A4 */
/* 00fe78 */ 0xa8, 0xa4,              /*  */
/* 00fe7a */                          /*  */
/* 00fe7a */                          /* CHR2:             ; ($F9BA ON A2) */
/* 00fe7a */                          /* ; "Y",0,"X$$",0 */
/* 00fe7a */ 0xd9, 0x00, 0xd8, 0xa4,  /* .byte $D9 ,$00 ,$D8 ,$A4 ,$A4 ,$00 */
/* 00fe7e */ 0xa4, 0x00,              /*  */
/* 00fe80 */                          /*  */
/* 00fe80 */                          /* MNEML:            ; ($F9C0 ON A2) */
/* 00fe80 */                          /* ; MNEML IS OF FORM: */
/* 00fe80 */                          /* ; (A) XXXXX000 */
/* 00fe80 */                          /* ; (B) XXXYY100 */
/* 00fe80 */                          /* ; (C) 1XXX1010 */
/* 00fe80 */                          /* ; (D) XXXYYY10 */
/* 00fe80 */                          /* ; (E) XXXYYY01 */
/* 00fe80 */                          /* ; (X=INDEX) */
/* 00fe80 */ 0x1c, 0x8a, 0x1c, 0x23,  /* .byte $1C ,$8A ,$1C ,$23 ,$5D ,$8B ,$1B ,$A1 */
/* 00fe84 */ 0x5d, 0x8b, 0x1b, 0xa1,  /*  */
/* 00fe88 */                          /*  */
/* 00fe88 */ 0x9d, 0x8a, 0x1d, 0x23,  /* .byte $9D ,$8A ,$1D ,$23 ,$9D ,$8B ,$1D ,$A1 */
/* 00fe8c */ 0x9d, 0x8b, 0x1d, 0xa1,  /*  */
/* 00fe90 */                          /*  */
/* 00fe90 */ 0x00, 0x29, 0x19, 0xae,  /* .byte $00 ,$29 ,$19 ,$AE ,$69 ,$A8 ,$19 ,$23 */
/* 00fe94 */ 0x69, 0xa8, 0x19, 0x23,  /*  */
/* 00fe98 */                          /*  */
/* 00fe98 */ 0x24, 0x53, 0x1b, 0x23,  /* .byte $24 ,$53 ,$1B ,$23 ,$24 ,$53 ,$19 ,$A1 */
/* 00fe9c */ 0x24, 0x53, 0x19, 0xa1,  /*  */
/* 00fea0 */                          /*  */
/* 00fea0 */ 0x00, 0x1a, 0x5b, 0x5b,  /* .byte $00 ,$1A ,$5B ,$5B ,$A5 ,$69 ,$24 ,$24 */
/* 00fea4 */ 0xa5, 0x69, 0x24, 0x24,  /*  */
/* 00fea8 */                          /*  */
/* 00fea8 */ 0xae, 0xae, 0xa8, 0xad,  /* .byte $AE ,$AE ,$A8 ,$AD ,$29 ,$00 ,$7C ,$00 */
/* 00feac */ 0x29, 0x00, 0x7c, 0x00,  /*  */
/* 00feb0 */                          /*  */
/* 00feb0 */ 0x15, 0x9c, 0x6d, 0x9c,  /* .byte $15 ,$9C ,$6D ,$9C ,$A5 ,$69 ,$29 ,$53 */
/* 00feb4 */ 0xa5, 0x69, 0x29, 0x53,  /*  */
/* 00feb8 */                          /*  */
/* 00feb8 */ 0x84, 0x13, 0x34, 0x11,  /* .byte $84 ,$13 ,$34 ,$11 ,$A5 ,$69 ,$23 ,$A0 */
/* 00febc */ 0xa5, 0x69, 0x23, 0xa0,  /*  */
/* 00fec0 */                          /*  */
/* 00fec0 */                          /* MNEMR: */
/* 00fec0 */ 0xd8, 0x62, 0x5a, 0x48,  /* .byte $D8 ,$62 ,$5A ,$48 ,$26 ,$62 ,$94 ,$88 */
/* 00fec4 */ 0x26, 0x62, 0x94, 0x88,  /*  */
/* 00fec8 */                          /*  */
/* 00fec8 */ 0x54, 0x44, 0xc8, 0x54,  /* .byte $54 ,$44 ,$C8 ,$54 ,$68 ,$44 ,$E8 ,$94 */
/* 00fecc */ 0x68, 0x44, 0xe8, 0x94,  /*  */
/* 00fed0 */                          /*  */
/* 00fed0 */ 0x00, 0xb4, 0x08, 0x84,  /* .byte $00 ,$B4 ,$08 ,$84 ,$74 ,$B4 ,$28 ,$6E */
/* 00fed4 */ 0x74, 0xb4, 0x28, 0x6e,  /*  */
/* 00fed8 */                          /*  */
/* 00fed8 */ 0x74, 0xf4, 0xcc, 0x4a,  /* .byte $74 ,$F4 ,$CC ,$4A ,$72 ,$F2 ,$A4 ,$8A */
/* 00fedc */ 0x72, 0xf2, 0xa4, 0x8a,  /*  */
/* 00fee0 */                          /*  */
/* 00fee0 */ 0x00, 0xaa, 0xa2, 0xa2,  /* .byte $00 ,$AA ,$A2 ,$A2 ,$74 ,$74 ,$74 ,$72 */
/* 00fee4 */ 0x74, 0x74, 0x74, 0x72,  /*  */
/* 00fee8 */                          /*  */
/* 00fee8 */ 0x44, 0x68, 0xb2, 0x32,  /* .byte $44 ,$68 ,$B2 ,$32 ,$B2 ,$00 ,$22 ,$00 */
/* 00feec */ 0xb2, 0x00, 0x22, 0x00,  /*  */
/* 00fef0 */                          /*  */
/* 00fef0 */ 0x1a, 0x1a, 0x26, 0x26,  /* .byte $1A ,$1A ,$26 ,$26 ,$72 ,$72 ,$88 ,$C8 */
/* 00fef4 */ 0x72, 0x72, 0x88, 0xc8,  /*  */
/* 00fef8 */                          /*  */
/* 00fef8 */ 0xc4, 0xca, 0x26, 0x48,  /* .byte $C4 ,$CA ,$26 ,$48 ,$44 ,$44 ,$A2 ,$C8 */
/* 00fefc */ 0x44, 0x44, 0xa2, 0xc8,  /*  */
/* 00ff00 */                          /*  */
/* 00ff00 */                          /* ;====================================== */
/* 00ff00 */                          /* ; WOZMON */
/* 00ff00 */                          /* RESET: */
/* 00ff00 */ 0xd8,                    /*    CLD            ; CLEAR DECIMAL MODE */
/* 00ff01 */ 0x58,                    /*    CLI            ; ENABLE INTERRUPTS */
/* 00ff02 */ 0xa0, 0x7f,              /*    LDY   #$7F     ; DSP DIRECTION MASK */
/* 00ff04 */ 0x8c, 0x12, 0xd0,        /*    STY   DSP      ; (DDR MODE ASSUMED) */
/* 00ff07 */ 0xa9, 0xa7,              /*    LDA   #$A7     ; KBD & DSP CTRL MASK */
/* 00ff09 */ 0x8d, 0x11, 0xd0,        /*    STA   KBDCR    ; ENABLE IRQS, SET */
/* 00ff0c */ 0x8d, 0x13, 0xd0,        /*    STA   DSPCR    ;  CA1,CB1 FOR POS */
/* 00ff0f */                          /*                   ; EDGE SENSE/OUT MODE */
/* 00ff0f */                          /* NOTCR: */
/* 00ff0f */ 0xc9, 0xdf,              /*    CMP   #BS      ; BACKSPACE KEY? */
/* 00ff11 */ 0xf0, 0x13,              /*    BEQ   BACKSP   ; */
/* 00ff13 */ 0xc9, 0x9b,              /*    CMP   #ESC     ; ESC KEY? */
/* 00ff15 */ 0xf0, 0x03,              /*    BEQ   ESCAPE   ; */
/* 00ff17 */ 0xc8,                    /*    INY            ; ADVANCE TEXT INDEX */
/* 00ff18 */ 0x10, 0x0f,              /*    BPL   NXTCHR   ; ESC IF LINE TOO BIG */
/* 00ff1a */                          /* ESCAPE: */
/* 00ff1a */ 0xa9, 0xdc,              /*    LDA   #PROMPT  ; PRINT PROMPT CHAR */
/* 00ff1c */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; */
/* 00ff1f */                          /* GETLIN: */
/* 00ff1f */ 0xa9, 0x8d,              /*    LDA   #CR      ; PRINT CR */
/* 00ff21 */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; */
/* 00ff24 */ 0xa0, 0x01,              /*    LDY   #1       ; START NEW LINE */
/* 00ff26 */                          /* BACKSP: */
/* 00ff26 */ 0x88,                    /*    DEY            ; BACK UP INDEX */
/* 00ff27 */ 0x30, 0xf6,              /*    BMI   GETLIN   ; REINIT IF EMPTY */
/* 00ff29 */                          /* NXTCHR: */
/* 00ff29 */ 0xad, 0x11, 0xd0,        /*    LDA   KBDCR    ; WAIT FOR KEY PRESS */
/* 00ff2c */ 0x10, 0xfb,              /*    BPL   NXTCHR   ; NO KEY YET */
/* 00ff2e */ 0xad, 0x10, 0xd0,        /*    LDA   KBD      ; LOAD CHAR */
/* 00ff31 */ 0x99, 0x00, 0x02,        /*    STA   IN,Y     ; ADD TO TEXT BUFFER */
/* 00ff34 */ 0x20, 0xef, 0xff,        /*    JSR   ECHO     ; PRINT CHAR */
/* 00ff37 */ 0xc9, 0x8d,              /*    CMP   #CR      ; */
/* 00ff39 */ 0xd0, 0xd4,              /*    BNE   NOTCR    ; */
/* 00ff3b */ 0xa0, 0xff,              /*    LDY   #$FF     ; */
/* 00ff3d */ 0xa9, 0x00,              /*    LDA   #0       ; DEFAULT MODE IS XAM */
/* 00ff3f */ 0xaa,                    /*    TAX            ; */
/* 00ff40 */                          /* STSTOR: */
/* 00ff40 */ 0x0a,                    /*    ASL            ; */
/* 00ff41 */                          /* STMODE: */
/* 00ff41 */ 0x85, 0x1b,              /*    STA   MODE     ; SET MODE FLAGS */
/* 00ff43 */                          /* BLSKIP: */
/* 00ff43 */ 0xc8,                    /*    INY            ; ADVANCE TEXT INDEX */
/* 00ff44 */                          /* NXTITM: */
/* 00ff44 */ 0xb9, 0x00, 0x02,        /*    LDA   IN,Y     ; GET CHAR */
/* 00ff47 */ 0xc9, 0x8d,              /*    CMP   #CR      ; */
/* 00ff49 */ 0xf0, 0xd4,              /*    BEQ   GETLIN   ; DONE IF CR */
/* 00ff4b */ 0xc9, 0xae,              /*    CMP   #$AE     ; {.} */
/* 00ff4d */ 0x90, 0xf4,              /*    BCC   BLSKIP   ; IGNORE CHAR < "." */
/* 00ff4f */ 0xf0, 0xf0,              /*    BEQ   STMODE   ; SET BLOCK XAM MODE */
/* 00ff51 */ 0xc9, 0xba,              /*    CMP   #$BA     ; {:} */
/* 00ff53 */ 0xf0, 0xeb,              /*    BEQ   STSTOR   ; SET STOR MODE */
/* 00ff55 */ 0xc9, 0xd2,              /*    CMP   #$D2     ;{R} */
/* 00ff57 */ 0xf0, 0x3b,              /*    BEQ   RUN      ; RUN THE PROGRAM */
/* 00ff59 */ 0x86, 0x18,              /*    STX   L        ; CLEAR INPUT VALUE */
/* 00ff5b */ 0x86, 0x19,              /*    STX   H        ; */
/* 00ff5d */ 0x84, 0x1a,              /*    STY   YSAV     ; SAVE Y FOR COMPARE */
/* 00ff5f */                          /* NXTHEX:           ; (OV ADDED) */
/* 00ff5f */ 0xb9, 0x00, 0x02,        /*    LDA   IN,Y     ; GET CHR FOR HEX TST */
/* 00ff62 */ 0x49, 0xb0,              /*    EOR   #$B0     ; MAP DIGITS TO 0-9 */
/* 00ff64 */ 0xc9, 0x0a,              /*    CMP   #10      ; DECIMAL DIGIT? */
/* 00ff66 */ 0x90, 0x06,              /*    BCC   DIGX */
/* 00ff68 */ 0x69, 0x88,              /*    ADC   #$88     ; "A"-"F" -> $FA-FF */
/* 00ff6a */ 0xc9, 0xfa,              /*    CMP   #$FA     ; HEX LETTER? */
/* 00ff6c */ 0x90, 0x11,              /*    BCC   NOTHEX   ; */
/* 00ff6e */                          /* DIGX: */
/* 00ff6e */ 0x0a,                    /*    ASL            ; */
/* 00ff6f */ 0x0a,                    /*    ASL            ; */
/* 00ff70 */ 0x0a,                    /*    ASL            ; */
/* 00ff71 */ 0x0a,                    /*    ASL            ; */
/* 00ff72 */ 0xa2, 0x04,              /*    LDX   #4       ; */
/* 00ff74 */                          /* HXSHIF: */
/* 00ff74 */ 0x0a,                    /*    ASL            ; */
/* 00ff75 */ 0x26, 0x18,              /*    ROL   L        ; */
/* 00ff77 */ 0x26, 0x19,              /*    ROL   H        ; */
/* 00ff79 */ 0xca,                    /*    DEX            ; */
/* 00ff7a */ 0xd0, 0xf8,              /*    BNE   HXSHIF   ; */
/* 00ff7c */ 0xc8,                    /*    INY            ; */
/* 00ff7d */ 0xd0, 0xe0,              /*    BNE   NXTHEX   ; NXTHEX, D0 E0 */
/* 00ff7f */                          /* NOTHEX: */
/* 00ff7f */ 0xc4, 0x1a,              /*    CPY   YSAV     ; */
/* 00ff81 */ 0xf0, 0x97,              /*    BEQ   ESCAPE   ; */
/* 00ff83 */ 0x24, 0x1b,              /*    BIT   MODE     ; */
/* 00ff85 */ 0x50, 0x10,              /*    BVC   NOTSTO   ; */
/* 00ff87 */ 0xa5, 0x18,              /*    LDA   L        ; */
/* 00ff89 */ 0x81, 0x16,              /*    STA   (STL,X)  ; */
/* 00ff8b */ 0xe6, 0x16,              /*    INC   STL      ; */
/* 00ff8d */ 0xd0, 0xb5,              /*    BNE   NXTITM   ; */
/* 00ff8f */ 0xe6, 0x17,              /*    INC   STH      ; */
/* 00ff91 */                          /* TONEXT: */
/* 00ff91 */ 0x4c, 0x44, 0xff,        /*    JMP   NXTITM   ; */
/* 00ff94 */                          /* RUN:              ; RUN USER'S PROGRAM */
/* 00ff94 */ 0x6c, 0x14, 0x00,        /*    JMP   (XAML) */
/* 00ff97 */                          /* NOTSTO: */
/* 00ff97 */ 0x30, 0x2b,              /*    BMI   XAMNXT   ; XAMNXT, 30 2B */
/* 00ff99 */ 0xa2, 0x02,              /*    LDX   #2       ; COPY 2 BYTES */
/* 00ff9b */                          /* SETADR: */
/* 00ff9b */ 0xb5, 0x17,              /*    LDA   L-1,X */
/* 00ff9d */ 0x95, 0x15,              /*    STA   STL-1,X */
/* 00ff9f */ 0x95, 0x13,              /*    STA   XAML-1,X */
/* 00ffa1 */ 0xca,                    /*    DEX */
/* 00ffa2 */ 0xd0, 0xf7,              /*    BNE   SETADR */
/* 00ffa4 */                          /* NXTPRN:           ; PRINT ADDR AND DATA */
/* 00ffa4 */ 0xd0, 0x14,              /*    BNE   PRDATA */
/* 00ffa6 */ 0xa9, 0x8d,              /*    LDA   #$8D */
/* 00ffa8 */ 0x20, 0xef, 0xff,        /*    JSR   ECHO */
/* 00ffab */ 0xa5, 0x15,              /*    LDA   XAMH */
/* 00ffad */ 0x20, 0xdc, 0xff,        /*    JSR   PRBYTE */
/* 00ffb0 */ 0xa5, 0x14,              /*    LDA   XAML */
/* 00ffb2 */ 0x20, 0xdc, 0xff,        /*    JSR   PRBYTE */
/* 00ffb5 */ 0xa9, 0xba,              /*    LDA   #$BA     ;':' */
/* 00ffb7 */ 0x20, 0xef, 0xff,        /*    JSR   ECHO */
/* 00ffba */                          /* PRDATA: */
/* 00ffba */ 0xa9, 0xa0,              /*    LDA   #$A0     ;' ' */
/* 00ffbc */ 0x20, 0xef, 0xff,        /*    JSR   ECHO */
/* 00ffbf */ 0xa1, 0x14,              /*    LDA   (XAML,X) */
/* 00ffc1 */ 0x20, 0xdc, 0xff,        /*    JSR   PRBYTE */
/* 00ffc4 */                          /* XAMNXT: */
/* 00ffc4 */ 0x86, 0x1b,              /*    STX   MODE */
/* 00ffc6 */ 0xa5, 0x14,              /*    LDA   XAML */
/* 00ffc8 */ 0xc5, 0x18,              /*    CMP   L */
/* 00ffca */ 0xa5, 0x15,              /*    LDA   XAMH */
/* 00ffcc */ 0xe5, 0x19,              /*    SBC   H */
/* 00ffce */ 0xb0, 0xc1,              /*    BCS   TONEXT */
/* 00ffd0 */ 0xe6, 0x14,              /*    INC   XAML */
/* 00ffd2 */ 0xd0, 0x02,              /*    BNE   MD8CHK */
/* 00ffd4 */ 0xe6, 0x15,              /*    INC   XAMH */
/* 00ffd6 */                          /* MD8CHK: */
/* 00ffd6 */ 0xa5, 0x14,              /*    LDA   XAML */
/* 00ffd8 */ 0x29, 0x07,              /*    AND   #$07 */
/* 00ffda */ 0x10, 0xc8,              /*    BPL   NXTPRN */
/* 00ffdc */                          /* PRBYTE: */
/* 00ffdc */ 0x48,                    /*    PHA            ; PRINT A AS HEX BYTE */
/* 00ffdd */ 0x4a,                    /*    LSR            ; (DESTRUCTIVE) */
/* 00ffde */ 0x4a,                    /*    LSR */
/* 00ffdf */ 0x4a,                    /*    LSR */
/* 00ffe0 */ 0x4a,                    /*    LSR */
/* 00ffe1 */ 0x20, 0xe5, 0xff,        /*    JSR   PRHEX */
/* 00ffe4 */ 0x68,                    /*    PLA */
/* 00ffe5 */                          /* PRHEX: */
/* 00ffe5 */ 0x29, 0x0f,              /*    AND   #$0F     ; PRINT A AS HEX DIG */
/* 00ffe7 */ 0x09, 0xb0,              /*    ORA   #$B0     ; '0' */
/* 00ffe9 */ 0xc9, 0xba,              /*    CMP   #$BA     ; '9'+1 */
/* 00ffeb */ 0x90, 0x02,              /*    BCC   ECHO */
/* 00ffed */ 0x69, 0x06,              /*    ADC   #6 */
/* 00ffef */                          /* ECHO: */
/* 00ffef */ 0x2c, 0x12, 0xd0,        /*    BIT   DSP      ; PRINT A AS ASCII */
/* 00fff2 */ 0x30, 0xfb,              /*    BMI   ECHO */
/* 00fff4 */ 0x8d, 0x12, 0xd0,        /*    STA   DSP */
/* 00fff7 */ 0x60,                    /*    RTS */
/* 00fff8 */                          /* ; */
/* 00fff8 */ 0xff, 0xff, 0x00, 0x0f,  /* .byte $FF ,$FF ,$00 ,$0F ,$00 ,$FF ,$00 ,$00 */
/* 00fffc */ 0x00, 0xff, 0x00, 0x00,  /*  */
/* 010000 */                          /*  */
/* 010000 */                          /*  */
/* 010000 */                          /*  */
/* Total bytes read (size of array) 1106 dec, 452 hex */
