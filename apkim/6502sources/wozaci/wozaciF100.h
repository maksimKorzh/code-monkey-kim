reading wozaciF100.lst

assuming ca65 format, skipping leading 4 lines
and assuming max. 4 data bytes per line
/* 000000 */                          /* ; */
/* 000000 */                          /* ; The WOZ Apple Cassette Interface for the Apple 1 */
/* 000000 */                          /* ; Written by Steve Wozniak somewhere around 1976 */
/* 000000 */                          /* ; */
/* 000000 */                          /* ;------------------------------------------------------------------------- */
/* 000000 */                          /*  */
/* 000000 */                          /*         .org $F100 */
/* 00f100 */                          /*  */
/* 00f100 */                          /* ;------------------------------------------------------------------------- */
/* 00f100 */                          /* ; Memory declaration */
/* 00f100 */                          /* ;------------------------------------------------------------------------- */
/* 00f100 */                          /*  */
/* 00f100 */                          /*         HEX1L = $24             ; End address of dump block */
/* 00f100 */                          /*         HEX1H = $25 */
/* 00f100 */                          /*         HEX2L = $26             ; Begin address of dump block */
/* 00f100 */                          /*         HEX2H = $27 */
/* 00f100 */                          /*         SAVEINDEX = $28         ; Save index in input buffer */
/* 00f100 */                          /*         LASTSTATE = $29         ; Last input state */
/* 00f100 */                          /*  */
/* 00f100 */                          /*         IN = $0200              ; Input buffer */
/* 00f100 */                          /*         FLIP = $C000            ; Output flip-flop */
/* 00f100 */                          /*         TAPEIN = $C081          ; Tape input */
/* 00f100 */                          /*         KBD = $D010             ; PIA.A keyboard input */
/* 00f100 */                          /*         KBDCR = $D011           ; PIA.A keyboard control register */
/* 00f100 */                          /*         ESCAPE = $FF1A          ; Escape back to monitor */
/* 00f100 */                          /*         ECHO = $FFEF            ; Echo character to terminal */
/* 00f100 */                          /*  */
/* 00f100 */                          /* ;------------------------------------------------------------------------- */
/* 00f100 */                          /* ; Constants */
/* 00f100 */                          /* ;------------------------------------------------------------------------- */
/* 00f100 */                          /*  */
/* 00f100 */                          /*         CR = $8D                ; Carriage Return */
/* 00f100 */                          /*         ESC = $9B               ; ASCII ESC */
/* 00f100 */                          /*  */
/* 00f100 */                          /* ;------------------------------------------------------------------------- */
/* 00f100 */                          /* ; Let’s get started */
/* 00f100 */                          /* ;------------------------------------------------------------------------- */
/* 00f100 */                          /*  */
/* 00f100 */                          /* .export WOZACI */
/* 00f100 */ 0xa9, 0xaa,              /* WOZACI: LDA #$AA                ; Print the Tape prompt '*' */
/* 00f102 */ 0x20, 0xef, 0xff,        /*         JSR ECHO */
/* 00f105 */ 0xa9, 0x8d,              /*         LDA #CR                 ; And drop the cursor one line */
/* 00f107 */ 0x20, 0xef, 0xff,        /*         JSR ECHO */
/* 00f10a */                          /*  */
/* 00f10a */ 0xa0, 0xff,              /*         LDY #<-1                ; Reset the input buffer index */
/* 00f10c */ 0xc8,                    /* NEXTCHAR: INY */
/* 00f10d */ 0xad, 0x11, 0xd0,        /* KBDWAIT: LDA KBDCR              ; Wait for a key */
/* 00f110 */ 0x10, 0xfb,              /*         BPL KBDWAIT             ; Still no key! */
/* 00f112 */                          /*  */
/* 00f112 */ 0xad, 0x10, 0xd0,        /*         LDA KBD                 ; Read key from keyboard */
/* 00f115 */ 0x99, 0x00, 0x02,        /*         STA IN,Y                ; Save it into buffer */
/* 00f118 */ 0x20, 0xef, 0xff,        /*         JSR ECHO                ; And type it on the screen */
/* 00f11b */ 0xc9, 0x9b,              /*         CMP #ESC */
/* 00f11d */ 0xf0, 0xe1,              /*         BEQ WOZACI              ; Start from scratch if ESC! */
/* 00f11f */ 0xc9, 0x8d,              /*         CMP #CR */
/* 00f121 */ 0xd0, 0xe9,              /*         BNE NEXTCHAR            ; Read keys until CR */
/* 00f123 */                          /*  */
/* 00f123 */ 0xa2, 0xff,              /*         LDX #<-1                ; Initialize parse buffer pointer */
/* 00f125 */                          /*  */
/* 00f125 */                          /* ;------------------------------------------------------------------------- */
/* 00f125 */                          /* ; Start parsing first or a new tape command */
/* 00f125 */                          /* ;------------------------------------------------------------------------- */
/* 00f125 */                          /*  */
/* 00f125 */ 0xa9, 0x00,              /* NEXTCMD: LDA #0                 ; Clear begin and end values */
/* 00f127 */ 0x85, 0x24,              /*         STA HEX1L */
/* 00f129 */ 0x85, 0x25,              /*         STA HEX1H */
/* 00f12b */ 0x85, 0x26,              /*         STA HEX2L */
/* 00f12d */ 0x85, 0x27,              /*         STA HEX2H */
/* 00f12f */                          /*  */
/* 00f12f */ 0xe8,                    /* NEXTCHR: INX                    ; Increment input pointer */
/* 00f130 */ 0xbd, 0x00, 0x02,        /*         LDA IN,X                ; Get next char from input line */
/* 00f133 */ 0xc9, 0xd2,              /*         CMP #$D2                ; Read command? 'R' */
/* 00f135 */ 0xf0, 0x56,              /*         BEQ READ                ; Yes! */
/* 00f137 */ 0xc9, 0xd7,              /*         CMP #$D7                ; Write command? 'W' */
/* 00f139 */ 0xf0, 0x35,              /*         BEQ WRITE               ; Yes! (note: CY=1) */
/* 00f13b */ 0xc9, 0xae,              /*         CMP #$AE                ; Separator?'.' */
/* 00f13d */ 0xf0, 0x27,              /*         BEQ SEP                 ; Yes! */
/* 00f13f */ 0xc9, 0x8d,              /*         CMP #CR                 ; End of line? */
/* 00f141 */ 0xf0, 0x20,              /*         BEQ GOESC               ; Escape to monitor! We’re done */
/* 00f143 */ 0xc9, 0xa0,              /*         CMP #$A0                ; Ignore spaces: ' ' */
/* 00f145 */ 0xf0, 0xe8,              /*         BEQ NEXTCHR */
/* 00f147 */ 0x49, 0xb0,              /*         EOR #$B0                ; Map digits to 0-9 '0' */
/* 00f149 */ 0xc9, 0x0a,              /*         CMP #9+1                ; Is it a decimal digit? */
/* 00f14b */ 0x90, 0x06,              /*         BCC DIG                 ; Yes! */
/* 00f14d */ 0x69, 0x88,              /*         ADC #$88                ; Map letter 'A'-'F' to $FA-$FF */
/* 00f14f */ 0xc9, 0xfa,              /*         CMP #$FA                ; Hex letter? */
/* 00f151 */ 0x90, 0xad,              /*         BCC WOZACI              ; No! Character not hex! */
/* 00f153 */                          /*  */
/* 00f153 */ 0x0a,                    /* DIG:    ASL                     ; Hex digit to MSD of A */
/* 00f154 */ 0x0a,                    /*         ASL */
/* 00f155 */ 0x0a,                    /*         ASL */
/* 00f156 */ 0x0a,                    /*         ASL */
/* 00f157 */                          /*  */
/* 00f157 */ 0xa0, 0x04,              /*         LDY #4                  ; Shift count */
/* 00f159 */ 0x0a,                    /* HEXSHIFT: ASL                   ; Hex digit left, MSB to carry */
/* 00f15a */ 0x26, 0x24,              /*         ROL HEX1L               ; Rotate into LSD */
/* 00f15c */ 0x26, 0x25,              /*         ROL HEX1H               ; Rotate into MSD */
/* 00f15e */ 0x88,                    /*         DEY                     ; Done 4 shifts? */
/* 00f15f */ 0xd0, 0xf8,              /*         BNE HEXSHIFT            ; No! Loop */
/* 00f161 */ 0xf0, 0xcc,              /*         BEQ NEXTCHR             ; Handle next character */
/* 00f163 */                          /*  */
/* 00f163 */                          /* ;------------------------------------------------------------------------- */
/* 00f163 */                          /* ; Return to monitor, prints \ first */
/* 00f163 */                          /* ;------------------------------------------------------------------------- */
/* 00f163 */                          /*  */
/* 00f163 */ 0x4c, 0x1a, 0xff,        /* GOESC: JMP ESCAPE               ; Escape back to monitor */
/* 00f166 */                          /*  */
/* 00f166 */                          /* ;------------------------------------------------------------------------- */
/* 00f166 */                          /* ; Separating . found. Copy HEX1 to Hex2. Doesn’t clear HEX1!!! */
/* 00f166 */                          /* ;------------------------------------------------------------------------- */
/* 00f166 */                          /*  */
/* 00f166 */ 0xa5, 0x24,              /* SEP:    LDA HEX1L               ; Copy hex value 1 to hex value 2 */
/* 00f168 */ 0x85, 0x26,              /*         STA HEX2L */
/* 00f16a */ 0xa5, 0x25,              /*         LDA HEX1H */
/* 00f16c */ 0x85, 0x27,              /*         STA HEX2H */
/* 00f16e */ 0xb0, 0xbf,              /*         BCS NEXTCHR             ; Always taken! */
/* 00f170 */                          /*  */
/* 00f170 */                          /* ;------------------------------------------------------------------------- */
/* 00f170 */                          /* ; Write a block of memory to tape */
/* 00f170 */                          /* ;------------------------------------------------------------------------- */
/* 00f170 */                          /*  */
/* 00f170 */ 0xa9, 0x40,              /* WRITE:  LDA #64                 ; Write 10 second header */
/* 00f172 */ 0x20, 0xcc, 0xf1,        /*         JSR WHEADER */
/* 00f175 */                          /*  */
/* 00f175 */ 0x88,                    /* WRNEXT: DEY                     ; Compensate timing for extra work */
/* 00f176 */ 0xa2, 0x00,              /*         LDX #0                  ; Get next byte to write */
/* 00f178 */ 0xa1, 0x26,              /*         LDA (HEX2L,X) */
/* 00f17a */                          /*  */
/* 00f17a */ 0xa2, 0x10,              /*         LDX #8*2                ; Shift 8 bits (decremented twice) */
/* 00f17c */ 0x0a,                    /* WBITLOOP: ASL                   ; Shift MSB to carry */
/* 00f17d */ 0x20, 0xdb, 0xf1,        /*         JSR WRITEBIT            ; Write this bit */
/* 00f180 */ 0xd0, 0xfa,              /*         BNE WBITLOOP            ; Do all 8 bits! */
/* 00f182 */                          /*  */
/* 00f182 */ 0x20, 0xf1, 0xf1,        /*         JSR INCADDR             ; Increment address */
/* 00f185 */ 0xa0, 0x1e,              /*         LDY #30                 ; Compensate timer for extra work */
/* 00f187 */ 0x90, 0xec,              /*         BCC WRNEXT              ; Not done yet! Write next byte */
/* 00f189 */                          /*  */
/* 00f189 */ 0xa6, 0x28,              /* RESTIDX: LDX SAVEINDEX          ; Restore index in input line */
/* 00f18b */ 0xb0, 0x98,              /*         BCS NEXTCMD             ; Always taken! */
/* 00f18d */                          /*  */
/* 00f18d */                          /* ;------------------------------------------------------------------------- */
/* 00f18d */                          /* ; Read from tape */
/* 00f18d */                          /* ;------------------------------------------------------------------------- */
/* 00f18d */                          /*  */
/* 00f18d */ 0x20, 0xbc, 0xf1,        /* READ:   JSR FULLCYCLE           ; Wait until full cycle is detected */
/* 00f190 */ 0xa9, 0x16,              /*         LDA #22                 ; Introduce some delay to allow */
/* 00f192 */ 0x20, 0xcc, 0xf1,        /*         JSR WHEADER             ; the tape speed to stabilize */
/* 00f195 */ 0x20, 0xbc, 0xf1,        /*         JSR FULLCYCLE           ; Synchronize with full cycle */
/* 00f198 */                          /*  */
/* 00f198 */ 0xa0, 0x1f,              /* NOTSTART: LDY #31               ; Try to detect the much shorter */
/* 00f19a */ 0x20, 0xbf, 0xf1,        /*         JSR CMPLEVEL            ; start bit */
/* 00f19d */ 0xb0, 0xf9,              /*         BCS NOTSTART            ; Start bit not detected yet! */
/* 00f19f */                          /*  */
/* 00f19f */ 0x20, 0xbf, 0xf1,        /*         JSR CMPLEVEL            ; Wait for 2nd phase of start bit */
/* 00f1a2 */                          /*  */
/* 00f1a2 */ 0xa0, 0x3a,              /*         LDY #58                 ; Set threshold value in middle */
/* 00f1a4 */ 0xa2, 0x08,              /* RDBYTE: LDX #8                  ; Receiver 8 bits */
/* 00f1a6 */ 0x48,                    /* RDBIT:  PHA */
/* 00f1a7 */ 0x20, 0xbc, 0xf1,        /*         JSR FULLCYCLE           ; Detect a full cycle */
/* 00f1aa */ 0x68,                    /*         PLA */
/* 00f1ab */ 0x2a,                    /*         ROL                     ; Roll new bit into result */
/* 00f1ac */ 0xa0, 0x39,              /*         LDY #57                 ; Set threshold value in middle */
/* 00f1ae */ 0xca,                    /*         DEX                     ; Decrement bit counter */
/* 00f1af */ 0xd0, 0xf5,              /*         BNE RDBIT               ; Read next bit! */
/* 00f1b1 */ 0x81, 0x26,              /*         STA (HEX2L,X)           ; Save new byte */
/* 00f1b3 */                          /*  */
/* 00f1b3 */ 0x20, 0xf1, 0xf1,        /*         JSR INCADDR             ; Increment address */
/* 00f1b6 */ 0xa0, 0x35,              /*         LDY #53                 ; Compensate threshold with workload */
/* 00f1b8 */ 0x90, 0xea,              /*         BCC RDBYTE              ; Do next byte if not done yet! */
/* 00f1ba */ 0xb0, 0xcd,              /*         BCS RESTIDX             ; Always taken! Restore parse index */
/* 00f1bc */                          /*  */
/* 00f1bc */ 0x20, 0xbf, 0xf1,        /* FULLCYCLE: JSR CMPLEVEL         ; Wait for two level changes */
/* 00f1bf */ 0x88,                    /* CMPLEVEL: DEY                   ; Decrement time counter */
/* 00f1c0 */ 0xad, 0x81, 0xc0,        /*         LDA TAPEIN              ; Get Tape In data */
/* 00f1c3 */ 0xc5, 0x29,              /*         CMP LASTSTATE           ; Same as before? */
/* 00f1c5 */ 0xf0, 0xf8,              /*         BEQ CMPLEVEL            ; Yes! */
/* 00f1c7 */ 0x85, 0x29,              /*         STA LASTSTATE           ; Save new data */
/* 00f1c9 */                          /*  */
/* 00f1c9 */ 0xc0, 0x80,              /*         CPY #128                ; Compare threshold */
/* 00f1cb */ 0x60,                    /*         RTS */
/* 00f1cc */                          /*  */
/* 00f1cc */                          /* ;------------------------------------------------------------------------- */
/* 00f1cc */                          /* ; Write header to tape */
/* 00f1cc */                          /* ; */
/* 00f1cc */                          /* ; The header consists of an asymmetric cycle, starting with one phase of */
/* 00f1cc */                          /* ; approximately (66+47)x5=565us, followed by a second phase of */
/* 00f1cc */                          /* ; approximately (44+47)x5=455us. */
/* 00f1cc */                          /* ; Total cycle duration is approximately 1020us ~ 1kHz. The actual */
/* 00f1cc */                          /* ; frequency will be a bit lower because of the additional workload between */
/* 00f1cc */                          /* ; the two loops. */
/* 00f1cc */                          /* ; The header ends with a short phase of (30+47)x5=385us and a normal */
/* 00f1cc */                          /* ; phase of (44+47)x5=455us. This start bit must be detected by the read */
/* 00f1cc */                          /* ; routine to trigger the reading of the actual data. */
/* 00f1cc */                          /* ;------------------------------------------------------------------------- */
/* 00f1cc */                          /*  */
/* 00f1cc */ 0x86, 0x28,              /* WHEADER: STX SAVEINDEX          ; Save index in input line */
/* 00f1ce */ 0xa0, 0x42,              /* HCOUNT: LDY #66                 ; Extra long delay */
/* 00f1d0 */ 0x20, 0xe0, 0xf1,        /*         JSR WDELAY              ; CY is constantly 1, writing a 1 */
/* 00f1d3 */ 0xd0, 0xf9,              /*         BNE HCOUNT              ; Do this 64 * 256 time! */
/* 00f1d5 */ 0x69, 0xfe,              /*         ADC #<-2                ; Decrement A (CY=1 all the time) */
/* 00f1d7 */ 0xb0, 0xf5,              /*         BCS HCOUNT              ; Not all done! */
/* 00f1d9 */ 0xa0, 0x1e,              /*         LDY #30                 ; Write a final short bit (start) */
/* 00f1db */                          /* ; */
/* 00f1db */                          /* ;------------------------------------------------------------------------- */
/* 00f1db */                          /* ; Write a full bit cycle */
/* 00f1db */                          /* ; */
/* 00f1db */                          /* ; Upon entry Y contains a compensated value for the first phase of 0 */
/* 00f1db */                          /* ; bit length. All subsequent loops don’t have to be time compensated. */
/* 00f1db */                          /* ;------------------------------------------------------------------------- */
/* 00f1db */                          /*  */
/* 00f1db */ 0x20, 0xe0, 0xf1,        /* WRITEBIT: JSR WDELAY            ; Do two equal phases */
/* 00f1de */ 0xa0, 0x2c,              /*         LDY #44                 ; Load 250us counter - compensation */
/* 00f1e0 */                          /*  */
/* 00f1e0 */ 0x88,                    /* WDELAY: DEY                     ; Delay 250us (one phase of 2kHz) */
/* 00f1e1 */ 0xd0, 0xfd,              /*         BNE WDELAY */
/* 00f1e3 */ 0x90, 0x05,              /*         BCC WRITE1              ; Write a '1' (2kHz) */
/* 00f1e5 */                          /*  */
/* 00f1e5 */ 0xa0, 0x2f,              /*         LDY #47                 ; Additional delay for '0' (1kHz) */
/* 00f1e7 */ 0x88,                    /* WDELAY0: DEY                    ; (delay 250us) */
/* 00f1e8 */ 0xd0, 0xfd,              /*         BNE WDELAY0 */
/* 00f1ea */                          /*  */
/* 00f1ea */ 0xbc, 0x00, 0xc0,        /* WRITE1: LDY FLIP,X              ; Flip the output bit */
/* 00f1ed */ 0xa0, 0x29,              /*         LDY #41                 ; Reload 250us cntr (compensation) */
/* 00f1ef */ 0xca,                    /*         DEX                     ; Decrement bit counter */
/* 00f1f0 */ 0x60,                    /*         RTS */
/* 00f1f1 */                          /*  */
/* 00f1f1 */                          /* ;------------------------------------------------------------------------- */
/* 00f1f1 */                          /* ; Increment current address and compare with last address */
/* 00f1f1 */                          /* ;------------------------------------------------------------------------- */
/* 00f1f1 */                          /*  */
/* 00f1f1 */ 0xa5, 0x26,              /* INCADDR: LDA HEX2L              ; Compare current address with */
/* 00f1f3 */ 0xc5, 0x24,              /*         CMP HEX1L               ; end address */
/* 00f1f5 */ 0xa5, 0x27,              /*         LDA HEX2H */
/* 00f1f7 */ 0xe5, 0x25,              /*         SBC HEX1H */
/* 00f1f9 */ 0xe6, 0x26,              /*         INC HEX2L               ; And increment current address */
/* 00f1fb */ 0xd0, 0x02,              /*         BNE NOCARRY             ; No carry to MSB! */
/* 00f1fd */ 0xe6, 0x27,              /*         INC HEX2H */
/* 00f1ff */ 0x60,                    /* NOCARRY: RTS */
/* 00f200 */                          /*  */
/* 00f200 */                          /* ;------------------------------------------------------------------------- */
/* 00f200 */                          /*  */
/* Total bytes read (size of array) 256 dec, 100 hex */
