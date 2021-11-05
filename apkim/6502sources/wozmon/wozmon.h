/* 000000 */                          /* ;  Written by Steve Wozniak in 1976 */
/* 000000 */                          /*  */
/* 000000 */                          /*  */
/* 000000 */                          /* ; Page 0 Variables */
/* 000000 */                          /*  */
/* 000000 */                          /* XAML            = $24           ;  Last "opened" location Low */
/* 000000 */                          /* XAMH            = $25           ;  Last "opened" location High */
/* 000000 */                          /* STL             = $26           ;  Store address Low */
/* 000000 */                          /* STH             = $27           ;  Store address High */
/* 000000 */                          /* L               = $28           ;  Hex value parsing Low */
/* 000000 */                          /* H               = $29           ;  Hex value parsing High */
/* 000000 */                          /* YSAV            = $2A           ;  Used to see if hex value is given */
/* 000000 */                          /* MODE            = $2B           ;  $00=XAM, $7F=STOR, $AE=BLOCK XAM */
/* 000000 */                          /*  */
/* 000000 */                          /*  */
/* 000000 */                          /* ; Other Variables */
/* 000000 */                          /*  */
/* 000000 */                          /* IN              = $0200         ;  Input buffer to $027F */
/* 000000 */                          /* KBD             = $D010         ;  PIA.A keyboard input */
/* 000000 */                          /* KBDCR           = $D011         ;  PIA.A keyboard control register */
/* 000000 */                          /* DSP             = $D012         ;  PIA.B display output register */
/* 000000 */                          /* DSPCR           = $D013         ;  PIA.B display control register */
/* 000000 */                          /*  */
/* 000000 */                          /*                .org $FF00 */
/* 00ff00 */                          /*                .export RESET */
/* 00ff00 */                          /*  */
/* 00ff00 */ 0xd8,                    /* RESET:          CLD             ; Clear decimal arithmetic mode. */
/* 00ff01 */ 0x58,                    /*                 CLI */
/* 00ff02 */ 0xa0, 0x7f,              /*                 LDY #$7F        ; Mask for DSP data direction register. */
/* 00ff04 */ 0x8c, 0x12, 0xd0,        /*                 STY DSP         ; Set it up. */
/* 00ff07 */ 0xa9, 0xa7,              /*                 LDA #$A7        ; KBD and DSP control register mask. */
/* 00ff09 */ 0x8d, 0x11, 0xd0,        /*                 STA KBDCR       ; Enable interrupts, set CA1, CB1, for */
/* 00ff0c */ 0x8d, 0x13, 0xd0,        /*                 STA DSPCR       ; positive edge sense/output mode. */
/* 00ff0f */ 0xc9, 0xdf,              /* NOTCR:          CMP #'_'+$80    ; "_"? */
/* 00ff11 */ 0xf0, 0x13,              /*                 BEQ BACKSPACE   ; Yes. */
/* 00ff13 */ 0xc9, 0x9b,              /*                 CMP #$9B        ; ESC? */
/* 00ff15 */ 0xf0, 0x03,              /*                 BEQ ESCAPE      ; Yes. */
/* 00ff17 */ 0xc8,                    /*                 INY             ; Advance text index. */
/* 00ff18 */ 0x10, 0x0f,              /*                 BPL NEXTCHAR    ; Auto ESC if > 127. */
/* 00ff1a */ 0xa9, 0xdc,              /* ESCAPE:         LDA #'\'+$80    ; "\". */
/* 00ff1c */ 0x20, 0xef, 0xff,        /*                 JSR ECHO        ; Output it. */
/* 00ff1f */ 0xa9, 0x8d,              /* GETLINE:        LDA #$8D        ; CR. */
/* 00ff21 */ 0x20, 0xef, 0xff,        /*                 JSR ECHO        ; Output it. */
/* 00ff24 */ 0xa0, 0x01,              /*                 LDY #$01        ; Initialize text index. */
/* 00ff26 */ 0x88,                    /* BACKSPACE:      DEY             ; Back up text index. */
/* 00ff27 */ 0x30, 0xf6,              /*                 BMI GETLINE     ; Beyond start of line, reinitialize. */
/* 00ff29 */ 0xad, 0x11, 0xd0,        /* NEXTCHAR:       LDA KBDCR       ; Key ready? */
/* 00ff2c */ 0x10, 0xfb,              /*                 BPL NEXTCHAR    ; Loop until ready. */
/* 00ff2e */ 0xad, 0x10, 0xd0,        /*                 LDA KBD         ; Load character. B7 should be ‘1’. */
/* 00ff31 */ 0x99, 0x00, 0x02,        /*                 STA IN,Y        ; Add to text buffer. */
/* 00ff34 */ 0x20, 0xef, 0xff,        /*                 JSR ECHO        ; Display character. */
/* 00ff37 */ 0xc9, 0x8d,              /*                 CMP #$8D        ; CR? */
/* 00ff39 */ 0xd0, 0xd4,              /*                 BNE NOTCR       ; No. */
/* 00ff3b */ 0xa0, 0xff,              /*                 LDY #$FF        ; Reset text index. */
/* 00ff3d */ 0xa9, 0x00,              /*                 LDA #$00        ; For XAM mode. */
/* 00ff3f */ 0xaa,                    /*                 TAX             ; 0->X. */
/* 00ff40 */ 0x0a,                    /* SETSTOR:        ASL             ; Leaves $7B if setting STOR mode. */
/* 00ff41 */ 0x85, 0x2b,              /* SETMODE:        STA MODE        ; $00=XAM $7B=STOR $AE=BLOK XAM */
/* 00ff43 */ 0xc8,                    /* BLSKIP:         INY             ; Advance text index. */
/* 00ff44 */ 0xb9, 0x00, 0x02,        /* NEXTITEM:       LDA IN,Y        ; Get character. */
/* 00ff47 */ 0xc9, 0x8d,              /*                 CMP #$8D        ; CR? */
/* 00ff49 */ 0xf0, 0xd4,              /*                 BEQ GETLINE     ; Yes, done this line. */
/* 00ff4b */ 0xc9, 0xae,              /*                 CMP #'.'+$80    ; "."? */
/* 00ff4d */ 0x90, 0xf4,              /*                 BCC BLSKIP      ; Skip delimiter. */
/* 00ff4f */ 0xf0, 0xf0,              /*                 BEQ SETMODE     ; Yes. Set STOR mode. */
/* 00ff51 */ 0xc9, 0xba,              /*                 CMP #':'+$80    ; ":"? */
/* 00ff53 */ 0xf0, 0xeb,              /*                 BEQ SETSTOR     ; Yes. Set STOR mode. */
/* 00ff55 */ 0xc9, 0xd2,              /*                 CMP #'R'+$80    ; "R"? */
/* 00ff57 */ 0xf0, 0x3b,              /*                 BEQ RUN         ; Yes. Run user program. */
/* 00ff59 */ 0x86, 0x28,              /*                 STX L           ; $00-> L. */
/* 00ff5b */ 0x86, 0x29,              /*                 STX H           ; and H. */
/* 00ff5d */ 0x84, 0x2a,              /*                 STY YSAV        ; Save Y for comparison. */
/* 00ff5f */ 0xb9, 0x00, 0x02,        /* NEXTHEX:        LDA IN,Y        ; Get character for hex test. */
/* 00ff62 */ 0x49, 0xb0,              /*                 EOR #$B0        ; Map digits to $0-9. */
/* 00ff64 */ 0xc9, 0x0a,              /*                 CMP #$0A        ; Digit? */
/* 00ff66 */ 0x90, 0x06,              /*                 BCC DIG         ; Yes. */
/* 00ff68 */ 0x69, 0x88,              /*                 ADC #$88        ; Map letter "A"-"F" to $FA-FF. */
/* 00ff6a */ 0xc9, 0xfa,              /*                 CMP #$FA        ; Hex letter? */
/* 00ff6c */ 0x90, 0x11,              /*                 BCC NOTHEX      ; No, character not hex. */
/* 00ff6e */ 0x0a,                    /* DIG:            ASL */
/* 00ff6f */ 0x0a,                    /*                 ASL             ; Hex digit to MSD of A. */
/* 00ff70 */ 0x0a,                    /*                 ASL */
/* 00ff71 */ 0x0a,                    /*                 ASL */
/* 00ff72 */ 0xa2, 0x04,              /*                 LDX #$04        ; Shift count. */
/* 00ff74 */ 0x0a,                    /* HEXSHIFT:       ASL             ; Hex digit left, MSB to carry. */
/* 00ff75 */ 0x26, 0x28,              /*                 ROL L           ; Rotate into LSD. */
/* 00ff77 */ 0x26, 0x29,              /*                 ROL H           ;  Rotate into MSD’s. */
/* 00ff79 */ 0xca,                    /*                 DEX             ; Done 4 shifts? */
/* 00ff7a */ 0xd0, 0xf8,              /*                 BNE HEXSHIFT    ; No, loop. */
/* 00ff7c */ 0xc8,                    /*                 INY             ; Advance text index. */
/* 00ff7d */ 0xd0, 0xe0,              /*                 BNE NEXTHEX     ; Always taken. Check next char for hex. */
/* 00ff7f */ 0xc4, 0x2a,              /* NOTHEX:         CPY YSAV        ; Check if L, H empty (no hex digits). */
/* 00ff81 */ 0xf0, 0x97,              /*                 BEQ ESCAPE      ; Yes, generate ESC sequence. */
/* 00ff83 */ 0x24, 0x2b,              /*                 BIT MODE        ; Test MODE byte. */
/* 00ff85 */ 0x50, 0x10,              /*                 BVC NOTSTOR     ;  B6=0 STOR 1 for XAM & BLOCK XAM */
/* 00ff87 */ 0xa5, 0x28,              /*                 LDA L           ; LSD’s of hex data. */
/* 00ff89 */ 0x81, 0x26,              /*                 STA (STL,X)     ; Store at current ‘store index’. */
/* 00ff8b */ 0xe6, 0x26,              /*                 INC STL         ; Increment store index. */
/* 00ff8d */ 0xd0, 0xb5,              /*                 BNE NEXTITEM    ; Get next item. (no carry). */
/* 00ff8f */ 0xe6, 0x27,              /*                 INC STH         ; Add carry to ‘store index’ high order. */
/* 00ff91 */ 0x4c, 0x44, 0xff,        /* TONEXTITEM:     JMP NEXTITEM    ; Get next command item. */
/* 00ff94 */ 0x6c, 0x24, 0x00,        /* RUN:            JMP (XAML)      ; Run at current XAM index. */
/* 00ff97 */ 0x30, 0x2b,              /* NOTSTOR:        BMI XAMNEXT     ; B7=0 for XAM, 1 for BLOCK XAM. */
/* 00ff99 */ 0xa2, 0x02,              /*                 LDX #$02        ; Byte count. */
/* 00ff9b */ 0xb5, 0x27,              /* SETADR:         LDA L-1,X       ; Copy hex data to */
/* 00ff9d */ 0x95, 0x25,              /*                 STA STL-1,X     ; ‘store index’. */
/* 00ff9f */ 0x95, 0x23,              /*                 STA XAML-1,X    ; And to ‘XAM index’. */
/* 00ffa1 */ 0xca,                    /*                 DEX             ; Next of 2 bytes. */
/* 00ffa2 */ 0xd0, 0xf7,              /*                 BNE SETADR      ; Loop unless X=0. */
/* 00ffa4 */ 0xd0, 0x14,              /* NXTPRNT:        BNE PRDATA      ; NE means no address to print. */
/* 00ffa6 */ 0xa9, 0x8d,              /*                 LDA #$8D        ; CR. */
/* 00ffa8 */ 0x20, 0xef, 0xff,        /*                 JSR ECHO        ; Output it. */
/* 00ffab */ 0xa5, 0x25,              /*                 LDA XAMH        ; ‘Examine index’ high-order byte. */
/* 00ffad */ 0x20, 0xdc, 0xff,        /*                 JSR PRBYTE      ; Output it in hex format. */
/* 00ffb0 */ 0xa5, 0x24,              /*                 LDA XAML        ; Low-order ‘examine index’ byte. */
/* 00ffb2 */ 0x20, 0xdc, 0xff,        /*                 JSR PRBYTE      ; Output it in hex format. */
/* 00ffb5 */ 0xa9, 0xba,              /*                 LDA #':'+$80    ; ":". */
/* 00ffb7 */ 0x20, 0xef, 0xff,        /*                 JSR ECHO        ; Output it. */
/* 00ffba */ 0xa9, 0xa0,              /* PRDATA:         LDA #$A0        ; Blank. */
/* 00ffbc */ 0x20, 0xef, 0xff,        /*                 JSR ECHO        ; Output it. */
/* 00ffbf */ 0xa1, 0x24,              /*                 LDA (XAML,X)    ; Get data byte at ‘examine index’. */
/* 00ffc1 */ 0x20, 0xdc, 0xff,        /*                 JSR PRBYTE      ; Output it in hex format. */
/* 00ffc4 */ 0x86, 0x2b,              /* XAMNEXT:        STX MODE        ; 0->MODE (XAM mode). */
/* 00ffc6 */ 0xa5, 0x24,              /*                 LDA XAML */
/* 00ffc8 */ 0xc5, 0x28,              /*                 CMP L           ; Compare ‘examine index’ to hex data. */
/* 00ffca */ 0xa5, 0x25,              /*                 LDA XAMH */
/* 00ffcc */ 0xe5, 0x29,              /*                 SBC H */
/* 00ffce */ 0xb0, 0xc1,              /*                 BCS TONEXTITEM  ; Not less, so no more data to output. */
/* 00ffd0 */ 0xe6, 0x24,              /*                 INC XAML */
/* 00ffd2 */ 0xd0, 0x02,              /*                 BNE MOD8CHK     ; Increment ‘examine index’. */
/* 00ffd4 */ 0xe6, 0x25,              /*                 INC XAMH */
/* 00ffd6 */ 0xa5, 0x24,              /* MOD8CHK:        LDA XAML        ; Check low-order ‘examine index’ byte */
/* 00ffd8 */ 0x29, 0x07,              /*                 AND #$07        ; For MOD 8=0 */
/* 00ffda */ 0x10, 0xc8,              /*                 BPL NXTPRNT     ; Always taken. */
/* 00ffdc */ 0x48,                    /* PRBYTE:         PHA             ; Save A for LSD. */
/* 00ffdd */ 0x4a,                    /*                 LSR */
/* 00ffde */ 0x4a,                    /*                 LSR */
/* 00ffdf */ 0x4a,                    /*                 LSR             ; MSD to LSD position. */
/* 00ffe0 */ 0x4a,                    /*                 LSR */
/* 00ffe1 */ 0x20, 0xe5, 0xff,        /*                 JSR PRHEX       ; Output hex digit. */
/* 00ffe4 */ 0x68,                    /*                 PLA             ; Restore A. */
/* 00ffe5 */ 0x29, 0x0f,              /* PRHEX:          AND #$0F        ; Mask LSD for hex print. */
/* 00ffe7 */ 0x09, 0xb0,              /*                 ORA #'0'+$80    ; Add "0". */
/* 00ffe9 */ 0xc9, 0xba,              /*                 CMP #$BA        ; Digit? */
/* 00ffeb */ 0x90, 0x02,              /*                 BCC ECHO        ; Yes, output it. */
/* 00ffed */ 0x69, 0x06,              /*                 ADC #$06        ; Add offset for letter. */
/* 00ffef */ 0x2c, 0x12, 0xd0,        /* ECHO:           BIT DSP         ; bit (B7) cleared yet? */
/* 00fff2 */ 0x30, 0xfb,              /*                 BMI ECHO        ; No, wait for display. */
/* 00fff4 */ 0x8d, 0x12, 0xd0,        /*                 STA DSP         ; Output character. Sets DA. */
/* 00fff7 */ 0x60,                    /*                 RTS             ; Return. */
/* 00fff8 */                          /*  */
/* 00fff8 */ 0x00,                    /*                 BRK             ; unused */
/* 00fff9 */ 0x00,                    /*                 BRK             ; unused */
/* 00fffa */                          /*  */
/* 00fffa */                          /* ; Interrupt Vectors */
/* 00fffa */                          /*  */
/* 00fffa */ 0x00, 0x0f,              /*                 .WORD $0F00     ; NMI */
/* 00fffc */ 0x00, 0xff,              /*                 .WORD RESET     ; RESET */
/* 00fffe */ 0x00, 0x00,              /*                 .WORD $0000     ; BRK/IRQ */
/* 00fffe */                          /*  */
/* Total bytes read (size of array) 256 dec, 100 hex */
