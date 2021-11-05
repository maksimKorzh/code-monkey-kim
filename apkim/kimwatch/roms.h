#define FALSE          0
#define TRUE           !(FALSE)

/*    unsigned char rom002[1024] = {
    unsigned char rom003[1024] = {
    unsigned char mchessROM[1393] = {
    unsigned char movitROM[95] = {
    unsigned char relocateROM[149] = {
    unsigned char branchROM[42] = {
    unsigned char disasmROM[505] = {
    unsigned char calcROM[8164] = {
    unsigned char fltptDemo[31] = {
    unsigned char vtl02ROM[1023] = {
    unsigned char hiLoDemo[110] = {
    unsigned char timerDemo[100] = {
    unsigned char fbkDemo[9] = {
    unsigned char apple1ROM[4094] = {
    life
*/
// KIM UNO software library
//
// if you want to add software, store it in a binary array here,
// and add the array to the ROMs in the memory map (cpu.c,read6502())
// you can also copy an array into RAM, see cpu.c,loadTestProgram().
//
// All ROMS together do not fit in a tiny Arduino Pro Mini, make your selection below.
// If you copy to RAM, you also have to choose which programs are loaded.

// --- ROM MAP ----------------------------------------------------------------
// ROM1: KIM-1 ROM002 (monitor main code)                                 $17C0
// ROM2: KIM-1 ROM003 (tape and RS-232 code)                              $1780
// disassembler: Baum & Wozniak disassember                               $2000
// baum-Wozniak mini-assembler ($3ddd) & Wozmon ($3f00)                   $3BAE
// calc: 6502 floating point library from crbond.com/calc65.htm           $5000
// special I/O area for programmable calculator (part of calc@5000)       $7000
// mchess: microchess from benlo.com/microchess recompiled to start at    $C000
// Apple-1 Basic                                                          $E000
// vtl-02: from 6502.org                                                  $DC00
// ----------------------------------------------------------------------------

// --- RAM MAP: sw preloaded in RAM from storage arrays below------------------
// relocate (first book of kim)                                 copied to $0110
// branch (first book of kim)                                   copied to $01A5
// movit (first book of kim)                                    copied to $1780
// either FIRST or HI_LO or TIME (prgs from 1st bk of KIM)      copied to $0200
// if CALC ROM selected, a demo program in RAM is               copied to $0210
// ----------------------------------------------------------------------------



// ============================================================================
// ============================================================================
// KIM ROM 002
// ============================================================================
// ============================================================================
// always installed. This array contains assembler output in comments.
#if _TARGET == PERSONAL_COMPUTER
    unsigned char rom002[1024] = {
#else
    const unsigned char rom002[1024] PROGMEM = {
#endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  ;
/* 0000 */                     //  ;       ** INTERRUPTS **
/* 0000 */                     //          .org  $1C00
/* 1C00 */                     //  ;
/* 1C00 */                     //  ;******************* 6530-002 I.C. *****************
/* 1C00 */                     //  ;       ** COPYRIGHT MOS TECHNOLOGY INC.
/* 1C00 */                     //  ;          DATE OCT 13, 1975 REV E
/* 1C00 */                     //  ;
/* 1C00 */                     //  ;       ** KIM **
/* 1C00 */                     //  ;         TTY INTERFACE   6530-002
/* 1C00 */                     //  ;         KEYBOARD INTERFACE,
/* 1C00 */                     //  ;         7-SEGMENT 6-DIGIT DISPLAY
/* 1C00 */                     //  ;
/* 1C00 */                     //  ;       TTY COMANDS:
/* 1C00 */                     //  ;         G    GOEXEC
/* 1C00 */                     //  ;         CR   OPEN NEXT CELL
/* 1C00 */                     //  ;         LF   OPEN PREVIOUS CELL
/* 1C00 */                     //  ;         .    MODIFY OPEN CELL
/* 1C00 */                     //  ;         SP   OPEN NEW CELL
/* 1C00 */                     //  ;         L    LOAD (OBJECT FORMAT)
/* 1C00 */                     //  ;         Q    DUMP FROM OPEN CELL ADDR TO HI LIMIT
/* 1C00 */                     //  ;         RO   RUB OUT - RETURN TO START KIM
/* 1C00 */                     //  ;               (ALL ILLEGAL CHARS ARE IGNORED)
/* 1C00 */                     //  ;
/* 1C00 */                     //  ;       KEYBOARD COMMANDS:
/* 1C00 */                     //  ;         ADDR  SETS MODE TO MODIFY CELL ADDRESS
/* 1C00 */                     //  ;         DATA  SETS MODE TO MODIFY DATA IN OPEN CELL
/* 1C00 */                     //  ;         STEP  INCREMENTS TO NEXT CELL
/* 1C00 */                     //  ;         RST   SYSTEM RESET
/* 1C00 */                     //  ;         RUN   GOEXEC
/* 1C00 */                     //  ;         STOP  $1C00 CAN BE LOADED INTO NMIV TO USE
/* 1C00 */                     //  ;         PC    DISPLAY PC (PROGRAM COUNTER)
/* 1C00 */                     //  ;
/* 1C00 */                     //          .org  $1C00
/* 1C00 */ 0x85, 0xF3,         //  SAVE    STA   ACC       ; KIM ENTRY VIA STOP (NMI)      1C00
/* 1C02 */ 0x68,               //          PLA             ; OR BRK (IRQ)
/* 1C03 */ 0x85, 0xF1,         //          STA   PREG
/* 1C05 */ 0x68,               //          PLA             ; KIM ENTRY VIA JSR (A LOST)    1C05
/* 1C06 */ 0x85, 0xEF,         //          STA   PCL
/* 1C08 */ 0x85, 0xFA,         //          STA   POINTL
/* 1C0A */ 0x68,               //          PLA
/* 1C0B */ 0x85, 0xF0,         //          STA   PCH
/* 1C0D */ 0x85, 0xFB,         //          STA   POINTH
/* 1C0F */ 0x84, 0xF4,         //          STY   YREG
/* 1C11 */ 0x86, 0xF5,         //          STX   XREG
/* 1C13 */ 0xBA,               //          TSX
/* 1C14 */ 0x86, 0xF2,         //          STX   SPUSER
/* 1C16 */ 0x20, 0x88, 0x1E,   //          JSR   INITS
/* 1C19 */ 0x4C, 0x4F, 0x1C,   //          JMP   START
/* 1C1C */                     //  ;
/* 1C1C */ 0x6C, 0xFA, 0x17,   //  NMIT    JMP   (NMIV)    ; NON-MASKABLE INTERRUPT TRAP   1C1C
/* 1C1F */ 0x6C, 0xFE, 0x17,   //  IRQT    JMP   (IRQV)    ; INTERRUPT TRAP                1C1F
/* 1C22 */ 0xA2, 0xFF,         //  RST     LDX   #$FF      ; KIM ENTRY VIA RST             1C22
/* 1C24 */ 0x9A,               //          TXS
/* 1C25 */ 0x86, 0xF2,         //          STX   SPUSER
/* 1C27 */ 0x20, 0x88, 0x1E,   //          JSR   INITS
/* 1C2A */ 0xA9, 0xFF,         //          LDA   #$FF      ; COUNT START BIT
/* 1C2C */ 0x8D, 0xF3, 0x17,   //          STA   CNTH30    ; ZERO CNTH30
/* 1C2F */ 0xA9, 0x01,         //          LDA   #$01      ; MASK HI ORDER BITS
/* 1C31 */ 0x2C, 0x40, 0x17,   //  DET1    BIT   SAD       ; TEST                          1C31
/* 1C34 */ 0xD0, 0x19,         //          BNE   START     ; KEYBD SSW TEST
/* 1C36 */ 0x30, 0xF9,         //          BMI   DET1      ; START BIT TEST
/* 1C38 */ 0xA9, 0xFC,         //          LDA   #$FC
/* 1C3A */ 0x18,               //  DET3    CLC             ; THIS LOOP COUNTS              1C3A
/* 1C3B */ 0x69, 0x01,         //          ADC   #$01      ; THE START BIT TIME
/* 1C3D */ 0x90, 0x03,         //          BCC   DET2
/* 1C3F */ 0xEE, 0xF3, 0x17,   //          INC   CNTH30
/* 1C42 */ 0xAC, 0x40, 0x17,   //  DET2    LDY   SAD       ; CHECK FOR END OF START BIT    1C42
/* 1C45 */ 0x10, 0xF3,         //          BPL   DET3
/* 1C47 */ 0x8D, 0xF2, 0x17,   //          STA   CNTL30
/* 1C4A */ 0xA2, 0x08,         //          LDX   #$08
/* 1C4C */ 0x20, 0x6A, 0x1E,   //          JSR   GET5      ; GET REST OF THE CHAR, TEST CHAR
/* 1C4F */                     //  ;       ** MAKE TTY/KB SELECTION **
/* 1C4F */ 0x20, 0x8C, 0x1E,   //  START   JSR   INIT1     ;                               1C4F
/* 1C52 */ 0xA9, 0x01,         //          LDA   #$01
/* 1C54 */ 0x2C, 0x40, 0x17,   //          BIT   SAD
/* 1C57 */ 0xD0, 0x1E,         //          BNE   TTYKB
/* 1C59 */ 0x20, 0x2F, 0x1E,   //          JSR   CRLF      ; PRT CR LF
/* 1C5C */ 0xA2, 0x0A,         //          LDX   #$0A      ; TYPE OUT KIM
/* 1C5E */ 0x20, 0x31, 0x1E,   //          JSR   PRTST
/* 1C61 */ 0x4C, 0xAF, 0x1D,   //          JMP   SHOW1
/* 1C64 */                     //  ;
/* 1C64 */ 0xA9, 0x00,         //  CLEAR   LDA   #$00
/* 1C66 */ 0x85, 0xF8,         //          STA   INL       ; CLEAR INPUT BUFFER
/* 1C68 */ 0x85, 0xF9,         //          STA   INH
/* 1C6A */ 0x20, 0x5A, 0x1E,   //  READ    JSR   GETCH     ; GET CHAR
/* 1C6D */ 0xC9, 0x01,         //          CMP   #$01
/* 1C6F */ 0xF0, 0x06,         //          BEQ   TTYKB
/* 1C71 */ 0x20, 0xAC, 0x1F,   //          JSR   PACK
/* 1C74 */ 0x4C, 0xDB, 0x1D,   //          JMP   SCAN
/* 1C77 */                     //  ;       ** MAIN ROUTINE FOR KEYBOARD AND DISPLAY **
/* 1C77 */ 0x20, 0x19, 0x1F,   //  TTYKB   JSR   SCAND     ; IF A=0 NO KEY                 1C77
/* 1C7A */ 0xD0, 0xD3,         //          BNE   START
/* 1C7C */ 0xA9, 0x01,         //  TTYKB1  LDA   #$01
/* 1C7E */ 0x2C, 0x40, 0x17,   //          BIT   SAD
/* 1C81 */ 0xF0, 0xCC,         //          BEQ   START
/* 1C83 */ 0x20, 0x19, 0x1F,   //          JSR   SCAND
/* 1C86 */ 0xF0, 0xF4,         //          BEQ   TTYKB1
/* 1C88 */ 0x20, 0x19, 0x1F,   //          JSR   SCAND
/* 1C8B */ 0xF0, 0xEF,         //          BEQ   TTYKB1
/* 1C8D */ 0x20, 0x6A, 0x1F,   //          JSR   GETKEY
/* 1C90 */ 0xC9, 0x15,         //          CMP   #$15
/* 1C92 */ 0x10, 0xBB,         //          BPL   START
/* 1C94 */ 0xC9, 0x14,         //          CMP   #$14
/* 1C96 */ 0xF0, 0x44,         //          BEQ   PCCMD     ; DISPLAY PC
/* 1C98 */ 0xC9, 0x10,         //          CMP   #$10      ; ADDR MODE=1
/* 1C9A */ 0xF0, 0x2C,         //          BEQ   ADDRM
/* 1C9C */ 0xC9, 0x11,         //          CMP   #$11      ; DATA MODE=1
/* 1C9E */ 0xF0, 0x2C,         //          BEQ   DATAM
/* 1CA0 */ 0xC9, 0x12,         //          CMP   #$12      ; STEP
/* 1CA2 */ 0xF0, 0x2F,         //          BEQ   STEP
/* 1CA4 */ 0xC9, 0x13,         //          CMP   #$13      ; RUN
/* 1CA6 */ 0xF0, 0x31,         //          BEQ   GOV
/* 1CA8 */ 0x0A,               //          ASL   A         ; SHIFT CHAR INTO HIGH
/* 1CA9 */ 0x0A,               //          ASL   A         ; ORDER NIBBLE
/* 1CAA */ 0x0A,               //          ASL   A
/* 1CAB */ 0x0A,               //          ASL   A
/* 1CAC */ 0x85, 0xFC,         //          STA   TEMP      ; STORE IN TEMP
/* 1CAE */ 0xA2, 0x04,         //          LDX   #$04
/* 1CB0 */ 0xA4, 0xFF,         //  DATA1   LDY   MODE      ; TEST MODE 1=ADDR
/* 1CB2 */ 0xD0, 0x0A,         //          BNE   ADDR      ; MODE=0 DATA
/* 1CB4 */ 0xB1, 0xFA,         //          LDA   (POINTL),Y ; GET DATA
/* 1CB6 */ 0x06, 0xFC,         //          ASL   TEMP      ; SHIFT CHAR
/* 1CB8 */ 0x2A,               //          ROL   A         ; SHIFT DATA
/* 1CB9 */ 0x91, 0xFA,         //          STA   (POINTL),Y ; STORE OUT DATA
/* 1CBB */ 0x4C, 0xC3, 0x1C,   //          JMP   DATA2
/* 1CBE */                     //  ;
/* 1CBE */ 0x0A,               //  ADDR    ASL   A         ; SHIFT CHAR
/* 1CBF */ 0x26, 0xFA,         //          ROL   POINTL    ; SHIFT ADDR
/* 1CC1 */ 0x26, 0xFB,         //          ROL   POINTH    ; SHIFT ADDR HI
/* 1CC3 */ 0xCA,               //  DATA2   DEX
/* 1CC4 */ 0xD0, 0xEA,         //          BNE   DATA1     ; DO 4 TIMES
/* 1CC6 */ 0xF0, 0x08,         //          BEQ   DATAM2    ; EXIT HERE
/* 1CC8 */ 0xA9, 0x01,         //  ADDRM   LDA   #$01
/* 1CCA */ 0xD0, 0x02,         //          BNE   DATAM1
/* 1CCC */ 0xA9, 0x00,         //  DATAM   LDA   #$00
/* 1CCE */ 0x85, 0xFF,         //  DATAM1  STA   MODE
/* 1CD0 */ 0x4C, 0x4F, 0x1C,   //  DATAM2  JMP   START
/* 1CD3 */                     //  ;
/* 1CD3 */ 0x20, 0x63, 0x1F,   //  STEP    JSR   INCPT     ;                               1CD3
/* 1CD6 */ 0x4C, 0x4F, 0x1C,   //          JMP   START
/* 1CD9 */                     //  ;
/* 1CD9 */ 0x4C, 0xC8, 0x1D,   //  GOV     JMP   GOEXEC    ;                               1CD9
/* 1CDC */                     //  ;       ** DISPLAY PC BY MOVING PC TO POINT **
/* 1CDC */ 0xA5, 0xEF,         //  PCCMD   LDA   PCL       ;                               1CDC
/* 1CDE */ 0x85, 0xFA,         //          STA   POINTL
/* 1CE0 */ 0xA5, 0xF0,         //          LDA   PCH
/* 1CE2 */ 0x85, 0xFB,         //          STA   POINTH
/* 1CE4 */ 0x4C, 0x4F, 0x1C,   //          JMP   START
/* 1CE7 */                     //  ;       ** LOAD PAPER TAPE FROM TTY **
/* 1CE7 */ 0x20, 0x5A, 0x1E,   //  LOAD    JSR   GETCH     ; LOOK FOR FIRST CHAR           1CE7
/* 1CEA */ 0xC9, 0x3B,         //          CMP   #$3B      ; SEMICOLON
/* 1CEC */ 0xD0, 0xF9,         //          BNE   LOAD
/* 1CEE */ 0xA9, 0x00,         //          LDA   #$00
/* 1CF0 */ 0x85, 0xF7,         //          STA   CHKSUM
/* 1CF2 */ 0x85, 0xF6,         //          STA   CHKHI
/* 1CF4 */ 0x20, 0x9D, 0x1F,   //          JSR   GETBYT    ; GET BYTE COUNT
/* 1CF7 */ 0xAA,               //          TAX             ; SAVE IN X INDEX
/* 1CF8 */ 0x20, 0x91, 0x1F,   //          JSR   CHK       ; COMPUTE CHECKSUM
/* 1CFB */ 0x20, 0x9D, 0x1F,   //          JSR   GETBYT    ; GET ADDRESS HI
/* 1CFE */ 0x85, 0xFB,         //          STA   POINTH
/* 1D00 */ 0x20, 0x91, 0x1F,   //          JSR   CHK
/* 1D03 */ 0x20, 0x9D, 0x1F,   //          JSR   GETBYT    ; GET ADDRESS LO
/* 1D06 */ 0x85, 0xFA,         //          STA   POINTL
/* 1D08 */ 0x20, 0x91, 0x1F,   //          JSR   CHK
/* 1D0B */ 0x8A,               //          TXA             ; IF CNT=0 DONT
/* 1D0C */ 0xF0, 0x0F,         //          BEQ   LOAD3     ; GET ANY DATA
/* 1D0E */ 0x20, 0x9D, 0x1F,   //  LOAD2   JSR   GETBYT    ; GET DATA
/* 1D11 */ 0x91, 0xFA,         //          STA   (POINTL),Y ; STORE DATA
/* 1D13 */ 0x20, 0x91, 0x1F,   //          JSR   CHK
/* 1D16 */ 0x20, 0x63, 0x1F,   //          JSR   INCPT     ; NEXT ADDRESS
/* 1D19 */ 0xCA,               //          DEX
/* 1D1A */ 0xD0, 0xF2,         //          BNE   LOAD2
/* 1D1C */ 0xE8,               //          INX             ; X=1 DATA RCD X=0 LAST RCD
/* 1D1D */ 0x20, 0x9D, 0x1F,   //  LOAD3   JSR   GETBYT    ; COMPARE CHKSUM
/* 1D20 */ 0xC5, 0xF6,         //          CMP   CHKHI
/* 1D22 */ 0xD0, 0x17,         //          BNE   LOADE1
/* 1D24 */ 0x20, 0x9D, 0x1F,   //          JSR   GETBYT
/* 1D27 */ 0xC5, 0xF7,         //          CMP   CHKSUM
/* 1D29 */ 0xD0, 0x13,         //          BNE   LOADER
/* 1D2B */ 0x8A,               //          TXA             ; X=0 LAST RECORD
/* 1D2C */ 0xD0, 0xB9,         //          BNE   LOAD
/* 1D2E */ 0xA2, 0x0C,         //          LDX   #$0C      ; X-OFF KIM
/* 1D30 */ 0xA9, 0x27,         //  LOAD8   LDA   #$27
/* 1D32 */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; DISABLE DATA IN
/* 1D35 */ 0x20, 0x31, 0x1E,   //          JSR   PRTST
/* 1D38 */ 0x4C, 0x4F, 0x1C,   //          JMP   START
/* 1D3B */                     //  ;
/* 1D3B */ 0x20, 0x9D, 0x1F,   //  LOADE1  JSR   GETBYT    ; DUMMY
/* 1D3E */ 0xA2, 0x11,         //  LOADER  LDX   #$11      ; X-OFF ERR KIM
/* 1D40 */ 0xD0, 0xEE,         //          BNE   LOAD8
/* 1D42 */                     //  ;       ** DUMP TO TTY FROM OPEN CELL ADDRESS TO
/* 1D42 */                     //  ;          LIMHL, LIMHH **
/* 1D42 */ 0xA9, 0x00,         //  DUMP    LDA   #$00      ;                               1D42
/* 1D44 */ 0x85, 0xF8,         //          STA   INL
/* 1D46 */ 0x85, 0xF9,         //          STA   INH       ; CLEAR RECORD COUNT
/* 1D48 */ 0xA9, 0x00,         //  DUMP0   LDA   #$00
/* 1D4A */ 0x85, 0xF6,         //          STA   CHKHI     ; CLEAR CHKSUM
/* 1D4C */ 0x85, 0xF7,         //          STA   CHKSUM
/* 1D4E */ 0x20, 0x2F, 0x1E,   //          JSR   CRLF      ; PRINT CR LF
/* 1D51 */ 0xA9, 0x3B,         //          LDA   #$3B      ; PRINT SEMICOLON
/* 1D53 */ 0x20, 0xA0, 0x1E,   //          JSR   OUTCH
/* 1D56 */ 0xA5, 0xFA,         //          LDA   POINTL    ; TEST POINT GT OR ET
/* 1D58 */ 0xCD, 0xF7, 0x17,   //          CMP   EAL       ; HI LIMIT GOTO EXIT
/* 1D5B */ 0xA5, 0xFB,         //          LDA   POINTH
/* 1D5D */ 0xED, 0xF8, 0x17,   //          SBC   EAH
/* 1D60 */ 0x90, 0x18,         //          BCC   DUMP4
/* 1D62 */ 0xA9, 0x00,         //          LDA   #$00      ; PRINT LAST RECORD
/* 1D64 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT    ; 0 BYTES
/* 1D67 */ 0x20, 0xCC, 0x1F,   //          JSR   OPEN
/* 1D6A */ 0x20, 0x1E, 0x1E,   //          JSR   PRTPNT
/* 1D6D */ 0xA5, 0xF6,         //          LDA   CHKHI     ; PRINT CHKSUM
/* 1D6F */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT    ; FOR LAST RECORD
/* 1D72 */ 0xA5, 0xF7,         //          LDA   CHKSUM
/* 1D74 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT
/* 1D77 */ 0x4C, 0x64, 0x1C,   //          JMP   CLEAR
/* 1D7A */                     //  ;
/* 1D7A */ 0xA9, 0x18,         //  DUMP4   LDA   #$18      ; PRINT 24 BYTE COUNT           1D7A
/* 1D7C */ 0xAA,               //          TAX             ; SAVE AS INDEX
/* 1D7D */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT
/* 1D80 */ 0x20, 0x91, 0x1F,   //          JSR   CHK
/* 1D83 */ 0x20, 0x1E, 0x1E,   //          JSR   PRTPNT
/* 1D86 */ 0xA0, 0x00,         //  DUMP2   LDY   #$00      ; PRINT 24 BYTES
/* 1D88 */ 0xB1, 0xFA,         //          LDA   (POINTL),Y ; GET DATA
/* 1D8A */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT    ; PRINT DATA
/* 1D8D */ 0x20, 0x91, 0x1F,   //          JSR   CHK       ; COMPUTE CHKSUM
/* 1D90 */ 0x20, 0x63, 0x1F,   //          JSR   INCPT     ; INCREMENT POINT
/* 1D93 */ 0xCA,               //          DEX
/* 1D94 */ 0xD0, 0xF0,         //          BNE   DUMP2
/* 1D96 */ 0xA5, 0xF6,         //          LDA   CHKHI     ; PRINT CHKSUM
/* 1D98 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT
/* 1D9B */ 0xA5, 0xF7,         //          LDA   CHKSUM
/* 1D9D */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT
/* 1DA0 */ 0xE6, 0xF8,         //          INC   INL       ; INCR RECORD COUNT
/* 1DA2 */ 0xD0, 0x02,         //          BNE   DUMP3
/* 1DA4 */ 0xE6, 0xF9,         //          INC   INH
/* 1DA6 */ 0x4C, 0x48, 0x1D,   //  DUMP3   JMP   DUMP0
/* 1DA9 */                     //  ;
/* 1DA9 */ 0x20, 0xCC, 0x1F,   //  SPACE   JSR   OPEN      ; OPEN NEW CELL                 1DA9
/* 1DAC */ 0x20, 0x2F, 0x1E,   //  SHOW    JSR   CRLF      ; PRINT CR LF
/* 1DAF */ 0x20, 0x1E, 0x1E,   //  SHOW1   JSR   PRTPNT
/* 1DB2 */ 0x20, 0x9E, 0x1E,   //          JSR   OUTSP     ; PRINT SPACE
/* 1DB5 */ 0xA0, 0x00,         //          LDY   #$00      ; PRINT DATA SPECIFIED
/* 1DB7 */ 0xB1, 0xFA,         //          LDA   (POINTL),Y ; BY POINT AD=LDA EXT
/* 1DB9 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT
/* 1DBC */ 0x20, 0x9E, 0x1E,   //          JSR   OUTSP     ; PRINT SPACE
/* 1DBF */ 0x4C, 0x64, 0x1C,   //          JMP   CLEAR
/* 1DC2 */                     //  ;
/* 1DC2 */ 0x20, 0x63, 0x1F,   //  RTRN    JSR   INCPT     ; OPEN NEXT CELL                1DC2
/* 1DC5 */ 0x4C, 0xAC, 0x1D,   //          JMP   SHOW
/* 1DC8 */                     //  ;
/* 1DC8 */ 0xA6, 0xF2,         //  GOEXEC  LDX   SPUSER    ;                               1DC8
/* 1DCA */ 0x9A,               //          TXS
/* 1DCB */ 0xA5, 0xFB,         //          LDA   POINTH    ; PROGRAM RUNS FROM
/* 1DCD */ 0x48,               //          PHA             ; OPEN CELL ADDRESS
/* 1DCE */ 0xA5, 0xFA,         //          LDA   POINTL
/* 1DD0 */ 0x48,               //          PHA
/* 1DD1 */ 0xA5, 0xF1,         //          LDA   PREG
/* 1DD3 */ 0x48,               //          PHA
/* 1DD4 */ 0xA6, 0xF5,         //          LDX   XREG      ; RESTORE REGS
/* 1DD6 */ 0xA4, 0xF4,         //          LDY   YREG
/* 1DD8 */ 0xA5, 0xF3,         //          LDA   ACC
/* 1DDA */ 0x40,               //          RTI
/* 1DDB */                     //  ;
/* 1DDB */ 0xC9, 0x20,         //  SCAN    CMP   #$20      ; OPEN CELL
/* 1DDD */ 0xF0, 0xCA,         //          BEQ   SPACE
/* 1DDF */ 0xC9, 0x7F,         //          CMP   #$7F      ; RUB OUT (KIM)
/* 1DE1 */ 0xF0, 0x1B,         //          BEQ   STV
/* 1DE3 */ 0xC9, 0x0D,         //          CMP   #$0D      ; NEXT CELL
/* 1DE5 */ 0xF0, 0xDB,         //          BEQ   RTRN
/* 1DE7 */ 0xC9, 0x0A,         //          CMP   #$0A      ;PREV CELL
/* 1DE9 */ 0xF0, 0x1C,         //          BEQ   FEED
/* 1DEB */ 0xC9, 0x2E,         //          CMP   #'.'      ; MODIFY CELL
/* 1DED */ 0xF0, 0x26,         //          BEQ   MODIFY
/* 1DEF */ 0xC9, 0x47,         //          CMP   #'G'      ; GO EXEC
/* 1DF1 */ 0xF0, 0xD5,         //          BEQ   GOEXEC
/* 1DF3 */ 0xC9, 0x51,         //          CMP   #'Q'      ; DUMP FROM OPEN CELL TO HI LIMIT
/* 1DF5 */ 0xF0, 0x0A,         //          BEQ   DUMPV
/* 1DF7 */ 0xC9, 0x4C,         //          CMP   #'L'      ; LOAD TAPE
/* 1DF9 */ 0xF0, 0x09,         //          BEQ   LOADV
/* 1DFB */ 0x4C, 0x6A, 0x1C,   //          JMP   READ      ; IGNORE ILLEGAL CHAR
/* 1DFE */                     //  ;
/* 1DFE */ 0x4C, 0x4F, 0x1C,   //  STV     JMP   START
/* 1E01 */ 0x4C, 0x42, 0x1D,   //  DUMPV   JMP   DUMP
/* 1E04 */ 0x4C, 0xE7, 0x1C,   //  LOADV   JMP   LOAD
/* 1E07 */                     //  ;
/* 1E07 */ 0x38,               //  FEED    SEC             ;                               1E07
/* 1E08 */ 0xA5, 0xFA,         //          LDA   POINTL    ; DEC DOUBLE BYTE
/* 1E0A */ 0xE9, 0x01,         //          SBC   #$01      ; AT POINTL AND POINTH
/* 1E0C */ 0x85, 0xFA,         //          STA   POINTL
/* 1E0E */ 0xB0, 0x02,         //          BCS   FEED1
/* 1E10 */ 0xC6, 0xFB,         //          DEC   POINTH
/* 1E12 */ 0x4C, 0xAC, 0x1D,   //  FEED1   JMP   SHOW
/* 1E15 */                     //  ;
/* 1E15 */ 0xA0, 0x00,         //  MODIFY  LDY   #$00      ; GET CONTENTS OF INPUT BUFF
/* 1E17 */ 0xA5, 0xF8,         //          LDA   INL       ; INL AND STORE IN LOC
/* 1E19 */ 0x91, 0xFA,         //          STA   (POINTL),Y ; SPECIFIED BY POINT
/* 1E1B */ 0x4C, 0xC2, 0x1D,   //          JMP   RTRN
/* 1E1E */                     //  ;
/* 1E1E */                     //  ;       ** SUBROUTINES FOLLOW **
/* 1E1E */ 0xA5, 0xFB,         //  PRTPNT  LDA   POINTH    ; PRINT POINTL, POINTH          1E1E
/* 1E20 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT
/* 1E23 */ 0x20, 0x91, 0x1F,   //          JSR   CHK
/* 1E26 */ 0xA5, 0xFA,         //          LDA   POINTL
/* 1E28 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT
/* 1E2B */ 0x20, 0x91, 0x1F,   //          JSR   CHK
/* 1E2E */ 0x60,               //          RTS
/* 1E2F */                     //  ;       **PRINT STRING OF ASCII CHARS FROM TOP+X TO TOP
/* 1E2F */ 0xA2, 0x07,         //  CRLF    LDX   #$07
/* 1E31 */ 0xBD, 0xD5, 0x1F,   //  PRTST   LDA   TOP,X
/* 1E34 */ 0x20, 0xA0, 0x1E,   //          JSR   OUTCH
/* 1E37 */ 0xCA,               //          DEX
/* 1E38 */ 0x10, 0xF7,         //          BPL   PRTST     ; STOP ON INDEX ZERO
/* 1E3A */ 0x60,               //          RTS
/* 1E3B */                     //  ;       ** PRINT 1 HEX BYTE AS 2 ASCII CHARS **
/* 1E3B */ 0x85, 0xFC,         //  PRTBYT  STA   TEMP      ;                               1E3B
/* 1E3D */ 0x4A,               //          LSR   A         ; SHIFT CHAR RIGHT 4 BITS
/* 1E3E */ 0x4A,               //          LSR   A
/* 1E3F */ 0x4A,               //          LSR   A
/* 1E40 */ 0x4A,               //          LSR   A
/* 1E41 */ 0x20, 0x4C, 0x1E,   //          JSR   HEXTA     ; CONVERT TO HEX AND PRINT
/* 1E44 */ 0xA5, 0xFC,         //          LDA   TEMP      ; GET OTHER HALF
/* 1E46 */ 0x20, 0x4C, 0x1E,   //          JSR   HEXTA     ; CONVERT TO HEX AND PRINT
/* 1E49 */ 0xA5, 0xFC,         //          LDA   TEMP      ; RESTORE BYTE IN A AND RETURN
/* 1E4B */ 0x60,               //          RTS
/* 1E4C */ 0x29, 0x0F,         //  HEXTA   AND   #$0F      ; MASK HI 4 BITS
/* 1E4E */ 0xC9, 0x0A,         //          CMP   #$0A
/* 1E50 */ 0x18,               //          CLC
/* 1E51 */ 0x30, 0x02,         //          BMI   HEXTA1
/* 1E53 */ 0x69, 0x07,         //          ADC   #$07      ; ALPHA HEX
/* 1E55 */ 0x69, 0x30,         //  HEXTA1  ADC   #$30      ; DEC HEX
/* 1E57 */ 0x4C, 0xA0, 0x1E,   //          JMP   OUTCH     ; PRINT CHAR
/* 1E5A */                     //  ;       ** GET 1 CHAR FROM TTY, CHAR IN A
/* 1E5A */ 0x86, 0xFD,         //  GETCH   STX   TMPX      ; SAVE X REG     1E5A
/* 1E5C */ 0xA2, 0x08,         //          LDX   #$08      ; SET UP 8-BIT COUNT
/* 1E5E */ 0xA9, 0x01,         //          LDA   #$01
/* 1E60 */ 0x2C, 0x40, 0x17,   //  GET1    BIT   SAD
/* 1E63 */ 0xD0, 0x22,         //          BNE   GET6
/* 1E65 */ 0x30, 0xF9,         //          BMI   GET1      ; WAIT FOR START BIT
/* 1E67 */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY     ; DELAY 1 BIT
/* 1E6A */ 0x20, 0xEB, 0x1E,   //  GET5    JSR   DEHALF    ; DELAY 1/2 BIT TIME
/* 1E6D */ 0xAD, 0x40, 0x17,   //  GET2    LDA   SAD       ; GET 8 BITS
/* 1E70 */ 0x29, 0x80,         //          AND   #$80      ; MASK OFF LOW ORDER BITS
/* 1E72 */ 0x46, 0xFE,         //          LSR   CHAR      ; SHIFT RIGHT CHAR
/* 1E74 */ 0x05, 0xFE,         //          ORA   CHAR
/* 1E76 */ 0x85, 0xFE,         //          STA   CHAR
/* 1E78 */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY     ; DELAY 1 BIT TIME
/* 1E7B */ 0xCA,               //          DEX
/* 1E7C */ 0xD0, 0xEF,         //          BNE   GET2      ; GET NEXT CHAR
/* 1E7E */ 0x20, 0xEB, 0x1E,   //          JSR   DEHALF    ; EXIT THIS ROUTINE
/* 1E81 */ 0xA6, 0xFD,         //          LDX   TMPX
/* 1E83 */ 0xA5, 0xFE,         //          LDA   CHAR
/* 1E85 */ 0x2A,               //          ROL   A         ; SHIFT OFF PARITY
/* 1E86 */ 0x4A,               //          LSR   A
/* 1E87 */ 0x60,               //  GET6    RTS
/* 1E88 */                     //  ;       ** INITIALIZATION FOR SIGMA **
/* 1E88 */ 0xA2, 0x01,         //  INITS   LDX   #$01      ; SET KB MODE TO ADDR           1E88
/* 1E8A */ 0x86, 0xFF,         //          STX   MODE
/* 1E8C */ 0xA2, 0x00,         //  INIT1   LDX   #$00
/* 1E8E */ 0x8E, 0x41, 0x17,   //          STX   PADD      ; FOR SIGMA USE SADD
/* 1E91 */ 0xA2, 0x3F,         //          LDX   #$3F
/* 1E93 */ 0x8E, 0x43, 0x17,   //          STX   PBDD      ; FOR SIGMA USE SBDD
/* 1E96 */ 0xA2, 0x07,         //          LDX   #$07      ; ENABLE DATA IN
/* 1E98 */ 0x8E, 0x42, 0x17,   //          STX   SBD       ; OUTPUT
/* 1E9B */ 0xD8,               //          CLD
/* 1E9C */ 0x78,               //          SEI
/* 1E9D */ 0x60,               //          RTS
/* 1E9E */                     //  ;       ** PRINT ONE CHAR IN A **
/* 1E9E */ 0xA9, 0x20,         //  OUTSP   LDA   #$20      ; PRINT SPACE                   1E9E
/* 1EA0 */ 0x85, 0xFE,         //  OUTCH   STA   CHAR
/* 1EA2 */ 0x86, 0xFD,         //          STX   TMPX
/* 1EA4 */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY     ; DELAY 10/11 BIT CODE SYNC
/* 1EA7 */ 0xAD, 0x42, 0x17,   //          LDA   SBD       ; START BIT
/* 1EAA */ 0x29, 0xFE,         //          AND   #$FE
/* 1EAC */ 0x8D, 0x42, 0x17,   //          STA   SBD
/* 1EAF */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY
/* 1EB2 */ 0xA2, 0x08,         //          LDX   #$08
/* 1EB4 */ 0xAD, 0x42, 0x17,   //  OUT1    LDA   SBD       ; DATA BIT
/* 1EB7 */ 0x29, 0xFE,         //          AND   #$FE
/* 1EB9 */ 0x46, 0xFE,         //          LSR   CHAR
/* 1EBB */ 0x69, 0x00,         //          ADC   #$00
/* 1EBD */ 0x8D, 0x42, 0x17,   //          STA   SBD
/* 1EC0 */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY
/* 1EC3 */ 0xCA,               //          DEX
/* 1EC4 */ 0xD0, 0xEE,         //          BNE   OUT1
/* 1EC6 */ 0xAD, 0x42, 0x17,   //          LDA   SBD       ; STOP BIT
/* 1EC9 */ 0x09, 0x01,         //          ORA   #$01
/* 1ECB */ 0x8D, 0x42, 0x17,   //          STA   SBD
/* 1ECE */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY     ; STOP BIT
/* 1ED1 */ 0xA6, 0xFD,         //          LDX   TMPX      ; RESTORE INDEX
/* 1ED3 */ 0x60,               //          RTS
/* 1ED4 */                     //  ;       ** DELAY 1 BIT TIME **
/* 1ED4 */ 0xAD, 0xF3, 0x17,   //  DELAY   LDA   CNTH30    ;                               1ED4
/* 1ED7 */ 0x8D, 0xF4, 0x17,   //          STA   TIMH
/* 1EDA */ 0xAD, 0xF2, 0x17,   //          LDA   CNTL30
/* 1EDD */ 0x38,               //  DE2     SEC
/* 1EDE */ 0xE9, 0x01,         //  DE4     SBC   #$01
/* 1EE0 */ 0xB0, 0x03,         //          BCS   DE3
/* 1EE2 */ 0xCE, 0xF4, 0x17,   //          DEC   TIMH
/* 1EE5 */ 0xAC, 0xF4, 0x17,   //  DE3     LDY   TIMH
/* 1EE8 */ 0x10, 0xF3,         //          BPL   DE2
/* 1EEA */ 0x60,               //          RTS
/* 1EEB */                     //  ;       ** DELAY 1/2 BIT TIME **
/* 1EEB */ 0xAD, 0xF3, 0x17,   //  DEHALF  LDA   CNTH30    ;                               1EEB
/* 1EEE */ 0x8D, 0xF4, 0x17,   //          STA   TIMH
/* 1EF1 */ 0xAD, 0xF2, 0x17,   //          LDA   CNTL30
/* 1EF4 */ 0x4A,               //          LSR   A
/* 1EF5 */ 0x4E, 0xF4, 0x17,   //          LSR   TIMH
/* 1EF8 */ 0x90, 0xE3,         //          BCC   DE2
/* 1EFA */ 0x09, 0x80,         //          ORA   #$80
/* 1EFC */ 0xB0, 0xE0,         //          BCS   DE4
/* 1EFE */                     //  ;       ** SUB TO DETERMINE IF KEY IS DEPRESSED OR
/* 1EFE */                     //  ;          CONDITION OF SSW KEY NOT DEPRESSED OR
/* 1EFE */                     //  ;          TTY MODE  A=0
/* 1EFE */                     //  ;          KEY DEPRESSED OR KB MODE  A NOT ZERO
/* 1EFE */ 0xA0, 0x03,         //  AK      LDY   #$03      ; 3 ROWS
/* 1F00 */ 0xA2, 0x01,         //          LDX   #$01      ; DIGIT 0
/* 1F02 */ 0xA9, 0xFF,         //  ONEKEY  LDA   #$FF
/* 1F04 */ 0x8E, 0x42, 0x17,   //  AK1     STX   SBD       ; OUTPUT DIGIT
/* 1F07 */ 0xE8,               //          INX             ; GET NEXT DIGIT
/* 1F08 */ 0xE8,               //          INX
/* 1F09 */ 0x2D, 0x40, 0x17,   //          AND   SAD       ; INPUT SEGMENTS
/* 1F0C */ 0x88,               //          DEY
/* 1F0D */ 0xD0, 0xF5,         //          BNE   AK1
/* 1F0F */ 0xA0, 0x07,         //          LDY   #$07
/* 1F11 */ 0x8C, 0x42, 0x17,   //          STY   SBD
/* 1F14 */ 0x09, 0x80,         //          ORA   #$80
/* 1F16 */ 0x49, 0xFF,         //          EOR   #$FF
/* 1F18 */ 0x60,               //          RTS
/* 1F19 */                     //  ;       ** OUTPUT TO 7-SEGMENT DISPLAY **
/* 1F19 */ 0xA0, 0x00,         //  SCAND   LDY   #$00      ; GET DATA                      1F19
/* 1F1B */ 0xB1, 0xFA,         //          LDA   (POINTL),Y ; SPECIFIED BY POINT
/* 1F1D */ 0x85, 0xF9,         //          STA   INH       ; SET UP DISPLAY BUFFER
/* 1F1F */ 0xA9, 0x7F,         //          LDA   #$7F      ; CHANGE SEG
/* 1F21 */ 0x8D, 0x41, 0x17,   //          STA   PADD      ; TO OUTPUT
/* 1F24 */ 0xA2, 0x09,         //          LDX   #$09      ; INIT DIGIT NUMBER
/* 1F26 */ 0xA0, 0x03,         //          LDY   #$03      ; OUTPUT 3 BYTES
/* 1F28 */ 0xB9, 0xF8, 0x00,   //  SCAND1  LDA   INL,Y     ; GET BYTE
/* 1F2B */ 0x4A,               //          LSR   A         ; GET MSD
/* 1F2C */ 0x4A,               //          LSR   A
/* 1F2D */ 0x4A,               //          LSR   A
/* 1F2E */ 0x4A,               //          LSR   A
/* 1F2F */ 0x20, 0x48, 0x1F,   //          JSR   CONVD     ; OUTPUT CHAR
/* 1F32 */ 0xB9, 0xF8, 0x00,   //          LDA   INL,Y     ; GET BYTE AGAIN
/* 1F35 */ 0x29, 0x0F,         //          AND   #$0F      ; GET LSD
/* 1F37 */ 0x20, 0x48, 0x1F,   //          JSR   CONVD     ; OUTPUT CHAR
/* 1F3A */ 0x88,               //          DEY             ; SET UP FOR NEXT BYTE
/* 1F3B */ 0xD0, 0xEB,         //          BNE   SCAND1
/* 1F3D */ 0x8E, 0x42, 0x17,   //          STX   SBD       ; ALL DIGITS OFF
/* 1F40 */ 0xA9, 0x00,         //          LDA   #$00      ; CHANGE SEGMENT
/* 1F42 */ 0x8D, 0x41, 0x17,   //          STA   PADD      ; TO INPUTS
/* 1F45 */ 0x4C, 0xFE, 0x1E,   //          JMP   AK        ; GET ANY KEY
/* 1F48 */                     //  ;       ** CONVERT AND DISPLAY HEX (USED BY SCAND ONLY)**
/* 1F48 */ 0x84, 0xFC,         //  CONVD   STY   TEMP
/* 1F4A */ 0xA8,               //          TAY             ; SAVE Y
/* 1F4B */ 0xB9, 0xE7, 0x1F,   //          LDA   TABLE,Y   ; USE CHAR AS INDEX
/* 1F4E */ 0xA0, 0x00,         //          LDY   #$00      ; LOOKUP CONVERSION
/* 1F50 */ 0x8C, 0x40, 0x17,   //          STY   SAD       ; TURN OFF SEGMENTS
/* 1F53 */ 0x8E, 0x42, 0x17,   //          STX   SBD       ; OUTPUT DIGIT ENABLE
/* 1F56 */ 0x8D, 0x40, 0x17,   //          STA   SAD       ; OUTPUT SEGMENTS
/* 1F59 */ 0xA0, 0x7F,         //          LDY   #$7F      ; DELAY 500 CYCLES
/* 1F5B */ 0x88,               //  CONVD1  DEY
/* 1F5C */ 0xD0, 0xFD,         //          BNE   CONVD1
/* 1F5E */ 0xE8,               //          INX             ; GET NEXT DIGIT NUMBER
/* 1F5F */ 0xE8,               //          INX             ; ADD 2
/* 1F60 */ 0xA4, 0xFC,         //          LDY   TEMP      ; RESTORE Y
/* 1F62 */ 0x60,               //          RTS
/* 1F63 */                     //  ;       ** INCREMENT POINT **
/* 1F63 */ 0xE6, 0xFA,         //  INCPT   INC   POINTL
/* 1F65 */ 0xD0, 0x02,         //          BNE   INCPT2
/* 1F67 */ 0xE6, 0xFB,         //          INC   POINTH
/* 1F69 */ 0x60,               //  INCPT2  RTS
/* 1F6A */                     //  ;       ** GET KEY FROM KEYPAD A=KEYVALUE **
/* 1F6A */ 0xA2, 0x21,         //  GETKEY  LDX   #$21      ; START AT DIGIT 0              1F6A
/* 1F6C */ 0xA0, 0x01,         //  GETKE5  LDY   #$01      ; GET 1 ROW
/* 1F6E */ 0x20, 0x02, 0x1F,   //          JSR   ONEKEY
/* 1F71 */ 0xD0, 0x07,         //          BNE   KEYIN     ; A=0 NO KEY
/* 1F73 */ 0xE0, 0x27,         //          CPX   #$27      ; TEST FOR DIGIT 2
/* 1F75 */ 0xD0, 0xF5,         //          BNE   GETKE5
/* 1F77 */ 0xA9, 0x15,         //          LDA   #$15      ; 15=NOKEY
/* 1F79 */ 0x60,               //          RTS
/* 1F7A */ 0xA0, 0xFF,         //  KEYIN   LDY   #$FF
/* 1F7C */ 0x0A,               //  KEYIN1  ASL   A         ; SHIFT LEFT
/* 1F7D */ 0xB0, 0x03,         //          BCS   KEYIN2    ; UNTIL Y=KEY NO
/* 1F7F */ 0xC8,               //          INY
/* 1F80 */ 0x10, 0xFA,         //          BPL   KEYIN1
/* 1F82 */ 0x8A,               //  KEYIN2  TXA
/* 1F83 */ 0x29, 0x0F,         //          AND   #$0F      ; MASK MSD
/* 1F85 */ 0x4A,               //          LSR   A         ; DIVIDE BY 2
/* 1F86 */ 0xAA,               //          TAX
/* 1F87 */ 0x98,               //          TYA
/* 1F88 */ 0x10, 0x03,         //          BPL   KEYIN4
/* 1F8A */ 0x18,               //  KEYIN3  CLC
/* 1F8B */ 0x69, 0x07,         //          ADC   #$07      ; MULT (X-1 TIMES A
/* 1F8D */ 0xCA,               //  KEYIN4  DEX
/* 1F8E */ 0xD0, 0xFA,         //          BNE   KEYIN3
/* 1F90 */ 0x60,               //          RTS
/* 1F91 */                     //  ;       ** COMPUTE CHECKSUM **
/* 1F91 */ 0x18,               //  CHK     CLC
/* 1F92 */ 0x65, 0xF7,         //          ADC   CHKSUM
/* 1F94 */ 0x85, 0xF7,         //          STA   CHKSUM
/* 1F96 */ 0xA5, 0xF6,         //          LDA   CHKHI
/* 1F98 */ 0x69, 0x00,         //          ADC   #$00
/* 1F9A */ 0x85, 0xF6,         //          STA   CHKHI
/* 1F9C */ 0x60,               //          RTS
/* 1F9D */                     //  ;       ** GET 2 HEX CHARS AND PACK INTO INL AND INH **
/* 1F9D */ 0x20, 0x5A, 0x1E,   //  GETBYT  JSR   GETCH
/* 1FA0 */ 0x20, 0xAC, 0x1F,   //          JSR   PACK
/* 1FA3 */ 0x20, 0x5A, 0x1E,   //          JSR   GETCH
/* 1FA6 */ 0x20, 0xAC, 0x1F,   //          JSR   PACK
/* 1FA9 */ 0xA5, 0xF8,         //          LDA   INL
/* 1FAB */ 0x60,               //          RTS
/* 1FAC */                     //  ;       ** SHIFT CHAR IN A INTO INL AND INH **
/* 1FAC */ 0xC9, 0x30,         //  PACK    CMP   #$30      ; CHECK FOR HEX  1FAC
/* 1FAE */ 0x30, 0x1B,         //          BMI   UPDAT2
/* 1FB0 */ 0xC9, 0x47,         //          CMP   #$47      ; NOT HEX EXIT
/* 1FB2 */ 0x10, 0x17,         //          BPL   UPDAT2
/* 1FB4 */ 0xC9, 0x40,         //          CMP   #$40      ; CONVERT TO HEX
/* 1FB6 */ 0x30, 0x03,         //          BMI   UPDATE
/* 1FB8 */ 0x18,               //          CLC
/* 1FB9 */ 0x69, 0x09,         //          ADC   #$09
/* 1FBB */ 0x2A,               //  UPDATE  ROL   A
/* 1FBC */ 0x2A,               //          ROL   A
/* 1FBD */ 0x2A,               //          ROL   A
/* 1FBE */ 0x2A,               //          ROL   A
/* 1FBF */ 0xA0, 0x04,         //          LDY   #$04      ; SHIFT INTO I/O BUFFER
/* 1FC1 */ 0x2A,               //  UPDAT1  ROL   A
/* 1FC2 */ 0x26, 0xF8,         //          ROL   INL
/* 1FC4 */ 0x26, 0xF9,         //          ROL   INH
/* 1FC6 */ 0x88,               //          DEY
/* 1FC7 */ 0xD0, 0xF8,         //          BNE   UPDAT1
/* 1FC9 */ 0xA9, 0x00,         //          LDA   #$00      ; A=0 IF HEX NUM
/* 1FCB */ 0x60,               //  UPDAT2  RTS
/* 1FCC */                     //  ;
/* 1FCC */ 0xA5, 0xF8,         //  OPEN    LDA   INL       ; MOVE I/O BUFFER TO POINT
/* 1FCE */ 0x85, 0xFA,         //          STA   POINTL
/* 1FD0 */ 0xA5, 0xF9,         //          LDA   INH       ; TRANSFER INH- POINTH
/* 1FD2 */ 0x85, 0xFB,         //          STA   POINTH
/* 1FD4 */ 0x60,               //          RTS
/* 1FD5 */                     //  ;
/* 1FD5 */                     //  ;       ** TABLES **
/* 1FD5 */ 0x00, 0x00, 0x00, 0x00,  // TOP .BYTE $00,$00,$00,$00,$00,$00,$0A,$0D ; 1FD5
/* 1FD9 */ 0x00, 0x00, 0x0A, 0x0D,  //
/* 1FDD */ 0x4D, 0x49, 0x4B, 0x20,  // .BYTE 'M','I','K',$20,$13 ; KIM
/* 1FE1 */ 0x13,               //
/* 1FE2 */ 0x52, 0x52, 0x45, 0x20,  // .BYTE 'R','R','E',$20,$13 ; ERR
/* 1FE6 */ 0x13,               //
/* 1FE7 */ 0xBF, 0x86, 0xDB, 0xCF,  // TABLE .BYTE $BF,$86,$DB,$CF,$E6,$ED,$FD,$87 ;0-7 1FE7
/* 1FEB */ 0xE6, 0xED, 0xFD, 0x87,  //
/* 1FEF */ 0xFF, 0xEF, 0xF7, 0xFC,  // .BYTE $FF,$EF,$F7,$FC,$B9,$DE,$F9,$F1 ;8-F HEX TO 7-SEG
/* 1FF3 */ 0xB9, 0xDE, 0xF9, 0xF1,  //
/* 1FF7 */                     //  ;
/* 1FF7 */                     //
/* 1FF7 */                     //  ; Fill unused locations with $FF
/* 1FF7 */ 0xFF, 0xFF, 0xFF,   //          .FILL 3, $FF
/* 1FFA */                     //
/* 1FFA */                     //  ;       ** INTERRUPT VECTORS **
/* 1FFA */                     //          .org  $1FFA
/* 1FFA */ 0x1C, 0x1C,         //  NMIENT  .WORD NMIT
/* 1FFC */ 0x22, 0x1C,         //  RSTENT  .WORD RST
/* 1FFE */ 0x1F, 0x1C          //  IRQENT  .WORD IRQT
};




// ============================================================================
// ============================================================================
// KIM ROM 003
// ============================================================================
// ============================================================================
// always installed
#if _TARGET == PERSONAL_COMPUTER
    unsigned char rom003[1024] = {
#else
    const unsigned char rom003[1024] PROGMEM = {
#endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  ;- - - - - - - - - - - - - - KIM.ASM - - - - - - - - - - -
/* 0000 */                     //  ; COPYRIGHT MOS TECHNOLOGY, INC
/* 0000 */                     //  ; DATE: OCT 18, 1975 REV-D
/* 0000 */                     //  ;************************ 6530-003 I.C. ******************
/* 0000 */                     //  ; 6530-003 I.C. IS AN AUDIO CASSETTE TAPE RECORDER
/* 0000 */                     //  ; EXTENSION OF THE BASIC KIM MONITOR.  IT FEATURES
/* 0000 */                     //  ; TWO ROUTINES:
/* 0000 */                     //  ;   LOADT - LOAD MEMORY FROM AUDIO TAPE
/* 0000 */                     //  ;     ID=00     IGNORE ID
/* 0000 */                     //  ;     ID=FF     IGNORE ID, USE SA FOR START ADDR
/* 0000 */                     //  ;     ID=01-FE  USE ADDRESS ON TAPE
/* 0000 */                     //  ;
/* 0000 */                     //  ;   DUMPT - STORE MEMORY ONTO AUDIO TAPE
/* 0000 */                     //  ;     ID=00     SHOULD NOT BE USED
/* 0000 */                     //  ;     ID=FF     SHOULD NOT BE USED
/* 0000 */                     //  ;     ID=01-FE  NORMAL ID RANGE
/* 0000 */                     //  ;     SAL       LSB STARTING ADDRESS OF PROGRAM
/* 0000 */                     //  ;     SAH       MSB
/* 0000 */                     //  ;     EAL       ENDING ADDRESS OF PROGRAM
/* 0000 */                     //  ;     EAH       MSB
/* 0000 */                     //  ;
/* 1800 */                     //          .org  $1800
/* 1800 */                     //  #define SAD      $1740     ; 6530 A DATA
/* 1800 */                     //  #define PADD     $1741     ; 6530 A DATA DIRECTION
/* 1800 */                     //  #define SBD      $1742     ; 6530 B DATA
/* 1800 */                     //  #define PBDD     $1743     ; 6530 B DATA DIRECTION
/* 1800 */                     //  #define CLK1T    $1744     ; DIV BY 1 TIME
/* 1800 */                     //  #define CLK8T    $1745     ; DIV BY 8 TIME
/* 1800 */                     //  #define CLK64T   $1746     ; DIV BY 64 TIME
/* 1800 */                     //  #define CLKKT    $1747     ; DIV BY 1024 TIME
/* 1800 */                     //  #define CLKRDI   $1747     ; READ TIME OUT BIT
/* 1800 */                     //  #define CLKRDT   $1746     ; READ TIME
/* 1800 */                     //  ;       ** MPU REG.  SAVX AREA IN PAGE 0 **
/* 1800 */                     //  #define PCL      $EF       ; PROGRAM CNT LOW
/* 1800 */                     //  #define PCH      $F0       ; PROGRAM CNT HI
/* 1800 */                     //  #define PREG     $F1       ; CURRENT STATUS REG
/* 1800 */                     //  #define SPUSER   $F2       ; CURRENT STACK POINTER
/* 1800 */                     //  #define ACC      $F3       ; ACCUMULATOR
/* 1800 */                     //  #define YREG     $F4       ; Y INDEX
/* 1800 */                     //  #define XREG     $F5       ; X INDEX
/* 1800 */                     //  ;       ** KIM FIXED AREA IN PAGE 0  **
/* 1800 */                     //  #define CHKHI    $F6
/* 1800 */                     //  #define CHKSUM   $F7
/* 1800 */                     //  #define INL      $F8       ; INPUT BUFFER)
/* 1800 */                     //  #define INH      $F9       ; INPUT BUFFER
/* 1800 */                     //  #define POINTL   $FA       ; LSB OF OPEN CELL
/* 1800 */                     //  #define POINTH   $FB       ; MSB OF OPEN CELL
/* 1800 */                     //  #define TEMP     $FC
/* 1800 */                     //  #define TMPX     $FD
/* 1800 */                     //  #define CHAR     $FE
/* 1800 */                     //  #define MODE     $FF
/* 1800 */                     //  ;       ** KIM FIXED AREA IN PAGE 23 **
/* 1800 */                     //  #define CHKL     $17E7
/* 1800 */                     //  #define CHKH     $17E8     ; CHKSUM
/* 1800 */                     //  #define SAVX     $17E9     ; (3-BYTES)
/* 1800 */                     //  #define VEB      $17EC     ; VOLATILE EXEC BLOCK (6-B)
/* 1800 */                     //  #define CNTL30   $17F2     ; TTY DELAY
/* 1800 */                     //  #define CNTH30   $17F3     ; TTY DELAY
/* 1800 */                     //  #define TIMH     $17F4
/* 1800 */                     //  #define SAL      $17F5     ; LOW STARTING ADDRESS
/* 1800 */                     //  #define SAH      $17F6     ; HI STARTING ADDRESS
/* 1800 */                     //  #define EAL      $17F7     ; LOW ENDING ADDRESS
/* 1800 */                     //  #define EAH      $17F8     ; HI ENDING ADDRESS
/* 1800 */                     //  #define ID       $17F9     ; TAPE PROGRAM ID NUMBER
/* 1800 */                     //  ;       ** INTERRUPT VECTORS **
/* 1800 */                     //  #define NMIV     $17FA     ; STOP VECTOR (STOP=1C00)
/* 1800 */                     //  #define RSTV     $17FC     ; RST VECTOR
/* 1800 */                     //  #define IRQV     $17FE     ; IRQ VECTOR (BRK=1C00)
/* 1800 */                     //
/* 1800 */                     //  ;
/* 1800 */                     //  ;       ** DUMP MEMORY TO TAPE **
/* 1800 */ 0xA9, 0xAD,         //  DUMPT   LDA   #$AD      ; LOAD ABSOLUTE INST            1800
/* 1802 */ 0x8D, 0xEC, 0x17,   //          STA   VEB
/* 1805 */ 0x20, 0x32, 0x19,   //          JSR   INTVEB
/* 1808 */ 0xA9, 0x27,         //          LDA   #$27      ; TURN OFF DATAIN PB5
/* 180A */ 0x8D, 0x42, 0x17,   //          STA   SBD
/* 180D */ 0xA9, 0xBF,         //          LDA   #$BF      ; CONVERT PB7 TO OUTPUT
/* 180F */ 0x8D, 0x43, 0x17,   //          STA   PBDD
/* 1812 */ 0xA2, 0x64,         //          LDX   #$64      ; 100 CHARS
/* 1814 */ 0xA9, 0x16,         //  DUMPT1  LDA   #$16      ; SYNC CHARS
/* 1816 */ 0x20, 0x7A, 0x19,   //          JSR   OUTCHT
/* 1819 */ 0xCA,               //          DEX
/* 181A */ 0xD0, 0xF8,         //          BNE   DUMPT1
/* 181C */ 0xA9, 0x2A,         //          LDA   #$2A      ; START CHAR
/* 181E */ 0x20, 0x7A, 0x19,   //          JSR   OUTCHT
/* 1821 */ 0xAD, 0xF9, 0x17,   //          LDA   ID        ; OUTPUT ID
/* 1824 */ 0x20, 0x61, 0x19,   //          JSR   OUTBT
/* 1827 */ 0xAD, 0xF5, 0x17,   //          LDA   SAL       ; OUTPUT STARTING
/* 182A */ 0x20, 0x5E, 0x19,   //          JSR   OUTBTC    ; ADDRESS
/* 182D */ 0xAD, 0xF6, 0x17,   //          LDA   SAH
/* 1830 */ 0x20, 0x5E, 0x19,   //          JSR   OUTBTC
/* 1833 */ 0xAD, 0xED, 0x17,   //  DUMPT2  LDA   VEB+1     ; CHECK FOR LAST
/* 1836 */ 0xCD, 0xF7, 0x17,   //          CMP   EAL       ; DATA BYTE
/* 1839 */ 0xAD, 0xEE, 0x17,   //          LDA   VEB+2
/* 183C */ 0xED, 0xF8, 0x17,   //          SBC   EAH
/* 183F */ 0x90, 0x24,         //          BCC   DUMPT4
/* 1841 */ 0xA9, 0x2F,         //          LDA   #'/'      ; OUTPUT END-OF-DATA CHAR
/* 1843 */ 0x20, 0x7A, 0x19,   //          JSR   OUTCHT
/* 1846 */ 0xAD, 0xE7, 0x17,   //          LDA   CHKL      ; LAST BYTE HAS BEEN
/* 1849 */ 0x20, 0x61, 0x19,   //          JSR   OUTBT     ; OUTPUT  NOW OUTPUT
/* 184C */ 0xAD, 0xE8, 0x17,   //          LDA   CHKH      ; CHKSUM
/* 184F */ 0x20, 0x61, 0x19,   //          JSR   OUTBT
/* 1852 */ 0xA2, 0x02,         //          LDX   #$02      ; 2 CHARS
/* 1854 */ 0xA9, 0x04,         //  DUMPT3  LDA   #$04      ; EOT CHAR
/* 1856 */ 0x20, 0x7A, 0x19,   //          JSR   OUTCHT
/* 1859 */ 0xCA,               //          DEX
/* 185A */ 0xD0, 0xF8,         //          BNE   DUMPT3
/* 185C */ 0xA9, 0x00,         //          LDA   #$00      ; DISPLAY 0000
/* 185E */ 0x85, 0xFA,         //          STA   POINTL    ; FOR NORMAL EXIT
/* 1860 */ 0x85, 0xFB,         //          STA   POINTH
/* 1862 */ 0x4C, 0x4F, 0x1C,   //          JMP   START
/* 1865 */ 0x20, 0xEC, 0x17,   //  DUMPT4  JSR   VEB       ; DATA BYTE OUTPUT
/* 1868 */ 0x20, 0x5E, 0x19,   //          JSR   OUTBTC
/* 186B */ 0x20, 0xEA, 0x19,   //          JSR   INCVEB
/* 186E */ 0x4C, 0x33, 0x18,   //          JMP   DUMPT2
/* 1871 */                     //  ;
/* 1871 */                     //  ;       ** LOAD MEMORY FROM TAPE **
/* 1871 */                     //  ;
/* 1871 */ 0x0F, 0x19,         //  TAB     .WORD LOAD12    ; 'LOAD12' ADDRESS            1871
/* 1873 */                     //  ;
/* 1873 */ 0xA9, 0x8D,         //  LOADT   LDA   #$8D      ; INIT VOLATILE EXECUTION       1873
/* 1875 */ 0x8D, 0xEC, 0x17,   //          STA   VEB       ; BLOCK WITH STA ABS.
/* 1878 */ 0x20, 0x32, 0x19,   //          JSR   INTVEB
/* 187B */ 0xA9, 0x4C,         //          LDA   #$4C      ; JUMP TYPE RTRN
/* 187D */ 0x8D, 0xEF, 0x17,   //          STA   VEB+3
/* 1880 */ 0xAD, 0x71, 0x18,   //          LDA   TAB
/* 1883 */ 0x8D, 0xF0, 0x17,   //          STA   VEB+4
/* 1886 */ 0xAD, 0x72, 0x18,   //          LDA   TAB+1
/* 1889 */ 0x8D, 0xF1, 0x17,   //          STA   VEB+5
/* 188C */ 0xA9, 0x07,         //          LDA   #$07      ; RESET PB5=0 (DATA-IN)
/* 188E */ 0x8D, 0x42, 0x17,   //          STA   SBD
/* 1891 */ 0xA9, 0xFF,         //  SYNC    LDA   #$FF      ; CLEAR SAVX FOR SYNC CHAR      1891
/* 1893 */ 0x8D, 0xE9, 0x17,   //          STA   SAVX
/* 1896 */ 0x20, 0x41, 0x1A,   //  SYNC1   JSR   RDBIT     ; GET A BIT
/* 1899 */ 0x4E, 0xE9, 0x17,   //          LSR   SAVX      ; SHIFT BIT INTO CHAR
/* 189C */ 0x0D, 0xE9, 0x17,   //          ORA   SAVX
/* 189F */ 0x8D, 0xE9, 0x17,   //          STA   SAVX
/* 18A2 */ 0xAD, 0xE9, 0x17,   //          LDA   SAVX      ; GET NEW CHAR
/* 18A5 */ 0xC9, 0x16,         //          CMP   #$16      ; SYNC CHAR
/* 18A7 */ 0xD0, 0xED,         //          BNE   SYNC1
/* 18A9 */ 0xA2, 0x0A,         //          LDX   #$0A      ; TEST FOR 10 SYNC CHARS
/* 18AB */ 0x20, 0x24, 0x1A,   //  SYNC2   JSR   RDCHT
/* 18AE */ 0xC9, 0x16,         //          CMP   #$16
/* 18B0 */ 0xD0, 0xDF,         //          BNE   SYNC      ; IF NOT 10 CHAR, RE-SYNC
/* 18B2 */ 0xCA,               //          DEX
/* 18B3 */ 0xD0, 0xF6,         //          BNE   SYNC2
/* 18B5 */ 0x20, 0x24, 0x1A,   //  LOADT4  JSR   RDCHT     ; LOOK FOR START OF
/* 18B8 */ 0xC9, 0x2A,         //          CMP   #$2A      ; DATA CHAR
/* 18BA */ 0xF0, 0x06,         //          BEQ   LOAD11
/* 18BC */ 0xC9, 0x16,         //          CMP   #$16      ; IF NOT , SHOULD BE SYNC
/* 18BE */ 0xD0, 0xD1,         //          BNE   SYNC
/* 18C0 */ 0xF0, 0xF3,         //          BEQ   LOADT4
/* 18C2 */ 0x20, 0xF3, 0x19,   //  LOAD11  JSR   RDBYT     ; READ ID FROM TAPE
/* 18C5 */ 0xCD, 0xF9, 0x17,   //          CMP   ID        ; COMPARE WITH REQUESTED ID
/* 18C8 */ 0xF0, 0x0D,         //          BEQ   LOADT5
/* 18CA */ 0xAD, 0xF9, 0x17,   //          LDA   ID        ; DEFAULT 00, READ RECORD
/* 18CD */ 0xC9, 0x00,         //          CMP   #$00      ; ANYWAY
/* 18CF */ 0xF0, 0x06,         //          BEQ   LOADT5
/* 18D1 */ 0xC9, 0xFF,         //          CMP   #$FF      ; DEFAULT FF, IGNORE SA ON
/* 18D3 */ 0xF0, 0x17,         //          BEQ   LOADT6    ; TAPE
/* 18D5 */ 0xD0, 0x9C,         //          BNE   LOADT
/* 18D7 */ 0x20, 0xF3, 0x19,   //  LOADT5  JSR   RDBYT     ; GET SA FROM TAPE
/* 18DA */ 0x20, 0x4C, 0x19,   //          JSR   CHKT
/* 18DD */ 0x8D, 0xED, 0x17,   //          STA   VEB+1     ; SAVX IN VEB+1,2
/* 18E0 */ 0x20, 0xF3, 0x19,   //          JSR   RDBYT
/* 18E3 */ 0x20, 0x4C, 0x19,   //          JSR   CHKT
/* 18E6 */ 0x8D, 0xEE, 0x17,   //          STA   VEB+2
/* 18E9 */ 0x4C, 0xF8, 0x18,   //          JMP   LOADT7
/* 18EC */                     //  ;
/* 18EC */ 0x20, 0xF3, 0x19,   //  LOADT6  JSR   RDBYT     ; GET SA BUT IGNORE             18EC
/* 18EF */ 0x20, 0x4C, 0x19,   //          JSR   CHKT
/* 18F2 */ 0x20, 0xF3, 0x19,   //          JSR   RDBYT
/* 18F5 */ 0x20, 0x4C, 0x19,   //          JSR   CHKT
/* 18F8 */ 0xA2, 0x02,         //  LOADT7  LDX   #$02      ; GET 2 CHARS
/* 18FA */ 0x20, 0x24, 0x1A,   //  LOAD13  JSR   RDCHT     ; GET CHAR (X)
/* 18FD */ 0xC9, 0x2F,         //          CMP   #$2F      ; LOOK FOR LAST CHAR
/* 18FF */ 0xF0, 0x14,         //          BEQ   LOADT8
/* 1901 */ 0x20, 0x00, 0x1A,   //          JSR   PACKT     ; CONVERT TO HEX
/* 1904 */ 0xD0, 0x23,         //          BNE   LOADT9    ; Y=1 NON-HEX CHAR
/* 1906 */ 0xCA,               //          DEX
/* 1907 */ 0xD0, 0xF1,         //          BNE   LOAD13
/* 1909 */ 0x20, 0x4C, 0x19,   //          JSR   CHKT      ; COMPARE CHECKSUM
/* 190C */ 0x4C, 0xEC, 0x17,   //          JMP   VEB       ; SAVX DATA IN MEMORY
/* 190F */ 0x20, 0xEA, 0x19,   //  LOAD12  JSR   INCVEB    ; INCR DATA POINTER
/* 1912 */ 0x4C, 0xF8, 0x18,   //          JMP   LOADT7
/* 1915 */                     //  ;
/* 1915 */ 0x20, 0xF3, 0x19,   //  LOADT8  JSR   RDBYT     ; END OF DATA, COMPARE CHKSUM   1915
/* 1918 */ 0xCD, 0xE7, 0x17,   //          CMP   CHKL
/* 191B */ 0xD0, 0x0C,         //          BNE   LOADT9
/* 191D */ 0x20, 0xF3, 0x19,   //          JSR   RDBYT
/* 1920 */ 0xCD, 0xE8, 0x17,   //          CMP   CHKH
/* 1923 */ 0xD0, 0x04,         //          BNE   LOADT9
/* 1925 */ 0xA9, 0x00,         //          LDA   #$00      ; NORMAL EXIT
/* 1927 */ 0xF0, 0x02,         //          BEQ   LOAD10
/* 1929 */ 0xA9, 0xFF,         //  LOADT9  LDA   #$FF      ; ERROR EXIT
/* 192B */ 0x85, 0xFA,         //  LOAD10  STA   POINTL
/* 192D */ 0x85, 0xFB,         //          STA   POINTH
/* 192F */ 0x4C, 0x4F, 0x1C,   //          JMP   START
/* 1932 */                     //  ;
/* 1932 */                     //  ;       ** SUBROUTINES BELOW **
/* 1932 */ 0xAD, 0xF5, 0x17,   //  INTVEB  LDA   SAL       ; MOVE SA TO VEB+1,2            1932
/* 1935 */ 0x8D, 0xED, 0x17,   //          STA   VEB+1
/* 1938 */ 0xAD, 0xF6, 0x17,   //          LDA   SAH
/* 193B */ 0x8D, 0xEE, 0x17,   //          STA   VEB+2
/* 193E */ 0xA9, 0x60,         //          LDA   #$60      ; RTS INST
/* 1940 */ 0x8D, 0xEF, 0x17,   //          STA   VEB+3
/* 1943 */ 0xA9, 0x00,         //          LDA   #$00      ; CLEAR CHKSUM AREA
/* 1945 */ 0x8D, 0xE7, 0x17,   //          STA   CHKL
/* 1948 */ 0x8D, 0xE8, 0x17,   //          STA   CHKH
/* 194B */ 0x60,               //          RTS
/* 194C */                     //  ;       ** COMPUTE CHKSUM FOR TAPE LOAD **
/* 194C */ 0xA8,               //  CHKT    TAY             ;                               194C
/* 194D */ 0x18,               //          CLC
/* 194E */ 0x6D, 0xE7, 0x17,   //          ADC   CHKL
/* 1951 */ 0x8D, 0xE7, 0x17,   //          STA   CHKL
/* 1954 */ 0xAD, 0xE8, 0x17,   //          LDA   CHKH
/* 1957 */ 0x69, 0x00,         //          ADC   #$00
/* 1959 */ 0x8D, 0xE8, 0x17,   //          STA   CHKH
/* 195C */ 0x98,               //          TYA
/* 195D */ 0x60,               //          RTS
/* 195E */                     //  ;       ** OUTPUT ONE BYTE **
/* 195E */ 0x20, 0x4C, 0x19,   //  OUTBTC  JSR   CHKT      ; COMPARE CHKSUM                195E
/* 1961 */ 0xA8,               //  OUTBT   TAY             ; SAVX DATA BYTE
/* 1962 */ 0x4A,               //          LSR   A         ; SHIFT OFF LSD
/* 1963 */ 0x4A,               //          LSR   A
/* 1964 */ 0x4A,               //          LSR   A
/* 1965 */ 0x4A,               //          LSR   A
/* 1966 */ 0x20, 0x6F, 0x19,   //          JSR   HEXOUT    ; OUTPUT MSD
/* 1969 */ 0x98,               //          TYA
/* 196A */ 0x20, 0x6F, 0x19,   //          JSR   HEXOUT    ; OUTPUT LSD
/* 196D */ 0x98,               //          TYA
/* 196E */ 0x60,               //          RTS
/* 196F */                     //  ;       ** CONVERT LSD OF A TO ASCII, OUTPUT TO TAPE **
/* 196F */ 0x29, 0x0F,         //  HEXOUT  AND   #$0F      ;                               196F
/* 1971 */ 0xC9, 0x0A,         //          CMP   #$0A
/* 1973 */ 0x18,               //          CLC
/* 1974 */ 0x30, 0x02,         //          BMI   HEX1
/* 1976 */ 0x69, 0x07,         //          ADC   #$07
/* 1978 */ 0x69, 0x30,         //  HEX1    ADC   #$30
/* 197A */                     //  ;       ** OUTPUT TO TAPE ONE ASCII CHAR **
/* 197A */ 0x8E, 0xE9, 0x17,   //  OUTCHT  STX   SAVX      ;                               197A
/* 197D */ 0x8C, 0xEA, 0x17,   //          STY   SAVX+1
/* 1980 */ 0xA0, 0x08,         //          LDY   #$08      ; START BIT
/* 1982 */ 0x20, 0x9E, 0x19,   //  CHT1    JSR   ONE
/* 1985 */ 0x4A,               //          LSR   A         ; GET DATA BIT
/* 1986 */ 0xB0, 0x06,         //          BCS   CHT2
/* 1988 */ 0x20, 0x9E, 0x19,   //          JSR   ONE       ; DATA BIT=1
/* 198B */ 0x4C, 0x91, 0x19,   //          JMP   CHT3
/* 198E */ 0x20, 0xC4, 0x19,   //  CHT2    JSR   ZRO       ; DATA BIT=0
/* 1991 */ 0x20, 0xC4, 0x19,   //  CHT3    JSR   ZRO
/* 1994 */ 0x88,               //          DEY
/* 1995 */ 0xD0, 0xEB,         //          BNE   CHT1
/* 1997 */ 0xAE, 0xE9, 0x17,   //          LDX   SAVX
/* 199A */ 0xAC, 0xEA, 0x17,   //          LDY   SAVX+1
/* 199D */ 0x60,               //          RTS
/* 199E */                     //  ;       ** OUTPUT 1 TO TAPE, 9 PULSES, 138 US EACH **
/* 199E */ 0xA2, 0x09,         //  ONE     LDX   #$09      ;                               199E
/* 19A0 */ 0x48,               //          PHA             ; SAVX A
/* 19A1 */ 0x2C, 0x47, 0x17,   //  ONE1    BIT   CLKRDI    ; WAIT FOR TIME OUT             19A1
/* 19A4 */ 0x10, 0xFB,         //          BPL   ONE1
/* 19A6 */ 0xA9, 0x7E,         //          LDA   #126
/* 19A8 */ 0x8D, 0x44, 0x17,   //          STA   CLK1T
/* 19AB */ 0xA9, 0xA7,         //          LDA   #$A7
/* 19AD */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; SET PB7 = 1
/* 19B0 */ 0x2C, 0x47, 0x17,   //  ONE2    BIT   CLKRDI    ;                               19B0
/* 19B3 */ 0x10, 0xFB,         //          BPL   ONE2
/* 19B5 */ 0xA9, 0x7E,         //          LDA   #126
/* 19B7 */ 0x8D, 0x44, 0x17,   //          STA   CLK1T
/* 19BA */ 0xA9, 0x27,         //          LDA   #$27
/* 19BC */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; RESET PB7=0
/* 19BF */ 0xCA,               //          DEX
/* 19C0 */ 0xD0, 0xDF,         //          BNE   ONE1
/* 19C2 */ 0x68,               //          PLA
/* 19C3 */ 0x60,               //          RTS
/* 19C4 */                     //  ;       ** OUTPUT 0 TO TAPE, 6 PULSES, 207 US EACH **
/* 19C4 */ 0xA2, 0x06,         //  ZRO     LDX   #$06      ;                               19C4
/* 19C6 */ 0x48,               //          PHA             ; SAVX A
/* 19C7 */ 0x2C, 0x47, 0x17,   //  ZRO1    BIT   CLKRDI    ;                               19C7
/* 19CA */ 0x10, 0xFB,         //          BPL   ZRO1
/* 19CC */ 0xA9, 0xC3,         //          LDA   #$C3
/* 19CE */ 0x8D, 0x44, 0x17,   //          STA   CLK1T
/* 19D1 */ 0xA9, 0xA7,         //          LDA   #$A7
/* 19D3 */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; SET PB7=1
/* 19D6 */ 0x2C, 0x47, 0x17,   //  ZRO2    BIT   CLKRDI
/* 19D9 */ 0x10, 0xFB,         //          BPL   ZRO2
/* 19DB */ 0xA9, 0xC3,         //          LDA   #195
/* 19DD */ 0x8D, 0x44, 0x17,   //          STA   CLK1T
/* 19E0 */ 0xA9, 0x27,         //          LDA   #$27
/* 19E2 */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; RESET PB7=0
/* 19E5 */ 0xCA,               //          DEX
/* 19E6 */ 0xD0, 0xDF,         //          BNE   ZRO1
/* 19E8 */ 0x68,               //          PLA             ; RESTORE A
/* 19E9 */ 0x60,               //          RTS
/* 19EA */                     //  ;       ** SUB TO INC VEB+1,2 **
/* 19EA */ 0xEE, 0xED, 0x17,   //  INCVEB  INC   VEB+1     ;                               19EA
/* 19ED */ 0xD0, 0x03,         //          BNE   INCVE1
/* 19EF */ 0xEE, 0xEE, 0x17,   //          INC   VEB+2
/* 19F2 */ 0x60,               //  INCVE1  RTS
/* 19F3 */                     //  ;       ** SUB TO READ BYTE FROM TAPE **
/* 19F3 */ 0x20, 0x24, 0x1A,   //  RDBYT   JSR   RDCHT     ;                              19F3
/* 19F6 */ 0x20, 0x00, 0x1A,   //          JSR   PACKT
/* 19F9 */ 0x20, 0x24, 0x1A,   //          JSR   RDCHT
/* 19FC */ 0x20, 0x00, 0x1A,   //          JSR   PACKT
/* 19FF */ 0x60,               //          RTS
/* 1A00 */                     //  ;       ** PACK A=ASCII INTO SAVX AS HEX DATA **
/* 1A00 */ 0xC9, 0x30,         //  PACKT   CMP   #$30      ;                               1A00
/* 1A02 */ 0x30, 0x1E,         //          BMI   PACKT3
/* 1A04 */ 0xC9, 0x47,         //          CMP   #$47
/* 1A06 */ 0x10, 0x1A,         //          BPL   PACKT3
/* 1A08 */ 0xC9, 0x40,         //          CMP   #$40
/* 1A0A */ 0x30, 0x03,         //          BMI   PACKT1
/* 1A0C */ 0x18,               //          CLC
/* 1A0D */ 0x69, 0x09,         //          ADC   #$09
/* 1A0F */ 0x2A,               //  PACKT1  ROL   A
/* 1A10 */ 0x2A,               //          ROL   A
/* 1A11 */ 0x2A,               //          ROL   A
/* 1A12 */ 0x2A,               //          ROL   A
/* 1A13 */ 0xA0, 0x04,         //          LDY   #$04
/* 1A15 */ 0x2A,               //  PACKT2  ROL   A
/* 1A16 */ 0x2E, 0xE9, 0x17,   //          ROL   SAVX
/* 1A19 */ 0x88,               //          DEY
/* 1A1A */ 0xD0, 0xF9,         //          BNE   PACKT2
/* 1A1C */ 0xAD, 0xE9, 0x17,   //          LDA   SAVX
/* 1A1F */ 0xA0, 0x00,         //          LDY   #$00      ; Y=0 VALID HEX CHAR
/* 1A21 */ 0x60,               //          RTS
/* 1A22 */ 0xC8,               //  PACKT3  INY             ; Y=1 NOT HEX
/* 1A23 */ 0x60,               //          RTS
/* 1A24 */                     //  ;       ** GET 1 CHAR FROM TAPE AND RETURN WITH
/* 1A24 */                     //  ;          CHAR IN A.  USE SAVX+1 TO ASM CHAR  **
/* 1A24 */ 0x8E, 0xEB, 0x17,   //  RDCHT   STX   SAVX+2    ;                              1A24
/* 1A27 */ 0xA2, 0x08,         //          LDX   #$08      ; READ 8 BITS
/* 1A29 */ 0x20, 0x41, 0x1A,   //  RDCHT1  JSR   RDBIT     ; GET NEXT DATA BIT
/* 1A2C */ 0x4E, 0xEA, 0x17,   //          LSR   SAVX+1    ; RIGHT SHIFT CHAR
/* 1A2F */ 0x0D, 0xEA, 0x17,   //          ORA   SAVX+1    ; OR IN SIGN BIT
/* 1A32 */ 0x8D, 0xEA, 0x17,   //          STA   SAVX+1    ; REPLACE CHAR
/* 1A35 */ 0xCA,               //          DEX
/* 1A36 */ 0xD0, 0xF1,         //          BNE   RDCHT1
/* 1A38 */ 0xAD, 0xEA, 0x17,   //          LDA   SAVX+1    ; MOVE CHAR INTO A
/* 1A3B */ 0x2A,               //          ROL   A         ; SHIFT OFF PARITY
/* 1A3C */ 0x4A,               //          LSR   A
/* 1A3D */ 0xAE, 0xEB, 0x17,   //          LDX   SAVX+2
/* 1A40 */ 0x60,               //          RTS
/* 1A41 */                     //  ;       ** THIS SUB GETS ONE BIT FROM TAPE AND
/* 1A41 */                     //  ;          RETURNS IT IN SIGN OF A  **
/* 1A41 */ 0x2C, 0x42, 0x17,   //  RDBIT   BIT   SBD       ; WAIT FOR END OF START BIT     1A41
/* 1A44 */ 0x10, 0xFB,         //          BPL   RDBIT
/* 1A46 */ 0xAD, 0x46, 0x17,   //          LDA   CLKRDT    ; GET START BIT TIME
/* 1A49 */ 0xA0, 0xFF,         //          LDY   #$FF      ; A=256-T1
/* 1A4B */ 0x8C, 0x46, 0x17,   //          STY   CLK64T    ; SET UP TIMER
/* 1A4E */ 0xA0, 0x14,         //          LDY   #$14
/* 1A50 */ 0x88,               //  RDBIT3  DEY             ; DELAY 100 MICRO SEC
/* 1A51 */ 0xD0, 0xFD,         //          BNE   RDBIT3
/* 1A53 */ 0x2C, 0x42, 0x17,   //  RDBIT2  BIT   SBD
/* 1A56 */ 0x30, 0xFB,         //          BMI   RDBIT2    ; WAIT FOR NEXT START BIT
/* 1A58 */ 0x38,               //          SEC
/* 1A59 */ 0xED, 0x46, 0x17,   //          SBC   CLKRDT    ; (256-T1)-(256-T2)=T2-T1
/* 1A5C */ 0xA0, 0xFF,         //          LDY   #$FF
/* 1A5E */ 0x8C, 0x46, 0x17,   //          STY   CLK64T    ; SET UP TIMER FOR NEXT BIT
/* 1A61 */ 0xA0, 0x07,         //          LDY   #$07
/* 1A63 */ 0x88,               //  RDBIT4  DEY             ; DELAY 50 MICROSEC
/* 1A64 */ 0xD0, 0xFD,         //          BNE   RDBIT4
/* 1A66 */ 0x49, 0xFF,         //          EOR   #$FF      ; COMPLEMENT SIGN OF A
/* 1A68 */ 0x29, 0x80,         //          AND   #$80      ; MASK ALL EXCEPT SIGN
/* 1A6A */ 0x60,               //          RTS
/* 1A6B */                     //  ;       ** PLLCAL OUTPUT 166 MICROSEC (6024 HZ)
/* 1A6B */                     //  ;          PULSE STRING
/* 1A6B */ 0xA9, 0x27,         //  PLLCAL  LDA   #$27      ;                              1A6B
/* 1A6D */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; TURN OFF DATIN PB5=1
/* 1A70 */ 0xA9, 0xBF,         //          LDA   #$BF      ; CONVERT PB7 TO OUTPUT
/* 1A72 */ 0x8D, 0x43, 0x17,   //          STA   PBDD
/* 1A75 */ 0x2C, 0x47, 0x17,   //  PLL1    BIT   CLKRDI
/* 1A78 */ 0x10, 0xFB,         //          BPL   PLL1
/* 1A7A */ 0xA9, 0x9A,         //          LDA   #154      ; WAIT 166 MICROSEC
/* 1A7C */ 0x8D, 0x44, 0x17,   //          STA   CLK1T
/* 1A7F */ 0xA9, 0xA7,         //          LDA   #$A7      ; OUTPUT PB7=1
/* 1A81 */ 0x8D, 0x42, 0x17,   //          STA   SBD
/* 1A84 */ 0x2C, 0x47, 0x17,   //  PLL2    BIT   CLKRDI
/* 1A87 */ 0x10, 0xFB,         //          BPL   PLL2
/* 1A89 */ 0xA9, 0x9A,         //          LDA   #154
/* 1A8B */ 0x8D, 0x44, 0x17,   //          STA   CLK1T
/* 1A8E */ 0xA9, 0x27,         //          LDA   #$27      ; PB7=0
/* 1A90 */ 0x8D, 0x42, 0x17,   //          STA   SBD
/* 1A93 */ 0x4C, 0x75, 0x1A,   //          JMP   PLL1
/* 1A96 */                     //
/* 1A96 */                     //  ; Fill unused locations with $FF
/* 1A96 */ 0xFF, 0xFF, 0xFF, 0xFF,  // .FILL 356, $FF
/* 1A9A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1A9E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AA2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AA6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AAA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AAE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AB2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AB6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1ABA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1ABE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AC2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AC6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1ACA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1ACE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AD2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AD6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1ADA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1ADE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AE2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AE6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AEA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AEE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AF2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AF6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AFA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1AFE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B02 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B06 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B0A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B0E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B12 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B16 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B1A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B1E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B22 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B26 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B2A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B2E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B32 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B36 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B3A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B3E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B42 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B46 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B4A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B4E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B52 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B56 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B5A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B5E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B62 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B66 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B6A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B6E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B72 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B76 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B7A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B7E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B82 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B86 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B8A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B8E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B92 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B96 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B9A */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1B9E */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BA2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BA6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BAA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BAE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BB2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BB6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BBA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BBE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BC2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BC6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BCA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BCE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BD2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BD6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BDA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BDE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BE2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BE6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BEA */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BEE */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BF2 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BF6 */ 0xFF, 0xFF, 0xFF, 0xFF,  //
/* 1BFA */                     //  ;
/* 1BFA */                     //  ;       ** INTERRUPTS **
/* 1BFA */                     //          .org  $1BFA
/* 1BFA */ 0x6B, 0x1A,         //  NMIP27  .WORD PLLCAL
/* 1BFC */ 0x6B, 0x1A,         //  RSTP27  .WORD PLLCAL
/* 1BFE */ 0x6B, 0x1A          //  IRQP27  .WORD PLLCAL
};







// ============================================================================
// ============================================================================
// MICROCHESS
// ============================================================================
// ============================================================================
// see benlo.com/microchess
#if _INCL_MCHESS == TRUE
  #if _TARGET == PERSONAL_COMPUTER
    unsigned char mchessROM[1393] = {
  #else
    const unsigned char mchessROM[1393] PROGMEM = {
  #endif
    0xA9, 0x00, 0x85, 0xB7, 0x20, 0x54, 0xC4, 0xD8, 0xA2, 0xFF, 0x9A, 0xA2,
    0xC8, 0x86, 0xB2, 0x20, 0x78, 0xC3, 0x20, 0x49, 0xC4, 0xC9, 0x43, 0xD0,
    0x12, 0xA2, 0x1F, 0xBD, 0x14, 0xC5, 0x95, 0x50, 0xCA, 0x10, 0xF8, 0xA2,
    0x1B, 0x86, 0xDC, 0xA9, 0xCC, 0xD0, 0x19, 0xC9, 0x45, 0xD0, 0x0E, 0x20,
    0xC1, 0xC1, 0x38, 0xA9, 0x01, 0xE5, 0xB7, 0x85, 0xB7, 0xA9, 0xEE, 0xD0,
    0x07, 0xC9, 0x40, 0xD0, 0x0B, 0x20, 0xB5, 0xC2, 0x85, 0xFB, 0x85, 0xFA,
    0x85, 0xF9, 0xD0, 0xBB, 0xC9, 0x0D, 0xD0, 0x06, 0x20, 0x5B, 0xC2, 0x4C,
    0xFD, 0xC0, 0xC9, 0x41, 0xF0, 0x03, 0x4C, 0xF6, 0xC0, 0x4C, 0x00, 0xFF,
    0xA6, 0xB5, 0x30, 0x5C, 0xA5, 0xB0, 0xF0, 0x08, 0xE0, 0x08, 0xD0, 0x04,
    0xC5, 0xE6, 0xF0, 0x2E, 0xF6, 0xE3, 0xC9, 0x01, 0xD0, 0x02, 0xF6, 0xE3,
    0x50, 0x1E, 0xA0, 0x0F, 0xA5, 0xB1, 0xD9, 0x60, 0x00, 0xF0, 0x03, 0x88,
    0x10, 0xF8, 0xB9, 0x45, 0xC5, 0xD5, 0xE4, 0x90, 0x04, 0x94, 0xE6, 0x95,
    0xE4, 0x18, 0x08, 0x75, 0xE5, 0x95, 0xE5, 0x28, 0xE0, 0x04, 0xF0, 0x03,
    0x30, 0x31, 0x60, 0xA5, 0xE8, 0x85, 0xDD, 0xA9, 0x00, 0x85, 0xB5, 0x20,
    0x5B, 0xC2, 0x20, 0xC1, 0xC1, 0x20, 0x0F, 0xC1, 0x20, 0xC1, 0xC1, 0xA9,
    0x08, 0x85, 0xB5, 0x20, 0x18, 0xC1, 0x20, 0x41, 0xC2, 0x4C, 0x12, 0xC3,
    0xE0, 0xF9, 0xD0, 0x0B, 0xA5, 0x60, 0xC5, 0xB1, 0xD0, 0x04, 0xA9, 0x00,
    0x85, 0xB4, 0x60, 0x50, 0xFD, 0xA0, 0x07, 0xA5, 0xB1, 0xD9, 0x60, 0x00,
    0xF0, 0x05, 0x88, 0xF0, 0xF1, 0x10, 0xF6, 0xB9, 0x45, 0xC5, 0xD5, 0xE2,
    0x90, 0x02, 0x95, 0xE2, 0xC6, 0xB5, 0xA9, 0xFB, 0xC5, 0xB5, 0xF0, 0x03,
    0x20, 0x35, 0xC2, 0xE6, 0xB5, 0x60, 0xC9, 0x08, 0xB0, 0x12, 0x20, 0x02,
    0xC3, 0xA2, 0x1F, 0xB5, 0x50, 0xC5, 0xFA, 0xF0, 0x03, 0xCA, 0x10, 0xF7,
    0x86, 0xFB, 0x86, 0xB0, 0x4C, 0x07, 0xC0, 0xA2, 0x10, 0xA9, 0x00, 0x95,
    0xDE, 0xCA, 0x10, 0xFB, 0xA9, 0x10, 0x85, 0xB0, 0xC6, 0xB0, 0x10, 0x01,
    0x60, 0x20, 0x2E, 0xC2, 0xA4, 0xB0, 0xA2, 0x08, 0x86, 0xB6, 0xC0, 0x08,
    0x10, 0x41, 0xC0, 0x06, 0x10, 0x2E, 0xC0, 0x04, 0x10, 0x1F, 0xC0, 0x01,
    0xF0, 0x09, 0x10, 0x0E, 0x20, 0x9D, 0xC1, 0xD0, 0xFB, 0xF0, 0xD9, 0x20,
    0xAB, 0xC1, 0xD0, 0xFB, 0xF0, 0xD2, 0xA2, 0x04, 0x86, 0xB6, 0x20, 0xAB,
    0xC1, 0xD0, 0xFB, 0xF0, 0xC7, 0x20, 0xAB, 0xC1, 0xA5, 0xB6, 0xC9, 0x04,
    0xD0, 0xF7, 0xF0, 0xBC, 0xA2, 0x10, 0x86, 0xB6, 0x20, 0x9D, 0xC1, 0xA5,
    0xB6, 0xC9, 0x08, 0xD0, 0xF7, 0xF0, 0xAD, 0xA2, 0x06, 0x86, 0xB6, 0x20,
    0xD9, 0xC1, 0x50, 0x05, 0x30, 0x03, 0x20, 0x60, 0xC0, 0x20, 0x2E, 0xC2,
    0xC6, 0xB6, 0xA5, 0xB6, 0xC9, 0x05, 0xF0, 0xEB, 0x20, 0xD9, 0xC1, 0x70,
    0x8F, 0x30, 0x8D, 0x20, 0x60, 0xC0, 0xA5, 0xB1, 0x29, 0xF0, 0xC9, 0x20,
    0xF0, 0xEE, 0x4C, 0x1C, 0xC1, 0x20, 0xD9, 0xC1, 0x30, 0x03, 0x20, 0x60,
    0xC0, 0x20, 0x2E, 0xC2, 0xC6, 0xB6, 0x60, 0x20, 0xD9, 0xC1, 0x90, 0x02,
    0x50, 0xF9, 0x30, 0x07, 0x08, 0x20, 0x60, 0xC0, 0x28, 0x50, 0xF0, 0x20,
    0x2E, 0xC2, 0xC6, 0xB6, 0x60, 0xA2, 0x0F, 0x38, 0xB4, 0x60, 0xA9, 0x77,
    0xF5, 0x50, 0x95, 0x60, 0x94, 0x50, 0x38, 0xA9, 0x77, 0xF5, 0x50, 0x95,
    0x50, 0xCA, 0x10, 0xEB, 0x60, 0xA5, 0xB1, 0xA6, 0xB6, 0x18, 0x7D, 0x34,
    0xC5, 0x85, 0xB1, 0x29, 0x88, 0xD0, 0x42, 0xA5, 0xB1, 0xA2, 0x20, 0xCA,
    0x30, 0x0E, 0xD5, 0x50, 0xD0, 0xF9, 0xE0, 0x10, 0x30, 0x33, 0xA9, 0x7F,
    0x69, 0x01, 0x70, 0x01, 0xB8, 0xA5, 0xB5, 0x30, 0x24, 0xC9, 0x08, 0x10,
    0x20, 0x48, 0x08, 0xA9, 0xF9, 0x85, 0xB5, 0x85, 0xB4, 0x20, 0x5B, 0xC2,
    0x20, 0xC1, 0xC1, 0x20, 0x18, 0xC1, 0x20, 0x3E, 0xC2, 0x28, 0x68, 0x85,
    0xB5, 0xA5, 0xB4, 0x30, 0x04, 0x38, 0xA9, 0xFF, 0x60, 0x18, 0xA9, 0x00,
    0x60, 0xA9, 0xFF, 0x18, 0xB8, 0x60, 0xA6, 0xB0, 0xB5, 0x50, 0x85, 0xB1,
    0x60, 0x20, 0x5B, 0xC2, 0x20, 0xC1, 0xC1, 0x20, 0x18, 0xC1, 0x20, 0xC1,
    0xC1, 0xBA, 0x86, 0xB3, 0xA6, 0xB2, 0x9A, 0x68, 0x85, 0xB6, 0x68, 0x85,
    0xB0, 0xAA, 0x68, 0x95, 0x50, 0x68, 0xAA, 0x68, 0x85, 0xB1, 0x95, 0x50,
    0x4C, 0x80, 0xC2, 0xBA, 0x86, 0xB3, 0xA6, 0xB2, 0x9A, 0xA5, 0xB1, 0x48,
    0xA8, 0xA2, 0x1F, 0xD5, 0x50, 0xF0, 0x03, 0xCA, 0x10, 0xF9, 0xA9, 0xCC,
    0x95, 0x50, 0x8A, 0x48, 0xA6, 0xB0, 0xB5, 0x50, 0x94, 0x50, 0x48, 0x8A,
    0x48, 0xA5, 0xB6, 0x48, 0xBA, 0x86, 0xB2, 0xA6, 0xB3, 0x9A, 0x60, 0xA6,
    0xE4, 0xEC, 0x45, 0xC5, 0xD0, 0x04, 0xA9, 0x00, 0xF0, 0x0A, 0xA6, 0xE3,
    0xD0, 0x06, 0xA6, 0xEE, 0xD0, 0x02, 0xA9, 0xFF, 0xA2, 0x04, 0x86, 0xB5,
    0xC5, 0xFA, 0x90, 0x0C, 0xF0, 0x0A, 0x85, 0xFA, 0xA5, 0xB0, 0x85, 0xFB,
    0xA5, 0xB1, 0x85, 0xF9, 0xA9, 0x2E, 0x4C, 0x60, 0xC4, 0xA6, 0xDC, 0x30,
    0x1C, 0xA5, 0xF9, 0xDD, 0x55, 0xC5, 0xD0, 0x11, 0xCA, 0xBD, 0x55, 0xC5,
    0x85, 0xFB, 0xCA, 0xBD, 0x55, 0xC5, 0x85, 0xF9, 0xCA, 0x86, 0xDC, 0xD0,
    0x1C, 0xA9, 0xFF, 0x85, 0xDC, 0xA2, 0x0C, 0x86, 0xB5, 0x86, 0xFA, 0xA2,
    0x14, 0x20, 0x11, 0xC1, 0xA2, 0x04, 0x86, 0xB5, 0x20, 0x0F, 0xC1, 0xA6,
    0xFA, 0xE0, 0x0F, 0x90, 0x12, 0xA6, 0xFB, 0xB5, 0x50, 0x85, 0xFA, 0x86,
    0xB0, 0xA5, 0xF9, 0x85, 0xB1, 0x20, 0x5B, 0xC2, 0x4C, 0x07, 0xC0, 0xA9,
    0xFF, 0x60, 0xA2, 0x04, 0x06, 0xF9, 0x26, 0xFA, 0xCA, 0xD0, 0xF9, 0x05,
    0xF9, 0x85, 0xF9, 0x85, 0xB1, 0x60, 0x18, 0xA9, 0x80, 0x65, 0xEB, 0x65,
    0xEC, 0x65, 0xED, 0x65, 0xE1, 0x65, 0xDF, 0x38, 0xE5, 0xF0, 0xE5, 0xF1,
    0xE5, 0xE2, 0xE5, 0xE0, 0xE5, 0xDE, 0xE5, 0xEF, 0xE5, 0xE3, 0xB0, 0x02,
    0xA9, 0x00, 0x4A, 0x18, 0x69, 0x40, 0x65, 0xEC, 0x65, 0xED, 0x38, 0xE5,
    0xE4, 0x4A, 0x18, 0x69, 0x90, 0x65, 0xDD, 0x65, 0xDD, 0x65, 0xDD, 0x65,
    0xDD, 0x65, 0xE1, 0x38, 0xE5, 0xE4, 0xE5, 0xE4, 0xE5, 0xE5, 0xE5, 0xE5,
    0xE5, 0xE0, 0xA6, 0xB1, 0xE0, 0x33, 0xF0, 0x16, 0xE0, 0x34, 0xF0, 0x12,
    0xE0, 0x22, 0xF0, 0x0E, 0xE0, 0x25, 0xF0, 0x0A, 0xA6, 0xB0, 0xF0, 0x09,
    0xB4, 0x50, 0xC0, 0x10, 0x10, 0x03, 0x18, 0x69, 0x02, 0x4C, 0x87, 0xC2,
    0x20, 0x17, 0xC4, 0x20, 0x3B, 0xC4, 0x20, 0x22, 0xC4, 0xA0, 0x00, 0x20,
    0xE9, 0xC3, 0xA9, 0x7C, 0x20, 0x60, 0xC4, 0xA2, 0x1F, 0x98, 0xD5, 0x50,
    0xF0, 0x40, 0xCA, 0x10, 0xF8, 0x98, 0x29, 0x01, 0x85, 0xFC, 0x98, 0x4A,
    0x4A, 0x4A, 0x4A, 0x29, 0x01, 0x18, 0x65, 0xFC, 0x29, 0x01, 0xF0, 0x03,
    0xA9, 0x2A, 0x2C, 0xA9, 0x20, 0x20, 0x60, 0xC4, 0x20, 0x60, 0xC4, 0xC8,
    0x98, 0x29, 0x08, 0xF0, 0xCD, 0xA9, 0x7C, 0x20, 0x60, 0xC4, 0x20, 0x34,
    0xC4, 0x20, 0x17, 0xC4, 0x20, 0xE9, 0xC3, 0x18, 0x98, 0x69, 0x08, 0xA8,
    0xC0, 0x80, 0xF0, 0x2B, 0xD0, 0xB4, 0xA5, 0xB7, 0xF0, 0x05, 0xBD, 0xD3,
    0xC4, 0xD0, 0x03, 0xBD, 0xC3, 0xC4, 0x20, 0x60, 0xC4, 0xBD, 0xF3, 0xC4,
    0x20, 0x60, 0xC4, 0xD0, 0xCA, 0x8A, 0x48, 0xA2, 0x19, 0xA9, 0x2D, 0x20,
    0x60, 0xC4, 0xCA, 0xD0, 0xFA, 0x68, 0xAA, 0x20, 0x17, 0xC4, 0x60, 0x20,
    0x22, 0xC4, 0xA5, 0xFB, 0x20, 0x64, 0xC4, 0xA9, 0x20, 0x20, 0x60, 0xC4,
    0xA5, 0xFA, 0x20, 0x64, 0xC4, 0xA9, 0x20, 0x20, 0x60, 0xC4, 0xA5, 0xF9,
    0x20, 0x64, 0xC4, 0xA9, 0x0D, 0x20, 0x60, 0xC4, 0xA9, 0x0A, 0x20, 0x60,
    0xC4, 0x60, 0xA2, 0x00, 0xA9, 0x20, 0x20, 0x60, 0xC4, 0x8A, 0x20, 0x64,
    0xC4, 0xE8, 0xE0, 0x08, 0xD0, 0xF2, 0xF0, 0xE3, 0x98, 0x29, 0x70, 0x20,
    0x64, 0xC4, 0x60, 0xA2, 0x00, 0xBD, 0x8A, 0xC4, 0xF0, 0x06, 0x20, 0x60,
    0xC4, 0xE8, 0xD0, 0xF5, 0x60, 0xA9, 0x3F, 0x20, 0x60, 0xC4, 0x20, 0x55,
    0xC4, 0x29, 0x4F, 0x60, 0x60, 0xAD, 0xF3, 0xCF, 0xC5, 0x01, 0xF0, 0xF9,
    0xAD, 0xF4, 0xCF, 0x60, 0x8D, 0xF1, 0xCF, 0x60, 0x48, 0x4A, 0x4A, 0x4A,
    0x4A, 0x20, 0x6D, 0xC4, 0x68, 0x29, 0x0F, 0x84, 0xFF, 0xA8, 0xB9, 0x7A,
    0xC4, 0xA4, 0xFF, 0x4C, 0x60, 0xC4, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
    0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x4D, 0x69,
    0x63, 0x72, 0x6F, 0x43, 0x68, 0x65, 0x73, 0x73, 0x20, 0x28, 0x63, 0x29,
    0x20, 0x31, 0x39, 0x39, 0x36, 0x2D, 0x32, 0x30, 0x30, 0x35, 0x20, 0x50,
    0x65, 0x74, 0x65, 0x72, 0x20, 0x4A, 0x65, 0x6E, 0x6E, 0x69, 0x6E, 0x67,
    0x73, 0x2C, 0x20, 0x77, 0x77, 0x77, 0x2E, 0x62, 0x65, 0x6E, 0x6C, 0x6F,
    0x2E, 0x63, 0x6F, 0x6D, 0x0D, 0x0A, 0x00, 0x57, 0x57, 0x57, 0x57, 0x57,
    0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x42,
    0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
    0x42, 0x42, 0x42, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57,
    0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x4B, 0x51, 0x52, 0x52, 0x42,
    0x42, 0x4E, 0x4E, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x4B,
    0x51, 0x52, 0x52, 0x42, 0x42, 0x4E, 0x4E, 0x50, 0x50, 0x50, 0x50, 0x50,
    0x50, 0x50, 0x50, 0x00, 0x03, 0x04, 0x00, 0x07, 0x02, 0x05, 0x01, 0x06,
    0x10, 0x17, 0x11, 0x16, 0x12, 0x15, 0x14, 0x13, 0x73, 0x74, 0x70, 0x77,
    0x72, 0x75, 0x71, 0x76, 0x60, 0x67, 0x61, 0x66, 0x62, 0x65, 0x64, 0x63,
    0x00, 0xF0, 0xFF, 0x01, 0x10, 0x11, 0x0F, 0xEF, 0xF1, 0xDF, 0xE1, 0xEE,
    0xF2, 0x12, 0x0E, 0x1F, 0x21, 0x0B, 0x0A, 0x06, 0x06, 0x04, 0x04, 0x04,
    0x04, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x99, 0x25, 0x0B,
    0x25, 0x01, 0x00, 0x33, 0x25, 0x07, 0x36, 0x34, 0x0D, 0x34, 0x34, 0x0E,
    0x52, 0x25, 0x0D, 0x45, 0x35, 0x04, 0x55, 0x22, 0x06, 0x43, 0x33, 0x0F,
    0xCC
};
#endif




// ============================================================================
// ============================================================================
// MOVIT utility, 1st of 3 utilities copied to RAM
// ============================================================================
// ============================================================================
// MOVIT utility, see http://users.telenet.be/kim1-6502/6502/fbok.html#p114
#if _TARGET == PERSONAL_COMPUTER
    unsigned char movitROM[95] = {
#else
    const unsigned char movitROM[100] PROGMEM = {
#endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  #define OSAL      $D0
/* 0000 */                     //  #define OSAH      $D1
/* 0000 */                     //  #define OEAL      $D2
/* 0000 */                     //  #define OEAH      $D3
/* 0000 */                     //  #define NSAL      $D4
/* 0000 */                     //  #define NSAH      $D5
/* 0000 */                     //  #define NEAL      $D6
/* 0000 */                     //  #define NEAH      $D7
/* 0000 */                     //  #define BCL       $D8
/* 0000 */                     //  #define BCH       $D9
/* 0000 */                     //
/* 1780 */                     //          .ORG    $1780
/* 1780 */                     //
/* 1780 */ 0xD8,               //  START:  CLD
/* 1781 */ 0xA0, 0xFF,         //          LDY     #$FF            ; STORE TEST VALUE
/* 1783 */ 0x38,               //          SEC
/* 1784 */ 0xA5, 0xD2,         //          LDA     OEAL            ; HOW MANY BYTES?
/* 1786 */ 0xE5, 0xD0,         //          SBC     OSAL            ; TO MOVE?
/* 1788 */ 0x85, 0xD8,         //          STA     BCL
/* 178A */ 0xA5, 0xD3,         //          LDA     OEAH
/* 178C */ 0xE5, 0xD1,         //          SBC     OSAH
/* 178E */ 0x85, 0xD9,         //          STA     BCH
/* 1790 */ 0x18,               //          CLC
/* 1791 */ 0xA5, 0xD8,         //          LDA     BCL             ; ADD THE COUNT TO
/* 1793 */ 0x65, 0xD4,         //          ADC     NSAL            ; THE NEW START TO
/* 1795 */ 0x85, 0xD6,         //          STA     NEAL            ; GET A NEW END
/* 1797 */ 0xA5, 0xD9,         //          LDA     BCH
/* 1799 */ 0x65, 0xD5,         //          ADC     NSAH
/* 179B */ 0x85, 0xD7,         //          STA     NEAH
/* 179D */ 0xE6, 0xD8,         //          INC     BCL             ; ADJUST THE BYTE COUNT
/* 179F */ 0xE6, 0xD9,         //          INC     BCH             ; TO PERMIT ZERO TESTING
/* 17A1 */ 0x38,               //          SEC
/* 17A2 */ 0xA5, 0xD4,         //          LDA     NSAL            ; IF NEW LOCATION
/* 17A4 */ 0xE5, 0xD0,         //          SBC     OSAL            ; HIGHER THAN OLD
/* 17A6 */ 0xA5, 0xD5,         //          LDA     NSAH            ; CARRY FLAG IS SET
/* 17A8 */ 0xE5, 0xD1,         //          SBC     OSAH
/* 17AA */ 0xA2, 0x00,         //  LOOP:   LDX     #$00            ; HIGH POINTER INDEX
/* 17AC */ 0x90, 0x02,         //          BCC     MOVE
/* 17AE */ 0xA2, 0x02,         //          LDX     #$02            ; LOW POINTER INDEX
/* 17B0 */ 0xA1, 0xD0,         //  MOVE:   LDA     (OSAL,X)        ; MOVE OLD. NOTE ERROR IN PRINTED LISTING
/* 17B2 */ 0x81, 0xD4,         //          STA     (NSAL,X)        ; TO NEW. NOTE ERROR IN PRINTED LISTING
/* 17B4 */ 0x90, 0x14,         //          BCC     DOWN
/* 17B6 */ 0xC6, 0xD2,         //          DEC     OEAL            ; ADJUST UP POINTER, (OLD)
/* 17B8 */ 0x98,               //          TYA                     ; BELOW ZERO?
/* 17B9 */ 0x45, 0xD2,         //          EOR     OEAL
/* 17BB */ 0xD0, 0x02,         //          BNE     NOT             ; NO, ENOUGH
/* 17BD */ 0xC6, 0xD3,         //          DEC     OEAH            ; YES, ADJUST THE HIGH BYTE
/* 17BF */ 0xC6, 0xD6,         //  NOT:    DEC     NEAL            ; ADJUST THE OTHER ONE (NEW)
/* 17C1 */ 0x98,               //          TYA
/* 17C2 */ 0x45, 0xD6,         //          EOR     NEAL            ; NEED HIGH BYTE ADJUSTED?
/* 17C4 */ 0xD0, 0x02,         //          BNE     NEIN            ; NO
/* 17C6 */ 0xC6, 0xD7,         //          DEC     NEAH            ; YES, DO IT
/* 17C8 */ 0xB0, 0x0C,         //  NEIN:   BCS     COUNT
/* 17CA */ 0xE6, 0xD0,         //  DOWN:   INC     OSAL            ; ADJUST "OLD" DOWN POINTER
/* 17CC */ 0xD0, 0x02,         //          BNE     NYET
/* 17CE */ 0xE6, 0xD1,         //          INC     OSAH            ; AND THE HIGH BYTE IF NEEDED
/* 17D0 */ 0xE6, 0xD4,         //  NYET:   INC     NSAL            ; AND THE "NEW" ONE
/* 17D2 */ 0xD0, 0x02,         //          BNE     COUNT
/* 17D4 */ 0xE6, 0xD5,         //          INC     NSAH
/* 17D6 */ 0xC6, 0xD8,         //  COUNT:  DEC     BCL             ; TICK OFF THE BYTES,
/* 17D8 */ 0xD0, 0x02,         //          BNE     ONE             ; ENOUGH FINGERS?
/* 17DA */ 0xC6, 0xD9,         //          DEC     BCH             ; USE THE OTHER HAND
/* 17DC */ 0xD0, 0xCC,         //  ONE:    BNE     LOOP            ; 'TIL THEY'RE ALL DONE
/* 17DE */ 0x00,               //  DONE:   BRK                     ; & BACK TO MONITOR
/* 17DF */                     //
/* 17DF */                     //          .END
};



// ============================================================================
// ============================================================================
// RELOCATE (2nd of 3 utilities that will be copied into RAM)
// ============================================================================
// ============================================================================
/* see http://users.telenet.be/kim1-6502/6502/fbok.html#p114 */
#if _TARGET == PERSONAL_COMPUTER
    unsigned char relocateROM[149] = {
#else
    const unsigned char relocateROM[149] PROGMEM = {
#endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  ;
/* 0000 */                     //  ; following addresses must be initialized
/* 0000 */                     //  ; by user prior to run
/* 0000 */                     //
/* 0000 */                     //  #define ALOC      $E0
/* 0000 */                     //  #define LIMIT     $E3
/* 0000 */                     //  #define PAGLIM    $E7           ; limit above which kill relocn
/* 0000 */                     //  #define ADJST     $E8           ; adjustment distance (signed)
/* 0000 */                     //  #define POINT     $EA           ; start of program
/* 0000 */                     //  #define BOUND     $EC           ; lower boundary for adjustment
/* 0000 */                     //
/* 0000 */                     //  ; main program starts here
/* 0000 */                     //
/* 0110 */                     //         .ORG    $0110
/* 0110 */                     //
/* 0110 */ 0xD8,               //  START:  CLD
/* 0111 */ 0xA0, 0x00,         //          LDY     #0
/* 0113 */ 0xB1, 0xEA,         //          LDA     (POINT),Y       ; get op code
/* 0115 */ 0xA8,               //          TAY                     ; +cache in Y
/* 0116 */ 0xA2, 0x07,         //          LDX     #7
/* 0118 */ 0x98,               //  LOOP:   TYA                     ; restore op code
/* 0119 */ 0x3D, 0x8E, 0x01,   //          AND     TAB1-1,X        ; remove unwanted bits
/* 011C */ 0x5D, 0x95, 0x01,   //          EOR     TAB2-1,X        ; & test the rest
/* 011F */ 0xF0, 0x03,         //          BEQ     FOUND
/* 0121 */ 0xCA,               //          DEX                     ; on to the next test
/* 0122 */ 0xD0, 0xF4,         //          BNE     LOOP            ; ...if any
/* 0124 */ 0xBC, 0x9D, 0x01,   //  FOUND:  LDY     TAB3,X          ; length or flag
/* 0127 */ 0x30, 0x0D,         //          BMI     TRIP            ; triple length?
/* 0129 */ 0xF0, 0x22,         //          BEQ     BRAN            ; branch?
/* 012B */ 0xE6, 0xEA,         //  SKIP:   INC     POINT           ; moving right along..
/* 012D */ 0xD0, 0x02,         //          BNE     INEX            ; ..to next op code
/* 012F */ 0xE6, 0xEB,         //          INC     POINT+1
/* 0131 */ 0x88,               //  INEX:   DEY
/* 0132 */ 0xD0, 0xF7,         //          BNE     SKIP
/* 0134 */ 0xF0, 0xDA,         //          BEQ     START
/* 0136 */                     //
/* 0136 */                     //  ; length 3 or illegal
/* 0136 */                     //
/* 0136 */ 0xC8,               //  TRIP:   INY
/* 0137 */ 0x30, 0xD9,         //          BMI     START+2         ; illegal/end to BRK halt
/* 0139 */ 0xC8,               //          INY                     ; set Y to 1
/* 013A */ 0xB1, 0xEA,         //          LDA     (POINT),Y       ; lo-order operand
/* 013C */ 0xAA,               //          TAX                     ; ...into X reg
/* 013D */ 0xC8,               //          INY                     ; Y=2
/* 013E */ 0xB1, 0xEA,         //          LDA     (POINT),Y       ; hi-order operand
/* 0140 */ 0x20, 0x79, 0x01,   //          JSR     ADJUST          ; change address, maybe
/* 0143 */ 0x91, 0xEA,         //          STA     (POINT),Y       ; ...and put it back
/* 0145 */ 0x88,               //          DEY                     ; Y=1
/* 0146 */ 0x8A,               //          TXA
/* 0147 */ 0x91, 0xEA,         //          STA     (POINT),Y       ; ...also hi-order
/* 0149 */ 0xA0, 0x03,         //          LDY     #3              ; Y=3
/* 014B */ 0x10, 0xDE,         //          BPL     SKIP
/* 014D */                     //
/* 014D */                     //  ; branch: check "to" and "from" address
/* 014D */                     //
/* 014D */ 0xC8,               //  BRAN:   INY                     ; Y=1
/* 014E */ 0xA6, 0xEA,         //          LDX     POINT           ; "from" addrs lo-order
/* 0150 */ 0xA5, 0xEB,         //          LDA     POINT+1         ; ...& hi-order
/* 0152 */ 0x20, 0x79, 0x01,   //          JSR     ADJUST          ; change, maybe
/* 0155 */ 0x86, 0xE0,         //          STX     ALOC            ; save lo-order only
/* 0157 */ 0xA2, 0xFF,         //          LDX     #$FF            ; flag for "back" branches
/* 0159 */ 0xB1, 0xEA,         //          LDA     (POINT),Y       ; get relative branch
/* 015B */ 0x18,               //          CLC
/* 015C */ 0x69, 0x02,         //          ADC     #2              ; adjust the offset
/* 015E */ 0x30, 0x01,         //          BMI     OVER            ; backwards branch?
/* 0160 */ 0xE8,               //          INX                     ; nope
/* 0161 */ 0x86, 0xE3,         //  OVER:   STX     LIMIT
/* 0163 */ 0x18,               //          CLC
/* 0164 */ 0x65, 0xEA,         //          ADC     POINT           ; calculate "to" lo-order
/* 0166 */ 0xAA,               //          TAX                     ; ...and put in X
/* 0167 */ 0xA5, 0xE3,         //          LDA     LIMIT           ; 00 or FF
/* 0169 */ 0x65, 0xEB,         //          ADC     POINT+1         ; "to" hi-order
/* 016B */ 0x20, 0x79, 0x01,   //          JSR     ADJUST          ; change, maybe
/* 016E */ 0xCA,               //          DEX                     ; readjust the offset
/* 016F */ 0xCA,               //          DEX
/* 0170 */ 0x8A,               //          TXA
/* 0171 */ 0x38,               //          SEC
/* 0172 */ 0xE5, 0xE0,         //          SBC     ALOC            ; recalculate relative branch
/* 0174 */ 0x91, 0xEA,         //          STA     (POINT),Y       ; and re-insert
/* 0176 */ 0xC8,               //          INY                     ; Y=2
/* 0177 */ 0x10, 0xB2,         //          BPL     SKIP
/* 0179 */                     //
/* 0179 */                     //  ; examine address and adjust, maybe
/* 0179 */                     //
/* 0179 */ 0xC5, 0xE7,         //  ADJUST: CMP     PAGLIM
/* 017B */ 0xB0, 0x11,         //          BCS     OUT             ; too high?
/* 017D */ 0xC5, 0xED,         //          CMP     BOUND+1
/* 017F */ 0xD0, 0x02,         //          BNE     TES2            ; hi-order?
/* 0181 */ 0xE4, 0xEC,         //          CPX     BOUND           ; lo-order?
/* 0183 */ 0x90, 0x09,         //  TES2:   BCC     OUT             ; too low?
/* 0185 */ 0x48,               //          PHA                     ; stack hi-order
/* 0186 */ 0x8A,               //          TXA
/* 0187 */ 0x18,               //          CLC
/* 0188 */ 0x65, 0xE8,         //          ADC     ADJST           ; adjust lo-order
/* 018A */ 0xAA,               //          TAX
/* 018B */ 0x68,               //          PLA                     ; unstack hi-order
/* 018C */ 0x65, 0xE9,         //          ADC     ADJST+1         ; and adjust
/* 018E */ 0x60,               //  OUT:    RTS
/* 018F */                     //
/* 018F */                     //  ; tables for op-code indentification
/* 018F */                     //
/* 018F */ 0x0C, 0x1F, 0x0D, 0x87,  // TAB1: .BYTE $0C, $1F, $0D, $87, $1F, $FF, $03
/* 0193 */ 0x1F, 0xFF, 0x03,   //
/* 0196 */ 0x0C, 0x19, 0x08, 0x00,  // TAB2: .BYTE $0C, $19, $08, $00, $10, $20, $03
/* 019A */ 0x10, 0x20, 0x03,   //
/* 019D */ 0x02, 0xFF, 0xFF, 0x01,  // TAB3: .BYTE $02, $FF, $FF, $01, $01, $00, $FF, $FE
/* 01A1 */ 0x01, 0x00, 0xFF, 0xFE,  //
/* 01A5 */                     //
/* 01A5 */                     //      .END
};



// ============================================================================
// ============================================================================
// BRANCH (3rd of 3 utilities that will be copied into RAM)
// ============================================================================
// ============================================================================
// see http://users.telenet.be/kim1-6502/6502/fbok.html#p114
#if _TARGET == PERSONAL_COMPUTER
    unsigned char branchROM[42] = {
#else
    const unsigned char branchROM[42] PROGMEM = {
#endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  ;
/* 0000 */                     //  ;  Branche from the First Book Of KIM-1
/* 0000 */                     //  ;
/* 0000 */                     //  #define LAST      $F3
/* 0000 */                     //  #define INH       $F9
/* 0000 */                     //  #define POINTL    $FA
/* 0000 */                     //  #define POINTH    $FB
/* 0000 */                     //
/* 0000 */                     //  #define SCANDS    $1F1F
/* 0000 */                     //  #define GETKEY    $1F6A
/* 0000 */                     //
/* 0000 */                     //          .ORG    $0000
/* 0000 */                     //
/* 0000 */ 0xD8,               //  START:  CLD
/* 0001 */ 0x18,               //          CLC
/* 0002 */ 0xA5, 0xFA,         //          LDA     POINTL
/* 0004 */ 0xE5, 0xFB,         //          SBC     POINTH
/* 0006 */ 0x85, 0xF9,         //          STA     INH
/* 0008 */ 0xC6, 0xF9,         //          DEC     INH
/* 000A */ 0x20, 0x1F, 0x1F,   //          JSR     SCANDS
/* 000D */ 0x20, 0x6A, 0x1F,   //          JSR     GETKEY
/* 0010 */ 0xC5, 0xF3,         //          CMP     LAST
/* 0012 */ 0xF0, 0xEC,         //          BEQ     START
/* 0014 */ 0x85, 0xF3,         //          STA     LAST
/* 0016 */ 0xC9, 0x10,         //          CMP     #$10
/* 0018 */ 0xB0, 0xE6,         //          BCS     START
/* 001A */ 0x0A,               //          ASL     A
/* 001B */ 0x0A,               //          ASL     A
/* 001C */ 0x0A,               //          ASL     A
/* 001D */ 0x0A,               //          ASL     A
/* 001E */ 0xA2, 0x04,         //          LDX     #4
/* 0020 */ 0x0A,               //  ADDR:   ASL     A
/* 0021 */ 0x26, 0xFA,         //          ROL     POINTL
/* 0023 */ 0x26, 0xFB,         //          ROL     POINTH
/* 0025 */ 0xCA,               //          DEX
/* 0026 */ 0xD0, 0xF8,         //          BNE     ADDR
/* 0028 */ 0xF0, 0xD6,         //          BEQ     START
/* 002A */                     //
/* 002A */                     //      .END
};



// ============================================================================
// ============================================================================
// BAUM/WOZNIAK DISASSEMBLER
// ============================================================================
// ============================================================================
//
#ifdef _INCL_DISASM
  #if _TARGET == PERSONAL_COMPUTER
    unsigned char disasmROM[505] = {
  #else
    const unsigned char disasmROM[505] PROGMEM = {
  #endif
    0x20, 0x0F, 0x20, 0x20, 0x9E, 0x1E, 0x20, 0x9E, 0x1E, 0x20, 0x9E, 0x1E,
    0x4C, 0x64, 0x1C, 0xA9, 0x0D, 0x85, 0x02, 0x20, 0x21, 0x20, 0x20, 0xFC,
    0x20, 0x85, 0x00, 0x84, 0x01, 0xC6, 0x02, 0xD0, 0xF2, 0x20, 0xE2, 0x20,
    0xA1, 0x00, 0xA8, 0x4A, 0x90, 0x0B, 0x4A, 0xB0, 0x17, 0xC9, 0x22, 0xF0,
    0x13, 0x29, 0x07, 0x09, 0x80, 0x4A, 0xAA, 0xBD, 0x1B, 0x21, 0xB0, 0x04,
    0x4A, 0x4A, 0x4A, 0x4A, 0x29, 0x0F, 0xD0, 0x04, 0xA0, 0x80, 0xA9, 0x00,
    0xAA, 0xBD, 0x5F, 0x21, 0x85, 0x03, 0x29, 0x03, 0x85, 0x04, 0x98, 0x29,
    0x8F, 0xAA, 0x98, 0xA0, 0x03, 0xE0, 0x8A, 0xF0, 0x0B, 0x4A, 0x90, 0x08,
    0x4A, 0x4A, 0x09, 0x20, 0x88, 0xD0, 0xFA, 0xC8, 0x88, 0xD0, 0xF2, 0x48,
    0xB1, 0x00, 0x20, 0x13, 0x21, 0xA2, 0x01, 0x20, 0xF3, 0x20, 0xC4, 0x04,
    0xC8, 0x90, 0xF1, 0xA2, 0x03, 0xC0, 0x04, 0x90, 0xF2, 0x68, 0xA8, 0xB9,
    0x79, 0x21, 0x85, 0x05, 0xB9, 0xB9, 0x21, 0x85, 0x06, 0xA9, 0x00, 0xA0,
    0x05, 0x06, 0x06, 0x26, 0x05, 0x2A, 0x88, 0xD0, 0xF8, 0x69, 0x3F, 0x20,
    0x0B, 0x21, 0xCA, 0xD0, 0xEC, 0x20, 0xF1, 0x20, 0xA2, 0x06, 0xE0, 0x03,
    0xD0, 0x12, 0xA4, 0x04, 0xF0, 0x0E, 0xA5, 0x03, 0xC9, 0xE8, 0xB1, 0x00,
    0xB0, 0x1C, 0x20, 0x13, 0x21, 0x88, 0xD0, 0xF2, 0x06, 0x03, 0x90, 0x0E,
    0xBD, 0x6C, 0x21, 0x20, 0x0B, 0x21, 0xBD, 0x72, 0x21, 0xF0, 0x03, 0x20,
    0x0B, 0x21, 0xCA, 0xD0, 0xD5, 0x60, 0x20, 0xFF, 0x20, 0xAA, 0xE8, 0xD0,
    0x01, 0xC8, 0x98, 0x20, 0x13, 0x21, 0x8A, 0x4C, 0x13, 0x21, 0x20, 0x2F,
    0x1E, 0xA5, 0x01, 0xA6, 0x00, 0x20, 0xDB, 0x20, 0xA9, 0x2D, 0x20, 0x0B,
    0x21, 0xA2, 0x03, 0xA9, 0x20, 0x20, 0x0B, 0x21, 0xCA, 0xD0, 0xF8, 0x60,
    0xA5, 0x04, 0x38, 0xA4, 0x01, 0xAA, 0x10, 0x01, 0x88, 0x65, 0x00, 0x90,
    0x01, 0xC8, 0x60, 0x84, 0x07, 0x20, 0xA0, 0x1E, 0xA4, 0x07, 0x60, 0x84,
    0x07, 0x20, 0x3B, 0x1E, 0xA4, 0x07, 0x60, 0x40, 0x02, 0x45, 0x03, 0xD0,
    0x08, 0x40, 0x09, 0x30, 0x22, 0x45, 0x33, 0xD0, 0x08, 0x40, 0x09, 0x40,
    0x02, 0x45, 0x33, 0xD0, 0x08, 0x40, 0x09, 0x40, 0x02, 0x45, 0xB3, 0xD0,
    0x08, 0x40, 0x09, 0x00, 0x22, 0x44, 0x33, 0xD0, 0x8C, 0x44, 0x00, 0x11,
    0x22, 0x44, 0x33, 0xD0, 0x8C, 0x44, 0x9A, 0x10, 0x22, 0x44, 0x33, 0xD0,
    0x08, 0x40, 0x09, 0x10, 0x22, 0x44, 0x33, 0xD0, 0x08, 0x40, 0x09, 0x62,
    0x13, 0x78, 0xA9, 0x00, 0x21, 0x01, 0x02, 0x00, 0x80, 0x59, 0x4D, 0x11,
    0x12, 0x06, 0x4A, 0x05, 0x1D, 0x2C, 0x29, 0x2C, 0x23, 0x28, 0x41, 0x59,
    0x00, 0x58, 0x00, 0x00, 0x00, 0x1C, 0x8A, 0x1C, 0x23, 0x5D, 0x8B, 0x1B,
    0xA1, 0x9D, 0x8A, 0x1D, 0x23, 0x9D, 0x8B, 0x1D, 0xA1, 0x00, 0x29, 0x19,
    0xAE, 0x69, 0xA8, 0x19, 0x23, 0x24, 0x53, 0x1B, 0x23, 0x24, 0x53, 0x19,
    0xA1, 0x00, 0x1A, 0x5B, 0x5B, 0xA5, 0x69, 0x24, 0x24, 0xAE, 0xAE, 0xA8,
    0xAD, 0x29, 0x00, 0x7C, 0x00, 0x15, 0x9C, 0x6D, 0x9C, 0xA5, 0x69, 0x29,
    0x53, 0x84, 0x13, 0x34, 0x11, 0xA5, 0x69, 0x23, 0xA0, 0xD8, 0x62, 0x5A,
    0x48, 0x26, 0x62, 0x94, 0x88, 0x54, 0x44, 0xC8, 0x54, 0x68, 0x44, 0xE8,
    0x94, 0x00, 0xB4, 0x08, 0x84, 0x74, 0xB4, 0x28, 0x6E, 0x74, 0xF4, 0xCC,
    0x4A, 0x72, 0xF2, 0xA4, 0x8A, 0x00, 0xAA, 0xA2, 0xA2, 0x74, 0x74, 0x74,
    0x72, 0x44, 0x68, 0xB2, 0x32, 0xB2, 0x00, 0x22, 0x00, 0x1A, 0x1A, 0x26,
    0x26, 0x72, 0x72, 0x88, 0xC8, 0xC4, 0xCA, 0x26, 0x48, 0x44, 0x44, 0xA2,
    0xC8
};
#endif



// ============================================================================
// ============================================================================
// C.R. BOND NUMERICAL LIBRARY
// ============================================================================
// ============================================================================
// see 6502.org. Some additional bug patches are applied below.
// note: in source code, supporting emulator code is installed for CALC as well
#if _INCL_CALCULATOR == TRUE
  #if _TARGET == PERSONAL_COMPUTER
    unsigned char calcROM[8164] = {
  #else
    const unsigned char calcROM[8164] PROGMEM = {
  #endif
//  0xA2, 0xFF, 0x9A, 0xD8, 0x0A, 0x0A, 0x18, 0x69, 0x11, 0xAA, 0xA9, 0x50,
    0xA2, 0xFF, 0xEA, 0xD8, 0x0A, 0x0A, 0x18, 0x69, 0x11, 0xAA, 0xA9, 0x50,
//  0x69, 0x00, 0x48, 0x8A, 0x48, 0x60, 0x20, 0xB6, 0x50, 0x00, 0x20, 0xAE,
    0x69, 0x00, 0x48, 0x8A, 0x48, 0x60, 0x20, 0xB6, 0x50, 0x60, 0x20, 0xAE,
//  0x50, 0x00, 0x20, 0x78, 0x53, 0x00, 0x20, 0x35, 0x55, 0x00, 0x20, 0xE6,
    0x50, 0x60, 0x20, 0x78, 0x53, 0x60, 0x20, 0x35, 0x55, 0x60, 0x20, 0xE6,
//  0x56, 0x00, 0x20, 0x3E, 0x66, 0x00, 0x20, 0xE2, 0x66, 0x00, 0x20, 0x2B,
    0x56, 0x60, 0x20, 0x3E, 0x66, 0x60, 0x20, 0xE2, 0x66, 0x60, 0x20, 0x2B,
//  0x67, 0x00, 0x20, 0x54, 0x66, 0x00, 0x20, 0x66, 0x66, 0x00, 0x20, 0x0A,
    0x67, 0x60, 0x20, 0x54, 0x66, 0x60, 0x20, 0x66, 0x66, 0x60, 0x20, 0x0A,
//  0x59, 0x00, 0x20, 0x33, 0x59, 0x00, 0x20, 0x4C, 0x59, 0x00, 0x20, 0x91,
    0x59, 0x60, 0x20, 0x33, 0x59, 0x60, 0x20, 0x4C, 0x59, 0x60, 0x20, 0x91,
//  0x59, 0x00, 0x20, 0xB5, 0x58, 0x00, 0x20, 0x8C, 0x59, 0x00, 0x20, 0x96,
    0x59, 0x60, 0x20, 0xB5, 0x58, 0x60, 0x20, 0x8C, 0x59, 0x60, 0x20, 0x96,
//  0x5B, 0x00, 0x20, 0x7D, 0x5B, 0x00, 0x20, 0x2F, 0x5C, 0x00, 0x20, 0x4E,
    0x5B, 0x60, 0x20, 0x7D, 0x5B, 0x60, 0x20, 0x2F, 0x5C, 0x60, 0x20, 0x4E,
//  0x5E, 0x00, 0x20, 0x66, 0x5E, 0x00, 0x20, 0x7E, 0x5E, 0x00, 0x20, 0x96,
    0x5E, 0x60, 0x20, 0x66, 0x5E, 0x60, 0x20, 0x7E, 0x5E, 0x60, 0x20, 0x96,
//  0x5E, 0x00, 0x20, 0x35, 0x61, 0x00, 0x20, 0x13, 0x63, 0x00, 0x20, 0x54,
    0x5E, 0x60, 0x20, 0x35, 0x61, 0x60, 0x20, 0x13, 0x63, 0x60, 0x20, 0x54,
//  0x63, 0x00, 0x20, 0x8D, 0x63, 0x00, 0x20, 0x0D, 0x64, 0x00, 0x20, 0x25,
    0x63, 0x60, 0x20, 0x8D, 0x63, 0x60, 0x20, 0x0D, 0x64, 0x60, 0x20, 0x25,
//  0x64, 0x00, 0x20, 0x3D, 0x64, 0x00, 0x20, 0x55, 0x64, 0x00, 0x20, 0xAE,
    0x64, 0x60, 0x20, 0x3D, 0x64, 0x60, 0x20, 0x55, 0x64, 0x60, 0x20, 0xAE,
//  0x64, 0x00, 0x20, 0x07, 0x65, 0x00, 0x20, 0x74, 0x65, 0x00, 0x20, 0x95,
    0x64, 0x60, 0x20, 0x07, 0x65, 0x60, 0x20, 0x74, 0x65, 0x60, 0x20, 0x95,
//  0x65, 0x00, 0x20, 0xB6, 0x65, 0x00, 0x20, 0xD7, 0x65, 0x00, 0x20, 0xEF,
    0x65, 0x60, 0x20, 0xB6, 0x65, 0x60, 0x20, 0xD7, 0x65, 0x60, 0x20, 0xEF,
//  0x65, 0x00, 0x20, 0x07, 0x66, 0x00, 0xAD, 0x68, 0x03, 0x49, 0x80, 0x8D,
    0x65, 0x60, 0x20, 0x07, 0x66, 0x60, 0xAD, 0x68, 0x03, 0x49, 0x80, 0x8D,

    0x68, 0x03, 0xF8, 0xAD, 0x62, 0x03, 0xD0, 0x09, 0xA9, 0x68, 0xA0, 0x03,
    0xA2, 0x70, 0x4C, 0x30, 0x52, 0xAD, 0x6A, 0x03, 0xD0, 0x09, 0xA9, 0x60,
    0xA0, 0x03, 0xA2, 0x70, 0x4C, 0x30, 0x52, 0xAD, 0x60, 0x03, 0x4D, 0x68,
    0x03, 0x10, 0x6C, 0x2C, 0x60, 0x03, 0x70, 0x07, 0x2C, 0x68, 0x03, 0x70,
    0x0A, 0x50, 0x42, 0x2C, 0x68, 0x03, 0x70, 0x4E, 0x20, 0x38, 0x53, 0x20,
    0xA3, 0x51, 0xA5, 0xC0, 0xF0, 0x09, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x70,
    0x4C, 0x30, 0x52, 0xA5, 0xC1, 0xC9, 0x12, 0xB0, 0xF1, 0xA9, 0x00, 0x85,
    0xC2, 0x20, 0xF0, 0x51, 0x20, 0x5E, 0x52, 0xB0, 0x0D, 0x20, 0x38, 0x53,
    0xA9, 0x00, 0x85, 0xC2, 0x20, 0xF0, 0x51, 0x20, 0x5E, 0x52, 0xAD, 0x60,
    0x03, 0x49, 0x40, 0x85, 0xC0, 0x20, 0xAC, 0x52, 0x60, 0x20, 0xC2, 0x51,
    0xB0, 0x03, 0x20, 0x38, 0x53, 0xA5, 0xC0, 0xC9, 0x00, 0xD0, 0xBF, 0x4C,
    0xFF, 0x50, 0x20, 0xC2, 0x51, 0x90, 0xF2, 0x20, 0x38, 0x53, 0xA5, 0xC0,
    0x4C, 0x31, 0x51, 0x2C, 0x60, 0x03, 0x70, 0x07, 0x2C, 0x68, 0x03, 0x70,
    0x0A, 0x50, 0x34, 0x2C, 0x68, 0x03, 0x70, 0x40, 0x20, 0x38, 0x53, 0x20,
    0xA3, 0x51, 0xA5, 0xC0, 0xF0, 0x09, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x70,
    0x4C, 0x30, 0x52, 0xA5, 0xC1, 0xC9, 0x12, 0xB0, 0xF1, 0xA9, 0x00, 0x85,
    0xC2, 0x20, 0xF0, 0x51, 0x20, 0x44, 0x52, 0x20, 0x8C, 0x52, 0xAD, 0x60,
    0x03, 0x85, 0xC0, 0x20, 0xAC, 0x52, 0x60, 0x20, 0xC2, 0x51, 0xB0, 0x03,
    0x20, 0x38, 0x53, 0xA5, 0xC0, 0xC9, 0x00, 0xD0, 0xCD, 0x4C, 0x6B, 0x51,
    0x20, 0xC2, 0x51, 0x90, 0xF2, 0x20, 0x38, 0x53, 0x4C, 0x8F, 0x51, 0xAD,
    0x60, 0x03, 0x29, 0x0F, 0x85, 0xC0, 0xAD, 0x68, 0x03, 0x29, 0x0F, 0x85,
    0xC1, 0x18, 0xAD, 0x61, 0x03, 0x6D, 0x69, 0x03, 0xAA, 0xA5, 0xC0, 0x65,
    0xC1, 0x85, 0xC0, 0x86, 0xC1, 0x60, 0xAD, 0x60, 0x03, 0x29, 0x0F, 0x85,
    0xC0, 0xAD, 0x68, 0x03, 0x29, 0x0F, 0x85, 0xC1, 0x38, 0xAD, 0x61, 0x03,
    0xED, 0x69, 0x03, 0xAA, 0xA5, 0xC0, 0xE5, 0xC1, 0xB0, 0x0D, 0x38, 0xAD,
    0x69, 0x03, 0xED, 0x61, 0x03, 0xAA, 0xA5, 0xC1, 0xE5, 0xC0, 0x18, 0x85,
    0xC0, 0x86, 0xC1, 0x60, 0xA0, 0x1F, 0xA9, 0x00, 0x99, 0x00, 0x03, 0x88,
    0x10, 0xFA, 0x18, 0xA5, 0xC1, 0x69, 0x11, 0xA8, 0xB9, 0x94, 0x6A, 0x4A,
    0xAA, 0xA0, 0x05, 0xB9, 0x62, 0x03, 0x99, 0x02, 0x03, 0xB9, 0x6A, 0x03,
    0x9D, 0x12, 0x03, 0xCA, 0x88, 0x10, 0xF0, 0xA5, 0xC1, 0x4A, 0x90, 0x13,
    0xA2, 0x0D, 0xA9, 0x00, 0xBC, 0x12, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x12,
    0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x60, 0x84, 0xF3, 0x85, 0xF2,
    0x86, 0xF4, 0xA9, 0x03, 0x85, 0xF5, 0xA0, 0x07, 0xB1, 0xF2, 0x91, 0xF4,
    0x88, 0x10, 0xF9, 0x60, 0xA2, 0x06, 0x18, 0xBD, 0x01, 0x03, 0x7D, 0x11,
    0x03, 0x9D, 0x11, 0x03, 0xCA, 0x10, 0xF4, 0xAD, 0x11, 0x03, 0xF0, 0x05,
    0x20, 0x5F, 0x53, 0xE6, 0xC2, 0x60, 0xA2, 0x08, 0x38, 0xBD, 0x01, 0x03,
    0xFD, 0x11, 0x03, 0x9D, 0x11, 0x03, 0xCA, 0x10, 0xF4, 0x90, 0x1C, 0xA9,
    0x0C, 0x85, 0xC0, 0xAD, 0x12, 0x03, 0x29, 0xF0, 0xD0, 0x09, 0x20, 0x4B,
    0x53, 0xE6, 0xC2, 0xC6, 0xC0, 0xD0, 0xF0, 0xA4, 0xC2, 0xB9, 0x34, 0x6B,
    0x85, 0xC2, 0x38, 0x60, 0xA2, 0x07, 0xA9, 0x50, 0x18, 0x7D, 0x11, 0x03,
    0x9D, 0x11, 0x03, 0xA9, 0x00, 0xCA, 0x10, 0xF5, 0xAD, 0x11, 0x03, 0xF0,
    0x0A, 0x20, 0x5F, 0x53, 0x18, 0xA5, 0xC2, 0x69, 0x01, 0x85, 0xC2, 0x60,
    0xA2, 0x05, 0x18, 0xBD, 0x12, 0x03, 0x9D, 0x72, 0x03, 0xCA, 0x10, 0xF7,
    0xAD, 0x72, 0x03, 0xD0, 0x07, 0x8D, 0x71, 0x03, 0x8D, 0x70, 0x03, 0x60,
    0xAD, 0x61, 0x03, 0x8D, 0x71, 0x03, 0xAD, 0x60, 0x03, 0x29, 0x0F, 0x8D,
    0x70, 0x03, 0x24, 0xC0, 0x50, 0x2F, 0x38, 0xAD, 0x71, 0x03, 0xE5, 0xC2,
    0xAA, 0xAD, 0x70, 0x03, 0xE9, 0x00, 0xB0, 0x2D, 0x38, 0xA5, 0xC2, 0xED,
    0x71, 0x03, 0x8D, 0x71, 0x03, 0xA9, 0x00, 0xED, 0x70, 0x03, 0x8D, 0x70,
    0x03, 0xAD, 0x60, 0x03, 0x29, 0xC0, 0x49, 0x40, 0x0D, 0x70, 0x03, 0x29,
    0xCF, 0x8D, 0x70, 0x03, 0x60, 0x18, 0xAD, 0x61, 0x03, 0x65, 0xC2, 0xAA,
    0xAD, 0x60, 0x03, 0x69, 0x00, 0x8D, 0x70, 0x03, 0x8E, 0x71, 0x03, 0x0D,
    0x71, 0x03, 0xD0, 0x0E, 0xAD, 0x60, 0x03, 0x29, 0x80, 0x0D, 0x70, 0x03,
    0x29, 0xCF, 0x8D, 0x70, 0x03, 0x60, 0xAD, 0x60, 0x03, 0x29, 0xC0, 0x0D,
    0x70, 0x03, 0x29, 0xCF, 0x8D, 0x70, 0x03, 0x60, 0xA0, 0x07, 0xBE, 0x60,
    0x03, 0xB9, 0x68, 0x03, 0x99, 0x60, 0x03, 0x8A, 0x99, 0x68, 0x03, 0x88,
    0x10, 0xF0, 0x60, 0xA2, 0x07, 0xA9, 0x00, 0xBC, 0x11, 0x03, 0x19, 0x54,
    0x69, 0x9D, 0x11, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x60, 0xA2,
    0x07, 0xA9, 0x00, 0xBC, 0x11, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x12, 0x03,
    0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0xA9, 0x00, 0x8D, 0x11, 0x03, 0x60,
    0xF8, 0xA2, 0x2F, 0xA9, 0x00, 0x9D, 0x00, 0x03, 0xCA, 0x10, 0xFA, 0x20,
    0xE2, 0x53, 0xAD, 0x60, 0x03, 0x4D, 0x68, 0x03, 0x29, 0x40, 0xF0, 0x0F,
    0x20, 0xC2, 0x51, 0xB0, 0x05, 0xAD, 0x68, 0x03, 0x90, 0x0B, 0xAD, 0x60,
    0x03, 0xB0, 0x06, 0x20, 0xA3, 0x51, 0xAD, 0x60, 0x03, 0x29, 0x40, 0x8D,
    0x70, 0x03, 0xA5, 0xC2, 0xF0, 0x11, 0xAD, 0x70, 0x03, 0xD0, 0x06, 0x20,
    0x19, 0x55, 0x4C, 0xBF, 0x53, 0x20, 0x27, 0x55, 0x4C, 0xBF, 0x53, 0xA5,
    0xC1, 0x8D, 0x71, 0x03, 0x05, 0xC0, 0xD0, 0x03, 0x8D, 0x70, 0x03, 0xA5,
    0xC0, 0x0D, 0x70, 0x03, 0x8D, 0x70, 0x03, 0xAD, 0x60, 0x03, 0x4D, 0x68,
    0x03, 0x29, 0x80, 0x0D, 0x70, 0x03, 0x8D, 0x70, 0x03, 0x60, 0xAD, 0x62,
    0x03, 0xF0, 0x18, 0xAD, 0x6A, 0x03, 0xF0, 0x13, 0x20, 0x4C, 0x54, 0x20,
    0x0A, 0x54, 0x20, 0x5E, 0x54, 0x20, 0xD5, 0x54, 0x20, 0x9B, 0x54, 0x20,
    0xE9, 0x54, 0x60, 0xA2, 0x07, 0xA9, 0x00, 0x9D, 0x70, 0x03, 0xCA, 0x10,
    0xF8, 0x60, 0xA9, 0x00, 0x85, 0xF0, 0xA9, 0x01, 0x85, 0xF1, 0x85, 0xF3,
    0x85, 0xF5, 0xA9, 0x10, 0x85, 0xF2, 0xA9, 0x20, 0x85, 0xF4, 0xA2, 0x07,
    0xBD, 0x09, 0x03, 0x9D, 0x10, 0x01, 0xCA, 0x10, 0xF7, 0xA2, 0x08, 0xA0,
    0x07, 0x18, 0xB1, 0xF2, 0x79, 0x10, 0x01, 0x91, 0xF4, 0x88, 0x10, 0xF6,
    0xD8, 0x18, 0xA5, 0xF2, 0x69, 0x10, 0x85, 0xF2, 0x18, 0xA5, 0xF4, 0x69,
    0x10, 0x85, 0xF4, 0xF8, 0xCA, 0xD0, 0xE0, 0x60, 0xA2, 0x05, 0xBD, 0x62,
    0x03, 0x9D, 0x0A, 0x03, 0xBD, 0x6A, 0x03, 0x9D, 0x1A, 0x03, 0xCA, 0x10,
    0xF1, 0x60, 0xA9, 0x03, 0x85, 0xF3, 0x85, 0xF5, 0xA9, 0x18, 0x85, 0xF2,
    0xA9, 0x28, 0x85, 0xF4, 0xA0, 0x07, 0xB1, 0xF2, 0x29, 0xF0, 0xF0, 0x09,
    0x85, 0xF0, 0xB1, 0xF0, 0x91, 0xF4, 0x88, 0x10, 0xF9, 0xA2, 0x06, 0xC6,
    0xF2, 0xC6, 0xF4, 0xA0, 0x07, 0xB1, 0xF2, 0x29, 0xF0, 0xF0, 0x0C, 0x85,
    0xF0, 0x18, 0xB1, 0xF0, 0x71, 0xF4, 0x91, 0xF4, 0x88, 0x10, 0xF7, 0xCA,
    0x10, 0xE5, 0x60, 0xA9, 0x19, 0x85, 0xF2, 0xA9, 0x29, 0x85, 0xF4, 0xA2,
    0x06, 0xC6, 0xF2, 0xC6, 0xF4, 0xA0, 0x07, 0xB1, 0xF2, 0x29, 0x0F, 0xF0,
    0x20, 0x0A, 0x0A, 0x0A, 0x0A, 0x85, 0xF0, 0x18, 0xB1, 0xF0, 0x71, 0xF4,
    0x91, 0xF4, 0x88, 0x10, 0xF7, 0x90, 0x0E, 0xA4, 0xF4, 0x88, 0xB9, 0x00,
    0x03, 0x69, 0x00, 0x99, 0x00, 0x03, 0x88, 0xB0, 0xF5, 0xCA, 0x10, 0xD1,
    0x60, 0xA2, 0x0F, 0xA9, 0x00, 0xBC, 0x20, 0x03, 0x19, 0x54, 0x69, 0x9D,
    0x20, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x60, 0xA9, 0x01, 0x85,
    0xC2, 0xAD, 0x23, 0x03, 0x29, 0xF0, 0xD0, 0x05, 0x20, 0x05, 0x55, 0xC6,
    0xC2, 0xA2, 0x05, 0xBD, 0x23, 0x03, 0x9D, 0x72, 0x03, 0xCA, 0x10, 0xF7,
    0x60, 0xA2, 0x06, 0xA9, 0x00, 0xBC, 0x22, 0x03, 0x19, 0x54, 0x69, 0x9D,
    0x22, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x60, 0x18, 0xA5, 0xC1,
    0x69, 0x01, 0x85, 0xC1, 0xA5, 0xC0, 0x69, 0x00, 0x85, 0xC0, 0x60, 0x38,
    0xA5, 0xC1, 0xE9, 0x01, 0x85, 0xC1, 0xA5, 0xC0, 0xE9, 0x00, 0x85, 0xC0,
    0x60, 0xF8, 0xAD, 0x6A, 0x03, 0xD0, 0x03, 0x4C, 0x47, 0x69, 0xAD, 0x62,
    0x03, 0xD0, 0x0B, 0xA9, 0x00, 0xA0, 0x07, 0x99, 0x70, 0x03, 0x88, 0x10,
    0xFA, 0x60, 0xA9, 0x00, 0xA2, 0x2F, 0x9D, 0x00, 0x03, 0xCA, 0x10, 0xFA,
    0xA2, 0x05, 0xBD, 0x6A, 0x03, 0x9D, 0x0A, 0x03, 0xBD, 0x62, 0x03, 0x9D,
    0x12, 0x03, 0xCA, 0x10, 0xF1, 0x20, 0x0A, 0x54, 0x20, 0xD2, 0x56, 0xA9,
    0x0F, 0x85, 0xC4, 0x20, 0x13, 0x56, 0xC6, 0xC4, 0xD0, 0xF9, 0xA9, 0x01,
    0x85, 0xC2, 0xAD, 0x18, 0x03, 0xF0, 0x12, 0x20, 0xD2, 0x56, 0xC6, 0xC2,
    0xA2, 0x05, 0xBD, 0x18, 0x03, 0x9D, 0x72, 0x03, 0xCA, 0x10, 0xF7, 0x30,
    0x0B, 0xA2, 0x05, 0xBD, 0x19, 0x03, 0x9D, 0x72, 0x03, 0xCA, 0x10, 0xF7,
    0xAD, 0x60, 0x03, 0x4D, 0x68, 0x03, 0x29, 0x40, 0xF0, 0x09, 0x20, 0xA3,
    0x51, 0xAD, 0x60, 0x03, 0x4C, 0xBD, 0x55, 0x20, 0xC2, 0x51, 0xAD, 0x68,
    0x03, 0xB0, 0x02, 0x49, 0x40, 0x29, 0x40, 0x8D, 0x70, 0x03, 0x2C, 0x70,
    0x03, 0x70, 0x19, 0xA5, 0xC2, 0xF0, 0x22, 0xA5, 0xC1, 0x05, 0xC0, 0xD0,
    0x19, 0xA9, 0x01, 0x85, 0xC1, 0xA9, 0x40, 0x0D, 0x70, 0x03, 0x8D, 0x70,
    0x03, 0x4C, 0xED, 0x55, 0xA5, 0xC2, 0xF0, 0x09, 0x20, 0x19, 0x55, 0x4C,
    0xED, 0x55, 0x20, 0x27, 0x55, 0xA5, 0xC1, 0x8D, 0x71, 0x03, 0xA5, 0xC0,
    0x0D, 0x70, 0x03, 0x8D, 0x70, 0x03, 0x29, 0x0F, 0x0D, 0x71, 0x03, 0xD0,
    0x03, 0x8D, 0x70, 0x03, 0xAD, 0x60, 0x03, 0x4D, 0x68, 0x03, 0x29, 0x80,
    0x0D, 0x70, 0x03, 0x8D, 0x70, 0x03, 0x60, 0xA9, 0x09, 0x85, 0xC3, 0xAD,
    0x10, 0x03, 0xAE, 0x11, 0x03, 0xCD, 0x90, 0x01, 0x90, 0x07, 0xD0, 0x7D,
    0xEC, 0x91, 0x01, 0xB0, 0x78, 0xC6, 0xC3, 0xCD, 0x80, 0x01, 0x90, 0x07,
    0xD0, 0x6F, 0xEC, 0x81, 0x01, 0xB0, 0x6A, 0xC6, 0xC3, 0xCD, 0x70, 0x01,
    0x90, 0x07, 0xD0, 0x61, 0xEC, 0x71, 0x01, 0xB0, 0x5C, 0xC6, 0xC3, 0xCD,
    0x60, 0x01, 0x90, 0x07, 0xD0, 0x53, 0xEC, 0x61, 0x01, 0xB0, 0x4E, 0xC6,
    0xC3, 0xCD, 0x50, 0x01, 0x90, 0x07, 0xD0, 0x45, 0xEC, 0x51, 0x01, 0xB0,
    0x40, 0xC6, 0xC3, 0xCD, 0x40, 0x01, 0x90, 0x07, 0xD0, 0x37, 0xEC, 0x41,
    0x01, 0xB0, 0x32, 0xC6, 0xC3, 0xCD, 0x30, 0x01, 0x90, 0x07, 0xD0, 0x29,
    0xEC, 0x31, 0x01, 0xB0, 0x24, 0xC6, 0xC3, 0xCD, 0x20, 0x01, 0x90, 0x07,
    0xD0, 0x1B, 0xEC, 0x21, 0x01, 0xB0, 0x16, 0xC6, 0xC3, 0xCD, 0x10, 0x01,
    0x90, 0x07, 0xD0, 0x0D, 0xEC, 0x11, 0x01, 0xB0, 0x08, 0xC6, 0xC3, 0x2D,
    0x1F, 0x03, 0x4C, 0xD2, 0x56, 0xAD, 0x1F, 0x03, 0x29, 0xF0, 0x05, 0xC3,
    0x8D, 0x1F, 0x03, 0x0A, 0x0A, 0x0A, 0x0A, 0x85, 0xF2, 0xA0, 0x07, 0x38,
    0xB9, 0x10, 0x03, 0xF1, 0xF2, 0x99, 0x10, 0x03, 0x88, 0x10, 0xF5, 0xB0,
    0x11, 0xCE, 0x1F, 0x03, 0xA0, 0x07, 0xB9, 0x10, 0x03, 0x79, 0x10, 0x01,
    0x99, 0x10, 0x03, 0x88, 0x10, 0xF4, 0xA2, 0x0F, 0xA9, 0x00, 0xBC, 0x10,
    0x03, 0x19, 0x54, 0x69, 0x9D, 0x10, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10,
    0xF1, 0x60, 0xF8, 0xAD, 0x62, 0x03, 0xD0, 0x0B, 0xA2, 0x07, 0xA9, 0x00,
    0x9D, 0x70, 0x03, 0xCA, 0x10, 0xFA, 0x60, 0x2C, 0x60, 0x03, 0x10, 0x03,
    0x4C, 0x23, 0x69, 0xA2, 0x0F, 0xA9, 0x00, 0x9D, 0x00, 0x03, 0xCA, 0x10,
    0xFA, 0xA2, 0x07, 0xBD, 0x62, 0x03, 0x9D, 0x02, 0x03, 0xCA, 0x10, 0xF7,
    0xAD, 0x61, 0x03, 0x29, 0x01, 0xD0, 0x13, 0xA2, 0x07, 0xA9, 0x00, 0xBC,
    0x00, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x01, 0x03, 0xB9, 0xF4, 0x69, 0xCA,
    0x10, 0xF1, 0xA2, 0x07, 0x18, 0xBD, 0x01, 0x03, 0x7D, 0x01, 0x03, 0x9D,
    0x11, 0x03, 0xCA, 0x10, 0xF4, 0xA2, 0x07, 0x18, 0xBD, 0x11, 0x03, 0x7D,
    0x11, 0x03, 0x9D, 0x11, 0x03, 0xCA, 0x10, 0xF4, 0xA2, 0x07, 0x18, 0xBD,
    0x01, 0x03, 0x7D, 0x11, 0x03, 0x9D, 0x01, 0x03, 0xCA, 0x10, 0xF4, 0xA9,
    0x01, 0x85, 0xF2, 0xA9, 0x00, 0x8D, 0x10, 0x03, 0xA9, 0x06, 0x85, 0xC3,
    0xA6, 0xF2, 0xA9, 0x05, 0x9D, 0x10, 0x03, 0x20, 0xE4, 0x57, 0xC6, 0xC3,
    0x30, 0x06, 0x20, 0x31, 0x58, 0x4C, 0x68, 0x57, 0xA2, 0x07, 0xA9, 0x00,
    0xBC, 0x11, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x11, 0x03, 0xB9, 0xF4, 0x69,
    0xCA, 0x10, 0xF1, 0xA2, 0x05, 0xBD, 0x11, 0x03, 0x9D, 0x72, 0x03, 0xCA,
    0x10, 0xF7, 0xAD, 0x61, 0x03, 0x8D, 0x71, 0x03, 0xAD, 0x60, 0x03, 0x29,
    0x0F, 0x8D, 0x70, 0x03, 0x4E, 0x70, 0x03, 0x6E, 0x71, 0x03, 0xAC, 0x70,
    0x03, 0xB9, 0x98, 0x6B, 0x8D, 0x70, 0x03, 0xAC, 0x71, 0x03, 0xB9, 0x98,
    0x6B, 0x8D, 0x71, 0x03, 0x2C, 0x60, 0x03, 0x50, 0x1E, 0xAD, 0x61, 0x03,
    0x29, 0x01, 0xF0, 0x0F, 0x18, 0x6D, 0x71, 0x03, 0x8D, 0x71, 0x03, 0xAD,
    0x70, 0x03, 0x69, 0x00, 0x8D, 0x70, 0x03, 0xA9, 0x40, 0x0D, 0x70, 0x03,
    0x8D, 0x70, 0x03, 0x60, 0xA6, 0xF2, 0x38, 0xBD, 0x00, 0x03, 0xFD, 0x10,
    0x03, 0x9D, 0x00, 0x03, 0xCA, 0x10, 0xF4, 0x90, 0x0D, 0xA6, 0xF2, 0x18,
    0xBD, 0x10, 0x03, 0x69, 0x10, 0x9D, 0x10, 0x03, 0x90, 0xE2, 0xA6, 0xF2,
    0xBD, 0x10, 0x03, 0x29, 0xF0, 0x09, 0x09, 0x9D, 0x10, 0x03, 0xA9, 0x50,
    0x9D, 0x11, 0x03, 0x20, 0x31, 0x58, 0xE6, 0xF2, 0xA6, 0xF2, 0x18, 0xBD,
    0x00, 0x03, 0x7D, 0x10, 0x03, 0x9D, 0x00, 0x03, 0xCA, 0x10, 0xF4, 0xB0,
    0x07, 0xA6, 0xF2, 0xDE, 0x0F, 0x03, 0x90, 0xE8, 0x60, 0xA2, 0x0F, 0xA9,
    0x00, 0xBC, 0x00, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x00, 0x03, 0xB9, 0xF4,
    0x69, 0xCA, 0x10, 0xF1, 0x60, 0xF8, 0xC6, 0xC6, 0xA2, 0x07, 0xBD, 0x88,
    0x03, 0x9D, 0x60, 0x03, 0x9D, 0x68, 0x03, 0xCA, 0x10, 0xF4, 0x20, 0x78,
    0x53, 0xA2, 0x07, 0xBD, 0x70, 0x03, 0x9D, 0x88, 0x03, 0xCA, 0x10, 0xF7,
    0xA0, 0x07, 0xB1, 0xF8, 0x99, 0x70, 0x03, 0x88, 0x10, 0xF8, 0x20, 0x9D,
    0x58, 0x20, 0xA9, 0x58, 0x20, 0x78, 0x53, 0xD8, 0x18, 0xA5, 0xF8, 0x69,
    0x08, 0x85, 0xF8, 0x90, 0x02, 0xE6, 0xF9, 0xF8, 0x20, 0x92, 0x58, 0x20,
    0xA9, 0x58, 0x20, 0xB6, 0x50, 0xC6, 0xC6, 0xD0, 0xDD, 0x60, 0xA0, 0x07,
    0xB1, 0xF8, 0x99, 0x60, 0x03, 0x88, 0x10, 0xF8, 0x60, 0xA2, 0x07, 0xBD,
    0x88, 0x03, 0x9D, 0x60, 0x03, 0xCA, 0x10, 0xF7, 0x60, 0xA2, 0x07, 0xBD,
    0x70, 0x03, 0x9D, 0x68, 0x03, 0xCA, 0x10, 0xF7, 0x60, 0x20, 0x4C, 0x59,
    0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x78, 0x20, 0x30, 0x52, 0xA9, 0x70, 0xA0,
    0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68,
    0x20, 0x30, 0x52, 0x20, 0x78, 0x53, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68,
    0x20, 0x30, 0x52, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52,
    0x20, 0xB6, 0x50, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52,
    0x20, 0xE6, 0x56, 0xA5, 0xC9, 0xC9, 0x01, 0xF0, 0x0C, 0xC9, 0x04, 0xF0,
    0x08, 0xAD, 0x70, 0x03, 0x09, 0x80, 0x8D, 0x70, 0x03, 0x60, 0x20, 0xB5,
    0x58, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9, 0x78,
    0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0xAD, 0x70,
    0x03, 0x29, 0x7F, 0xA6, 0xC9, 0xE0, 0x03, 0x90, 0x02, 0x09, 0x80, 0x8D,
    0x70, 0x03, 0x60, 0x20, 0xB5, 0x58, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68,
    0x20, 0x30, 0x52, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52,
    0x20, 0x35, 0x55, 0x60, 0xA9, 0x00, 0x85, 0xCA, 0xF8, 0xA9, 0x00, 0x85,
    0xC8, 0x20, 0x57, 0x67, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30,
    0x52, 0xAD, 0x72, 0x03, 0xD0, 0x01, 0x60, 0x20, 0xA9, 0x59, 0xAD, 0x70,
    0x03, 0x05, 0xC8, 0x8D, 0x70, 0x03, 0xA5, 0xCA, 0xF0, 0x15, 0xA9, 0xA0,
    0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9, 0x70, 0xA0, 0x03, 0xA2,
    0x68, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0x60, 0xA9, 0x01, 0x4C, 0x4E,
    0x59, 0x20, 0x0A, 0x59, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30,
    0x52, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x4C, 0x35,
    0x55, 0xAD, 0x62, 0x03, 0xD0, 0x0B, 0xA9, 0x00, 0xA2, 0x07, 0x9D, 0x70,
    0x03, 0xCA, 0x10, 0xFA, 0x60, 0x2C, 0x60, 0x03, 0x50, 0x1A, 0xAD, 0x60,
    0x03, 0x29, 0x0F, 0xD0, 0x07, 0xAD, 0x61, 0x03, 0xC9, 0x08, 0x90, 0x0C,
    0xA2, 0x07, 0xBD, 0x60, 0x03, 0x9D, 0x70, 0x03, 0xCA, 0x10, 0xF7, 0x60,
    0xA2, 0x2F, 0xA9, 0x00, 0x9D, 0x00, 0x03, 0xCA, 0x10, 0xFA, 0xA9, 0x48,
    0x85, 0xF2, 0xA9, 0x6E, 0x85, 0xF3, 0xAD, 0x61, 0x03, 0x4A, 0xD8, 0x69,
    0x05, 0xF8, 0xAA, 0xA0, 0x05, 0xB9, 0x62, 0x03, 0x9D, 0x02, 0x03, 0xCA,
    0x88, 0x10, 0xF6, 0xAD, 0x61, 0x03, 0x29, 0x01, 0xF0, 0x13, 0xA2, 0x0C,
    0xA9, 0x00, 0xBC, 0x02, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x02, 0x03, 0xB9,
    0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x20, 0x24, 0x5B, 0x20, 0xC2, 0x5A, 0xA9,
    0x00, 0x8D, 0x60, 0x03, 0x8D, 0x61, 0x03, 0xA9, 0x08, 0x85, 0xC7, 0xAD,
    0x02, 0x03, 0x29, 0xF0, 0xD0, 0x2A, 0xA9, 0x40, 0x8D, 0x60, 0x03, 0xEE,
    0x61, 0x03, 0xA9, 0x00, 0xA2, 0x0C, 0xBC, 0x02, 0x03, 0x19, 0x54, 0x69,
    0x9D, 0x02, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0xC6, 0xC7, 0xD0,
    0xDA, 0xA9, 0x00, 0xA2, 0x07, 0x9D, 0x70, 0x03, 0xCA, 0x10, 0xFA, 0x60,
    0xA2, 0x05, 0xBD, 0x02, 0x03, 0x9D, 0x62, 0x03, 0xCA, 0x10, 0xF7, 0xAD,
    0x11, 0x03, 0xF0, 0x0C, 0xA2, 0x07, 0xBD, 0x60, 0x03, 0x9D, 0x70, 0x03,
    0xCA, 0x10, 0xF7, 0x60, 0xA9, 0x08, 0x85, 0xC7, 0xA9, 0x40, 0x8D, 0x68,
    0x03, 0xA9, 0x00, 0x8D, 0x69, 0x03, 0xAD, 0x12, 0x03, 0x29, 0xF0, 0xD0,
    0x26, 0xEE, 0x69, 0x03, 0xA9, 0x00, 0xA2, 0x0A, 0xBC, 0x12, 0x03, 0x19,
    0x54, 0x69, 0x9D, 0x12, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0xC6,
    0xC7, 0xD0, 0xDF, 0xA2, 0x07, 0xBD, 0x80, 0x6D, 0x9D, 0x70, 0x03, 0xCA,
    0x10, 0xF7, 0x60, 0xA2, 0x05, 0xBD, 0x12, 0x03, 0x9D, 0x6A, 0x03, 0xCA,
    0x10, 0xF7, 0x20, 0x35, 0x55, 0x60, 0xA2, 0x10, 0x8E, 0x12, 0x03, 0xA2,
    0x0A, 0xBD, 0x02, 0x03, 0x9D, 0x22, 0x03, 0xBD, 0x12, 0x03, 0x9D, 0x32,
    0x03, 0xCA, 0x10, 0xF1, 0xA9, 0x00, 0x85, 0xC5, 0xA6, 0xC5, 0xDE, 0x40,
    0x03, 0x30, 0x38, 0xF8, 0xA2, 0x0A, 0xBD, 0x02, 0x03, 0x9D, 0x22, 0x03,
    0xBD, 0x12, 0x03, 0x9D, 0x32, 0x03, 0xCA, 0x10, 0xF1, 0xA5, 0xC5, 0x20,
    0x87, 0x66, 0xA2, 0x0A, 0x38, 0xBD, 0x12, 0x03, 0xFD, 0x22, 0x03, 0x9D,
    0x12, 0x03, 0xCA, 0x10, 0xF4, 0xA2, 0x0A, 0x18, 0xBD, 0x02, 0x03, 0x7D,
    0x32, 0x03, 0x9D, 0x02, 0x03, 0xCA, 0x10, 0xF4, 0x4C, 0xDC, 0x5A, 0xE6,
    0xC5, 0xA9, 0x05, 0xC5, 0xC5, 0xB0, 0xB9, 0x60, 0xA9, 0x05, 0x85, 0xC5,
    0xA2, 0x00, 0x8E, 0x40, 0x03, 0xA0, 0x07, 0x38, 0xB9, 0x02, 0x03, 0xF1,
    0xF2, 0x99, 0x02, 0x03, 0x88, 0x10, 0xF5, 0x90, 0x06, 0xFE, 0x40, 0x03,
    0x4C, 0x2D, 0x5B, 0xA0, 0x07, 0x18, 0xB9, 0x02, 0x03, 0x71, 0xF2, 0x99,
    0x02, 0x03, 0x88, 0x10, 0xF5, 0xE8, 0xA9, 0x00, 0x9D, 0x40, 0x03, 0xD8,
    0x18, 0xA5, 0xF2, 0x69, 0x08, 0x85, 0xF2, 0x90, 0x02, 0xE6, 0xF3, 0xF8,
    0xC6, 0xC5, 0x10, 0xC5, 0x60, 0xA2, 0x09, 0xA9, 0x00, 0x9D, 0x30, 0x03,
    0xCA, 0x10, 0xFA, 0xA0, 0x07, 0xB1, 0xF4, 0x91, 0xF6, 0x88, 0x10, 0xF9,
    0x60, 0x20, 0x96, 0x5B, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30,
    0x52, 0xA9, 0x80, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0xAE,
    0x50, 0x60, 0xAD, 0x62, 0x03, 0xD0, 0x03, 0x4C, 0x19, 0x69, 0xAD, 0x60,
    0x03, 0x29, 0x80, 0x85, 0xCC, 0xA9, 0x60, 0x29, 0x7F, 0x8D, 0x60, 0x03,
    0xA9, 0x60, 0x85, 0xF2, 0xA9, 0x03, 0x85, 0xF3, 0xA9, 0x90, 0x85, 0xF4,
    0xA9, 0x03, 0x85, 0xF5, 0x20, 0x04, 0x69, 0xA9, 0x60, 0xA0, 0x03, 0xA2,
    0x68, 0x20, 0x30, 0x52, 0x20, 0x78, 0x53, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2,
    0x60, 0x20, 0x30, 0x52, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30,
    0x52, 0x20, 0xAE, 0x50, 0xAD, 0x72, 0x03, 0xF0, 0x08, 0xAD, 0x70, 0x03,
    0x10, 0x0F, 0x4C, 0x2F, 0x69, 0xA9, 0x80, 0xA0, 0x6D, 0xA2, 0x70, 0x20,
    0x30, 0x52, 0x4C, 0x26, 0x5C, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20,
    0x30, 0x52, 0x20, 0xE6, 0x56, 0xA9, 0x90, 0xA0, 0x03, 0xA2, 0x60, 0x20,
    0x30, 0x52, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20,
    0x35, 0x55, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20,
    0x2F, 0x5C, 0xAD, 0x70, 0x03, 0x05, 0xCC, 0x8D, 0x70, 0x03, 0x60, 0xF8,
    0xA9, 0x00, 0x85, 0xCA, 0x85, 0xC8, 0x2C, 0x60, 0x03, 0x10, 0x0C, 0xA9,
    0x80, 0x85, 0xC8, 0xA9, 0x4F, 0x2D, 0x60, 0x03, 0x8D, 0x60, 0x03, 0x70,
    0x0E, 0x20, 0x3E, 0x66, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30,
    0x52, 0xE6, 0xCA, 0xAD, 0x60, 0x03, 0x29, 0x0F, 0xD0, 0x07, 0xAD, 0x61,
    0x03, 0xC9, 0x07, 0x90, 0x24, 0xA9, 0x00, 0xC5, 0xCA, 0xF0, 0x0C, 0xA9,
    0x80, 0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0xA5,
    0xC8, 0x0D, 0x60, 0x03, 0x8D, 0x60, 0x03, 0xA9, 0x60, 0xA0, 0x03, 0xA2,
    0x70, 0x20, 0x30, 0x52, 0x60, 0xA2, 0x4F, 0xA9, 0x00, 0x9D, 0x00, 0x03,
    0xCA, 0x10, 0xFA, 0xAD, 0x61, 0x03, 0x4A, 0xD8, 0x18, 0x69, 0x05, 0xF8,
    0xAA, 0xA0, 0x05, 0xB9, 0x62, 0x03, 0x9D, 0x02, 0x03, 0xCA, 0x88, 0x10,
    0xF6, 0xAD, 0x61, 0x03, 0x29, 0x01, 0xF0, 0x16, 0xA2, 0x07, 0xA9, 0x00,
    0xBC, 0x02, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x03, 0x03, 0xB9, 0xF4, 0x69,
    0xCA, 0x10, 0xF1, 0x8D, 0x02, 0x03, 0xA2, 0x00, 0x86, 0xC5, 0xA9, 0x10,
    0x8D, 0x12, 0x03, 0xA9, 0x00, 0x85, 0xC5, 0xA2, 0x06, 0xBD, 0x02, 0x03,
    0x9D, 0x22, 0x03, 0xBD, 0x12, 0x03, 0x9D, 0x32, 0x03, 0xCA, 0x10, 0xF1,
    0xA5, 0xC5, 0xF0, 0x03, 0x20, 0x87, 0x66, 0xF8, 0x38, 0xA2, 0x06, 0xBD,
    0x02, 0x03, 0xFD, 0x32, 0x03, 0x9D, 0x02, 0x03, 0xCA, 0x10, 0xF4, 0x90,
    0x17, 0x18, 0xA2, 0x06, 0xBD, 0x12, 0x03, 0x7D, 0x22, 0x03, 0x9D, 0x12,
    0x03, 0xCA, 0x10, 0xF4, 0xA6, 0xC5, 0xFE, 0x40, 0x03, 0x4C, 0xD3, 0x5C,
    0xA2, 0x06, 0x18, 0xBD, 0x02, 0x03, 0x7D, 0x32, 0x03, 0x9D, 0x02, 0x03,
    0xCA, 0x10, 0xF4, 0xE6, 0xC5, 0xA9, 0x06, 0xC5, 0xC5, 0xB0, 0xA8, 0xA9,
    0x0A, 0x85, 0xC7, 0xA9, 0x00, 0x8D, 0x61, 0x03, 0xA9, 0x40, 0x8D, 0x60,
    0x03, 0xA9, 0x00, 0x8D, 0x61, 0x03, 0xAD, 0x02, 0x03, 0x29, 0xF0, 0xD0,
    0x1A, 0xEE, 0x61, 0x03, 0xA9, 0x00, 0xA2, 0x0A, 0xBC, 0x02, 0x03, 0x19,
    0x54, 0x69, 0x9D, 0x02, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0xC6,
    0xC7, 0xD0, 0xDF, 0xA2, 0x05, 0xBD, 0x02, 0x03, 0x9D, 0x62, 0x03, 0xCA,
    0x10, 0xF7, 0xA2, 0x07, 0xBD, 0x10, 0x03, 0x9D, 0x68, 0x03, 0xCA, 0x10,
    0xF7, 0x20, 0x35, 0x55, 0xA2, 0x0F, 0xA9, 0x00, 0x9D, 0x00, 0x03, 0xCA,
    0x10, 0xFA, 0xAD, 0x70, 0x03, 0x29, 0x0F, 0xD0, 0x35, 0xAD, 0x71, 0x03,
    0xC9, 0x09, 0xB0, 0x2E, 0x4A, 0x18, 0xD8, 0x69, 0x05, 0xAA, 0xA0, 0x05,
    0xB9, 0x72, 0x03, 0x9D, 0x02, 0x03, 0xCA, 0x88, 0x10, 0xF6, 0xA9, 0x61,
    0x29, 0x01, 0xF0, 0x16, 0xA2, 0x07, 0xA9, 0x00, 0xBC, 0x02, 0x03, 0x19,
    0x54, 0x69, 0x9D, 0x03, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x8D,
    0x02, 0x03, 0xA9, 0x48, 0x85, 0xF2, 0xA9, 0x6E, 0x85, 0xF3, 0xA2, 0x00,
    0x86, 0xC5, 0xDE, 0x40, 0x03, 0x30, 0x11, 0xA0, 0x07, 0xF8, 0x18, 0xB9,
    0x02, 0x03, 0x71, 0xF2, 0x99, 0x02, 0x03, 0x88, 0x10, 0xF5, 0x30, 0xEA,
    0xE6, 0xC5, 0xD8, 0x18, 0xA5, 0xF2, 0x69, 0x08, 0x85, 0xF2, 0x90, 0x02,
    0xE6, 0xF3, 0xA6, 0xC5, 0xE0, 0x07, 0x90, 0xD6, 0xA9, 0x00, 0x8D, 0x70,
    0x03, 0x8D, 0x71, 0x03, 0xAD, 0x02, 0x03, 0x29, 0xF0, 0xD0, 0x1E, 0xA9,
    0x40, 0x8D, 0x70, 0x03, 0xA2, 0x0F, 0xA9, 0x00, 0xBC, 0x02, 0x03, 0x19,
    0x54, 0x69, 0x9D, 0x02, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0xEE,
    0x71, 0x03, 0x4C, 0xFC, 0x5D, 0xA2, 0x05, 0xBD, 0x02, 0x03, 0x9D, 0x72,
    0x03, 0xCA, 0x10, 0xF7, 0xA5, 0xCA, 0xF0, 0x15, 0xA9, 0x80, 0xA0, 0x6D,
    0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20,
    0x30, 0x52, 0x20, 0xAE, 0x50, 0xAD, 0x70, 0x03, 0x05, 0xC8, 0x8D, 0x70,
    0x03, 0x60, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9,
    0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0x4C,
    0x96, 0x5B, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9,
    0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0x4C,
    0x7D, 0x5B, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9,
    0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0x4C,
    0x2F, 0x5C, 0xAD, 0x60, 0x03, 0x10, 0x03, 0x4C, 0x23, 0x69, 0xAD, 0x62,
    0x03, 0xD0, 0x03, 0x4C, 0x2F, 0x69, 0xA2, 0x07, 0xBD, 0x60, 0x03, 0x9D,
    0x90, 0x03, 0xCA, 0x10, 0xF7, 0xAD, 0x60, 0x03, 0xD0, 0x3A, 0xAD, 0x61,
    0x03, 0xD0, 0x35, 0xAD, 0x62, 0x03, 0xC9, 0x10, 0xD0, 0x2E, 0xA9, 0xA0,
    0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20, 0xAE, 0x50, 0xAD, 0x72,
    0x03, 0xD0, 0x03, 0x4C, 0x19, 0x69, 0xAD, 0x70, 0x03, 0xC9, 0x40, 0xD0,
    0x0A, 0xAD, 0x71, 0x03, 0xC9, 0x03, 0x90, 0x03, 0x4C, 0xD8, 0x60, 0xA9,
    0x90, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x0E, 0x69, 0xA2,
    0x05, 0xBD, 0x62, 0x03, 0x9D, 0x02, 0x03, 0xCA, 0x10, 0xF7, 0xA9, 0x00,
    0x85, 0xC3, 0xA9, 0x06, 0x85, 0xC5, 0x20, 0x58, 0x60, 0xA5, 0xC3, 0x20,
    0x9C, 0x60, 0x20, 0x64, 0x60, 0xB0, 0x08, 0xA6, 0xC3, 0xFE, 0x30, 0x03,
    0x4C, 0x06, 0x5F, 0x20, 0x76, 0x60, 0xE6, 0xC3, 0xC6, 0xC5, 0xD0, 0xE2,
    0xA2, 0x06, 0x38, 0xA9, 0x00, 0xFD, 0x02, 0x03, 0x9D, 0x02, 0x03, 0xCA,
    0x10, 0xF5, 0xA2, 0x07, 0xA9, 0x00, 0xBC, 0x02, 0x03, 0x19, 0x54, 0x69,
    0x9D, 0x03, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x8D, 0x02, 0x03,
    0xA9, 0x00, 0x85, 0xF2, 0xA9, 0x6F, 0x85, 0xF3, 0xA9, 0x00, 0x85, 0xC3,
    0xA9, 0x06, 0x85, 0xC4, 0xA6, 0xC3, 0xDE, 0x30, 0x03, 0x30, 0x10, 0xA0,
    0x06, 0x18, 0xB9, 0x02, 0x03, 0x71, 0xF2, 0x99, 0x02, 0x03, 0x88, 0x10,
    0xF5, 0x30, 0xE9, 0xD8, 0x18, 0xA5, 0xF2, 0x69, 0x08, 0x85, 0xF2, 0x90,
    0x02, 0xE6, 0xF3, 0xF8, 0xE6, 0xC3, 0xC6, 0xC4, 0xD0, 0xD6, 0xAD, 0x02,
    0x03, 0x29, 0xF0, 0xD0, 0x07, 0x20, 0x88, 0x60, 0xE6, 0xC4, 0xD0, 0xF2,
    0xA2, 0x05, 0xBD, 0x02, 0x03, 0x9D, 0x9A, 0x03, 0xCA, 0x10, 0xF7, 0xA9,
    0x80, 0x8D, 0x98, 0x03, 0xA5, 0xC4, 0x8D, 0x99, 0x03, 0xF0, 0x08, 0xA9,
    0x40, 0x0D, 0x98, 0x03, 0x8D, 0x98, 0x03, 0x20, 0xEB, 0x5F, 0xA9, 0xA0,
    0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20, 0xB6, 0x50, 0xA9, 0x70,
    0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9, 0x38, 0xA0, 0x6D, 0xA2,
    0x68, 0x20, 0x30, 0x52, 0x20, 0x78, 0x53, 0xA2, 0x07, 0xBD, 0x98, 0x03,
    0x9D, 0x60, 0x03, 0xCA, 0x10, 0xF7, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68,
    0x20, 0x30, 0x52, 0x20, 0xB6, 0x50, 0x60, 0xA2, 0x05, 0xA9, 0x00, 0x9D,
    0x62, 0x03, 0xCA, 0x10, 0xFA, 0xAD, 0x60, 0x03, 0x29, 0x7F, 0x8D, 0x60,
    0x03, 0x29, 0x40, 0x0A, 0xAA, 0xAD, 0x60, 0x03, 0x29, 0x0F, 0xD0, 0x29,
    0x8E, 0x60, 0x03, 0xAD, 0x61, 0x03, 0x29, 0xF0, 0xD0, 0x13, 0xAD, 0x61,
    0x03, 0x0A, 0x0A, 0x0A, 0x0A, 0x8D, 0x62, 0x03, 0xA9, 0x00, 0x8D, 0x61,
    0x03, 0x8E, 0x60, 0x03, 0x60, 0x8D, 0x62, 0x03, 0xA9, 0x01, 0x8D, 0x61,
    0x03, 0x8E, 0x60, 0x03, 0x60, 0x0A, 0x0A, 0x0A, 0x0A, 0x8D, 0x62, 0x03,
    0xAD, 0x61, 0x03, 0x4A, 0x4A, 0x4A, 0x4A, 0x0D, 0x62, 0x03, 0x8D, 0x62,
    0x03, 0xAD, 0x61, 0x03, 0x0A, 0x0A, 0x0A, 0x0A, 0x8D, 0x63, 0x03, 0xA9,
    0x02, 0x8D, 0x61, 0x03, 0x8E, 0x60, 0x03, 0x60, 0xA0, 0x06, 0xB9, 0x02,
    0x03, 0x99, 0x22, 0x03, 0x88, 0x10, 0xF7, 0x60, 0xD8, 0x18, 0xA2, 0x07,
    0xF8, 0xBD, 0x02, 0x03, 0x7D, 0x22, 0x03, 0x9D, 0x02, 0x03, 0xCA, 0x10,
    0xF4, 0x60, 0xD8, 0x38, 0xF8, 0xA2, 0x07, 0xBD, 0x02, 0x03, 0xFD, 0x22,
    0x03, 0x9D, 0x02, 0x03, 0xCA, 0x10, 0xF4, 0x60, 0xA2, 0x08, 0xA9, 0x00,
    0xBC, 0x02, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x02, 0x03, 0xB9, 0xF4, 0x69,
    0xCA, 0x10, 0xF1, 0x60, 0x85, 0xC4, 0x29, 0x01, 0xF0, 0x16, 0xA9, 0x00,
    0xA2, 0x07, 0xBC, 0x22, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x23, 0x03, 0xB9,
    0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x8D, 0x22, 0x03, 0xA5, 0xC4, 0x4A, 0xF0,
    0x1A, 0xD8, 0x18, 0x69, 0x07, 0xF8, 0xA8, 0xA2, 0x07, 0xBD, 0x22, 0x03,
    0x99, 0x22, 0x03, 0x88, 0xCA, 0x10, 0xF6, 0xA9, 0x00, 0x99, 0x22, 0x03,
    0x88, 0x10, 0xFA, 0x60, 0xA9, 0x80, 0x85, 0xF6, 0xA9, 0x6F, 0x85, 0xF7,
    0xA9, 0x03, 0x85, 0xC3, 0xA2, 0x07, 0xBD, 0x70, 0x03, 0x9D, 0x98, 0x03,
    0xCA, 0x10, 0xF7, 0xA0, 0x07, 0xB1, 0xF6, 0x99, 0x70, 0x03, 0x88, 0x10,
    0xF8, 0xA0, 0x07, 0xB9, 0x70, 0x03, 0x99, 0x60, 0x03, 0xB9, 0x98, 0x03,
    0x99, 0x68, 0x03, 0x88, 0x10, 0xF1, 0x20, 0x78, 0x53, 0xC6, 0xC3, 0x30,
    0x23, 0xD8, 0x18, 0xA5, 0xF6, 0x69, 0x08, 0x85, 0xF6, 0x90, 0x02, 0xE6,
    0xF7, 0xF8, 0xA0, 0x07, 0xB1, 0xF6, 0x99, 0x60, 0x03, 0xB9, 0x70, 0x03,
    0x99, 0x68, 0x03, 0x88, 0x10, 0xF2, 0x20, 0xB6, 0x50, 0x4C, 0xF9, 0x60,
    0x60, 0xA9, 0x00, 0x85, 0xCD, 0xAD, 0x60, 0x03, 0x29, 0x0F, 0xF0, 0x03,
    0x4C, 0x2F, 0x69, 0xAD, 0x62, 0x03, 0xD0, 0x09, 0xA9, 0xA0, 0xA0, 0x6D,
    0xA2, 0x70, 0x4C, 0x30, 0x52, 0x2C, 0x60, 0x03, 0x10, 0x0A, 0xA9, 0x80,
    0x85, 0xCD, 0x4D, 0x60, 0x03, 0x8D, 0x60, 0x03, 0x2C, 0x60, 0x03, 0x50,
    0x10, 0xAD, 0x60, 0x03, 0x29, 0x0F, 0xD0, 0xDC, 0xAD, 0x61, 0x03, 0xC9,
    0x12, 0xB0, 0xD5, 0x90, 0x12, 0xAD, 0x61, 0x03, 0xC9, 0x04, 0xB0, 0xC4,
    0xC9, 0x03, 0xD0, 0x07, 0xAD, 0x62, 0x03, 0xC9, 0x23, 0xB0, 0xB9, 0xA9,
    0x40, 0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20, 0x78, 0x53, 0xA9,
    0x70, 0xA0, 0x03, 0xA2, 0x90, 0x20, 0x30, 0x52, 0xA9, 0x70, 0xA0, 0x03,
    0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0xE2, 0x66, 0xA9, 0x70, 0xA0, 0x03,
    0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x98, 0x20,
    0x30, 0x52, 0x20, 0xAE, 0x50, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20,
    0x30, 0x52, 0xA9, 0x38, 0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20,
    0x78, 0x53, 0xA9, 0x00, 0x85, 0xC0, 0x2C, 0x70, 0x03, 0x50, 0x10, 0xAD,
    0x71, 0x03, 0xC9, 0x12, 0x90, 0x03, 0x4C, 0x48, 0x61, 0xAA, 0xBD, 0x94,
    0x6A, 0x85, 0xC0, 0x20, 0x30, 0x62, 0xAC, 0x99, 0x03, 0xA9, 0x00, 0x8D,
    0x99, 0x03, 0xA2, 0x03, 0x0E, 0x9B, 0x03, 0x2E, 0x9A, 0x03, 0x2E, 0x99,
    0x03, 0x2E, 0x98, 0x03, 0xCA, 0x10, 0xF1, 0x88, 0x10, 0xEC, 0xAD, 0x98,
    0x03, 0x8D, 0x70, 0x03, 0xAD, 0x99, 0x03, 0x8D, 0x71, 0x03, 0x24, 0xCD,
    0x10, 0x15, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9,
    0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0x60,
    0x20, 0x0E, 0x69, 0xA2, 0x05, 0xA5, 0xC0, 0x4A, 0xD8, 0x18, 0x69, 0x05,
    0xA8, 0xBD, 0x72, 0x03, 0x99, 0x02, 0x03, 0x9D, 0x12, 0x03, 0x88, 0xCA,
    0x10, 0xF3, 0xA5, 0xC0, 0x8D, 0x40, 0x03, 0x4A, 0x90, 0x16, 0xA2, 0x08,
    0xA9, 0x00, 0xBC, 0x01, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x02, 0x03, 0xB9,
    0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x9D, 0x02, 0x03, 0xA9, 0x00, 0xA0, 0x6F,
    0x85, 0xF2, 0x84, 0xF3, 0xA9, 0x07, 0x85, 0xC1, 0xA9, 0x00, 0x85, 0xC3,
    0xA9, 0x00, 0xAA, 0xF8, 0x38, 0xA0, 0x07, 0xB9, 0x02, 0x03, 0xF1, 0xF2,
    0x99, 0x02, 0x03, 0x99, 0x22, 0x03, 0x88, 0x10, 0xF2, 0xE8, 0xB0, 0xEB,
    0xCA, 0x8A, 0xA6, 0xC3, 0x9D, 0x30, 0x03, 0xE6, 0xC3, 0x18, 0xA0, 0x07,
    0xB9, 0x02, 0x03, 0x71, 0xF2, 0x99, 0x02, 0x03, 0x88, 0x10, 0xF5, 0xD8,
    0x18, 0xA5, 0xF2, 0x69, 0x08, 0x85, 0xF2, 0x90, 0x02, 0xE6, 0xF3, 0xC6,
    0xC1, 0x10, 0xC1, 0xA9, 0x00, 0x85, 0xC3, 0xA9, 0x10, 0x8D, 0x02, 0x03,
    0xA2, 0x07, 0x86, 0xC5, 0xA2, 0x07, 0xBD, 0x02, 0x03, 0x9D, 0x22, 0x03,
    0xCA, 0x10, 0xF7, 0xA6, 0xC3, 0xDE, 0x30, 0x03, 0x30, 0x23, 0xA2, 0x07,
    0xBD, 0x00, 0x03, 0x9D, 0x20, 0x03, 0xCA, 0x10, 0xF7, 0xA5, 0xC3, 0x20,
    0x9C, 0x60, 0xF8, 0xA2, 0x05, 0x18, 0xBD, 0x02, 0x03, 0x7D, 0x22, 0x03,
    0x9D, 0x02, 0x03, 0xCA, 0x10, 0xF4, 0x4C, 0xCF, 0x62, 0xE6, 0xC3, 0xC6,
    0xC5, 0x10, 0xD0, 0xA2, 0x05, 0xBD, 0x02, 0x03, 0x9D, 0x72, 0x03, 0xCA,
    0x10, 0xF7, 0xA9, 0x00, 0x8D, 0x70, 0x03, 0x8D, 0x71, 0x03, 0x60, 0x20,
    0x35, 0x61, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9,
    0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0xA9,
    0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xAD, 0x60, 0x03, 0x09,
    0x80, 0x8D, 0x60, 0x03, 0x20, 0xB6, 0x50, 0xA9, 0x70, 0xA0, 0x03, 0xA2,
    0x60, 0x20, 0x30, 0x52, 0xA9, 0xA8, 0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30,
    0x52, 0x4C, 0x78, 0x53, 0x20, 0x35, 0x61, 0xA9, 0x70, 0xA0, 0x03, 0xA2,
    0x68, 0x20, 0x30, 0x52, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30,
    0x52, 0x20, 0x35, 0x55, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30,
    0x52, 0x20, 0xB6, 0x50, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30,
    0x52, 0xA9, 0xA8, 0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x4C, 0x78,
    0x53, 0x20, 0x35, 0x61, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30,
    0x52, 0xA2, 0x07, 0xBD, 0x70, 0x03, 0x9D, 0x98, 0x03, 0xCA, 0x10, 0xF7,
    0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55,
    0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA2, 0x07, 0xBD,
    0x70, 0x03, 0x9D, 0xA0, 0x03, 0xCA, 0x10, 0xF7, 0xA9, 0x98, 0xA0, 0x03,
    0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0xAE, 0x50, 0xA2, 0x07, 0xBD, 0x98,
    0x03, 0x9D, 0x60, 0x03, 0xCA, 0x10, 0xF7, 0xA2, 0x07, 0xBD, 0xA0, 0x03,
    0x9D, 0x68, 0x03, 0xCA, 0x10, 0xF7, 0xA2, 0x07, 0xBD, 0x70, 0x03, 0x9D,
    0x98, 0x03, 0xCA, 0x10, 0xF7, 0x20, 0xB6, 0x50, 0xA2, 0x07, 0xBD, 0x70,
    0x03, 0x9D, 0x68, 0x03, 0xCA, 0x10, 0xF7, 0xA2, 0x07, 0xBD, 0x98, 0x03,
    0x9D, 0x60, 0x03, 0xCA, 0x10, 0xF7, 0x4C, 0x35, 0x55, 0x20, 0x13, 0x63,
    0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9, 0xA0, 0xA0,
    0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x4C, 0x35, 0x55, 0x20, 0x54, 0x63,
    0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9, 0xA0, 0xA0,
    0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x4C, 0x35, 0x55, 0x20, 0x8D, 0x63,
    0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9, 0xA0, 0xA0,
    0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x4C, 0x35, 0x55, 0xA2, 0x07, 0xBD,
    0x60, 0x03, 0x9D, 0x90, 0x03, 0xCA, 0x10, 0xF7, 0xA9, 0x60, 0xA0, 0x03,
    0xA2, 0x68, 0x20, 0x30, 0x52, 0x20, 0x78, 0x53, 0xA9, 0x70, 0xA0, 0x03,
    0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20,
    0x30, 0x52, 0x20, 0xB6, 0x50, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20,
    0x30, 0x52, 0x20, 0xE6, 0x56, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20,
    0x30, 0x52, 0xA9, 0x90, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20,
    0xB6, 0x50, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x4C,
    0x96, 0x5E, 0xA2, 0x07, 0xBD, 0x60, 0x03, 0x9D, 0x90, 0x03, 0xCA, 0x10,
    0xF7, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20, 0x78,
    0x53, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9, 0xA0,
    0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20, 0xAE, 0x50, 0xA9, 0x70,
    0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0xE6, 0x56, 0xA9, 0x70,
    0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9, 0x90, 0xA0, 0x03, 0xA2,
    0x60, 0x20, 0x30, 0x52, 0x20, 0xB6, 0x50, 0xA9, 0x70, 0xA0, 0x03, 0xA2,
    0x60, 0x20, 0x30, 0x52, 0x4C, 0x96, 0x5E, 0xA2, 0x07, 0xBD, 0x60, 0x03,
    0x9D, 0x90, 0x03, 0xCA, 0x10, 0xF7, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x68,
    0x20, 0x30, 0x52, 0x20, 0xB6, 0x50, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x60,
    0x20, 0x30, 0x52, 0xA9, 0x90, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52,
    0xA2, 0x07, 0xBD, 0x70, 0x03, 0x9D, 0x90, 0x03, 0xCA, 0x10, 0xF7, 0x20,
    0xAE, 0x50, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9,
    0x90, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0xA9,
    0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x96, 0x5E, 0xA9,
    0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9, 0xA8, 0xA0, 0x6D,
    0xA2, 0x68, 0x20, 0x30, 0x52, 0x4C, 0x78, 0x53, 0xA9, 0x60, 0xA0, 0x03,
    0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20,
    0x30, 0x52, 0x20, 0x35, 0x55, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20,
    0x30, 0x52, 0x4C, 0x55, 0x64, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x68, 0x20,
    0x30, 0x52, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20,
    0x35, 0x55, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x4C,
    0xAE, 0x64, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0xA9,
    0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0xA9,
    0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x4C, 0x07, 0x65, 0x20,
    0x96, 0x5E, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9,
    0x30, 0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x4C, 0x35, 0x55, 0x20,
    0x96, 0x5E, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9,
    0x38, 0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x4C, 0x35, 0x55, 0xA9,
    0x68, 0x85, 0xF2, 0xA9, 0x03, 0x85, 0xF3, 0xA9, 0xA0, 0x85, 0xF4, 0xA9,
    0x03, 0x85, 0xF5, 0x20, 0x04, 0x69, 0x20, 0x96, 0x5E, 0xA9, 0x70, 0xA0,
    0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9, 0xA0, 0xA0, 0x03, 0xA2, 0x68,
    0x20, 0x30, 0x52, 0x20, 0x78, 0x53, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60,
    0x20, 0x30, 0x52, 0x4C, 0x35, 0x61, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x68,
    0x20, 0x30, 0x52, 0xA9, 0xA0, 0xA0, 0x6D, 0xA2, 0x60, 0x20, 0x30, 0x52,
    0x20, 0x35, 0x55, 0x60, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x70, 0x20, 0x30,
    0x52, 0xAD, 0x70, 0x03, 0x29, 0x7F, 0x8D, 0x70, 0x03, 0x60, 0xA9, 0x60,
    0xA0, 0x03, 0xA2, 0x70, 0x20, 0x30, 0x52, 0xAD, 0x70, 0x03, 0x49, 0x80,
    0x8D, 0x70, 0x03, 0x60, 0xA0, 0x07, 0x18, 0xB9, 0x02, 0x03, 0x71, 0xF2,
    0x99, 0x02, 0x03, 0x88, 0x10, 0xF5, 0x60, 0x85, 0xC4, 0x29, 0x01, 0xF0,
    0x2C, 0xA9, 0x00, 0xA2, 0x07, 0xBC, 0x22, 0x03, 0x19, 0x54, 0x69, 0x9D,
    0x23, 0x03, 0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x8D, 0x22, 0x03, 0xA9,
    0x00, 0xA2, 0x07, 0xBC, 0x32, 0x03, 0x19, 0x54, 0x69, 0x9D, 0x33, 0x03,
    0xB9, 0xF4, 0x69, 0xCA, 0x10, 0xF1, 0x8D, 0x32, 0x03, 0xA5, 0xC4, 0x4A,
    0xF0, 0x23, 0xD8, 0x18, 0x69, 0x07, 0xF8, 0xA8, 0xA2, 0x07, 0xBD, 0x22,
    0x03, 0x99, 0x22, 0x03, 0xBD, 0x32, 0x03, 0x99, 0x32, 0x03, 0x88, 0xCA,
    0x10, 0xF0, 0xA9, 0x00, 0x99, 0x22, 0x03, 0x99, 0x32, 0x03, 0x88, 0x10,
    0xF7, 0x60, 0xA9, 0x0F, 0x2C, 0x60, 0x03, 0x70, 0x2E, 0xD0, 0x36, 0x20,
    0x21, 0x67, 0xAE, 0x61, 0x03, 0xE0, 0x12, 0xB0, 0x21, 0xD8, 0xA9, 0x0B,
    0x38, 0xFD, 0x94, 0x6A, 0xF8, 0x4A, 0xF0, 0x0C, 0xAA, 0xA0, 0x05, 0xA9,
    0x00, 0x99, 0x72, 0x03, 0x88, 0xCA, 0xD0, 0xF9, 0x90, 0x08, 0xA9, 0xF0,
    0x39, 0x72, 0x03, 0x99, 0x72, 0x03, 0x60, 0xA9, 0x28, 0xA0, 0x6D, 0xA2,
    0x70, 0x20, 0x30, 0x52, 0x60, 0xA9, 0x60, 0xA0, 0x03, 0xA2, 0x70, 0x20,
    0x30, 0x52, 0x60, 0xA2, 0x07, 0xBD, 0x60, 0x03, 0x9D, 0xA0, 0x03, 0xCA,
    0x10, 0xF7, 0x20, 0xE2, 0x66, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20,
    0x30, 0x52, 0xA9, 0xA0, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20,
    0xAE, 0x50, 0xAD, 0x72, 0x03, 0xD0, 0x03, 0x8D, 0x70, 0x03, 0x60, 0xA9,
    0x60, 0x85, 0xF2, 0xA9, 0x6D, 0x85, 0xF3, 0xA9, 0x80, 0x85, 0xF4, 0xA9,
    0x03, 0x85, 0xF5, 0x20, 0x04, 0x69, 0x20, 0x52, 0x68, 0xAD, 0x70, 0x03,
    0x10, 0x15, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9,
    0x60, 0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20, 0xB6, 0x50, 0xA9,
    0x90, 0x85, 0xF2, 0xA9, 0x6D, 0x85, 0xF3, 0xA9, 0x70, 0x85, 0xF4, 0xA9,
    0x03, 0x85, 0xF5, 0x20, 0x80, 0x68, 0x10, 0x07, 0xA9, 0x04, 0x85, 0xC9,
    0x4C, 0xCF, 0x67, 0xA9, 0x58, 0x85, 0xF2, 0xA9, 0x6D, 0x85, 0xF3, 0x20,
    0x80, 0x68, 0x10, 0x07, 0xA9, 0x03, 0x85, 0xC9, 0x4C, 0xCF, 0x67, 0xA9,
    0x80, 0x85, 0xF2, 0xA9, 0x6D, 0x85, 0xF3, 0x20, 0x80, 0x68, 0x10, 0x07,
    0xA9, 0x02, 0x85, 0xC9, 0x4C, 0xCF, 0x67, 0xA9, 0x01, 0x85, 0xC9, 0xA5,
    0xC9, 0x8D, 0x00, 0x03, 0xC9, 0x03, 0x90, 0x15, 0xA9, 0x70, 0xA0, 0x03,
    0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9, 0x58, 0xA0, 0x6D, 0xA2, 0x68, 0x20,
    0x30, 0x52, 0x20, 0xAE, 0x50, 0xA5, 0xC9, 0x29, 0x01, 0xD0, 0x04, 0xA9,
    0x80, 0x85, 0xC8, 0xA9, 0x80, 0x85, 0xF2, 0xA9, 0x6D, 0x85, 0xF3, 0x20,
    0x80, 0x68, 0x10, 0x15, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30,
    0x52, 0xA9, 0x58, 0xA0, 0x6D, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20, 0xAE,
    0x50, 0xAD, 0x70, 0x03, 0x29, 0x7F, 0x8D, 0x70, 0x03, 0xA9, 0x70, 0x85,
    0xF4, 0xA9, 0x03, 0x85, 0xF5, 0xA9, 0x98, 0x85, 0xF2, 0xA9, 0x6D, 0x85,
    0xF3, 0x20, 0x80, 0x68, 0x10, 0x1B, 0xA9, 0x80, 0xA0, 0x6D, 0xA2, 0x60,
    0x20, 0x30, 0x52, 0xA9, 0x70, 0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52,
    0x20, 0xAE, 0x50, 0xA5, 0xCA, 0x49, 0x01, 0x85, 0xCA, 0x60, 0xA9, 0x80,
    0xA0, 0x03, 0xA2, 0x68, 0x20, 0x30, 0x52, 0x20, 0x35, 0x55, 0xA9, 0x70,
    0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0x20, 0x2B, 0x67, 0xA9, 0x70,
    0xA0, 0x03, 0xA2, 0x60, 0x20, 0x30, 0x52, 0xA9, 0x80, 0xA0, 0x03, 0xA2,
    0x68, 0x20, 0x30, 0x52, 0x20, 0x78, 0x53, 0x60, 0xF8, 0xA0, 0x00, 0xB1,
    0xF2, 0x51, 0xF4, 0x10, 0x05, 0xB1, 0xF2, 0x09, 0x01, 0x60, 0x0A, 0x10,
    0x20, 0xB1, 0xF2, 0x30, 0x16, 0xA0, 0x02, 0xB1, 0xF2, 0xD0, 0x03, 0xA9,
    0x80, 0x60, 0xB1, 0xF4, 0xD0, 0x01, 0x60, 0xA0, 0x00, 0xB1, 0xF2, 0x0A,
    0x09, 0x01, 0x60, 0xB1, 0xF4, 0x0A, 0x09, 0x01, 0x60, 0xB1, 0xF2, 0x51,
    0xF4, 0xD0, 0x23, 0xC8, 0xB1, 0xF2, 0x51, 0xF4, 0xD0, 0x1C, 0xA0, 0x07,
    0xA2, 0x05, 0xA9, 0x00, 0x85, 0xC0, 0x38, 0xB1, 0xF4, 0xF1, 0xF2, 0xF0,
    0x02, 0xE6, 0xC0, 0x88, 0xCA, 0x10, 0xF4, 0x90, 0x20, 0xA5, 0xC0, 0xD0,
    0x19, 0x60, 0xA0, 0x01, 0x38, 0xB1, 0xF4, 0xF1, 0xF2, 0x88, 0xB1, 0xF4,
    0xF1, 0xF2, 0xB0, 0x10, 0xB1, 0xF2, 0x29, 0xC0, 0xF0, 0x07, 0xC9, 0xC0,
    0xF0, 0x03, 0xA9, 0x81, 0x60, 0xA9, 0x01, 0x60, 0xB1, 0xF4, 0x29, 0xC0,
    0xF0, 0xF4, 0xC9, 0xC0, 0xF0, 0xF0, 0xD0, 0xF1, 0xA0, 0x07, 0xB1, 0xF2,
    0x91, 0xF4, 0x88, 0x10, 0xF9, 0x60, 0xA2, 0x5F, 0xA9, 0x00, 0x9D, 0x00,
    0x03, 0xCA, 0x10, 0xFA, 0x60, 0xA9, 0x28, 0xA0, 0x6D, 0xA2, 0x70, 0x20,
    0x30, 0x52, 0x60, 0xA9, 0xD0, 0xA0, 0x6D, 0xA2, 0x70, 0x20, 0x30, 0x52,
    0x4C, 0x53, 0x69, 0xA9, 0xC8, 0xA0, 0x6D, 0xA2, 0x70, 0x20, 0x30, 0x52,
    0x4C, 0x53, 0x69, 0xA9, 0xC0, 0xA0, 0x6D, 0xA2, 0x70, 0x20, 0x30, 0x52,
    0x4C, 0x53, 0x69, 0xA9, 0xD8, 0xA0, 0x6D, 0xA2, 0x70, 0x20, 0x30, 0x52,
    0x4C, 0x53, 0x69, 0x00, 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
    0x80, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x30,
    0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
    0x80, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x30,
    0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
    0x80, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x30,
    0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
    0x80, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
    0x12, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x33, 0x34, 0x35,
    0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,
    0x4E, 0x4F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x51, 0x52, 0x53,
    0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31,
    0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40, 0x41, 0x42, 0x43,
    0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55,
    0x56, 0x57, 0x58, 0x59, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x90, 0x91,
    0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x00, 0x00, 0x00, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x25, 0x26, 0x27, 0x28, 0x29, 0x00, 0x00, 0x00, 0x30, 0x31, 0x32, 0x33,
    0x34, 0x35, 0x36, 0x37, 0x35, 0x36, 0x37, 0x38, 0x39, 0x00, 0x00, 0x00,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x45, 0x46, 0x47, 0x49,
    0x49, 0x00, 0x00, 0x00, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x55, 0x56, 0x57, 0x58, 0x59, 0x00, 0x00, 0x00, 0x60, 0x61, 0x62, 0x63,
    0x64, 0x65, 0x66, 0x67, 0x65, 0x66, 0x67, 0x68, 0x69, 0x00, 0x00, 0x00,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x00, 0x00, 0x00, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x55, 0x56, 0x57, 0x58, 0x59, 0x00, 0x00, 0x00, 0x60, 0x61, 0x62, 0x63,
    0x64, 0x65, 0x66, 0x67, 0x65, 0x66, 0x67, 0x68, 0x69, 0x00, 0x00, 0x00,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x00, 0x00, 0x00, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x85, 0x86, 0x87, 0x88, 0x89, 0x00, 0x00, 0x00, 0x90, 0x91, 0x92, 0x93,
    0x94, 0x95, 0x96, 0x97, 0x95, 0x96, 0x97, 0x98, 0x99, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x18, 0x19, 0x1A,
    0x1B, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x21, 0x22, 0x23,
    0x24, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x41, 0x42, 0x43, 0x44, 0x48, 0x49, 0x4A,
    0x4B, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x81, 0x82, 0x83,
    0x84, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA8, 0xA9, 0xAA,
    0xAB, 0xAC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0xB1, 0xB2, 0xB3,
    0xB4, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x30, 0x31, 0x32, 0x33,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0x69, 0x31,
    0x47, 0x18, 0x05, 0x60, 0x00, 0x00, 0x23, 0x02, 0x58, 0x50, 0x92, 0x99,
    0x40, 0x01, 0x43, 0x42, 0x94, 0x48, 0x19, 0x03, 0x00, 0x00, 0x27, 0x18,
    0x28, 0x18, 0x28, 0x46, 0x00, 0x00, 0x14, 0x14, 0x21, 0x35, 0x62, 0x37,
    0x00, 0x00, 0x31, 0x41, 0x59, 0x26, 0x53, 0x59, 0x00, 0x00, 0x62, 0x83,
    0x18, 0x53, 0x07, 0x18, 0x40, 0x01, 0x70, 0x71, 0x06, 0x78, 0x11, 0x87,
    0x40, 0x01, 0x31, 0x83, 0x09, 0x88, 0x61, 0x84, 0x40, 0x01, 0x15, 0x91,
    0x54, 0x94, 0x30, 0x92, 0x00, 0x00, 0x15, 0x70, 0x79, 0x63, 0x26, 0x79,
    0x80, 0x00, 0x15, 0x70, 0x79, 0x63, 0x26, 0x79, 0x00, 0x00, 0x47, 0x12,
    0x38, 0x89, 0x80, 0x38, 0x40, 0x01, 0x78, 0x53, 0x98, 0x16, 0x33, 0x98,
    0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0x50, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x70, 0x79, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x99, 0x99, 0x99,
    0x99, 0x99, 0x99, 0x99, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x23, 0x15, 0x39, 0x31, 0x67, 0x00,
    0xC0, 0x03, 0x13, 0x85, 0x37, 0x04, 0x26, 0x40, 0x40, 0x02, 0x41, 0x66,
    0x35, 0x84, 0x67, 0x69, 0xC0, 0x01, 0x49, 0x99, 0x99, 0x05, 0x34, 0x55,
    0x40, 0x01, 0x99, 0x99, 0x99, 0x95, 0x34, 0x64, 0x40, 0x01, 0x78, 0x53,
    0x98, 0x16, 0x33, 0x97, 0x40, 0x02, 0x99, 0x66, 0x86, 0x52, 0x49, 0x12,
    0x40, 0x03, 0x99, 0x99, 0x66, 0x66, 0x86, 0x67, 0x40, 0x04, 0x99, 0x99,
    0x99, 0x66, 0x66, 0x67, 0x40, 0x05, 0x99, 0x99, 0x99, 0x99, 0x66, 0x67,
    0x40, 0x06, 0x99, 0x99, 0x99, 0x99, 0x99, 0x67, 0x40, 0x06, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x07, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x85, 0x39, 0x81, 0x63, 0x39, 0x74, 0x48, 0x00, 0x99, 0x66, 0x86,
    0x52, 0x49, 0x11, 0x62, 0x00, 0x09, 0x99, 0x96, 0x66, 0x68, 0x66, 0x65,
    0x00, 0x00, 0x99, 0x99, 0x99, 0x66, 0x66, 0x67, 0x00, 0x00, 0x09, 0x99,
    0x99, 0x99, 0x96, 0x67, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x40, 0x01, 0x69, 0x31, 0x47, 0x18, 0x05, 0x60, 0x40, 0x02, 0x95, 0x31,
    0x01, 0x79, 0x80, 0x43, 0x40, 0x03, 0x99, 0x50, 0x33, 0x08, 0x53, 0x17,
    0x40, 0x04, 0x99, 0x95, 0x00, 0x33, 0x30, 0x84, 0x40, 0x05, 0x99, 0x99,
    0x50, 0x00, 0x33, 0x33, 0x40, 0x06, 0x99, 0x99, 0x95, 0x00, 0x00, 0x33,
    0x40, 0x07, 0x99, 0x99, 0x99, 0x50, 0x00, 0x00, 0x40, 0x08, 0x99, 0x99,
    0x99, 0x95, 0x00, 0x00, 0x40, 0x09, 0x99, 0x99, 0x99, 0x99, 0x50, 0x00,
    0x40, 0x10, 0x99, 0x99, 0x99, 0x99, 0x95, 0x00, 0x40, 0x11, 0x99, 0x99,
    0x99, 0x99, 0x99, 0x50, 0x06, 0x93, 0x14, 0x71, 0x80, 0x55, 0x99, 0x45,
    0x00, 0x95, 0x31, 0x01, 0x79, 0x80, 0x43, 0x25, 0x00, 0x09, 0x95, 0x03,
    0x30, 0x85, 0x31, 0x68, 0x00, 0x00, 0x99, 0x95, 0x00, 0x33, 0x30, 0x84,
    0x00, 0x00, 0x09, 0x99, 0x95, 0x00, 0x03, 0x33, 0x00, 0x00, 0x00, 0x99,
    0x99, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x99, 0x99, 0x95, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x99, 0x99, 0x99, 0x95, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0xC0, 0x01, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33, 0xC0, 0x01, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x12, 0x30,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x33, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0xC0, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
    0x00, 0x00, 0x00, 0x00, 0xC0, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x50, 0x00
};

// put a little demo program in RAM:
  #if _TARGET == PERSONAL_COMPUTER
    unsigned char fltptDemo[31] = {
  #else
    const unsigned char fltptDemo[31] PROGMEM = {
  #endif
    0x20, 0xA1, 0x70, 0x20, 0xA2, 0x70, 0xA9, 0x02, 0x20, 0x00, 0x50, 0x20, 0xD3, 0x70,
    0x20, 0xAA, 0x70,
    0xF0, 0x08,
    0x20, 0x31, 0x70, 0xA9, 0x04, 0x20, 0x00, 0x50, 0x20, 0xD3, 0x70, 0x00
};
#endif


// ============================================================================
// ============================================================================
// VTL-02 PROGRAMMING LANGUAGE
// ============================================================================
// ============================================================================
// see 6502.org
//
#if _INCL_VTL02 == TRUE
    #if _TARGET == PERSONAL_COMPUTER
        unsigned char vtl02ROM[1023] = {
    #else
        const unsigned char vtl02ROM[1023] PROGMEM = {
    #endif
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
};
#endif



// ============================================================================
// ============================================================================
// Hi-Lo program from The First Book Of KIM-1
// ============================================================================
// ============================================================================
#if _DEMO_HILO == TRUE
    #if _TARGET == PERSONAL_COMPUTER
        unsigned char hiLoDemo[110] = {
    #else
        const unsigned char hiLoDemo[110] PROGMEM = {
    #endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  #define RND      $E0
/* 0000 */                     //  #define NGUESS   $E1
/* 0000 */                     //  #define LAST     $E2
/* 0000 */                     //  #define INH      $F9
/* 0000 */                     //  #define POINTL   $FA
/* 0000 */                     //  #define POINTH   $FB
/* 0000 */                     //
/* 0000 */                     //  #define SCANDS   $1F1F
/* 0000 */                     //  #define KEYIN    $1F40
/* 0000 */                     //  #define GETKEY   $1F6A
/* 0000 */                     //
/* 0200 */                     //          .ORG    $0200
/* 0200 */                     //
/* 0200 */                     //
/* 0200 */ 0xF8,               //  START:  SED
/* 0201 */ 0xA5, 0xE0,         //  TOP:    LDA     RND             ; generate random #
/* 0203 */ 0x38,               //          SEC                     ; 01 to 98
/* 0204 */ 0x69, 0x00,         //          ADC     #0
/* 0206 */ 0xA2, 0x01,         //          LDX     #1              ; overflow at 99
/* 0208 */ 0xC9, 0x99,         //          CMP     #$99
/* 020A */ 0xD0, 0x01,         //          BNE     OVR0
/* 020C */ 0x8A,               //          TXA
/* 020D */ 0x85, 0xE0,         //  OVR0:   STA     RND
/* 020F */ 0x20, 0x40, 0x1F,   //          JSR     KEYIN
/* 0212 */ 0xD0, 0xED,         //          BNE     TOP
/* 0214 */ 0xD8,               //          CLD                     ; initialize:
/* 0215 */ 0xA9, 0x99,         //          LDA     #$99            ; hi
/* 0217 */ 0x85, 0xFB,         //          STA     POINTH
/* 0219 */ 0xA9, 0x00,         //          LDA     #0
/* 021B */ 0x85, 0xFA,         //          STA     POINTL          ; and lo
/* 021D */ 0xA2, 0xA0,         //  RSET:   LDX     #$A0            ; guess counter
/* 021F */ 0x86, 0xF9,         //  NSET:   STX     INH
/* 0221 */ 0x86, 0xE1,         //          STX     NGUESS
/* 0223 */ 0x20, 0x1F, 0x1F,   //  GUESS:  JSR     SCANDS          ; light display
/* 0226 */ 0x20, 0x6A, 0x1F,   //          JSR     GETKEY          ; test key
/* 0229 */ 0xC9, 0x13,         //          CMP     #$13            ; go key?
/* 022B */ 0xF0, 0xD3,         //          BEQ     START
/* 022D */ 0xC5, 0xE2,         //          CMP     LAST
/* 022F */ 0xF0, 0xF2,         //          BEQ     GUESS           ; same key?
/* 0231 */ 0x85, 0xE2,         //          STA     LAST
/* 0233 */ 0xC9, 0x0A,         //          CMP     #$0A            ; 'A' key?
/* 0235 */ 0xF0, 0x10,         //          BEQ     EVAL            ; yes, evaluate guess
/* 0237 */ 0xB0, 0xEA,         //          BCS     GUESS           ; no key?
/* 0239 */ 0x0A,               //          ASL     A               ; roll character
/* 023A */ 0x0A,               //          ASL     A               ; ..into..
/* 023B */ 0x0A,               //          ASL     A               ; position..
/* 023C */ 0x0A,               //          ASL     A
/* 023D */ 0xA2, 0x03,         //          LDX     #3
/* 023F */ 0x0A,               //  LOOP:   ASL     A               ; ..then
/* 0240 */ 0x26, 0xF9,         //          ROL     INH             ; ..into
/* 0242 */ 0xCA,               //          DEX                     ; ..display
/* 0243 */ 0x10, 0xFA,         //          BPL     LOOP
/* 0245 */ 0x30, 0xDC,         //          BMI     GUESS
/* 0247 */ 0xA5, 0xF9,         //  EVAL:   LDA     INH             ; guess lower..
/* 0249 */ 0xC5, 0xE0,         //          CMP     RND             ; ..than number?
/* 024B */ 0x90, 0x06,         //          BCC     OVR1            ; yes, skip
/* 024D */ 0xC5, 0xFB,         //          CMP     POINTH          ; no, check hi
/* 024F */ 0xB0, 0xD2,         //          BCS     GUESS           ; out of range?
/* 0251 */ 0x85, 0xFB,         //          STA     POINTH
/* 0253 */ 0xA6, 0xE0,         //  OVR1:   LDX     RND             ; number lower..
/* 0255 */ 0xE4, 0xF9,         //          CPX     INH             ; ..than guess?
/* 0257 */ 0x90, 0x08,         //          BCC     OVR2            ; yes, skip
/* 0259 */ 0xA6, 0xFA,         //          LDX     POINTL          ; no,check lo
/* 025B */ 0xE4, 0xF9,         //          CPX     INH
/* 025D */ 0xB0, 0xC4,         //          BCS     GUESS           ; out of range?
/* 025F */ 0x85, 0xFA,         //          STA     POINTL
/* 0261 */ 0xA6, 0xE1,         //  OVR2:   LDX     NGUESS          ; 'guess' number
/* 0263 */ 0xE8,               //          INX                     ; ..plus 1
/* 0264 */ 0xE0, 0xAA,         //          CPX #$AA                ; past limit?
/* 0266 */ 0xF0, 0xB5,         //          BEQ RSET                ; yes, reset
/* 0268 */ 0xD0, 0xB5,         //          BNE NSET
/* 026A */                     //      .END
};
#endif



// ============================================================================
// ============================================================================
// Timer program from The First Book Of KIM-1
// ============================================================================
// ============================================================================
#if _DEMO_TIMER == TRUE
    #if _TARGET == PERSONAL_COMPUTER
        unsigned char timerDemo[100] = {
    #else
        const unsigned char timerDemo[100] PROGMEM = {
    #endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  ;
/* 0000 */                     //  ;    The First Book of KIM-1
/* 0000 */                     //  ;    Timer program on page 106
/* 0000 */                     //  ;
/* 0000 */                     //  #define INH       $F9
/* 0000 */                     //  #define POINTL    $FA
/* 0000 */                     //  #define POINTH    $FB
/* 0000 */                     //
/* 0000 */                     //  #define SCANDS    $1F1F
/* 0000 */                     //  #define GETKEY    $1F6A
/* 0000 */                     //  ;       ROM       $1C00
/* 0000 */                     //  #define ROM       $0003
/* 0000 */                     //
/* 0200 */                     //         .ORG    $0200
/* 0200 */                     //
/* 0200 */ 0xA9, 0x00,         //  BEGN:   LDA     #$00
/* 0202 */ 0x85, 0xF9,         //          STA     INH             ; ZERO DISPLAY
/* 0204 */ 0x85, 0xFA,         //          STA     POINTL
/* 0206 */ 0x85, 0xFB,         //          STA     POINTH
/* 0208 */ 0x20, 0x1F, 0x1F,   //  HOLD:   JSR     SCANDS          ; LIGHT DISPLAY
/* 020B */ 0x20, 0x6A, 0x1F,   //          JSR     GETKEY
/* 020E */ 0xC9, 0x04,         //          CMP     #$04            ; KEY 4?
/* 0210 */ 0xD0, 0x01,         //          BNE     CONT
/* 0212 */                     //  ;---    JMP     $1C64           ; RETURN TO KIM
/* 0212 */ 0x00,               //          BRK                     ; RETURN TO KIM
/* 0213 */ 0xC9, 0x02,         //  CONT:   CMP     #$02            ; KEY 2?
/* 0215 */ 0xF0, 0xE9,         //          BEQ     BEGN            ; BACK TO ZERO
/* 0217 */ 0xC9, 0x01,         //          CMP     #$01            ; KEY 1?
/* 0219 */ 0xD0, 0xED,         //          BNE     HOLD
/* 021B */ 0xA9, 0x40,         //          LDA     #$9C       ;AaW
/* 021D */ 0x8D, 0x06, 0x17,   //          STA     $1706           ; SET TIMER
/* 0220 */ 0x20, 0x1F, 0x1F,   //  DISP:   JSR     SCANDS          ; DISPLAY VALUE
/* 0223 */ 0xAD, 0x07, 0x17,   //  CLCK:   LDA     $1707           ; CHECK TIMER
/* 0226 */ 0xF0, 0xFB,         //          BEQ     CLCK
/* 0228 */ 0x85, 0x03,         //          STA     ROM             ; DELAY 4 MICROSEC.
/* 022A */ 0xA9, 0x40,         //          LDA     #$9C       ;AaW
/* 022C */ 0x8D, 0x06, 0x17,   //          STA     $1706           ; SET TIMER
/* 022F */ 0x18,               //          CLC
/* 0230 */ 0xF8,               //          SED                     ; SET FLAGS
/* 0231 */ 0xA5, 0xF9,         //          LDA     INH
/* 0233 */ 0x69, 0x01,         //          ADC     #$01            ; INC. 100THS
/* 0235 */ 0x85, 0xF9,         //          STA     INH
/* 0237 */ 0xA5, 0xFA,         //          LDA     POINTL
/* 0239 */ 0x69, 0x00,         //          ADC     #$00            ; INC. SECONDS
/* 023B */ 0x85, 0xFA,         //          STA     POINTL
/* 023D */ 0xC9, 0x60,         //          CMP     #$60            ; STOP AT 60
/* 023F */ 0xD0, 0x0B,         //          BNE     CKEY
/* 0241 */ 0xA9, 0x00,         //          LDA     #$00
/* 0243 */ 0x85, 0xFA,         //          STA     POINTL          ; ZERO SECONDS
/* 0245 */ 0xA5, 0xFB,         //          LDA     POINTH
/* 0247 */ 0x18,               //          CLC
/* 0248 */ 0x69, 0x01,         //          ADC     #$01            ; INC. MINUTES
/* 024A */ 0x85, 0xFB,         //          STA     POINTH
/* 024C */ 0xD8,               //  CKEY:   CLD
/* 024D */ 0x20, 0x6A, 0x1F,   //          JSR     GETKEY          ; READ KEYBOARD
/* 0250 */ 0xC9, 0x00,         //          CMP     #$00            ; KEY 0?
/* 0252 */ 0xD0, 0xCC,         //          BNE     DISP
/* 0254 */ 0xF0, 0xB2,         //          BEQ     HOLD            ; STOP
/* 0256 */                     //         .end
};       // testProgram()
#endif




// ============================================================================
// ============================================================================
// Timer program from The First Book Of KIM-1
// ============================================================================
// ============================================================================
#if _DEMO_FIRST == TRUE
    #if _TARGET == PERSONAL_COMPUTER
        unsigned char fbkDemo[9] = {
    #else
        const unsigned char fbkDemo[9] PROGMEM = {
    #endif
	0xA5, 0x10, 0xA6, 0x11, 0x85, 0x11, 0x86, 0x10, 0x00
};
#endif






// ============================================================================
// ============================================================================
// Apple-1 Basic
// ============================================================================
// ============================================================================
#if _INCL_A1BASIC == TRUE
    #if _TARGET == PERSONAL_COMPUTER
        unsigned char apple1ROM[4096] = {
    #else
        const unsigned char apple1ROM[4096] PROGMEM = {
    #endif
/*  * /	0x-,	0xit,	0x2a,		/ * 15 */
/*  * /	0x:,	0xa1,	0xas,		*/
/*  * /	0xe:,	0xa1,	0xas,		*/
/*  */					
/*  */					/* ; Apple 1 BASIC */
/*  */					/* ; */
/*  */					/* ; Modifications to build with CC65 by Jeff Tranter <tranter@po */
/*  */					/* ; */
/*  */					/* ; Apple 1 BASIC was written by Steve Wozniak */
/*  */					/* ; Uses disassembly copyright 2003 Eric Smith <eric@brouhaha.co */
/*  */					/* ; http://www.brouhaha.com/~eric/retrocomputing/apple/apple1/ba */
/*  */					
/*  */					/* Z1d     =       $1D */
/*  */					/* ch      =       $24     ; horizontal cursor location */
/*  */					/* var     =       $48 */
/*  */					/* lomem   =       $4A     ; lower limit of memory used by BASIC */
/*  */					/* himem   =       $4C     ; upper limit of memory used by BASIC */
/*  */					/* rnd     =       $4E     ; random number (2 bytes) */
/*  */					
/*  */					/* ; The noun stack and syntax stack appear to overlap, which is */
/*  */					/* ; they apparently are not used simultaneously. */
/*  */					
/*  */					/* ; The noun stack size appears to be 32 entries, based on LDX # */
/*  */					/* ; instruction at e67f.  However, there seems to be enough room */
/*  */					/* ; another 8 entries.  The noun stack builds down from noun_stk */
/*  */					/* ; to noun_stk_<part>+$00, indexed by the X register. */
/*  */					
/*  */					/* ; Noun stack usage appears to be: */
/*  */					/* ;   integer: */
/*  */					/* ;       (noun_stk_h_int,noun_stk_l) = value */
/*  */					/* ;       noun_stk_h_str = 0 */
/*  */					/* ;   string: */
/*  */					/* ;       (noun_stk_h_str,noun_stk_l) = pointer to string */
/*  */					/* ;       noun_stk_h_int = any */
/*  */					/* ; Since noun_stk_h_str determines whether stack entry is integ */
/*  */					/* ; strings can't start in zero page. */
/*  */					
/*  */					/* noun_stk_l =    $50 */
/*  */					/* syn_stk_h =     $58     ; through $77 */
/*  */					/* noun_stk_h_str = $78 */
/*  */					/* syn_stk_l  =    $80     ; through $9F */
/*  */					/* noun_stk_h_int = $A0 */
/*  */					/* txtndxstk  =    $A8     ; through $C7 */
/*  */					/* text_index =    $C8     ; index into text being tokenized (in */
/*  */					/* leadbl  =       $C9     ; leading blanks */
/*  */					/* pp      =       $CA     ; pointer to end of program (2 bytes) */
/*  */					/* pv      =       $CC     ; pointer to end of variable storage ( */
/*  */					/* acc     =       $CE     ; (2 bytes) */
/*  */					/* srch    =       $D0 */
/*  */					/* tokndxstk =     $D1 */
/*  */					/* srch2   =       $D2 */
/*  */					/* if_flag =       $D4 */
/*  */					/* cr_flag =       $D5 */
/*  */					/* current_verb =  $D6 */
/*  */					/* precedence =    $D7 */
/*  */					/* x_save  =       $D8 */
/*  */					/* run_flag =      $D9 */
/*  */					/* aux     =       $DA */
/*  */					/* pline   =       $DC     ; pointer to current program line (2 b */
/*  */					/* pverb   =       $E0     ; pointer to current verb (2 bytes) */
/*  */					/* p1      =       $E2 */
/*  */					/* p2      =       $E4 */
/*  */					/* p3      =       $E6 */
/*  */					/* token_index =   $F1    ; pointer used to write tokens into buf */
/*  */					/* pcon    =       $F2    ; temp used in decimal output (2 bytes) */
/*  */					/* auto_inc =      $F4 */
/*  */					/* auto_ln =       $F6 */
/*  */					/* auto_flag =     $F8 */
/*  */					/* char    =       $F9 */
/*  */					/* leadzr  =       $FA */
/*  */					/* for_nest_count = $FB    ; count of active (nested) FOR loops */
/*  */					/* gosub_nest_count = $FC  ; count of active (nested) subroutines */
/*  */					/* synstkdx =      $FD */
/*  */					/* synpag  =       $FE */
/*  */					
/*  */					/* ; GOSUB stack, max eight entries */
/*  */					/* ; note that the Apple II version has sixteen entries */
/*  */					/* gstk_pverbl     =       $0100    ; saved pverb */
/*  */					/* gstk_pverbh     =       $0108 */
/*  */					/* gstk_plinel     =       $0110    ; saved pline */
/*  */					/* gstk_plineh     =       $0118 */
/*  */					
/*  */					/* ; FOR stack, max eight entries */
/*  */					/* ; note that the Apple II version has sixteen entries */
/*  */					/* fstk_varl       =       $0120   ; pointer to index variable */
/*  */					/* fstk_varh       =       $0128 */
/*  */					/* fstk_stepl      =       $0130   ; step value */
/*  */					/* fstk_steph      =       $0138 */
/*  */					/* fstk_plinel     =       $0140   ; saved pline */
/*  */					/* fstk_plineh     =       $0148 */
/*  */					/* fstk_pverbl     =       $0150   ; saved pverb */
/*  */					/* fstk_pverbh     =       $0158 */
/*  */					/* fstk_tol        =       $0160   ; "to" (limit) value */
/*  */					/* fstk_toh        =       $0168 */
/*  */					/* buffer  =       $0200 */
/*  */					/* KBD     =       $D010 */
/*  */					/* KBDCR   =       $D011 */
/*  */					/* DSP     =       $D012 */
/*  */					
/*  */					/* .org    $E000 */
/* E000 */					/* .export START */
/* E000 */	0x4C,	0xB0,	0xE2,		/* START:  JMP     cold            ; BASIC cold start entry point */
/* E003 */					
/* E003 */					/* ; Get character for keyboard, return in A. */
//KBDCR point =================================================================================
/* E003 */	0xAD,	0x11,	0xD0,		/* rdkey:  LDA     KBDCR           ; Read control register */
/* E006 */	0x10,	0xFB,			/* BPL     rdkey           ; Loop if no key pressed */
//KBD patch ===================================================================================
/* E008 */	0xAD,	0x10,	0xD0,		/* LDA     KBD             ; Read key data */
/* E00B */	0x60,				/* RTS                     ; and return */
/* E00C */					
/* E00C */	0x8A,				/* Se00c:  TXA */
/* E00D */	0x29,	0x20,			/* AND     #$20 */
/* E00F */	0xF0,	0x23,			/* BEQ     Le034 */
/* E011 */					
/* E011 */	0xA9,	0xA0,			/* Se011:  LDA     #$A0 */
/* E013 */	0x85,	0xE4,			/* STA     p2 */
/* E015 */	0x4C,	0xC9,	0xE3,		/* JMP     cout */
/* E018 */					
/* E018 */	0xA9,	0x20,			/* Se018:  LDA     #$20 */
/* E01A */					
/* E01A */	0xC5,	0x24,			/* Se01a:  CMP     ch */
/* E01C */	0xB0,	0x0C,			/* BCS     nextbyte */
/* E01E */	0xA9,	0x8D,			/* LDA     #$8D */
/* E020 */	0xA0,	0x07,			/* LDY     #$07 */
/* E022 */	0x20,	0xC9,	0xE3,		/* Le022:  JSR     cout */
/* E025 */	0xA9,	0xA0,			/* LDA     #$A0 */
/* E027 */	0x88,				/* DEY */
/* E028 */	0xD0,	0xF8,			/* BNE     Le022 */
/* E02A */					
/* E02A */	0xA0,	0x00,			/* nextbyte:       LDY     #$00 */
/* E02C */	0xB1,	0xE2,			/* LDA     (p1),Y */
/* E02E */	0xE6,	0xE2,			/* INC     p1 */
/* E030 */	0xD0,	0x02,			/* BNE     Le034 */
/* E032 */	0xE6,	0xE3,			/* INC     p1+1 */
/* E034 */	0x60,				/* Le034:  RTS */
/* E035 */					
/* E035 */					/* ; token $75 - "," in LIST command */
/* E035 */	0x20,	0x15,	0xE7,		/* list_comman:    JSR     get16bit */
/* E038 */	0x20,	0x76,	0xE5,		/* JSR     find_line2 */
/* E03B */	0xA5,	0xE2,			/* Le03b:  LDA     p1 */
/* E03D */	0xC5,	0xE6,			/* CMP     p3 */
/* E03F */	0xA5,	0xE3,			/* LDA     p1+1 */
/* E041 */	0xE5,	0xE7,			/* SBC     p3+1 */
/* E043 */	0xB0,	0xEF,			/* BCS     Le034 */
/* E045 */	0x20,	0x6D,	0xE0,		/* JSR     list_line */
/* E048 */	0x4C,	0x3B,	0xE0,		/* JMP     Le03b */
/* E04B */					
/* E04B */					/* ; token $76 - LIST command w/ no args */
/* E04B */	0xA5,	0xCA,			/* list_all:       LDA     pp */
/* E04D */	0x85,	0xE2,			/* STA     p1 */
/* E04F */	0xA5,	0xCB,			/* LDA     pp+1 */
/* E051 */	0x85,	0xE3,			/* STA     p1+1 */
/* E053 */	0xA5,	0x4C,			/* LDA     himem */
/* E055 */	0x85,	0xE6,			/* STA     p3 */
/* E057 */	0xA5,	0x4D,			/* LDA     himem+1 */
/* E059 */	0x85,	0xE7,			/* STA     p3+1 */
/* E05B */	0xD0,	0xDE,			/* BNE     Le03b */
/* E05D */					
/* E05D */					/* ; token $74 - LIST command w/ line number(s) */
/* E05D */	0x20,	0x15,	0xE7,		/* list_cmd:       JSR     get16bit */
/* E060 */	0x20,	0x6D,	0xE5,		/* JSR     find_line */
/* E063 */	0xA5,	0xE4,			/* LDA     p2 */
/* E065 */	0x85,	0xE2,			/* STA     p1 */
/* E067 */	0xA5,	0xE5,			/* LDA     p2+1 */
/* E069 */	0x85,	0xE3,			/* STA     p1+1 */
/* E06B */	0xB0,	0xC7,			/* BCS     Le034 */
/* E06D */					
/* E06D */					/* ; list one program line */
/* E06D */	0x86,	0xD8,			/* list_line:      STX     x_save */
/* E06F */	0xA9,	0xA0,			/* LDA     #$A0 */
/* E071 */	0x85,	0xFA,			/* STA     leadzr */
/* E073 */	0x20,	0x2A,	0xE0,		/* JSR     nextbyte */
/* E076 */	0x98,				/* TYA */
/* E077 */					
/* E077 */					/* ; list an integer (line number or literal) */
/* E077 */	0x85,	0xE4,			/* list_int:       STA     p2 */
/* E079 */	0x20,	0x2A,	0xE0,		/* JSR     nextbyte */
/* E07C */	0xAA,				/* TAX */
/* E07D */	0x20,	0x2A,	0xE0,		/* JSR     nextbyte */
/* E080 */	0x20,	0x1B,	0xE5,		/* JSR     prdec */
/* E083 */	0x20,	0x18,	0xE0,		/* Le083:  JSR     Se018 */
/* E086 */	0x84,	0xFA,			/* STY     leadzr */
/* E088 */	0xAA,				/* TAX */
/* E089 */	0x10,	0x18,			/* BPL     list_token */
/* E08B */	0x0A,				/* ASL */
/* E08C */	0x10,	0xE9,			/* BPL     list_int */
/* E08E */	0xA5,	0xE4,			/* LDA     p2 */
/* E090 */	0xD0,	0x03,			/* BNE     Le095 */
/* E092 */	0x20,	0x11,	0xE0,		/* JSR     Se011 */
/* E095 */	0x8A,				/* Le095:  TXA */
/* E096 */	0x20,	0xC9,	0xE3,		/* Le096:  JSR     cout */
/* E099 */	0xA9,	0x25,			/* Le099:  LDA     #$25 */
/* E09B */	0x20,	0x1A,	0xE0,		/* JSR     Se01a */
/* E09E */	0xAA,				/* TAX */
/* E09F */	0x30,	0xF5,			/* BMI     Le096 */
/* E0A1 */	0x85,	0xE4,			/* STA     p2 */
/* E0A3 */					
/* E0A3 */					/* ; list a single token */
/* E0A3 */	0xC9,	0x01,			/* list_token:     CMP     #$01 */
/* E0A5 */	0xD0,	0x05,			/* BNE     Le0ac */
/* E0A7 */	0xA6,	0xD8,			/* LDX     x_save */
/* E0A9 */	0x4C,	0xCD,	0xE3,		/* JMP     crout */
/* E0AC */	0x48,				/* Le0ac:  PHA */
/* E0AD */	0x84,	0xCE,			/* STY     acc */
/* E0AF */	0xA2,	0xED,			/* LDX     #$ED */
/* E0B1 */	0x86,	0xCF,			/* STX     acc+1 */
/* E0B3 */	0xC9,	0x51,			/* CMP     #$51 */
/* E0B5 */	0x90,	0x04,			/* BCC     Le0bb */
/* E0B7 */	0xC6,	0xCF,			/* DEC     acc+1 */
/* E0B9 */	0xE9,	0x50,			/* SBC     #$50 */
/* E0BB */	0x48,				/* Le0bb:  PHA */
/* E0BC */	0xB1,	0xCE,			/* LDA     (acc),Y */
/* E0BE */	0xAA,				/* Le0be:  TAX */
/* E0BF */	0x88,				/* DEY */
/* E0C0 */	0xB1,	0xCE,			/* LDA     (acc),Y */
/* E0C2 */	0x10,	0xFA,			/* BPL     Le0be */
/* E0C4 */	0xE0,	0xC0,			/* CPX     #$C0 */
/* E0C6 */	0xB0,	0x04,			/* BCS     Le0cc */
/* E0C8 */	0xE0,	0x00,			/* CPX     #$00 */
/* E0CA */	0x30,	0xF2,			/* BMI     Le0be */
/* E0CC */	0xAA,				/* Le0cc:  TAX */
/* E0CD */	0x68,				/* PLA */
/* E0CE */	0xE9,	0x01,			/* SBC     #$01 */
/* E0D0 */	0xD0,	0xE9,			/* BNE     Le0bb */
/* E0D2 */	0x24,	0xE4,			/* BIT     p2 */
/* E0D4 */	0x30,	0x03,			/* BMI     Le0d9 */
/* E0D6 */	0x20,	0xF8,	0xEF,		/* JSR     Seff8 */
/* E0D9 */	0xB1,	0xCE,			/* Le0d9:  LDA     (acc),Y */
/* E0DB */	0x10,	0x10,			/* BPL     Le0ed */
/* E0DD */	0xAA,				/* TAX */
/* E0DE */	0x29,	0x3F,			/* AND     #$3F */
/* E0E0 */	0x85,	0xE4,			/* STA     p2 */
/* E0E2 */	0x18,				/* CLC */
/* E0E3 */	0x69,	0xA0,			/* ADC     #$A0 */
/* E0E5 */	0x20,	0xC9,	0xE3,		/* JSR     cout */
/* E0E8 */	0x88,				/* DEY */
/* E0E9 */	0xE0,	0xC0,			/* CPX     #$C0 */
/* E0EB */	0x90,	0xEC,			/* BCC     Le0d9 */
/* E0ED */	0x20,	0x0C,	0xE0,		/* Le0ed:  JSR     Se00c */
/* E0F0 */	0x68,				/* PLA */
/* E0F1 */	0xC9,	0x5D,			/* CMP     #$5D */
/* E0F3 */	0xF0,	0xA4,			/* BEQ     Le099 */
/* E0F5 */	0xC9,	0x28,			/* CMP     #$28 */
/* E0F7 */	0xD0,	0x8A,			/* BNE     Le083 */
/* E0F9 */	0xF0,	0x9E,			/* BEQ     Le099 */
/* E0FB */					
/* E0FB */					/* ; token $2A - left paren for substring like A$(3,5) */
/* E0FB */	0x20,	0x18,	0xE1,		/* paren_substr:   JSR     Se118 */
/* E0FE */	0x95,	0x50,			/* STA     noun_stk_l,X */
/* E100 */	0xD5,	0x78,			/* CMP     noun_stk_h_str,X */
/* E102 */	0x90,	0x11,			/* Le102:  BCC     Le115 */
/* E104 */	0xA0,	0x2B,			/* string_err:     LDY     #$2B */
/* E106 */	0x4C,	0xE0,	0xE3,		/* go_errmess_1:   JMP     print_err_msg */
/* E109 */					
/* E109 */					/* ; token $2B - comma for substring like A$(3,5) */
/* E109 */	0x20,	0x34,	0xEE,		/* comma_substr:   JSR     getbyte */
/* E10C */	0xD5,	0x50,			/* CMP     noun_stk_l,X */
/* E10E */	0x90,	0xF4,			/* BCC     string_err */
/* E110 */	0x20,	0xE4,	0xEF,		/* JSR     Sefe4 */
/* E113 */	0x95,	0x78,			/* STA     noun_stk_h_str,X */
/* E115 */	0x4C,	0x23,	0xE8,		/* Le115:  JMP     left_paren */
/* E118 */					
/* E118 */	0x20,	0x34,	0xEE,		/* Se118:  JSR     getbyte */
/* E11B */	0xF0,	0xE7,			/* BEQ     string_err */
/* E11D */	0x38,				/* SEC */
/* E11E */	0xE9,	0x01,			/* SBC     #$01 */
/* E120 */	0x60,				/* RTS */
/* E121 */					
/* E121 */					/* ; token $42 - left paren for string array as dest */
/* E121 */					/* ; A$(1)="FOO" */
/* E121 */	0x20,	0x18,	0xE1,		/* str_arr_dest:   JSR     Se118 */
/* E124 */	0x95,	0x50,			/* STA     noun_stk_l,X */
/* E126 */	0x18,				/* CLC */
/* E127 */	0xF5,	0x78,			/* SBC     noun_stk_h_str,X */
/* E129 */	0x4C,	0x02,	0xE1,		/* JMP     Le102 */
/* E12C */	0xA0,	0x14,			/* Le12c:  LDY     #$14 */
/* E12E */	0xD0,	0xD6,			/* BNE     go_errmess_1 */
/* E130 */					
/* E130 */					/* ; token $43 - comma, next var in DIM statement is string */
/* E130 */					/* ; token $4E - "DIM", next var in DIM is string */
/* E130 */	0x20,	0x18,	0xE1,		/* dim_str:        JSR     Se118 */
/* E133 */	0xE8,				/* INX */
/* E134 */	0xB5,	0x50,			/* Le134:  LDA     noun_stk_l,X */
/* E136 */	0x85,	0xDA,			/* STA     aux */
/* E138 */	0x65,	0xCE,			/* ADC     acc */
/* E13A */	0x48,				/* PHA */
/* E13B */	0xA8,				/* TAY */
/* E13C */	0xB5,	0x78,			/* LDA     noun_stk_h_str,X */
/* E13E */	0x85,	0xDB,			/* STA     aux+1 */
/* E140 */	0x65,	0xCF,			/* ADC     acc+1 */
/* E142 */	0x48,				/* PHA */
/* E143 */	0xC4,	0xCA,			/* CPY     pp */
/* E145 */	0xE5,	0xCB,			/* SBC     pp+1 */
/* E147 */	0xB0,	0xE3,			/* BCS     Le12c */
/* E149 */	0xA5,	0xDA,			/* LDA     aux */
/* E14B */	0x69,	0xFE,			/* ADC     #$FE */
/* E14D */	0x85,	0xDA,			/* STA     aux */
/* E14F */	0xA9,	0xFF,			/* LDA     #$FF */
/* E151 */	0xA8,				/* TAY */
/* E152 */	0x65,	0xDB,			/* ADC     aux+1 */
/* E154 */	0x85,	0xDB,			/* STA     aux+1 */
/* E156 */	0xC8,				/* Le156:  INY */
/* E157 */	0xB1,	0xDA,			/* LDA     (aux),Y */
/* E159 */	0xD9,	0xCC,	0x00,		/* CMP     pv,Y */
/* E15C */	0xD0,	0x0F,			/* BNE     Le16d */
/* E15E */	0x98,				/* TYA */
/* E15F */	0xF0,	0xF5,			/* BEQ     Le156 */
/* E161 */	0x68,				/* Le161:  PLA */
/* E162 */	0x91,	0xDA,			/* STA     (aux),Y */
/* E164 */	0x99,	0xCC,	0x00,		/* STA     pv,Y */
/* E167 */	0x88,				/* DEY */
/* E168 */	0x10,	0xF7,			/* BPL     Le161 */
/* E16A */	0xE8,				/* INX */
/* E16B */	0x60,				/* RTS */
/* E16C */	0xEA,				/* NOP */
/* E16D */	0xA0,	0x80,			/* Le16d:  LDY     #$80 */
/* E16F */	0xD0,	0x95,			/* Le16f:  BNE     go_errmess_1 */
/* E171 */					
/* E171 */					/* ; token ??? */
/* E171 */	0xA9,	0x00,			/* input_str:      LDA     #$00 */
/* E173 */	0x20,	0x0A,	0xE7,		/* JSR     push_a_noun_stk */
/* E176 */	0xA0,	0x02,			/* LDY     #$02 */
/* E178 */	0x94,	0x78,			/* STY     noun_stk_h_str,X */
/* E17A */	0x20,	0x0A,	0xE7,		/* JSR     push_a_noun_stk */
/* E17D */	0xA9,	0xBF,			/* LDA     #$BF                    ; '?' */
/* E17F */	0x20,	0xC9,	0xE3,		/* JSR     cout */
/* E182 */	0xA0,	0x00,			/* LDY     #$00 */
/* E184 */	0x20,	0x9E,	0xE2,		/* JSR     read_line */
/* E187 */	0x94,	0x78,			/* STY     noun_stk_h_str,X */
/* E189 */	0xEA,				/* NOP */
/* E18A */	0xEA,				/* NOP */
/* E18B */	0xEA,				/* NOP */
/* E18C */					
/* E18C */					/* ; token $70 - string literal */
/* E18C */	0xB5,	0x51,			/* string_lit:     LDA     noun_stk_l+1,X */
/* E18E */	0x85,	0xCE,			/* STA     acc */
/* E190 */	0xB5,	0x79,			/* LDA     noun_stk_h_str+1,X */
/* E192 */	0x85,	0xCF,			/* STA     acc+1 */
/* E194 */	0xE8,				/* INX */
/* E195 */	0xE8,				/* INX */
/* E196 */	0x20,	0xBC,	0xE1,		/* JSR     Se1bc */
/* E199 */	0xB5,	0x4E,			/* Le199:  LDA     rnd,X */
/* E19B */	0xD5,	0x76,			/* CMP     syn_stk_h+30,X */
/* E19D */	0xB0,	0x15,			/* BCS     Le1b4 */
/* E19F */	0xF6,	0x4E,			/* INC     rnd,X */
/* E1A1 */	0xA8,				/* TAY */
/* E1A2 */	0xB1,	0xCE,			/* LDA     (acc),Y */
/* E1A4 */	0xB4,	0x50,			/* LDY     noun_stk_l,X */
/* E1A6 */	0xC4,	0xE4,			/* CPY     p2 */
/* E1A8 */	0x90,	0x04,			/* BCC     Le1ae */
/* E1AA */	0xA0,	0x83,			/* LDY     #$83 */
/* E1AC */	0xD0,	0xC1,			/* BNE     Le16f */
/* E1AE */	0x91,	0xDA,			/* Le1ae:  STA     (aux),Y */
/* E1B0 */	0xF6,	0x50,			/* INC     noun_stk_l,X */
/* E1B2 */	0x90,	0xE5,			/* BCC     Le199 */
/* E1B4 */	0xB4,	0x50,			/* Le1b4:  LDY     noun_stk_l,X */
/* E1B6 */	0x8A,				/* TXA */
/* E1B7 */	0x91,	0xDA,			/* STA     (aux),Y */
/* E1B9 */	0xE8,				/* INX */
/* E1BA */	0xE8,				/* INX */
/* E1BB */	0x60,				/* RTS */
/* E1BC */					
/* E1BC */	0xB5,	0x51,			/* Se1bc:  LDA     noun_stk_l+1,X */
/* E1BE */	0x85,	0xDA,			/* STA     aux */
/* E1C0 */	0x38,				/* SEC */
/* E1C1 */	0xE9,	0x02,			/* SBC     #$02 */
/* E1C3 */	0x85,	0xE4,			/* STA     p2 */
/* E1C5 */	0xB5,	0x79,			/* LDA     noun_stk_h_str+1,X */
/* E1C7 */	0x85,	0xDB,			/* STA     aux+1 */
/* E1C9 */	0xE9,	0x00,			/* SBC     #$00 */
/* E1CB */	0x85,	0xE5,			/* STA     p2+1 */
/* E1CD */	0xA0,	0x00,			/* LDY     #$00 */
/* E1CF */	0xB1,	0xE4,			/* LDA     (p2),Y */
/* E1D1 */	0x18,				/* CLC */
/* E1D2 */	0xE5,	0xDA,			/* SBC     aux */
/* E1D4 */	0x85,	0xE4,			/* STA     p2 */
/* E1D6 */	0x60,				/* RTS */
/* E1D7 */					
/* E1D7 */					/* ; token $39 - "=" for string equality operator */
/* E1D7 */	0xB5,	0x53,			/* string_eq:      LDA     noun_stk_l+3,X */
/* E1D9 */	0x85,	0xCE,			/* STA     acc */
/* E1DB */	0xB5,	0x7B,			/* LDA     noun_stk_h_str+3,X */
/* E1DD */	0x85,	0xCF,			/* STA     acc+1 */
/* E1DF */	0xB5,	0x51,			/* LDA     noun_stk_l+1,X */
/* E1E1 */	0x85,	0xDA,			/* STA     aux */
/* E1E3 */	0xB5,	0x79,			/* LDA     noun_stk_h_str+1,X */
/* E1E5 */	0x85,	0xDB,			/* STA     aux+1 */
/* E1E7 */	0xE8,				/* INX */
/* E1E8 */	0xE8,				/* INX */
/* E1E9 */	0xE8,				/* INX */
/* E1EA */	0xA0,	0x00,			/* LDY     #$00 */
/* E1EC */	0x94,	0x78,			/* STY     noun_stk_h_str,X */
/* E1EE */	0x94,	0xA0,			/* STY     noun_stk_h_int,X */
/* E1F0 */	0xC8,				/* INY */
/* E1F1 */	0x94,	0x50,			/* STY     noun_stk_l,X */
/* E1F3 */	0xB5,	0x4D,			/* Le1f3:  LDA     himem+1,X */
/* E1F5 */	0xD5,	0x75,			/* CMP     syn_stk_h+29,X */
/* E1F7 */	0x08,				/* PHP */
/* E1F8 */	0x48,				/* PHA */
/* E1F9 */	0xB5,	0x4F,			/* LDA     rnd+1,X */
/* E1FB */	0xD5,	0x77,			/* CMP     syn_stk_h+31,X */
/* E1FD */	0x90,	0x07,			/* BCC     Le206 */
/* E1FF */	0x68,				/* PLA */
/* E200 */	0x28,				/* PLP */
/* E201 */	0xB0,	0x02,			/* BCS     Le205 */
/* E203 */	0x56,	0x50,			/* Le203:  LSR     noun_stk_l,X */
/* E205 */	0x60,				/* Le205:  RTS */
/* E206 */	0xA8,				/* Le206:  TAY */
/* E207 */	0xB1,	0xCE,			/* LDA     (acc),Y */
/* E209 */	0x85,	0xE4,			/* STA     p2 */
/* E20B */	0x68,				/* PLA */
/* E20C */	0xA8,				/* TAY */
/* E20D */	0x28,				/* PLP */
/* E20E */	0xB0,	0xF3,			/* BCS     Le203 */
/* E210 */	0xB1,	0xDA,			/* LDA     (aux),Y */
/* E212 */	0xC5,	0xE4,			/* CMP     p2 */
/* E214 */	0xD0,	0xED,			/* BNE     Le203 */
/* E216 */	0xF6,	0x4F,			/* INC     rnd+1,X */
/* E218 */	0xF6,	0x4D,			/* INC     himem+1,X */
/* E21A */	0xB0,	0xD7,			/* BCS     Le1f3 */
/* E21C */					
/* E21C */					/* ; token $3A - "#" for string inequality operator */
/* E21C */	0x20,	0xD7,	0xE1,		/* string_neq:     JSR     string_eq */
/* E21F */	0x4C,	0x36,	0xE7,		/* JMP     not_op */
/* E222 */					
/* E222 */					/* ; token $14 - "*" for numeric multiplication */
/* E222 */	0x20,	0x54,	0xE2,		/* mult_op:        JSR     Se254 */
/* E225 */	0x06,	0xCE,			/* Le225:  ASL     acc */
/* E227 */	0x26,	0xCF,			/* ROL     acc+1 */
/* E229 */	0x90,	0x0D,			/* BCC     Le238 */
/* E22B */	0x18,				/* CLC */
/* E22C */	0xA5,	0xE6,			/* LDA     p3 */
/* E22E */	0x65,	0xDA,			/* ADC     aux */
/* E230 */	0x85,	0xE6,			/* STA     p3 */
/* E232 */	0xA5,	0xE7,			/* LDA     p3+1 */
/* E234 */	0x65,	0xDB,			/* ADC     aux+1 */
/* E236 */	0x85,	0xE7,			/* STA     p3+1 */
/* E238 */	0x88,				/* Le238:  DEY */
/* E239 */	0xF0,	0x09,			/* BEQ     Le244 */
/* E23B */	0x06,	0xE6,			/* ASL     p3 */
/* E23D */	0x26,	0xE7,			/* ROL     p3+1 */
/* E23F */	0x10,	0xE4,			/* BPL     Le225 */
/* E241 */	0x4C,	0x7E,	0xE7,		/* JMP     Le77e */
/* E244 */	0xA5,	0xE6,			/* Le244:  LDA     p3 */
/* E246 */	0x20,	0x08,	0xE7,		/* JSR     push_ya_noun_stk */
/* E249 */	0xA5,	0xE7,			/* LDA     p3+1 */
/* E24B */	0x95,	0xA0,			/* STA     noun_stk_h_int,X */
/* E24D */	0x06,	0xE5,			/* ASL     p2+1 */
/* E24F */	0x90,	0x28,			/* BCC     Le279 */
/* E251 */	0x4C,	0x6F,	0xE7,		/* JMP     negate */
/* E254 */					
/* E254 */	0xA9,	0x55,			/* Se254:  LDA     #$55 */
/* E256 */	0x85,	0xE5,			/* STA     p2+1 */
/* E258 */	0x20,	0x5B,	0xE2,		/* JSR     Se25b */
/* E25B */					
/* E25B */	0xA5,	0xCE,			/* Se25b:  LDA     acc */
/* E25D */	0x85,	0xDA,			/* STA     aux */
/* E25F */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E261 */	0x85,	0xDB,			/* STA     aux+1 */
/* E263 */	0x20,	0x15,	0xE7,		/* JSR     get16bit */
/* E266 */	0x84,	0xE6,			/* STY     p3 */
/* E268 */	0x84,	0xE7,			/* STY     p3+1 */
/* E26A */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E26C */	0x10,	0x09,			/* BPL     Le277 */
/* E26E */	0xCA,				/* DEX */
/* E26F */	0x06,	0xE5,			/* ASL     p2+1 */
/* E271 */	0x20,	0x6F,	0xE7,		/* JSR     negate */
/* E274 */	0x20,	0x15,	0xE7,		/* JSR     get16bit */
/* E277 */	0xA0,	0x10,			/* Le277:  LDY     #$10 */
/* E279 */	0x60,				/* Le279:  RTS */
/* E27A */					
/* E27A */					/* ; token $1f - "MOD" */
/* E27A */	0x20,	0x6C,	0xEE,		/* mod_op: JSR     See6c */
/* E27D */	0xF0,	0xC5,			/* BEQ     Le244 */
/* E27F */	0xFF,				/* .byte   $FF */
/* E280 */	0xC9,	0x84,			/* Le280:  CMP     #$84 */
/* E282 */	0xD0,	0x02,			/* BNE     Le286 */
/* E284 */	0x46,	0xF8,			/* LSR     auto_flag */
/* E286 */	0xC9,	0xDF,			/* Le286:  CMP     #$DF */
/* E288 */	0xF0,	0x11,			/* BEQ     Le29b */
/* E28A */	0xC9,	0x9B,			/* CMP     #$9B */
/* E28C */	0xF0,	0x06,			/* BEQ     Le294 */
/* E28E */	0x99,	0x00,	0x02,		/* STA     buffer,Y */
/* E291 */	0xC8,				/* INY */
/* E292 */	0x10,	0x0A,			/* BPL     read_line */
/* E294 */	0xA0,	0x8B,			/* Le294:  LDY     #$8B */
/* E296 */	0x20,	0xC4,	0xE3,		/* JSR     Se3c4 */
/* E299 */					
/* E299 */	0xA0,	0x01,			/* Se299:  LDY     #$01 */
/* E29B */	0x88,				/* Le29b:  DEY */
/* E29C */	0x30,	0xF6,			/* BMI     Le294 */
/* E29E */					
/* E29E */					/* ; read a line from keyboard (using rdkey) into buffer */
/* E29E */	0x20,	0x03,	0xE0,		/* read_line:      JSR     rdkey */
/* E2A1 */	0xEA,				/* NOP */
/* E2A2 */	0xEA,				/* NOP */
/* E2A3 */	0x20,	0xC9,	0xE3,		/* JSR     cout */
// ----------------------------------------------------------------------- 8d 8a point 2/2
/* E2A6 */	0xC9,	0x8D,			/* CMP     #$8D */
/* E2A8 */	0xD0,	0xD6,			/* BNE     Le280 */
/* E2AA */	0xA9,	0xDF,			/* LDA     #$DF */
/* E2AC */	0x99,	0x00,	0x02,		/* STA     buffer,Y */
/* E2AF */	0x60,				/* RTS */
/* E2B0 */	0x20,	0xD3,	0xEF,		/* cold:   JSR     mem_init_4k */
/* E2B3 */					/* .export warm */
/* E2B3 */	0x20,	0xCD,	0xE3,		/* warm:   JSR     crout           ; BASIC warm start entry point */
/* E2B6 */	0x46,	0xD9,			/* Le2b6:  LSR     run_flag */
/* E2B8 */	0xA9,	0xBE,			/* LDA     #'>'+$80        ; Prompt character (high bit s */
/* E2BA */	0x20,	0xC9,	0xE3,		/* JSR     cout */
/* E2BD */	0xA0,	0x00,			/* LDY     #$00 */
/* E2BF */	0x84,	0xFA,			/* STY     leadzr */
/* E2C1 */	0x24,	0xF8,			/* BIT     auto_flag */
/* E2C3 */	0x10,	0x0C,			/* BPL     Le2d1 */
/* E2C5 */	0xA6,	0xF6,			/* LDX     auto_ln */
/* E2C7 */	0xA5,	0xF7,			/* LDA     auto_ln+1 */
/* E2C9 */	0x20,	0x1B,	0xE5,		/* JSR     prdec */
/* E2CC */	0xA9,	0xA0,			/* LDA     #$A0 */
/* E2CE */	0x20,	0xC9,	0xE3,		/* JSR     cout */
/* E2D1 */	0xA2,	0xFF,			/* Le2d1:  LDX     #$FF */
/* E2D3 */	0x9A,				/* TXS */
/* E2D4 */	0x20,	0x9E,	0xE2,		/* JSR     read_line */
/* E2D7 */	0x84,	0xF1,			/* STY     token_index */
/* E2D9 */	0x8A,				/* TXA */
/* E2DA */	0x85,	0xC8,			/* STA     text_index */
/* E2DC */	0xA2,	0x20,			/* LDX     #$20 */
/* E2DE */	0x20,	0x91,	0xE4,		/* JSR     Se491 */
/* E2E1 */	0xA5,	0xC8,			/* LDA     text_index */
/* E2E3 */	0x69,	0x00,			/* ADC     #$00 */
/* E2E5 */	0x85,	0xE0,			/* STA     pverb */
/* E2E7 */	0xA9,	0x00,			/* LDA     #$00 */
/* E2E9 */	0xAA,				/* TAX */
/* E2EA */	0x69,	0x02,			/* ADC     #$02 */
/* E2EC */	0x85,	0xE1,			/* STA     pverb+1 */
/* E2EE */	0xA1,	0xE0,			/* LDA     (pverb,X) */
/* E2F0 */	0x29,	0xF0,			/* AND     #$F0 */
/* E2F2 */	0xC9,	0xB0,			/* CMP     #$B0 */
/* E2F4 */	0xF0,	0x03,			/* BEQ     Le2f9 */
/* E2F6 */	0x4C,	0x83,	0xE8,		/* JMP     Le883 */
/* E2F9 */	0xA0,	0x02,			/* Le2f9:  LDY     #$02 */
/* E2FB */	0xB1,	0xE0,			/* Le2fb:  LDA     (pverb),Y */
/* E2FD */	0x99,	0xCD,	0x00,		/* STA     pv+1,Y */
/* E300 */	0x88,				/* DEY */
/* E301 */	0xD0,	0xF8,			/* BNE     Le2fb */
/* E303 */	0x20,	0x8A,	0xE3,		/* JSR     Se38a */
/* E306 */	0xA5,	0xF1,			/* LDA     token_index */
/* E308 */	0xE5,	0xC8,			/* SBC     text_index */
/* E30A */	0xC9,	0x04,			/* CMP     #$04 */
/* E30C */	0xF0,	0xA8,			/* BEQ     Le2b6 */
/* E30E */	0x91,	0xE0,			/* STA     (pverb),Y */
/* E310 */	0xA5,	0xCA,			/* LDA     pp */
/* E312 */	0xF1,	0xE0,			/* SBC     (pverb),Y */
/* E314 */	0x85,	0xE4,			/* STA     p2 */
/* E316 */	0xA5,	0xCB,			/* LDA     pp+1 */
/* E318 */	0xE9,	0x00,			/* SBC     #$00 */
/* E31A */	0x85,	0xE5,			/* STA     p2+1 */
/* E31C */	0xA5,	0xE4,			/* LDA     p2 */
/* E31E */	0xC5,	0xCC,			/* CMP     pv */
/* E320 */	0xA5,	0xE5,			/* LDA     p2+1 */
/* E322 */	0xE5,	0xCD,			/* SBC     pv+1 */
/* E324 */	0x90,	0x45,			/* BCC     Le36b */
/* E326 */	0xA5,	0xCA,			/* Le326:  LDA     pp */
/* E328 */	0xF1,	0xE0,			/* SBC     (pverb),Y */
/* E32A */	0x85,	0xE6,			/* STA     p3 */
/* E32C */	0xA5,	0xCB,			/* LDA     pp+1 */
/* E32E */	0xE9,	0x00,			/* SBC     #$00 */
/* E330 */	0x85,	0xE7,			/* STA     p3+1 */
/* E332 */	0xB1,	0xCA,			/* LDA     (pp),Y */
/* E334 */	0x91,	0xE6,			/* STA     (p3),Y */
/* E336 */	0xE6,	0xCA,			/* INC     pp */
/* E338 */	0xD0,	0x02,			/* BNE     Le33c */
/* E33A */	0xE6,	0xCB,			/* INC     pp+1 */
/* E33C */	0xA5,	0xE2,			/* Le33c:  LDA     p1 */
/* E33E */	0xC5,	0xCA,			/* CMP     pp */
/* E340 */	0xA5,	0xE3,			/* LDA     p1+1 */
/* E342 */	0xE5,	0xCB,			/* SBC     pp+1 */
/* E344 */	0xB0,	0xE0,			/* BCS     Le326 */
/* E346 */	0xB5,	0xE4,			/* Le346:  LDA     p2,X */
/* E348 */	0x95,	0xCA,			/* STA     pp,X */
/* E34A */	0xCA,				/* DEX */
/* E34B */	0x10,	0xF9,			/* BPL     Le346 */
/* E34D */	0xB1,	0xE0,			/* LDA     (pverb),Y */
/* E34F */	0xA8,				/* TAY */
/* E350 */	0x88,				/* Le350:  DEY */
/* E351 */	0xB1,	0xE0,			/* LDA     (pverb),Y */
/* E353 */	0x91,	0xE6,			/* STA     (p3),Y */
/* E355 */	0x98,				/* TYA */
/* E356 */	0xD0,	0xF8,			/* BNE     Le350 */
/* E358 */	0x24,	0xF8,			/* BIT     auto_flag */
/* E35A */	0x10,	0x09,			/* BPL     Le365 */
/* E35C */	0xB5,	0xF7,			/* Le35c:  LDA     auto_ln+1,X */
/* E35E */	0x75,	0xF5,			/* ADC     auto_inc+1,X */
/* E360 */	0x95,	0xF7,			/* STA     auto_ln+1,X */
/* E362 */	0xE8,				/* INX */
/* E363 */	0xF0,	0xF7,			/* BEQ     Le35c */
/* E365 */	0x10,	0x7E,			/* Le365:  BPL     Le3e5 */
/* E367 */	0x00,	0x00,	0x00,	0x00,	/* .byte   $00,$00,$00,$00 */
/* E36B */	0xA0,	0x14,			/* Le36b:  LDY     #$14 */
/* E36D */	0xD0,	0x71,			/* BNE     print_err_msg */
/* E36F */					
/* E36F */					/* ; token $0a - "," in DEL command */
/* E36F */	0x20,	0x15,	0xE7,		/* del_comma:      JSR     get16bit */
/* E372 */	0xA5,	0xE2,			/* LDA     p1 */
/* E374 */	0x85,	0xE6,			/* STA     p3 */
/* E376 */	0xA5,	0xE3,			/* LDA     p1+1 */
/* E378 */	0x85,	0xE7,			/* STA     p3+1 */
/* E37A */	0x20,	0x75,	0xE5,		/* JSR     find_line1 */
/* E37D */	0xA5,	0xE2,			/* LDA     p1 */
/* E37F */	0x85,	0xE4,			/* STA     p2 */
/* E381 */	0xA5,	0xE3,			/* LDA     p1+1 */
/* E383 */	0x85,	0xE5,			/* STA     p2+1 */
/* E385 */	0xD0,	0x0E,			/* BNE     Le395 */
/* E387 */					
/* E387 */					/* ; token $09 - "DEL" */
/* E387 */	0x20,	0x15,	0xE7,		/* del_cmd:        JSR     get16bit */
/* E38A */					
/* E38A */	0x20,	0x6D,	0xE5,		/* Se38a:  JSR     find_line */
/* E38D */	0xA5,	0xE6,			/* LDA     p3 */
/* E38F */	0x85,	0xE2,			/* STA     p1 */
/* E391 */	0xA5,	0xE7,			/* LDA     p3+1 */
/* E393 */	0x85,	0xE3,			/* STA     p1+1 */
/* E395 */	0xA0,	0x00,			/* Le395:  LDY     #$00 */
/* E397 */	0xA5,	0xCA,			/* Le397:  LDA     pp */
/* E399 */	0xC5,	0xE4,			/* CMP     p2 */
/* E39B */	0xA5,	0xCB,			/* LDA     pp+1 */
/* E39D */	0xE5,	0xE5,			/* SBC     p2+1 */
/* E39F */	0xB0,	0x16,			/* BCS     Le3b7 */
/* E3A1 */	0xA5,	0xE4,			/* LDA     p2 */
/* E3A3 */	0xD0,	0x02,			/* BNE     Le3a7 */
/* E3A5 */	0xC6,	0xE5,			/* DEC     p2+1 */
/* E3A7 */	0xC6,	0xE4,			/* Le3a7:  DEC     p2 */
/* E3A9 */	0xA5,	0xE6,			/* LDA     p3 */
/* E3AB */	0xD0,	0x02,			/* BNE     Le3af */
/* E3AD */	0xC6,	0xE7,			/* DEC     p3+1 */
/* E3AF */	0xC6,	0xE6,			/* Le3af:  DEC     p3 */
/* E3B1 */	0xB1,	0xE4,			/* LDA     (p2),Y */
/* E3B3 */	0x91,	0xE6,			/* STA     (p3),Y */
/* E3B5 */	0x90,	0xE0,			/* BCC     Le397 */
/* E3B7 */	0xA5,	0xE6,			/* Le3b7:  LDA     p3 */
/* E3B9 */	0x85,	0xCA,			/* STA     pp */
/* E3BB */	0xA5,	0xE7,			/* LDA     p3+1 */
/* E3BD */	0x85,	0xCB,			/* STA     pp+1 */
/* E3BF */	0x60,				/* RTS */
/* E3C0 */	0x20,	0xC9,	0xE3,		/* Le3c0:  JSR     cout */
/* E3C3 */	0xC8,				/* INY */
/* E3C4 */					
/* E3C4 */	0xB9,	0x00,	0xEB,		/* Se3c4:  LDA     error_msg_tbl,Y */
/* E3C7 */	0x30,	0xF7,			/* BMI     Le3c0 */
/* E3C9 */					
// ------------------------------------------------------------------------ 8a 8d point1 1/2
/* E3C9 */	0xC9,	0x8D,			/* cout:   CMP     #$8D */
/* E3CB */	0xD0,	0x06,			/* BNE     Le3d3 */
/* E3CD */					
/* E3CD */	0xA9,	0x00,			/* crout:  LDA     #$00            ; character output */
/* E3CF */	0x85,	0x24,			/* STA     ch */
/* E3D1 */	0xA9,	0x8D,			/* LDA     #$8D */
/* E3D3 */	0xE6,	0x24,			/* Le3d3:  INC     ch */
/* E3D5 */					
/* E3D5 */					/* ; Send character to display. Char is in A. */
// ======= DSP patch point ==================================================================
/* E3D5 */	0x2C,	0x12,	0xD0,		/* Le3d5:  BIT     DSP          ; See if display ready */
/* E3D8 */	0x30,	0xFB,			/* BMI     Le3d5        ; Loop if not */
// ======= DSP patch point ==================================================================
/* E3DA */	0x8D,	0x12,	0xD0,		/* STA     DSP          ; Write display data */
/* E3DD */	0x60,				/* RTS                  ; and return */
/* E3DE */					
/* E3DE */	0xA0,	0x06,			/* too_long_err:   LDY     #$06 */
/* E3E0 */					
/* E3E0 */	0x20,	0xD3,	0xEE,		/* print_err_msg:  JSR     print_err_msg1  ; print error message */
/* E3E3 */	0x24,	0xD9,			/* BIT     run_flag */
/* E3E5 */	0x30,	0x03,			/* Le3e5:  BMI     Le3ea */
/* E3E7 */	0x4C,	0xB6,	0xE2,		/* JMP     Le2b6 */
/* E3EA */	0x4C,	0x9A,	0xEB,		/* Le3ea:  JMP     Leb9a */
/* E3ED */	0x2A,				/* Le3ed:  ROL */
/* E3EE */	0x69,	0xA0,			/* ADC     #$A0 */
/* E3F0 */	0xDD,	0x00,	0x02,		/* CMP     buffer,X */
/* E3F3 */	0xD0,	0x53,			/* BNE     Le448 */
/* E3F5 */	0xB1,	0xFE,			/* LDA     (synpag),Y */
/* E3F7 */	0x0A,				/* ASL */
/* E3F8 */	0x30,	0x06,			/* BMI     Le400 */
/* E3FA */	0x88,				/* DEY */
/* E3FB */	0xB1,	0xFE,			/* LDA     (synpag),Y */
/* E3FD */	0x30,	0x29,			/* BMI     Le428 */
/* E3FF */	0xC8,				/* INY */
/* E400 */	0x86,	0xC8,			/* Le400:  STX     text_index */
/* E402 */	0x98,				/* TYA */
/* E403 */	0x48,				/* PHA */
/* E404 */	0xA2,	0x00,			/* LDX     #$00 */
/* E406 */	0xA1,	0xFE,			/* LDA     (synpag,X) */
/* E408 */	0xAA,				/* TAX */
/* E409 */	0x4A,				/* Le409:  LSR */
/* E40A */	0x49,	0x48,			/* EOR     #$48 */
/* E40C */	0x11,	0xFE,			/* ORA     (synpag),Y */
/* E40E */	0xC9,	0xC0,			/* CMP     #$C0 */
/* E410 */	0x90,	0x01,			/* BCC     Le413 */
/* E412 */	0xE8,				/* INX */
/* E413 */	0xC8,				/* Le413:  INY */
/* E414 */	0xD0,	0xF3,			/* BNE     Le409 */
/* E416 */	0x68,				/* PLA */
/* E417 */	0xA8,				/* TAY */
/* E418 */	0x8A,				/* TXA */
/* E419 */	0x4C,	0xC0,	0xE4,		/* JMP     Le4c0 */
/* E41C */					
/* E41C */					/* ; write a token to the buffer */
/* E41C */					/* ; buffer [++tokndx] = A */
/* E41C */	0xE6,	0xF1,			/* put_token:      INC     token_index */
/* E41E */	0xA6,	0xF1,			/* LDX     token_index */
/* E420 */	0xF0,	0xBC,			/* BEQ     too_long_err */
/* E422 */	0x9D,	0x00,	0x02,		/* STA     buffer,X */
/* E425 */	0x60,				/* Le425:  RTS */
/* E426 */	0xA6,	0xC8,			/* Le426:  LDX     text_index */
/* E428 */	0xA9,	0xA0,			/* Le428:  LDA     #$A0 */
/* E42A */	0xE8,				/* Le42a:  INX */
/* E42B */	0xDD,	0x00,	0x02,		/* CMP     buffer,X */
/* E42E */	0xB0,	0xFA,			/* BCS     Le42a */
/* E430 */	0xB1,	0xFE,			/* LDA     (synpag),Y */
/* E432 */	0x29,	0x3F,			/* AND     #$3F */
/* E434 */	0x4A,				/* LSR */
/* E435 */	0xD0,	0xB6,			/* BNE     Le3ed */
/* E437 */	0xBD,	0x00,	0x02,		/* LDA     buffer,X */
/* E43A */	0xB0,	0x06,			/* BCS     Le442 */
/* E43C */	0x69,	0x3F,			/* ADC     #$3F */
/* E43E */	0xC9,	0x1A,			/* CMP     #$1A */
/* E440 */	0x90,	0x6F,			/* BCC     Le4b1 */
/* E442 */	0x69,	0x4F,			/* Le442:  ADC     #$4F */
/* E444 */	0xC9,	0x0A,			/* CMP     #$0A */
/* E446 */	0x90,	0x69,			/* BCC     Le4b1 */
/* E448 */	0xA6,	0xFD,			/* Le448:  LDX     synstkdx */
/* E44A */	0xC8,				/* Le44a:  INY */
/* E44B */	0xB1,	0xFE,			/* LDA     (synpag),Y */
/* E44D */	0x29,	0xE0,			/* AND     #$E0 */
/* E44F */	0xC9,	0x20,			/* CMP     #$20 */
/* E451 */	0xF0,	0x7A,			/* BEQ     Le4cd */
/* E453 */	0xB5,	0xA8,			/* LDA     txtndxstk,X */
/* E455 */	0x85,	0xC8,			/* STA     text_index */
/* E457 */	0xB5,	0xD1,			/* LDA     tokndxstk,X */
/* E459 */	0x85,	0xF1,			/* STA     token_index */
/* E45B */	0x88,				/* Le45b:  DEY */
/* E45C */	0xB1,	0xFE,			/* LDA     (synpag),Y */
/* E45E */	0x0A,				/* ASL */
/* E45F */	0x10,	0xFA,			/* BPL     Le45b */
/* E461 */	0x88,				/* DEY */
/* E462 */	0xB0,	0x38,			/* BCS     Le49c */
/* E464 */	0x0A,				/* ASL */
/* E465 */	0x30,	0x35,			/* BMI     Le49c */
/* E467 */	0xB4,	0x58,			/* LDY     syn_stk_h,X */
/* E469 */	0x84,	0xFF,			/* STY     synpag+1 */
/* E46B */	0xB4,	0x80,			/* LDY     syn_stk_l,X */
/* E46D */	0xE8,				/* INX */
/* E46E */	0x10,	0xDA,			/* BPL     Le44a */
/* E470 */	0xF0,	0xB3,			/* Le470:  BEQ     Le425 */
/* E472 */	0xC9,	0x7E,			/* CMP     #$7E */
/* E474 */	0xB0,	0x22,			/* BCS     Le498 */
/* E476 */	0xCA,				/* DEX */
/* E477 */	0x10,	0x04,			/* BPL     Le47d */
/* E479 */	0xA0,	0x06,			/* LDY     #$06 */
/* E47B */	0x10,	0x29,			/* BPL     go_errmess_2 */
/* E47D */	0x94,	0x80,			/* Le47d:  STY     syn_stk_l,X */
/* E47F */	0xA4,	0xFF,			/* LDY     synpag+1 */
/* E481 */	0x94,	0x58,			/* STY     syn_stk_h,X */
/* E483 */	0xA4,	0xC8,			/* LDY     text_index */
/* E485 */	0x94,	0xA8,			/* STY     txtndxstk,X */
/* E487 */	0xA4,	0xF1,			/* LDY     token_index */
/* E489 */	0x94,	0xD1,			/* STY     tokndxstk,X */
/* E48B */	0x29,	0x1F,			/* AND     #$1F */
/* E48D */	0xA8,				/* TAY */
/* E48E */	0xB9,	0x20,	0xEC,		/* LDA     syntabl_index,Y */
/* E491 */					
/* E491 */	0x0A,				/* Se491:  ASL */
/* E492 */	0xA8,				/* TAY */
/* E493 */	0xA9,	0x76,			/* LDA     #$76 */
/* E495 */	0x2A,				/* ROL */
/* E496 */	0x85,	0xFF,			/* STA     synpag+1 */
/* E498 */	0xD0,	0x01,			/* Le498:  BNE     Le49b */
/* E49A */	0xC8,				/* INY */
/* E49B */	0xC8,				/* Le49b:  INY */
/* E49C */	0x86,	0xFD,			/* Le49c:  STX     synstkdx */
/* E49E */	0xB1,	0xFE,			/* LDA     (synpag),Y */
/* E4A0 */	0x30,	0x84,			/* BMI     Le426 */
/* E4A2 */	0xD0,	0x05,			/* BNE     Le4a9 */
/* E4A4 */	0xA0,	0x0E,			/* LDY     #$0E */
/* E4A6 */	0x4C,	0xE0,	0xE3,		/* go_errmess_2:   JMP     print_err_msg */
/* E4A9 */	0xC9,	0x03,			/* Le4a9:  CMP     #$03 */
/* E4AB */	0xB0,	0xC3,			/* BCS     Le470 */
/* E4AD */	0x4A,				/* LSR */
/* E4AE */	0xA6,	0xC8,			/* LDX     text_index */
/* E4B0 */	0xE8,				/* INX */
/* E4B1 */	0xBD,	0x00,	0x02,		/* Le4b1:  LDA     buffer,X */
/* E4B4 */	0x90,	0x04,			/* BCC     Le4ba */
/* E4B6 */	0xC9,	0xA2,			/* CMP     #$A2 */
/* E4B8 */	0xF0,	0x0A,			/* BEQ     Le4c4 */
/* E4BA */	0xC9,	0xDF,			/* Le4ba:  CMP     #$DF */
/* E4BC */	0xF0,	0x06,			/* BEQ     Le4c4 */
/* E4BE */	0x86,	0xC8,			/* STX     text_index */
/* E4C0 */	0x20,	0x1C,	0xE4,		/* Le4c0:  JSR     put_token */
/* E4C3 */	0xC8,				/* INY */
/* E4C4 */	0x88,				/* Le4c4:  DEY */
/* E4C5 */	0xA6,	0xFD,			/* LDX     synstkdx */
/* E4C7 */	0xB1,	0xFE,			/* Le4c7:  LDA     (synpag),Y */
/* E4C9 */	0x88,				/* DEY */
/* E4CA */	0x0A,				/* ASL */
/* E4CB */	0x10,	0xCF,			/* BPL     Le49c */
/* E4CD */	0xB4,	0x58,			/* Le4cd:  LDY     syn_stk_h,X */
/* E4CF */	0x84,	0xFF,			/* STY     synpag+1 */
/* E4D1 */	0xB4,	0x80,			/* LDY     syn_stk_l,X */
/* E4D3 */	0xE8,				/* INX */
/* E4D4 */	0xB1,	0xFE,			/* LDA     (synpag),Y */
/* E4D6 */	0x29,	0x9F,			/* AND     #$9F */
/* E4D8 */	0xD0,	0xED,			/* BNE     Le4c7 */
/* E4DA */	0x85,	0xF2,			/* STA     pcon */
/* E4DC */	0x85,	0xF3,			/* STA     pcon+1 */
/* E4DE */	0x98,				/* TYA */
/* E4DF */	0x48,				/* PHA */
/* E4E0 */	0x86,	0xFD,			/* STX     synstkdx */
/* E4E2 */	0xB4,	0xD0,			/* LDY     srch,X */
/* E4E4 */	0x84,	0xC9,			/* STY     leadbl */
/* E4E6 */	0x18,				/* CLC */
/* E4E7 */	0xA9,	0x0A,			/* Le4e7:  LDA     #$0A */
/* E4E9 */	0x85,	0xF9,			/* STA     char */
/* E4EB */	0xA2,	0x00,			/* LDX     #$00 */
/* E4ED */	0xC8,				/* INY */
/* E4EE */	0xB9,	0x00,	0x02,		/* LDA     buffer,Y */
/* E4F1 */	0x29,	0x0F,			/* AND     #$0F */
/* E4F3 */	0x65,	0xF2,			/* Le4f3:  ADC     pcon */
/* E4F5 */	0x48,				/* PHA */
/* E4F6 */	0x8A,				/* TXA */
/* E4F7 */	0x65,	0xF3,			/* ADC     pcon+1 */
/* E4F9 */	0x30,	0x1C,			/* BMI     Le517 */
/* E4FB */	0xAA,				/* TAX */
/* E4FC */	0x68,				/* PLA */
/* E4FD */	0xC6,	0xF9,			/* DEC     char */
/* E4FF */	0xD0,	0xF2,			/* BNE     Le4f3 */
/* E501 */	0x85,	0xF2,			/* STA     pcon */
/* E503 */	0x86,	0xF3,			/* STX     pcon+1 */
/* E505 */	0xC4,	0xF1,			/* CPY     token_index */
/* E507 */	0xD0,	0xDE,			/* BNE     Le4e7 */
/* E509 */	0xA4,	0xC9,			/* LDY     leadbl */
/* E50B */	0xC8,				/* INY */
/* E50C */	0x84,	0xF1,			/* STY     token_index */
/* E50E */	0x20,	0x1C,	0xE4,		/* JSR     put_token */
/* E511 */	0x68,				/* PLA */
/* E512 */	0xA8,				/* TAY */
/* E513 */	0xA5,	0xF3,			/* LDA     pcon+1 */
/* E515 */	0xB0,	0xA9,			/* BCS     Le4c0 */
/* E517 */	0xA0,	0x00,			/* Le517:  LDY     #$00 */
/* E519 */	0x10,	0x8B,			/* BPL     go_errmess_2 */
/* E51B */					
/* E51B */	0x85,	0xF3,			/* prdec:  STA     pcon+1  ; output A:X in decimal */
/* E51D */	0x86,	0xF2,			/* STX     pcon */
/* E51F */	0xA2,	0x04,			/* LDX     #$04 */
/* E521 */	0x86,	0xC9,			/* STX     leadbl */
/* E523 */	0xA9,	0xB0,			/* Le523:  LDA     #$B0 */
/* E525 */	0x85,	0xF9,			/* STA     char */
/* E527 */	0xA5,	0xF2,			/* Le527:  LDA     pcon */
/* E529 */	0xDD,	0x63,	0xE5,		/* CMP     dectabl,X */
/* E52C */	0xA5,	0xF3,			/* LDA     pcon+1 */
/* E52E */	0xFD,	0x68,	0xE5,		/* SBC     dectabh,X */
/* E531 */	0x90,	0x0D,			/* BCC     Le540 */
/* E533 */	0x85,	0xF3,			/* STA     pcon+1 */
/* E535 */	0xA5,	0xF2,			/* LDA     pcon */
/* E537 */	0xFD,	0x63,	0xE5,		/* SBC     dectabl,X */
/* E53A */	0x85,	0xF2,			/* STA     pcon */
/* E53C */	0xE6,	0xF9,			/* INC     char */
/* E53E */	0xD0,	0xE7,			/* BNE     Le527 */
/* E540 */	0xA5,	0xF9,			/* Le540:  LDA     char */
/* E542 */	0xE8,				/* INX */
/* E543 */	0xCA,				/* DEX */
/* E544 */	0xF0,	0x0E,			/* BEQ     Le554 */
/* E546 */	0xC9,	0xB0,			/* CMP     #$B0 */
/* E548 */	0xF0,	0x02,			/* BEQ     Le54c */
/* E54A */	0x85,	0xC9,			/* STA     leadbl */
/* E54C */	0x24,	0xC9,			/* Le54c:  BIT     leadbl */
/* E54E */	0x30,	0x04,			/* BMI     Le554 */
/* E550 */	0xA5,	0xFA,			/* LDA     leadzr */
/* E552 */	0xF0,	0x0B,			/* BEQ     Le55f */
/* E554 */	0x20,	0xC9,	0xE3,		/* Le554:  JSR     cout */
/* E557 */	0x24,	0xF8,			/* BIT     auto_flag */
/* E559 */	0x10,	0x04,			/* BPL     Le55f */
/* E55B */	0x99,	0x00,	0x02,		/* STA     buffer,Y */
/* E55E */	0xC8,				/* INY */
/* E55F */	0xCA,				/* Le55f:  DEX */
/* E560 */	0x10,	0xC1,			/* BPL     Le523 */
/* E562 */	0x60,				/* RTS */
/* E563 */					/* ; powers of 10 table, low byte */
/* E563 */	0x01,	0x0A,	0x64,	0xE8,	/* dectabl:        .byte   $01,$0A,$64,$E8,$10             ; "..d */
/* E567 */	0x10,				
/* E568 */					
/* E568 */					/* ; powers of 10 table, high byte */
/* E568 */	0x00,	0x00,	0x00,	0x03,	/* dectabh:        .byte   $00,$00,$00,$03,$27             ; "... */
/* E56C */	0x27,				
/* E56D */					
/* E56D */	0xA5,	0xCA,			/* find_line:      LDA     pp */
/* E56F */	0x85,	0xE6,			/* STA     p3 */
/* E571 */	0xA5,	0xCB,			/* LDA     pp+1 */
/* E573 */	0x85,	0xE7,			/* STA     p3+1 */
/* E575 */					
/* E575 */	0xE8,				/* find_line1:     INX */
/* E576 */					
/* E576 */	0xA5,	0xE7,			/* find_line2:     LDA     p3+1 */
/* E578 */	0x85,	0xE5,			/* STA     p2+1 */
/* E57A */	0xA5,	0xE6,			/* LDA     p3 */
/* E57C */	0x85,	0xE4,			/* STA     p2 */
/* E57E */	0xC5,	0x4C,			/* CMP     himem */
/* E580 */	0xA5,	0xE5,			/* LDA     p2+1 */
/* E582 */	0xE5,	0x4D,			/* SBC     himem+1 */
/* E584 */	0xB0,	0x26,			/* BCS     Le5ac */
/* E586 */	0xA0,	0x01,			/* LDY     #$01 */
/* E588 */	0xB1,	0xE4,			/* LDA     (p2),Y */
/* E58A */	0xE5,	0xCE,			/* SBC     acc */
/* E58C */	0xC8,				/* INY */
/* E58D */	0xB1,	0xE4,			/* LDA     (p2),Y */
/* E58F */	0xE5,	0xCF,			/* SBC     acc+1 */
/* E591 */	0xB0,	0x19,			/* BCS     Le5ac */
/* E593 */	0xA0,	0x00,			/* LDY     #$00 */
/* E595 */	0xA5,	0xE6,			/* LDA     p3 */
/* E597 */	0x71,	0xE4,			/* ADC     (p2),Y */
/* E599 */	0x85,	0xE6,			/* STA     p3 */
/* E59B */	0x90,	0x03,			/* BCC     Le5a0 */
/* E59D */	0xE6,	0xE7,			/* INC     p3+1 */
/* E59F */	0x18,				/* CLC */
/* E5A0 */	0xC8,				/* Le5a0:  INY */
/* E5A1 */	0xA5,	0xCE,			/* LDA     acc */
/* E5A3 */	0xF1,	0xE4,			/* SBC     (p2),Y */
/* E5A5 */	0xC8,				/* INY */
/* E5A6 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E5A8 */	0xF1,	0xE4,			/* SBC     (p2),Y */
/* E5AA */	0xB0,	0xCA,			/* BCS     find_line2 */
/* E5AC */	0x60,				/* Le5ac:  RTS */
/* E5AD */					
/* E5AD */					/* ; token $0B - "NEW" */
/* E5AD */	0x46,	0xF8,			/* new_cmd:        LSR     auto_flag */
/* E5AF */	0xA5,	0x4C,			/* LDA     himem */
/* E5B1 */	0x85,	0xCA,			/* STA     pp */
/* E5B3 */	0xA5,	0x4D,			/* LDA     himem+1 */
/* E5B5 */	0x85,	0xCB,			/* STA     pp+1 */
/* E5B7 */					
/* E5B7 */					/* ; token $0C - "CLR" */
/* E5B7 */	0xA5,	0x4A,			/* clr:    LDA     lomem */
/* E5B9 */	0x85,	0xCC,			/* STA     pv */
/* E5BB */	0xA5,	0x4B,			/* LDA     lomem+1 */
/* E5BD */	0x85,	0xCD,			/* STA     pv+1 */
/* E5BF */	0xA9,	0x00,			/* LDA     #$00 */
/* E5C1 */	0x85,	0xFB,			/* STA     for_nest_count */
/* E5C3 */	0x85,	0xFC,			/* STA     gosub_nest_count */
/* E5C5 */	0x85,	0xFE,			/* STA     synpag */
/* E5C7 */	0xA9,	0x00,			/* LDA     #$00 */
/* E5C9 */	0x85,	0x1D,			/* STA     Z1d */
/* E5CB */	0x60,				/* RTS */
/* E5CC */	0xA5,	0xD0,			/* Le5cc:  LDA     srch */
/* E5CE */	0x69,	0x05,			/* ADC     #$05 */
/* E5D0 */	0x85,	0xD2,			/* STA     srch2 */
/* E5D2 */	0xA5,	0xD1,			/* LDA     tokndxstk */
/* E5D4 */	0x69,	0x00,			/* ADC     #$00 */
/* E5D6 */	0x85,	0xD3,			/* STA     srch2+1 */
/* E5D8 */	0xA5,	0xD2,			/* LDA     srch2 */
/* E5DA */	0xC5,	0xCA,			/* CMP     pp */
/* E5DC */	0xA5,	0xD3,			/* LDA     srch2+1 */
/* E5DE */	0xE5,	0xCB,			/* SBC     pp+1 */
/* E5E0 */	0x90,	0x03,			/* BCC     Le5e5 */
/* E5E2 */	0x4C,	0x6B,	0xE3,		/* JMP     Le36b */
/* E5E5 */	0xA5,	0xCE,			/* Le5e5:  LDA     acc */
/* E5E7 */	0x91,	0xD0,			/* STA     (srch),Y */
/* E5E9 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E5EB */	0xC8,				/* INY */
/* E5EC */	0x91,	0xD0,			/* STA     (srch),Y */
/* E5EE */	0xA5,	0xD2,			/* LDA     srch2 */
/* E5F0 */	0xC8,				/* INY */
/* E5F1 */	0x91,	0xD0,			/* STA     (srch),Y */
/* E5F3 */	0xA5,	0xD3,			/* LDA     srch2+1 */
/* E5F5 */	0xC8,				/* INY */
/* E5F6 */	0x91,	0xD0,			/* STA     (srch),Y */
/* E5F8 */	0xA9,	0x00,			/* LDA     #$00 */
/* E5FA */	0xC8,				/* INY */
/* E5FB */	0x91,	0xD0,			/* STA     (srch),Y */
/* E5FD */	0xC8,				/* INY */
/* E5FE */	0x91,	0xD0,			/* STA     (srch),Y */
/* E600 */	0xA5,	0xD2,			/* LDA     srch2 */
/* E602 */	0x85,	0xCC,			/* STA     pv */
/* E604 */	0xA5,	0xD3,			/* LDA     srch2+1 */
/* E606 */	0x85,	0xCD,			/* STA     pv+1 */
/* E608 */	0xA5,	0xD0,			/* LDA     srch */
/* E60A */	0x90,	0x43,			/* BCC     Le64f */
/* E60C */	0x85,	0xCE,			/* execute_var:    STA     acc */
/* E60E */	0x84,	0xCF,			/* STY     acc+1 */
/* E610 */	0x20,	0xFF,	0xE6,		/* JSR     get_next_prog_byte */
/* E613 */	0x30,	0x0E,			/* BMI     Le623 */
/* E615 */	0xC9,	0x40,			/* CMP     #$40 */
/* E617 */	0xF0,	0x0A,			/* BEQ     Le623 */
/* E619 */	0x4C,	0x28,	0xE6,		/* JMP     Le628 */
/* E61C */	0x06,	0xC9,	0x49,	0xD0,	/* .byte   $06,$C9,$49,$D0,$07,$A9,$49 */
/* E620 */	0x07,	0xA9,	0x49,		
/* E623 */	0x85,	0xCF,			/* Le623:  STA     acc+1 */
/* E625 */	0x20,	0xFF,	0xE6,		/* JSR     get_next_prog_byte */
/* E628 */	0xA5,	0x4B,			/* Le628:  LDA     lomem+1 */
/* E62A */	0x85,	0xD1,			/* STA     tokndxstk */
/* E62C */	0xA5,	0x4A,			/* LDA     lomem */
/* E62E */	0x85,	0xD0,			/* Le62e:  STA     srch */
/* E630 */	0xC5,	0xCC,			/* CMP     pv */
/* E632 */	0xA5,	0xD1,			/* LDA     tokndxstk */
/* E634 */	0xE5,	0xCD,			/* SBC     pv+1 */
/* E636 */	0xB0,	0x94,			/* BCS     Le5cc */
/* E638 */	0xB1,	0xD0,			/* LDA     (srch),Y */
/* E63A */	0xC8,				/* INY */
/* E63B */	0xC5,	0xCE,			/* CMP     acc */
/* E63D */	0xD0,	0x06,			/* BNE     Le645 */
/* E63F */	0xB1,	0xD0,			/* LDA     (srch),Y */
/* E641 */	0xC5,	0xCF,			/* CMP     acc+1 */
/* E643 */	0xF0,	0x0E,			/* BEQ     Le653 */
/* E645 */	0xC8,				/* Le645:  INY */
/* E646 */	0xB1,	0xD0,			/* LDA     (srch),Y */
/* E648 */	0x48,				/* PHA */
/* E649 */	0xC8,				/* INY */
/* E64A */	0xB1,	0xD0,			/* LDA     (srch),Y */
/* E64C */	0x85,	0xD1,			/* STA     tokndxstk */
/* E64E */	0x68,				/* PLA */
/* E64F */	0xA0,	0x00,			/* Le64f:  LDY     #$00 */
/* E651 */	0xF0,	0xDB,			/* BEQ     Le62e */
/* E653 */	0xA5,	0xD0,			/* Le653:  LDA     srch */
/* E655 */	0x69,	0x03,			/* ADC     #$03 */
/* E657 */	0x20,	0x0A,	0xE7,		/* JSR     push_a_noun_stk */
/* E65A */	0xA5,	0xD1,			/* LDA     tokndxstk */
/* E65C */	0x69,	0x00,			/* ADC     #$00 */
/* E65E */	0x95,	0x78,			/* STA     noun_stk_h_str,X */
/* E660 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E662 */	0xC9,	0x40,			/* CMP     #$40 */
/* E664 */	0xD0,	0x1C,			/* BNE     fetch_prog_byte */
/* E666 */	0x88,				/* DEY */
/* E667 */	0x98,				/* TYA */
/* E668 */	0x20,	0x0A,	0xE7,		/* JSR     push_a_noun_stk */
/* E66B */	0x88,				/* DEY */
/* E66C */	0x94,	0x78,			/* STY     noun_stk_h_str,X */
/* E66E */	0xA0,	0x03,			/* LDY     #$03 */
/* E670 */	0xF6,	0x78,			/* Le670:  INC     noun_stk_h_str,X */
/* E672 */	0xC8,				/* INY */
/* E673 */	0xB1,	0xD0,			/* LDA     (srch),Y */
/* E675 */	0x30,	0xF9,			/* BMI     Le670 */
/* E677 */	0x10,	0x09,			/* BPL     fetch_prog_byte */
/* E679 */					
/* E679 */	0xA9,	0x00,			/* execute_stmt:   LDA     #$00 */
/* E67B */	0x85,	0xD4,			/* STA     if_flag */
/* E67D */	0x85,	0xD5,			/* STA     cr_flag */
/* E67F */	0xA2,	0x20,			/* LDX     #$20 */
/* E681 */					
/* E681 */					/* ; push old verb on stack for later use in precedence test */
/* E681 */	0x48,				/* push_old_verb:  PHA */
/* E682 */	0xA0,	0x00,			/* fetch_prog_byte:        LDY     #$00 */
/* E684 */	0xB1,	0xE0,			/* LDA     (pverb),Y */
/* E686 */	0x10,	0x18,			/* Le686:  BPL     execute_token */
/* E688 */	0x0A,				/* ASL */
/* E689 */	0x30,	0x81,			/* BMI     execute_var */
/* E68B */	0x20,	0xFF,	0xE6,		/* JSR     get_next_prog_byte */
/* E68E */	0x20,	0x08,	0xE7,		/* JSR     push_ya_noun_stk */
/* E691 */	0x20,	0xFF,	0xE6,		/* JSR     get_next_prog_byte */
/* E694 */	0x95,	0xA0,			/* STA     noun_stk_h_int,X */
/* E696 */	0x24,	0xD4,			/* Le696:  BIT     if_flag */
/* E698 */	0x10,	0x01,			/* BPL     Le69b */
/* E69A */	0xCA,				/* DEX */
/* E69B */	0x20,	0xFF,	0xE6,		/* Le69b:  JSR     get_next_prog_byte */
/* E69E */	0xB0,	0xE6,			/* BCS     Le686 */
/* E6A0 */					
/* E6A0 */	0xC9,	0x28,			/* execute_token:  CMP     #$28 */
/* E6A2 */	0xD0,	0x1F,			/* BNE     execute_verb */
/* E6A4 */	0xA5,	0xE0,			/* LDA     pverb */
/* E6A6 */	0x20,	0x0A,	0xE7,		/* JSR     push_a_noun_stk */
/* E6A9 */	0xA5,	0xE1,			/* LDA     pverb+1 */
/* E6AB */	0x95,	0x78,			/* STA     noun_stk_h_str,X */
/* E6AD */	0x24,	0xD4,			/* BIT     if_flag */
/* E6AF */	0x30,	0x0B,			/* BMI     Le6bc */
/* E6B1 */	0xA9,	0x01,			/* LDA     #$01 */
/* E6B3 */	0x20,	0x0A,	0xE7,		/* JSR     push_a_noun_stk */
/* E6B6 */	0xA9,	0x00,			/* LDA     #$00 */
/* E6B8 */	0x95,	0x78,			/* STA     noun_stk_h_str,X */
/* E6BA */	0xF6,	0x78,			/* Le6ba:  INC     noun_stk_h_str,X */
/* E6BC */	0x20,	0xFF,	0xE6,		/* Le6bc:  JSR     get_next_prog_byte */
/* E6BF */	0x30,	0xF9,			/* BMI     Le6ba */
/* E6C1 */	0xB0,	0xD3,			/* BCS     Le696 */
/* E6C3 */	0x24,	0xD4,			/* execute_verb:   BIT     if_flag */
/* E6C5 */	0x10,	0x06,			/* BPL     Le6cd */
/* E6C7 */	0xC9,	0x04,			/* CMP     #$04 */
/* E6C9 */	0xB0,	0xD0,			/* BCS     Le69b */
/* E6CB */	0x46,	0xD4,			/* LSR     if_flag */
/* E6CD */	0xA8,				/* Le6cd:  TAY */
/* E6CE */	0x85,	0xD6,			/* STA     current_verb */
/* E6D0 */	0xB9,	0x98,	0xE9,		/* LDA     verb_prec_tbl,Y */
/* E6D3 */	0x29,	0x55,			/* AND     #$55 */
/* E6D5 */	0x0A,				/* ASL */
/* E6D6 */	0x85,	0xD7,			/* STA     precedence */
/* E6D8 */	0x68,				/* Le6d8:  PLA */
/* E6D9 */	0xA8,				/* TAY */
/* E6DA */	0xB9,	0x98,	0xE9,		/* LDA     verb_prec_tbl,Y */
/* E6DD */	0x29,	0xAA,			/* AND     #$AA */
/* E6DF */	0xC5,	0xD7,			/* CMP     precedence */
/* E6E1 */	0xB0,	0x09,			/* BCS     do_verb */
/* E6E3 */	0x98,				/* TYA */
/* E6E4 */	0x48,				/* PHA */
/* E6E5 */	0x20,	0xFF,	0xE6,		/* JSR     get_next_prog_byte */
/* E6E8 */	0xA5,	0xD6,			/* LDA     current_verb */
/* E6EA */	0x90,	0x95,			/* BCC     push_old_verb */
/* E6EC */	0xB9,	0x10,	0xEA,		/* do_verb:        LDA     verb_adr_l,Y */
/* E6EF */	0x85,	0xCE,			/* STA     acc */
/* E6F1 */	0xB9,	0x88,	0xEA,		/* LDA     verb_adr_h,Y */
/* E6F4 */	0x85,	0xCF,			/* STA     acc+1 */
/* E6F6 */	0x20,	0xFC,	0xE6,		/* JSR     Se6fc */
/* E6F9 */	0x4C,	0xD8,	0xE6,		/* JMP     Le6d8 */
/* E6FC */					
/* E6FC */	0x6C,	0xCE,	0x00,		/* Se6fc:  JMP     (acc) */
/* E6FF */					
/* E6FF */	0xE6,	0xE0,			/* get_next_prog_byte:     INC     pverb */
/* E701 */	0xD0,	0x02,			/* BNE     Le705 */
/* E703 */	0xE6,	0xE1,			/* INC     pverb+1 */
/* E705 */	0xB1,	0xE0,			/* Le705:  LDA     (pverb),Y */
/* E707 */	0x60,				/* RTS */
/* E708 */					
/* E708 */	0x94,	0x77,			/* push_ya_noun_stk:       STY     syn_stk_h+31,X */
/* E70A */					
/* E70A */	0xCA,				/* push_a_noun_stk:        DEX */
/* E70B */	0x30,	0x03,			/* BMI     Le710 */
/* E70D */	0x95,	0x50,			/* STA     noun_stk_l,X */
/* E70F */	0x60,				/* RTS */
/* E710 */	0xA0,	0x66,			/* Le710:  LDY     #$66 */
/* E712 */	0x4C,	0xE0,	0xE3,		/* go_errmess_3:   JMP     print_err_msg */
/* E715 */					
/* E715 */	0xA0,	0x00,			/* get16bit:       LDY     #$00 */
/* E717 */	0xB5,	0x50,			/* LDA     noun_stk_l,X */
/* E719 */	0x85,	0xCE,			/* STA     acc */
/* E71B */	0xB5,	0xA0,			/* LDA     noun_stk_h_int,X */
/* E71D */	0x85,	0xCF,			/* STA     acc+1 */
/* E71F */	0xB5,	0x78,			/* LDA     noun_stk_h_str,X */
/* E721 */	0xF0,	0x0E,			/* BEQ     Le731 */
/* E723 */	0x85,	0xCF,			/* STA     acc+1 */
/* E725 */	0xB1,	0xCE,			/* LDA     (acc),Y */
/* E727 */	0x48,				/* PHA */
/* E728 */	0xC8,				/* INY */
/* E729 */	0xB1,	0xCE,			/* LDA     (acc),Y */
/* E72B */	0x85,	0xCF,			/* STA     acc+1 */
/* E72D */	0x68,				/* PLA */
/* E72E */	0x85,	0xCE,			/* STA     acc */
/* E730 */	0x88,				/* DEY */
/* E731 */	0xE8,				/* Le731:  INX */
/* E732 */	0x60,				/* RTS */
/* E733 */					
/* E733 */					/* ; token $16 - "=" for numeric equality operator */
/* E733 */	0x20,	0x4A,	0xE7,		/* eq_op:  JSR     neq_op */
/* E736 */					
/* E736 */					/* ; token $37 - "NOT" */
/* E736 */	0x20,	0x15,	0xE7,		/* not_op: JSR     get16bit */
/* E739 */	0x98,				/* TYA */
/* E73A */	0x20,	0x08,	0xE7,		/* JSR     push_ya_noun_stk */
/* E73D */	0x95,	0xA0,			/* STA     noun_stk_h_int,X */
/* E73F */	0xC5,	0xCE,			/* CMP     acc */
/* E741 */	0xD0,	0x06,			/* BNE     Le749 */
/* E743 */	0xC5,	0xCF,			/* CMP     acc+1 */
/* E745 */	0xD0,	0x02,			/* BNE     Le749 */
/* E747 */	0xF6,	0x50,			/* INC     noun_stk_l,X */
/* E749 */	0x60,				/* Le749:  RTS */
/* E74A */					
/* E74A */					/* ; token $17 - "#" for numeric inequality operator */
/* E74A */					/* ; token $1B - "<>" for numeric inequality operator */
/* E74A */	0x20,	0x82,	0xE7,		/* neq_op: JSR     subtract */
/* E74D */	0x20,	0x59,	0xE7,		/* JSR     sgn_fn */
/* E750 */					
/* E750 */					/* ; token $31 - "ABS" */
/* E750 */	0x20,	0x15,	0xE7,		/* abs_fn: JSR     get16bit */
/* E753 */	0x24,	0xCF,			/* BIT     acc+1 */
/* E755 */	0x30,	0x1B,			/* BMI     Se772 */
/* E757 */	0xCA,				/* Le757:  DEX */
/* E758 */	0x60,				/* Le758:  RTS */
/* E759 */					
/* E759 */					/* ; token $30 - "SGN" */
/* E759 */	0x20,	0x15,	0xE7,		/* sgn_fn: JSR     get16bit */
/* E75C */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E75E */	0xD0,	0x04,			/* BNE     Le764 */
/* E760 */	0xA5,	0xCE,			/* LDA     acc */
/* E762 */	0xF0,	0xF3,			/* BEQ     Le757 */
/* E764 */	0xA9,	0xFF,			/* Le764:  LDA     #$FF */
/* E766 */	0x20,	0x08,	0xE7,		/* JSR     push_ya_noun_stk */
/* E769 */	0x95,	0xA0,			/* STA     noun_stk_h_int,X */
/* E76B */	0x24,	0xCF,			/* BIT     acc+1 */
/* E76D */	0x30,	0xE9,			/* BMI     Le758 */
/* E76F */					
/* E76F */					/* ; token $36 - "-" for unary negation */
/* E76F */	0x20,	0x15,	0xE7,		/* negate: JSR     get16bit */
/* E772 */					
/* E772 */	0x98,				/* Se772:  TYA */
/* E773 */	0x38,				/* SEC */
/* E774 */	0xE5,	0xCE,			/* SBC     acc */
/* E776 */	0x20,	0x08,	0xE7,		/* JSR     push_ya_noun_stk */
/* E779 */	0x98,				/* TYA */
/* E77A */	0xE5,	0xCF,			/* SBC     acc+1 */
/* E77C */	0x50,	0x23,			/* BVC     Le7a1 */
/* E77E */	0xA0,	0x00,			/* Le77e:  LDY     #$00 */
/* E780 */	0x10,	0x90,			/* BPL     go_errmess_3 */
/* E782 */					
/* E782 */					/* ; token $13 - "-" for numeric subtraction */
/* E782 */	0x20,	0x6F,	0xE7,		/* subtract:       JSR     negate */
/* E785 */					
/* E785 */					/* ; token $12 - "+" for numeric addition */
/* E785 */	0x20,	0x15,	0xE7,		/* add:    JSR     get16bit */
/* E788 */	0xA5,	0xCE,			/* LDA     acc */
/* E78A */	0x85,	0xDA,			/* STA     aux */
/* E78C */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E78E */	0x85,	0xDB,			/* STA     aux+1 */
/* E790 */	0x20,	0x15,	0xE7,		/* JSR     get16bit */
/* E793 */					
/* E793 */	0x18,				/* Se793:  CLC */
/* E794 */	0xA5,	0xCE,			/* LDA     acc */
/* E796 */	0x65,	0xDA,			/* ADC     aux */
/* E798 */	0x20,	0x08,	0xE7,		/* JSR     push_ya_noun_stk */
/* E79B */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E79D */	0x65,	0xDB,			/* ADC     aux+1 */
/* E79F */	0x70,	0xDD,			/* BVS     Le77e */
/* E7A1 */	0x95,	0xA0,			/* Le7a1:  STA     noun_stk_h_int,X */
/* E7A3 */					
/* E7A3 */					/* ; token $35 - "+" for unary positive */
/* E7A3 */	0x60,				/* unary_pos:      RTS */
/* E7A4 */					
/* E7A4 */					/* ; token $50 - "TAB" function */
/* E7A4 */	0x20,	0x15,	0xE7,		/* tab_fn: JSR     get16bit */
/* E7A7 */	0xA4,	0xCE,			/* LDY     acc */
/* E7A9 */	0xF0,	0x05,			/* BEQ     Le7b0 */
/* E7AB */	0x88,				/* DEY */
/* E7AC */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E7AE */	0xF0,	0x0C,			/* BEQ     Le7bc */
/* E7B0 */	0x60,				/* Le7b0:  RTS */
/* E7B1 */					
/* E7B1 */					/* ; horizontal tab */
/* E7B1 */	0xA5,	0x24,			/* tabout: LDA     ch */
/* E7B3 */	0x09,	0x07,			/* ORA     #$07 */
/* E7B5 */	0xA8,				/* TAY */
/* E7B6 */	0xC8,				/* INY */
/* E7B7 */	0xA9,	0xA0,			/* Le7b7:  LDA     #$A0 */
/* E7B9 */	0x20,	0xC9,	0xE3,		/* JSR     cout */
/* E7BC */	0xC4,	0x24,			/* Le7bc:  CPY     ch */
/* E7BE */	0xB0,	0xF7,			/* BCS     Le7b7 */
/* E7C0 */	0x60,				/* RTS */
/* E7C1 */					
/* E7C1 */					/* ; token $49 - "," in print, numeric follows */
/* E7C1 */	0x20,	0xB1,	0xE7,		/* print_com_num:  JSR     tabout */
/* E7C4 */					
/* E7C4 */					/* ; token $62 - "PRINT" numeric */
/* E7C4 */	0x20,	0x15,	0xE7,		/* print_num:      JSR     get16bit */
/* E7C7 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E7C9 */	0x10,	0x0A,			/* BPL     Le7d5 */
/* E7CB */	0xA9,	0xAD,			/* LDA     #$AD */
/* E7CD */	0x20,	0xC9,	0xE3,		/* JSR     cout */
/* E7D0 */	0x20,	0x72,	0xE7,		/* JSR     Se772 */
/* E7D3 */	0x50,	0xEF,			/* BVC     print_num */
/* E7D5 */	0x88,				/* Le7d5:  DEY */
/* E7D6 */	0x84,	0xD5,			/* STY     cr_flag */
/* E7D8 */	0x86,	0xCF,			/* STX     acc+1 */
/* E7DA */	0xA6,	0xCE,			/* LDX     acc */
/* E7DC */	0x20,	0x1B,	0xE5,		/* JSR     prdec */
/* E7DF */	0xA6,	0xCF,			/* LDX     acc+1 */
/* E7E1 */	0x60,				/* RTS */
/* E7E2 */					
/* E7E2 */					/* ; token $0D - "AUTO" command */
/* E7E2 */	0x20,	0x15,	0xE7,		/* auto_cmd:       JSR     get16bit */
/* E7E5 */	0xA5,	0xCE,			/* LDA     acc */
/* E7E7 */	0x85,	0xF6,			/* STA     auto_ln */
/* E7E9 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E7EB */	0x85,	0xF7,			/* STA     auto_ln+1 */
/* E7ED */	0x88,				/* DEY */
/* E7EE */	0x84,	0xF8,			/* STY     auto_flag */
/* E7F0 */	0xC8,				/* INY */
/* E7F1 */	0xA9,	0x0A,			/* LDA     #$0A */
/* E7F3 */	0x85,	0xF4,			/* Le7f3:  STA     auto_inc */
/* E7F5 */	0x84,	0xF5,			/* STY     auto_inc+1 */
/* E7F7 */	0x60,				/* RTS */
/* E7F8 */					
/* E7F8 */					/* ; token $0E - "," in AUTO command */
/* E7F8 */	0x20,	0x15,	0xE7,		/* auto_com:       JSR     get16bit */
/* E7FB */	0xA5,	0xCE,			/* LDA     acc */
/* E7FD */	0xA4,	0xCF,			/* LDY     acc+1 */
/* E7FF */	0x10,	0xF2,			/* BPL     Le7f3 */
/* E801 */					
/* E801 */					/* ; token $56 - "=" in FOR statement */
/* E801 */					/* ; token $71 - "=" in LET (or implied LET) statement */
/* E801 */	0x20,	0x15,	0xE7,		/* var_assign:     JSR     get16bit */
/* E804 */	0xB5,	0x50,			/* LDA     noun_stk_l,X */
/* E806 */	0x85,	0xDA,			/* STA     aux */
/* E808 */	0xB5,	0x78,			/* LDA     noun_stk_h_str,X */
/* E80A */	0x85,	0xDB,			/* STA     aux+1 */
/* E80C */	0xA5,	0xCE,			/* LDA     acc */
/* E80E */	0x91,	0xDA,			/* STA     (aux),Y */
/* E810 */	0xC8,				/* INY */
/* E811 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E813 */	0x91,	0xDA,			/* STA     (aux),Y */
/* E815 */	0xE8,				/* INX */
/* E816 */					
/* E816 */	0x60,				/* Te816:  RTS */
/* E817 */					
/* E817 */					/* ; token $00 - begining of line */
/* E817 */					/* begin_line: */
/* E817 */	0x68,				/* PLA */
/* E818 */	0x68,				/* PLA */
/* E819 */					
/* E819 */					/* ; token $03 - ":" statement separator */
/* E819 */	0x24,	0xD5,			/* colon:  BIT     cr_flag */
/* E81B */	0x10,	0x05,			/* BPL     Le822 */
/* E81D */					
/* E81D */					/* ; token $63 - "PRINT" with no arg */
/* E81D */	0x20,	0xCD,	0xE3,		/* print_cr:       JSR     crout */
/* E820 */					
/* E820 */					/* ; token $47 - ";" at end of print statement */
/* E820 */	0x46,	0xD5,			/* print_semi:     LSR     cr_flag */
/* E822 */	0x60,				/* Le822:  RTS */
/* E823 */					
/* E823 */					
/* E823 */					/* ; token $22 - "(" in string DIM */
/* E823 */					/* ; token $34 - "(" in numeric DIM */
/* E823 */					/* ; token $38 - "(" in numeric expression */
/* E823 */					/* ; token $3F - "(" in some PEEK, RND, SGN, ABS (PDL) */
/* E823 */	0xA0,	0xFF,			/* left_paren:     LDY     #$FF */
/* E825 */	0x84,	0xD7,			/* STY     precedence */
/* E827 */					
/* E827 */					/* ; token $72 - ")" everywhere */
/* E827 */	0x60,				/* right_paren:    RTS */
/* E828 */					
/* E828 */					/* ; token $60 - "IF" statement */
/* E828 */	0x20,	0xCD,	0xEF,		/* if_stmt:        JSR     Sefcd */
/* E82B */	0xF0,	0x07,			/* BEQ     Le834 */
/* E82D */	0xA9,	0x25,			/* LDA     #$25 */
/* E82F */	0x85,	0xD6,			/* STA     current_verb */
/* E831 */	0x88,				/* DEY */
/* E832 */	0x84,	0xD4,			/* STY     if_flag */
/* E834 */	0xE8,				/* Le834:  INX */
/* E835 */	0x60,				/* RTS */
/* E836 */					/* ; RUN without CLR, used by Apple DOS */
/* E836 */	0xA5,	0xCA,			/* run_warm:       LDA     pp */
/* E838 */	0xA4,	0xCB,			/* LDY     pp+1 */
/* E83A */	0xD0,	0x5A,			/* BNE     Le896 */
/* E83C */					
/* E83C */					/* ; token $5C - "GOSUB" statement */
/* E83C */	0xA0,	0x41,			/* gosub_stmt:     LDY     #$41 */
/* E83E */	0xA5,	0xFC,			/* LDA     gosub_nest_count */
/* E840 */	0xC9,	0x08,			/* CMP     #$08 */
/* E842 */	0xB0,	0x5E,			/* BCS     go_errmess_4 */
/* E844 */	0xA8,				/* TAY */
/* E845 */	0xE6,	0xFC,			/* INC     gosub_nest_count */
/* E847 */	0xA5,	0xE0,			/* LDA     pverb */
/* E849 */	0x99,	0x00,	0x01,		/* STA     gstk_pverbl,Y */
/* E84C */	0xA5,	0xE1,			/* LDA     pverb+1 */
/* E84E */	0x99,	0x08,	0x01,		/* STA     gstk_pverbh,Y */
/* E851 */	0xA5,	0xDC,			/* LDA     pline */
/* E853 */	0x99,	0x10,	0x01,		/* STA     gstk_plinel,Y */
/* E856 */	0xA5,	0xDD,			/* LDA     pline+1 */
/* E858 */	0x99,	0x18,	0x01,		/* STA     gstk_plineh,Y */
/* E85B */					
/* E85B */					/* ; token $24 - "THEN" */
/* E85B */					/* ; token $5F - "GOTO" statement */
/* E85B */	0x20,	0x15,	0xE7,		/* goto_stmt:      JSR     get16bit */
/* E85E */	0x20,	0x6D,	0xE5,		/* JSR     find_line */
/* E861 */	0x90,	0x04,			/* BCC     Le867 */
/* E863 */	0xA0,	0x37,			/* LDY     #$37 */
/* E865 */	0xD0,	0x3B,			/* BNE     go_errmess_4 */
/* E867 */	0xA5,	0xE4,			/* Le867:  LDA     p2 */
/* E869 */	0xA4,	0xE5,			/* LDY     p2+1 */
/* E86B */					
/* E86B */					/* ; loop to run a program */
/* E86B */	0x85,	0xDC,			/* run_loop:       STA     pline */
/* E86D */	0x84,	0xDD,			/* STY     pline+1 */
// ====== KBD patch point ========================================================
/* E86F */	0x2C,	0x11,	0xD0,		/* BIT     KBDCR */
/* E872 */	0x30,	0x4F,			/* BMI     Le8c3 */
/* E874 */	0x18,				/* CLC */
/* E875 */	0x69,	0x03,			/* ADC     #$03 */
/* E877 */	0x90,	0x01,			/* BCC     Le87a */
/* E879 */	0xC8,				/* INY */
/* E87A */	0xA2,	0xFF,			/* Le87a:  LDX     #$FF */
/* E87C */	0x86,	0xD9,			/* STX     run_flag */
/* E87E */	0x9A,				/* TXS */
/* E87F */	0x85,	0xE0,			/* STA     pverb */
/* E881 */	0x84,	0xE1,			/* STY     pverb+1 */
/* E883 */	0x20,	0x79,	0xE6,		/* Le883:  JSR     execute_stmt */
/* E886 */	0x24,	0xD9,			/* BIT     run_flag */
/* E888 */	0x10,	0x49,			/* BPL     end_stmt */
/* E88A */	0x18,				/* CLC */
/* E88B */	0xA0,	0x00,			/* LDY     #$00 */
/* E88D */	0xA5,	0xDC,			/* LDA     pline */
/* E88F */	0x71,	0xDC,			/* ADC     (pline),Y */
/* E891 */	0xA4,	0xDD,			/* LDY     pline+1 */
/* E893 */	0x90,	0x01,			/* BCC     Le896 */
/* E895 */	0xC8,				/* INY */
/* E896 */	0xC5,	0x4C,			/* Le896:  CMP     himem */
/* E898 */	0xD0,	0xD1,			/* BNE     run_loop */
/* E89A */	0xC4,	0x4D,			/* CPY     himem+1 */
/* E89C */	0xD0,	0xCD,			/* BNE     run_loop */
/* E89E */	0xA0,	0x34,			/* LDY     #$34 */
/* E8A0 */	0x46,	0xD9,			/* LSR     run_flag */
/* E8A2 */	0x4C,	0xE0,	0xE3,		/* go_errmess_4:   JMP     print_err_msg */
/* E8A5 */					
/* E8A5 */					/* ; token $5B - "RETURN" statement */
/* E8A5 */	0xA0,	0x4A,			/* return_stmt:    LDY     #$4A */
/* E8A7 */	0xA5,	0xFC,			/* LDA     gosub_nest_count */
/* E8A9 */	0xF0,	0xF7,			/* BEQ     go_errmess_4 */
/* E8AB */	0xC6,	0xFC,			/* DEC     gosub_nest_count */
/* E8AD */	0xA8,				/* TAY */
/* E8AE */	0xB9,	0x0F,	0x01,		/* LDA     gstk_plinel-1,Y */
/* E8B1 */	0x85,	0xDC,			/* STA     pline */
/* E8B3 */	0xB9,	0x17,	0x01,		/* LDA     gstk_plineh-1,Y */
/* E8B6 */	0x85,	0xDD,			/* STA     pline+1 */
/* E8B8 */	0xBE,	0xFF,	0x00,		/* LDX     a:synpag+1,Y            ; force absolute addre */
/* E8BB */	0xB9,	0x07,	0x01,		/* LDA     gstk_pverbh-1,Y */
/* E8BE */	0xA8,				/* Le8be:  TAY */
/* E8BF */	0x8A,				/* TXA */
/* E8C0 */	0x4C,	0x7A,	0xE8,		/* JMP     Le87a */
/* E8C3 */	0xA0,	0x63,			/* Le8c3:  LDY     #$63 */
/* E8C5 */	0x20,	0xC4,	0xE3,		/* JSR     Se3c4 */
/* E8C8 */	0xA0,	0x01,			/* LDY     #$01 */
/* E8CA */	0xB1,	0xDC,			/* LDA     (pline),Y */
/* E8CC */	0xAA,				/* TAX */
/* E8CD */	0xC8,				/* INY */
/* E8CE */	0xB1,	0xDC,			/* LDA     (pline),Y */
/* E8D0 */	0x20,	0x1B,	0xE5,		/* JSR     prdec */
/* E8D3 */					
/* E8D3 */					/* ; token $51 - "END" statement */
/* E8D3 */	0x4C,	0xB3,	0xE2,		/* end_stmt:       JMP     warm */
/* E8D6 */	0xC6,	0xFB,			/* Le8d6:  DEC     for_nest_count */
/* E8D8 */					
/* E8D8 */					/* ; token $59 - "NEXT" statement */
/* E8D8 */					/* ; token $5A - "," in NEXT statement */
/* E8D8 */	0xA0,	0x5B,			/* next_stmt:      LDY     #$5B */
/* E8DA */	0xA5,	0xFB,			/* LDA     for_nest_count */
/* E8DC */	0xF0,	0xC4,			/* Le8dc:  BEQ     go_errmess_4 */
/* E8DE */	0xA8,				/* TAY */
/* E8DF */	0xB5,	0x50,			/* LDA     noun_stk_l,X */
/* E8E1 */	0xD9,	0x1F,	0x01,		/* CMP     fstk_varl-1,Y */
/* E8E4 */	0xD0,	0xF0,			/* BNE     Le8d6 */
/* E8E6 */	0xB5,	0x78,			/* LDA     noun_stk_h_str,X */
/* E8E8 */	0xD9,	0x27,	0x01,		/* CMP     fstk_varh-1,Y */
/* E8EB */	0xD0,	0xE9,			/* BNE     Le8d6 */
/* E8ED */	0xB9,	0x2F,	0x01,		/* LDA     fstk_stepl-1,Y */
/* E8F0 */	0x85,	0xDA,			/* STA     aux */
/* E8F2 */	0xB9,	0x37,	0x01,		/* LDA     fstk_steph-1,Y */
/* E8F5 */	0x85,	0xDB,			/* STA     aux+1 */
/* E8F7 */	0x20,	0x15,	0xE7,		/* JSR     get16bit */
/* E8FA */	0xCA,				/* DEX */
/* E8FB */	0x20,	0x93,	0xE7,		/* JSR     Se793 */
/* E8FE */	0x20,	0x01,	0xE8,		/* JSR     var_assign */
/* E901 */	0xCA,				/* DEX */
/* E902 */	0xA4,	0xFB,			/* LDY     for_nest_count */
/* E904 */	0xB9,	0x67,	0x01,		/* LDA     fstk_toh-1,Y */
/* E907 */	0x95,	0x9F,			/* STA     syn_stk_l+31,X */
/* E909 */	0xB9,	0x5F,	0x01,		/* LDA     fstk_tol-1,Y */
/* E90C */	0xA0,	0x00,			/* LDY     #$00 */
/* E90E */	0x20,	0x08,	0xE7,		/* JSR     push_ya_noun_stk */
/* E911 */	0x20,	0x82,	0xE7,		/* JSR     subtract */
/* E914 */	0x20,	0x59,	0xE7,		/* JSR     sgn_fn */
/* E917 */	0x20,	0x15,	0xE7,		/* JSR     get16bit */
/* E91A */	0xA4,	0xFB,			/* LDY     for_nest_count */
/* E91C */	0xA5,	0xCE,			/* LDA     acc */
/* E91E */	0xF0,	0x05,			/* BEQ     Le925 */
/* E920 */	0x59,	0x37,	0x01,		/* EOR     fstk_steph-1,Y */
/* E923 */	0x10,	0x12,			/* BPL     Le937 */
/* E925 */	0xB9,	0x3F,	0x01,		/* Le925:  LDA     fstk_plinel-1,Y */
/* E928 */	0x85,	0xDC,			/* STA     pline */
/* E92A */	0xB9,	0x47,	0x01,		/* LDA     fstk_plineh-1,Y */
/* E92D */	0x85,	0xDD,			/* STA     pline+1 */
/* E92F */	0xBE,	0x4F,	0x01,		/* LDX     fstk_pverbl-1,Y */
/* E932 */	0xB9,	0x57,	0x01,		/* LDA     fstk_pverbh-1,Y */
/* E935 */	0xD0,	0x87,			/* BNE     Le8be */
/* E937 */	0xC6,	0xFB,			/* Le937:  DEC     for_nest_count */
/* E939 */	0x60,				/* RTS */
/* E93A */					
/* E93A */					/* ; token $55 - "FOR" statement */
/* E93A */	0xA0,	0x54,			/* for_stmt:       LDY     #$54 */
/* E93C */	0xA5,	0xFB,			/* LDA     for_nest_count */
/* E93E */	0xC9,	0x08,			/* CMP     #$08 */
/* E940 */	0xF0,	0x9A,			/* BEQ     Le8dc */
/* E942 */	0xE6,	0xFB,			/* INC     for_nest_count */
/* E944 */	0xA8,				/* TAY */
/* E945 */	0xB5,	0x50,			/* LDA     noun_stk_l,X */
/* E947 */	0x99,	0x20,	0x01,		/* STA     fstk_varl,Y */
/* E94A */	0xB5,	0x78,			/* LDA     noun_stk_h_str,X */
/* E94C */	0x99,	0x28,	0x01,		/* STA     fstk_varh,Y */
/* E94F */	0x60,				/* RTS */
/* E950 */					
/* E950 */					/* ; token $57 - "TO" */
/* E950 */	0x20,	0x15,	0xE7,		/* to_clause:      JSR     get16bit */
/* E953 */	0xA4,	0xFB,			/* LDY     for_nest_count */
/* E955 */	0xA5,	0xCE,			/* LDA     acc */
/* E957 */	0x99,	0x5F,	0x01,		/* STA     fstk_tol-1,Y */
/* E95A */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E95C */	0x99,	0x67,	0x01,		/* STA     fstk_toh-1,Y */
/* E95F */	0xA9,	0x01,			/* LDA     #$01 */
/* E961 */	0x99,	0x2F,	0x01,		/* STA     fstk_stepl-1,Y */
/* E964 */	0xA9,	0x00,			/* LDA     #$00 */
/* E966 */	0x99,	0x37,	0x01,		/* Le966:  STA     fstk_steph-1,Y */
/* E969 */	0xA5,	0xDC,			/* LDA     pline */
/* E96B */	0x99,	0x3F,	0x01,		/* STA     fstk_plinel-1,Y */
/* E96E */	0xA5,	0xDD,			/* LDA     pline+1 */
/* E970 */	0x99,	0x47,	0x01,		/* STA     fstk_plineh-1,Y */
/* E973 */	0xA5,	0xE0,			/* LDA     pverb */
/* E975 */	0x99,	0x4F,	0x01,		/* STA     fstk_pverbl-1,Y */
/* E978 */	0xA5,	0xE1,			/* LDA     pverb+1 */
/* E97A */	0x99,	0x57,	0x01,		/* STA     fstk_pverbh-1,Y */
/* E97D */	0x60,				/* RTS */
/* E97E */					
/* E97E */	0x20,	0x15,	0xE7,		/* Te97e:  JSR     get16bit */
/* E981 */	0xA4,	0xFB,			/* LDY     for_nest_count */
/* E983 */	0xA5,	0xCE,			/* LDA     acc */
/* E985 */	0x99,	0x2F,	0x01,		/* STA     fstk_stepl-1,Y */
/* E988 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* E98A */	0x4C,	0x66,	0xE9,		/* JMP     Le966 */
/* E98D */	0x00,	0x00,	0x00,	0x00,	/* .byte   $00,$00,$00,$00,$00,$00,$00,$00 ; "........" */
/* E991 */	0x00,	0x00,	0x00,	0x00,	
/* E995 */	0x00,	0x00,	0x00,		/* .byte   $00,$00,$00                     ; "..." */
/* E998 */					
/* E998 */					/* ; verb precedence */
/* E998 */					/* ; (verb_prec[token]&0xAA)>>1 for left (?) */
/* E998 */					/* ; verb_prec[token]&0x55 for right (?) */
/* E998 */					/* verb_prec_tbl: */
/* E998 */	0x00,	0x00,	0x00,	0xAB,	/* .byte   $00,$00,$00,$AB,$03,$03,$03,$03 ; "...+...." */
/* E99C */	0x03,	0x03,	0x03,	0x03,	
/* E9A0 */	0x03,	0x03,	0x03,	0x03,	/* .byte   $03,$03,$03,$03,$03,$03,$03,$03 ; "........" */
/* E9A4 */	0x03,	0x03,	0x03,	0x03,	
/* E9A8 */	0x03,	0x03,	0x3F,	0x3F,	/* .byte   $03,$03,$3F,$3F,$C0,$C0,$3C,$3C ; "..??@@<<" */
/* E9AC */	0xC0,	0xC0,	0x3C,	0x3C,	
/* E9B0 */	0x3C,	0x3C,	0x3C,	0x3C,	/* .byte   $3C,$3C,$3C,$3C,$3C,$30,$0F,$C0 ; "<<<<<0.@" */
/* E9B4 */	0x3C,	0x30,	0x0F,	0xC0,	
/* E9B8 */	0xCC,	0xFF,	0x55,	0x00,	/* .byte   $CC,$FF,$55,$00,$AB,$AB,$03,$03 ; "L.U.++.." */
/* E9BC */	0xAB,	0xAB,	0x03,	0x03,	
/* E9C0 */	0xFF,	0xFF,	0x55,	0xFF,	/* .byte   $FF,$FF,$55,$FF,$FF,$55,$CF,$CF ; "..U..UOO" */
/* E9C4 */	0xFF,	0x55,	0xCF,	0xCF,	
/* E9C8 */	0xCF,	0xCF,	0xCF,	0xFF,	/* .byte   $CF,$CF,$CF,$FF,$55,$C3,$C3,$C3 ; "OOO.UCCC" */
/* E9CC */	0x55,	0xC3,	0xC3,	0xC3,	
/* E9D0 */	0x55,	0xF0,	0xF0,	0xCF,	/* .byte   $55,$F0,$F0,$CF,$56,$56,$56,$55 ; "UppOVVVU" */
/* E9D4 */	0x56,	0x56,	0x56,	0x55,	
/* E9D8 */	0xFF,	0xFF,	0x55,	0x03,	/* .byte   $FF,$FF,$55,$03,$03,$03,$03,$03 ; "..U....." */
/* E9DC */	0x03,	0x03,	0x03,	0x03,	
/* E9E0 */	0x03,	0x03,	0xFF,	0xFF,	/* .byte   $03,$03,$FF,$FF,$FF,$03,$03,$03 ; "........" */
/* E9E4 */	0xFF,	0x03,	0x03,	0x03,	
/* E9E8 */	0x03,	0x03,	0x03,	0x03,	/* .byte   $03,$03,$03,$03,$03,$03,$03,$03 ; "........" */
/* E9EC */	0x03,	0x03,	0x03,	0x03,	
/* E9F0 */	0x03,	0x03,	0x03,	0x03,	/* .byte   $03,$03,$03,$03,$03,$00,$AB,$03 ; "......+." */
/* E9F4 */	0x03,	0x00,	0xAB,	0x03,	
/* E9F8 */	0x57,	0x03,	0x03,	0x03,	/* .byte   $57,$03,$03,$03,$03,$07,$03,$03 ; "W......." */
/* E9FC */	0x03,	0x07,	0x03,	0x03,	
/* EA00 */	0x03,	0x03,	0x03,	0x03,	/* .byte   $03,$03,$03,$03,$03,$03,$03,$03 ; "........" */
/* EA04 */	0x03,	0x03,	0x03,	0x03,	
/* EA08 */	0x03,	0x03,	0xAA,	0xFF,	/* .byte   $03,$03,$AA,$FF,$FF,$FF,$FF,$FF ; "..*....." */
/* EA0C */	0xFF,	0xFF,	0xFF,	0xFF,	
/* EA10 */					/* verb_adr_l: */
/* EA10 */	0x17,	0xFF,	0xFF,	0x19,	/* .byte   $17,$FF,$FF,$19,$5D,$35,$4B,$F2 ; "....]5Kr" */
/* EA14 */	0x5D,	0x35,	0x4B,	0xF2,	
/* EA18 */	0xEC,	0x87,	0x6F,	0xAD,	/* .byte   $EC,$87,$6F,$AD,$B7,$E2,$F8,$54 ; "l.o-7bxT" */
/* EA1C */	0xB7,	0xE2,	0xF8,	0x54,	
/* EA20 */	0x80,	0x96,	0x85,	0x82,	/* .byte   $80,$96,$85,$82,$22,$10,$33,$4A ; "....".3J" */
/* EA24 */	0x22,	0x10,	0x33,	0x4A,	
/* EA28 */	0x13,	0x06,	0x0B,	0x4A,	/* .byte   $13,$06,$0B,$4A,$01,$40,$47,$7A ; "...J.@Gz" */
/* EA2C */	0x01,	0x40,	0x47,	0x7A,	
/* EA30 */	0x00,	0xFF,	0x23,	0x09,	/* .byte   $00,$FF,$23,$09,$5B,$16,$B6,$CB ; "..#.[.6K" */
/* EA34 */	0x5B,	0x16,	0xB6,	0xCB,	
/* EA38 */	0xFF,	0xFF,	0xFB,	0xFF,	/* .byte   $FF,$FF,$FB,$FF,$FF,$24,$F6,$4E ; "..{..$vN" */
/* EA3C */	0xFF,	0x24,	0xF6,	0x4E,	
/* EA40 */	0x59,	0x50,	0x00,	0xFF,	/* .byte   $59,$50,$00,$FF,$23,$A3,$6F,$36 ; "YP..##o6" */
/* EA44 */	0x23,	0xA3,	0x6F,	0x36,	
/* EA48 */	0x23,	0xD7,	0x1C,	0x22,	/* .byte   $23,$D7,$1C,$22,$C2,$AE,$BA,$23 ; "#W."B.:#" */
/* EA4C */	0xC2,	0xAE,	0xBA,	0x23,	
/* EA50 */	0xFF,	0xFF,	0x21,	0x30,	/* .byte   $FF,$FF,$21,$30,$1E,$03,$C4,$20 ; "..!0..D " */
/* EA54 */	0x1E,	0x03,	0xC4,	0x20,	
/* EA58 */	0x00,	0xC1,	0xFF,	0xFF,	/* .byte   $00,$C1,$FF,$FF,$FF,$A0,$30,$1E ; ".A... 0." */
/* EA5C */	0xFF,	0xA0,	0x30,	0x1E,	
/* EA60 */	0xA4,	0xD3,	0xB6,	0xBC,	/* .byte   $A4,$D3,$B6,$BC,$AA,$3A,$01,$50 ; "$S6<*:.P" */
/* EA64 */	0xAA,	0x3A,	0x01,	0x50,	
/* EA68 */	0x7E,	0xD8,	0xD8,	0xA5,	/* .byte   $7E,$D8,$D8,$A5,$3C,$FF,$16,$5B ; "~XX%<..[" */
/* EA6C */	0x3C,	0xFF,	0x16,	0x5B,	
/* EA70 */	0x28,	0x03,	0xC4,	0x1D,	/* .byte   $28,$03,$C4,$1D,$00,$0C,$4E,$00 ; "(.D...N." */
/* EA74 */	0x00,	0x0C,	0x4E,	0x00,	
/* EA78 */	0x3E,	0x00,	0xA6,	0xB0,	/* .byte   $3E,$00,$A6,$B0,$00,$BC,$C6,$57 ; ">.&0.<FW" */
/* EA7C */	0x00,	0xBC,	0xC6,	0x57,	
/* EA80 */	0x8C,	0x01,	0x27,	0xFF,	/* .byte   $8C,$01,$27,$FF,$FF,$FF,$FF,$FF ; "..'....." */
/* EA84 */	0xFF,	0xFF,	0xFF,	0xFF,	
/* EA88 */					/* verb_adr_h: */
/* EA88 */	0xE8,	0xFF,	0xFF,	0xE8,	/* .byte   $E8,$FF,$FF,$E8,$E0,$E0,$E0,$EF ; "h..h```o" */
/* EA8C */	0xE0,	0xE0,	0xE0,	0xEF,	
/* EA90 */	0xEF,	0xE3,	0xE3,	0xE5,	/* .byte   $EF,$E3,$E3,$E5,$E5,$E7,$E7,$EE ; "occeeggn" */
/* EA94 */	0xE5,	0xE7,	0xE7,	0xEE,	
/* EA98 */	0xEF,	0xEF,	0xE7,	0xE7,	/* .byte   $EF,$EF,$E7,$E7,$E2,$EF,$E7,$E7 ; "ooggbogg" */
/* EA9C */	0xE2,	0xEF,	0xE7,	0xE7,	
/* EAA0 */	0xEC,	0xEC,	0xEC,	0xE7,	/* .byte   $EC,$EC,$EC,$E7,$EC,$EC,$EC,$E2 ; "lllglllb" */
/* EAA4 */	0xEC,	0xEC,	0xEC,	0xE2,	
/* EAA8 */	0x00,	0xFF,	0xE8,	0xE1,	/* .byte   $00,$FF,$E8,$E1,$E8,$E8,$EF,$EB ; "..hahhok" */
/* EAAC */	0xE8,	0xE8,	0xEF,	0xEB,	
/* EAB0 */	0xFF,	0xFF,	0xE0,	0xFF,	/* .byte   $FF,$FF,$E0,$FF,$FF,$EF,$EE,$EF ; "..`..ono" */
/* EAB4 */	0xFF,	0xEF,	0xEE,	0xEF,	
/* EAB8 */	0xE7,	0xE7,	0x00,	0xFF,	/* .byte   $E7,$E7,$00,$FF,$E8,$E7,$E7,$E7 ; "gg..hggg" */
/* EABC */	0xE8,	0xE7,	0xE7,	0xE7,	
/* EAC0 */	0xE8,	0xE1,	0xE2,	0xEE,	/* .byte   $E8,$E1,$E2,$EE,$EE,$EE,$EE,$E8 ; "habnnnnh" */
/* EAC4 */	0xEE,	0xEE,	0xEE,	0xE8,	
/* EAC8 */	0xFF,	0xFF,	0xE1,	0xE1,	/* .byte   $FF,$FF,$E1,$E1,$EF,$EE,$E7,$E8 ; "..aaongh" */
/* EACC */	0xEF,	0xEE,	0xE7,	0xE8,	
/* EAD0 */	0xEE,	0xE7,	0xFF,	0xFF,	/* .byte   $EE,$E7,$FF,$FF,$FF,$EE,$E1,$EF ; "ng...nao" */
/* EAD4 */	0xFF,	0xEE,	0xE1,	0xEF,	
/* EAD8 */	0xE7,	0xE8,	0xEF,	0xEF,	/* .byte   $E7,$E8,$EF,$EF,$EB,$E9,$E8,$E9 ; "ghookihi" */
/* EADC */	0xEB,	0xE9,	0xE8,	0xE9,	
/* EAE0 */	0xE9,	0xE8,	0xE8,	0xE8,	/* .byte   $E9,$E8,$E8,$E8,$E8,$FF,$E8,$E8 ; "ihhhh.hh" */
/* EAE4 */	0xE8,	0xFF,	0xE8,	0xE8,	
/* EAE8 */	0xE8,	0xEE,	0xE7,	0xE8,	/* .byte   $E8,$EE,$E7,$E8,$EF,$EF,$EE,$EF ; "hnghoono" */
/* EAEC */	0xEF,	0xEF,	0xEE,	0xEF,	
/* EAF0 */	0xEE,	0xEF,	0xEE,	0xEE,	/* .byte   $EE,$EF,$EE,$EE,$EF,$EE,$EE,$EE ; "nonnonnn" */
/* EAF4 */	0xEF,	0xEE,	0xEE,	0xEE,	
/* EAF8 */	0xE1,	0xE8,	0xE8,	0xFF,	/* .byte   $E1,$E8,$E8,$FF,$FF,$FF,$FF,$FF ; "ahh....." */
/* EAFC */	0xFF,	0xFF,	0xFF,	0xFF,	
/* EB00 */					
/* EB00 */					/* ; Error message strings. Last character has high bit unset. */
/* EB00 */					/* error_msg_tbl: */
/* EB00 */	0xBE,	0xB3,	0xB2,	0xB7,	/* .byte   $BE,$B3,$B2,$B7,$B6,$37         ; ">32767" */
/* EB04 */	0xB6,	0x37,			
/* EB06 */	0xD4,	0xCF,	0xCF,	0xA0,	/* .byte   $D4,$CF,$CF,$A0,$CC,$CF,$CE,$47 ; "TOO LONG" */
/* EB0A */	0xCC,	0xCF,	0xCE,	0x47,	
/* EB0E */	0xD3,	0xD9,	0xCE,	0xD4,	/* .byte   $D3,$D9,$CE,$D4,$C1,$58         ; "SYNTAX" */
/* EB12 */	0xC1,	0x58,			
/* EB14 */	0xCD,	0xC5,	0xCD,	0xA0,	/* .byte   $CD,$C5,$CD,$A0,$C6,$D5,$CC,$4C ; "MEM FULL" */
/* EB18 */	0xC6,	0xD5,	0xCC,	0x4C,	
/* EB1C */	0xD4,	0xCF,	0xCF,	0xA0,	/* .byte   $D4,$CF,$CF,$A0,$CD,$C1,$CE,$D9,$A0,$D0,$C1,$D */
/* EB20 */	0xCD,	0xC1,	0xCE,	0xD9,	
/* EB24 */	0xA0,	0xD0,	0xC1,	0xD2,	0xC5, 0xCE, 0x53,
/* EB2B */	0xD3,	0xD4,	0xD2,	0xC9,	/* .byte   $D3,$D4,$D2,$C9,$CE,$47         ; "STRING" */
/* EB2F */	0xCE,	0x47,			
/* EB31 */	0xCE,	0xCF,	0xA0,	0xC5,	/* .byte   $CE,$CF,$A0,$C5,$CE,$44         ; "NO END" */
/* EB35 */	0xCE,	0x44,			
/* EB37 */	0xC2,	0xC1,	0xC4,	0xA0,	/* .byte   $C2,$C1,$C4,$A0,$C2,$D2,$C1,$CE,$C3,$48 ; "BAD */
/* EB3B */	0xC2,	0xD2,	0xC1,	0xCE,	
/* EB3F */	0xC3,	0x48,			
/* EB41 */	0xBE,	0xB8,	0xA0,	0xC7,	/* .byte   $BE,$B8,$A0,$C7,$CF,$D3,$D5,$C2,$53     ; ">8 */
/* EB45 */	0xCF,	0xD3,	0xD5,	0xC2,	
/* EB49 */	0x53,				
/* EB4A */	0xC2,	0xC1,	0xC4,	0xA0,	/* .byte   $C2,$C1,$C4,$A0,$D2,$C5,$D4,$D5,$D2,$4E ; "BAD */
/* EB4E */	0xD2,	0xC5,	0xD4,	0xD5,	
/* EB52 */	0xD2,	0x4E,			
/* EB54 */	0xBE,	0xB8,	0xA0,	0xC6,	/* .byte   $BE,$B8,$A0,$C6,$CF,$D2,$53     ; ">8 FORS" */
/* EB58 */	0xCF,	0xD2,	0x53,		
/* EB5B */	0xC2,	0xC1,	0xC4,	0xA0,	/* .byte   $C2,$C1,$C4,$A0,$CE,$C5,$D8,$54 ; "BAD NEXT" */
/* EB5F */	0xCE,	0xC5,	0xD8,	0x54,	
/* EB63 */	0xD3,	0xD4,	0xCF,	0xD0,	/* .byte   $D3,$D4,$CF,$D0,$D0,$C5,$C4,$A0,$C1,$D4,$20 ; */
/* EB67 */	0xD0,	0xC5,	0xC4,	0xA0,	
/* EB6B */	0xC1,	0xD4,	0x20,		
/* EB6E */	0xAA,	0xAA,	0xAA,	0x20,	/* .byte   $AA,$AA,$AA,$20                 ; "*** " */
/* EB72 */	0xA0,	0xC5,	0xD2,	0xD2,	/* .byte   $A0,$C5,$D2,$D2,$0D             ; " ERR.\n" */
/* EB76 */	0x0D,				
/* EB77 */	0xBE,	0xB2,	0xB5,	0x35,	/* .byte   $BE,$B2,$B5,$35                 ; ">255" */
/* EB7B */	0xD2,	0xC1,	0xCE,	0xC7,	/* .byte   $D2,$C1,$CE,$C7,$45             ; RANGE" */
/* EB7F */	0x45,				
/* EB80 */	0xC4,	0xC9,	0x4D,		/* .byte   $C4,$C9,$4D                     ; "DIM" */
/* EB83 */	0xD3,	0xD4,	0xD2,	0xA0,	/* .byte   $D3,$D4,$D2,$A0,$CF,$D6,$C6,$4C ; "STR OVFL" */
/* EB87 */	0xCF,	0xD6,	0xC6,	0x4C,	
/* EB8B */	0xDC,	0x0D,			/* .byte   $DC,$0D                         ; "\\\n" */
/* EB8D */	0xD2,	0xC5,	0xD4,	0xD9,	/* .byte   $D2,$C5,$D4,$D9,$D0,$C5,$A0,$CC,$C9,$CE,$C5,$8 */
/* EB91 */	0xD0,	0xC5,	0xA0,	0xCC,	
/* EB95 */	0xC9,	0xCE,	0xC5,	0x8D,	
/* EB99 */	0x3F,				/* .byte   $3F                             ; "?" */
/* EB9A */	0x46,	0xD9,			/* Leb9a:  LSR     run_flag */
/* EB9C */	0x90,	0x03,			/* BCC     Leba1 */
/* EB9E */	0x4C,	0xC3,	0xE8,		/* JMP     Le8c3 */
/* EBA1 */	0xA6,	0xCF,			/* Leba1:  LDX     acc+1 */
/* EBA3 */	0x9A,				/* TXS */
/* EBA4 */	0xA6,	0xCE,			/* LDX     acc */
/* EBA6 */	0xA0,	0x8D,			/* LDY     #$8D */
/* EBA8 */	0xD0,	0x02,			/* BNE     Lebac */
/* EBAA */					
/* EBAA */					/* ; token $54 - "INPUT" statement, numeric, no prompt */
/* EBAA */	0xA0,	0x99,			/* input_num_stmt: LDY     #$99 */
/* EBAC */	0x20,	0xC4,	0xE3,		/* Lebac:  JSR     Se3c4 */
/* EBAF */	0x86,	0xCE,			/* STX     acc */
/* EBB1 */	0xBA,				/* TSX */
/* EBB2 */	0x86,	0xCF,			/* STX     acc+1 */
/* EBB4 */	0xA0,	0xFE,			/* LDY     #$FE */
/* EBB6 */	0x84,	0xD9,			/* STY     run_flag */
/* EBB8 */	0xC8,				/* INY */
/* EBB9 */	0x84,	0xC8,			/* STY     text_index */
/* EBBB */	0x20,	0x99,	0xE2,		/* JSR     Se299 */
/* EBBE */	0x84,	0xF1,			/* STY     token_index */
/* EBC0 */	0xA2,	0x20,			/* LDX     #$20 */
/* EBC2 */	0xA9,	0x30,			/* LDA     #$30 */
/* EBC4 */	0x20,	0x91,	0xE4,		/* JSR     Se491 */
/* EBC7 */	0xE6,	0xD9,			/* INC     run_flag */
/* EBC9 */	0xA6,	0xCE,			/* LDX     acc */
/* EBCB */					
/* EBCB */					/* ; token $27 - "," numeric input */
/* EBCB */	0xA4,	0xC8,			/* input_num_comma:        LDY     text_index */
/* EBCD */	0x0A,				/* ASL */
/* EBCE */	0x85,	0xCE,			/* Lebce:  STA     acc */
/* EBD0 */	0xC8,				/* INY */
/* EBD1 */	0xB9,	0x00,	0x02,		/* LDA     buffer,Y */
/* EBD4 */	0xC9,	0x74,			/* CMP     #$74 */
/* EBD6 */	0xF0,	0xD2,			/* BEQ     input_num_stmt */
/* EBD8 */	0x49,	0xB0,			/* EOR     #$B0 */
/* EBDA */	0xC9,	0x0A,			/* CMP     #$0A */
/* EBDC */	0xB0,	0xF0,			/* BCS     Lebce */
/* EBDE */	0xC8,				/* INY */
/* EBDF */	0xC8,				/* INY */
/* EBE0 */	0x84,	0xC8,			/* STY     text_index */
/* EBE2 */	0xB9,	0x00,	0x02,		/* LDA     buffer,Y */
/* EBE5 */	0x48,				/* PHA */
/* EBE6 */	0xB9,	0xFF,	0x01,		/* LDA     buffer-1,Y */
/* EBE9 */	0xA0,	0x00,			/* LDY     #$00 */
/* EBEB */	0x20,	0x08,	0xE7,		/* JSR     push_ya_noun_stk */
/* EBEE */	0x68,				/* PLA */
/* EBEF */	0x95,	0xA0,			/* STA     noun_stk_h_int,X */
/* EBF1 */	0xA5,	0xCE,			/* LDA     acc */
/* EBF3 */	0xC9,	0xC7,			/* CMP     #$C7 */
/* EBF5 */	0xD0,	0x03,			/* BNE     Lebfa */
/* EBF7 */	0x20,	0x6F,	0xE7,		/* JSR     negate */
/* EBFA */	0x4C,	0x01,	0xE8,		/* Lebfa:  JMP     var_assign */
/* EBFD */					
/* EBFD */	0xFF,	0xFF,	0xFF,	0x50,	/* .byte   $FF,$FF,$FF,$50 */
/* EC01 */					
/* EC01 */	0x20,	0x13,	0xEC,		/* Tec01:  JSR     Tec13 */
/* EC04 */	0xD0,	0x15,			/* BNE     Lec1b */
/* EC06 */					
/* EC06 */	0x20,	0x0B,	0xEC,		/* Tec06:  JSR     Tec0b */
/* EC09 */	0xD0,	0x10,			/* BNE     Lec1b */
/* EC0B */					
/* EC0B */	0x20,	0x82,	0xE7,		/* Tec0b:  JSR     subtract */
/* EC0E */	0x20,	0x6F,	0xE7,		/* JSR     negate */
/* EC11 */	0x50,	0x03,			/* BVC     Lec16 */
/* EC13 */					
/* EC13 */	0x20,	0x82,	0xE7,		/* Tec13:  JSR     subtract */
/* EC16 */	0x20,	0x59,	0xE7,		/* Lec16:  JSR     sgn_fn */
/* EC19 */	0x56,	0x50,			/* LSR     noun_stk_l,X */
/* EC1B */	0x4C,	0x36,	0xE7,		/* Lec1b:  JMP     not_op */
/* EC1E */					
/* EC1E */	0xFF,	0xFF,			/* .byte   $FF,$FF */
/* EC20 */					
/* EC20 */					/* ; indexes into syntabl */
/* EC20 */					/* syntabl_index: */
/* EC20 */	0xC1,	0xFF,	0x7F,	0xD1,	/* .byte   $C1,$FF,$7F,$D1,$CC,$C7,$CF,$CE ; "A..QLGON" */
/* EC24 */	0xCC,	0xC7,	0xCF,	0xCE,	
/* EC28 */	0xC5,	0x9A,	0x98,	0x8B,	/* .byte   $C5,$9A,$98,$8B,$96,$95,$93,$BF ; "E......?" */
/* EC2C */	0x96,	0x95,	0x93,	0xBF,	
/* EC30 */	0xB2,	0x32,	0x2D,	0x2B,	/* .byte   $B2,$32,$2D,$2B,$BC,$B0,$AC,$BE ; "22-+<0,>" */
/* EC34 */	0xBC,	0xB0,	0xAC,	0xBE,	
/* EC38 */	0x35,	0x8E,	0x61,	0xFF,	/* .byte   $35,$8E,$61,$FF,$FF,$FF,$DD,$FB ; "5.a...]{" */
/* EC3C */	0xFF,	0xFF,	0xDD,	0xFB,	
/* EC40 */					
/* EC40 */	0x20,	0xC9,	0xEF,		/* Tec40:  JSR     Sefc9 */
/* EC43 */	0x15,	0x4F,			/* ORA     rnd+1,X */
/* EC45 */	0x10,	0x05,			/* BPL     Lec4c */
/* EC47 */					
/* EC47 */	0x20,	0xC9,	0xEF,		/* Tec47:  JSR     Sefc9 */
/* EC4A */	0x35,	0x4F,			/* AND     rnd+1,X */
/* EC4C */	0x95,	0x50,			/* Lec4c:  STA     noun_stk_l,X */
/* EC4E */	0x10,	0xCB,			/* BPL     Lec1b */
/* EC50 */	0x4C,	0xC9,	0xEF,		/* JMP     Sefc9 */
/* EC53 */	0x40,	0x60,	0x8D,	0x60,	/* .byte   $40,$60,$8D,$60,$8B,$00,$7E,$8C ; "@`.`..~." */
/* EC57 */	0x8B,	0x00,	0x7E,	0x8C,	
/* EC5B */	0x33,	0x00,	0x00,	0x60,	/* .byte   $33,$00,$00,$60,$03,$BF,$12,$00 ; "3..`.?.." */
/* EC5F */	0x03,	0xBF,	0x12,	0x00,	
/* EC63 */	0x40,	0x89,	0xC9,	0x47,	/* .byte   $40,$89,$C9,$47,$9D,$17,$68,$9D ; "@.IG..h." */
/* EC67 */	0x9D,	0x17,	0x68,	0x9D,	
/* EC6B */	0x0A,	0x00,	0x40,	0x60,	/* .byte   $0A,$00,$40,$60,$8D,$60,$8B,$00 ; "..@`.`.." */
/* EC6F */	0x8D,	0x60,	0x8B,	0x00,	
/* EC73 */	0x7E,	0x8C,	0x3C,	0x00,	/* .byte   $7E,$8C,$3C,$00,$00,$60,$03,$BF ; "~.<..`.?" */
/* EC77 */	0x00,	0x60,	0x03,	0xBF,	
/* EC7B */	0x1B,	0x4B,	0x67,	0xB4,	/* .byte   $1B,$4B,$67,$B4,$A1,$07,$8C,$07 ; ".Kg4!..." */
/* EC7F */	0xA1,	0x07,	0x8C,	0x07,	
/* EC83 */	0xAE,	0xA9,	0xAC,	0xA8,	/* .byte   $AE,$A9,$AC,$A8,$67,$8C,$07,$B4 ; ".),(g..4" */
/* EC87 */	0x67,	0x8C,	0x07,	0xB4,	
/* EC8B */	0xAF,	0xAC,	0xB0,	0x67,	/* .byte   $AF,$AC,$B0,$67,$9D,$B2,$AF,$AC ; "/,0g.2/," */
/* EC8F */	0x9D,	0xB2,	0xAF,	0xAC,	
/* EC93 */	0xAF,	0xA3,	0x67,	0x8C,	/* .byte   $AF,$A3,$67,$8C,$07,$A5,$AB,$AF ; "/#g..%+/" */
/* EC97 */	0x07,	0xA5,	0xAB,	0xAF,	
/* EC9B */	0xB0,	0xF4,	0xAE,	0xA9,	/* .byte   $B0,$F4,$AE,$A9,$B2,$B0,$7F,$0E ; "0t.)20.." */
/* EC9F */	0xB2,	0xB0,	0x7F,	0x0E,	
/* ECA3 */	0x27,	0xB4,	0xAE,	0xA9,	/* .byte   $27,$B4,$AE,$A9,$B2,$B0,$7F,$0E ; "'4.)20.." */
/* ECA7 */	0xB2,	0xB0,	0x7F,	0x0E,	
/* ECAB */	0x28,	0xB4,	0xAE,	0xA9,	/* .byte   $28,$B4,$AE,$A9,$B2,$B0,$64,$07 ; "(4.)20d." */
/* ECAF */	0xB2,	0xB0,	0x64,	0x07,	
/* ECB3 */	0xA6,	0xA9,	0x67,	0xAF,	/* .byte   $A6,$A9,$67,$AF,$B4,$AF,$A7,$78 ; "&)g/4/'x" */
/* ECB7 */	0xB4,	0xAF,	0xA7,	0x78,	
/* ECBB */	0xB4,	0xA5,	0xAC,	0x78,	/* .byte   $B4,$A5,$AC,$78,$7F,$02,$AD,$A5 ; "4%,x..-%" */
/* ECBF */	0x7F,	0x02,	0xAD,	0xA5,	
/* ECC3 */	0xB2,	0x67,	0xA2,	0xB5,	/* .byte   $B2,$67,$A2,$B5,$B3,$AF,$A7,$EE ; "2g"53/'n" */
/* ECC7 */	0xB3,	0xAF,	0xA7,	0xEE,	
/* ECCB */	0xB2,	0xB5,	0xB4,	0xA5,	/* .byte   $B2,$B5,$B4,$A5,$B2,$7E,$8C,$39 ; "254%2~.9" */
/* ECCF */	0xB2,	0x7E,	0x8C,	0x39,	
/* ECD3 */	0xB4,	0xB8,	0xA5,	0xAE,	/* .byte   $B4,$B8,$A5,$AE,$67,$B0,$A5,$B4 ; "48%.g0%4" */
/* ECD7 */	0x67,	0xB0,	0xA5,	0xB4,	
/* ECDB */	0xB3,	0x27,	0xAF,	0xB4,	/* .byte   $B3,$27,$AF,$B4,$07,$9D,$19,$B2 ; "3'/4...2" */
/* ECDF */	0x07,	0x9D,	0x19,	0xB2,	
/* ECE3 */	0xAF,	0xA6,	0x7F,	0x05,	/* .byte   $AF,$A6,$7F,$05,$37,$B4,$B5,$B0 ; "/&..7450" */
/* ECE7 */	0x37,	0xB4,	0xB5,	0xB0,	
/* ECEB */	0xAE,	0xA9,	0x7F,	0x05,	/* .byte   $AE,$A9,$7F,$05,$28,$B4,$B5,$B0 ; ".)..(450" */
/* ECEF */	0x28,	0xB4,	0xB5,	0xB0,	
/* ECF3 */	0xAE,	0xA9,	0x7F,	0x05,	/* .byte   $AE,$A9,$7F,$05,$2A,$B4,$B5,$B0 ; ".)..*450" */
/* ECF7 */	0x2A,	0xB4,	0xB5,	0xB0,	
/* ECFB */	0xAE,	0xA9,	0xE4,	0xAE,	/* .byte   $AE,$A9,$E4,$AE,$A5,$00,$FF,$FF ; ".)d.%..." */
/* ECFF */	0xA5,	0x00,	0xFF,	0xFF,	
/* ED03 */					/* syntabl2: */
/* ED03 */	0x47,	0xA2,	0xA1,	0xB4,	/* .byte   $47,$A2,$A1,$B4,$7F,$0D,$30,$AD ; "G"!4..0-" */
/* ED07 */	0x7F,	0x0D,	0x30,	0xAD,	
/* ED0B */	0xA9,	0xA4,	0x7F,	0x0D,	/* .byte   $A9,$A4,$7F,$0D,$23,$AD,$A9,$A4 ; ")$..#-)$" */
/* ED0F */	0x23,	0xAD,	0xA9,	0xA4,	
/* ED13 */	0x67,	0xAC,	0xAC,	0xA1,	/* .byte   $67,$AC,$AC,$A1,$A3,$00,$40,$80 ; "g,,!#.@." */
/* ED17 */	0xA3,	0x00,	0x40,	0x80,	
/* ED1B */	0xC0,	0xC1,	0x80,	0x00,	/* .byte   $C0,$C1,$80,$00,$47,$8C,$68,$8C ; "@A..G.h." */
/* ED1F */	0x47,	0x8C,	0x68,	0x8C,	
/* ED23 */	0xDB,	0x67,	0x9B,	0x68,	/* .byte   $DB,$67,$9B,$68,$9B,$50,$8C,$63 ; "[g.h.P.c" */
/* ED27 */	0x9B,	0x50,	0x8C,	0x63,	
/* ED2B */	0x8C,	0x7F,	0x01,	0x51,	/* .byte   $8C,$7F,$01,$51,$07,$88,$29,$84 ; "...Q..)." */
/* ED2F */	0x07,	0x88,	0x29,	0x84,	
/* ED33 */	0x80,	0xC4,	0x80,	0x57,	/* .byte   $80,$C4,$80,$57,$71,$07,$88,$14 ; ".D.Wq..." */
/* ED37 */	0x71,	0x07,	0x88,	0x14,	
/* ED3B */	0xED,	0xA5,	0xAD,	0xAF,	/* .byte   $ED,$A5,$AD,$AF,$AC,$ED,$A5,$AD ; "m%-/,m%-" */
/* ED3F */	0xAC,	0xED,	0xA5,	0xAD,	
/* ED43 */	0xA9,	0xA8,	0xF2,	0xAF,	/* .byte   $A9,$A8,$F2,$AF,$AC,$AF,$A3,$71 ; ")(r/,/#q" */
/* ED47 */	0xAC,	0xAF,	0xA3,	0x71,	
/* ED4B */	0x08,	0x88,	0xAE,	0xA5,	/* .byte   $08,$88,$AE,$A5,$AC,$68,$83,$08 ; "...%,h.." */
/* ED4F */	0xAC,	0x68,	0x83,	0x08,	
/* ED53 */	0x68,	0x9D,	0x08,	0x71,	/* .byte   $68,$9D,$08,$71,$07,$88,$60,$76 ; "h..q..`v" */
/* ED57 */	0x07,	0x88,	0x60,	0x76,	
/* ED5B */	0xB4,	0xAF,	0xAE,	0x76,	/* .byte   $B4,$AF,$AE,$76,$8D,$76,$8B,$51 ; "4/.v.v.Q" */
/* ED5F */	0x8D,	0x76,	0x8B,	0x51,	
/* ED63 */	0x07,	0x88,	0x19,	0xB8,	/* .byte   $07,$88,$19,$B8,$A4,$AE,$B2,$F2 ; "...8$.2r" */
/* ED67 */	0xA4,	0xAE,	0xB2,	0xF2,	
/* ED6B */	0xB3,	0xB5,	0xF3,	0xA2,	/* .byte   $B3,$B5,$F3,$A2,$A1,$EE,$A7,$B3 ; "35s"!n'3" */
/* ED6F */	0xA1,	0xEE,	0xA7,	0xB3,	
/* ED73 */	0xE4,	0xAE,	0xB2,	0xEB,	/* .byte   $E4,$AE,$B2,$EB,$A5,$A5,$B0,$51 ; "d.2k%%0Q" */
/* ED77 */	0xA5,	0xA5,	0xB0,	0x51,	
/* ED7B */	0x07,	0x88,	0x39,	0x81,	/* .byte   $07,$88,$39,$81,$C1,$4F,$7F,$0F ; "..9.AO.." */
/* ED7F */	0xC1,	0x4F,	0x7F,	0x0F,	
/* ED83 */	0x2F,	0x00,	0x51,	0x06,	/* .byte   $2F,$00,$51,$06,$88,$29,$C2,$0C ; "/.Q..)B." */
/* ED87 */	0x88,	0x29,	0xC2,	0x0C,	
/* ED8B */	0x82,	0x57,	0x8C,	0x6A,	/* .byte   $82,$57,$8C,$6A,$8C,$42,$AE,$A5 ; ".W.j.B.%" */
/* ED8F */	0x8C,	0x42,	0xAE,	0xA5,	
/* ED93 */	0xA8,	0xB4,	0x60,	0xAE,	/* .byte   $A8,$B4,$60,$AE,$A5,$A8,$B4,$4F ; "(4`.%(4O" */
/* ED97 */	0xA5,	0xA8,	0xB4,	0x4F,	
/* ED9B */	0x7E,	0x1E,	0x35,	0x8C,	/* .byte   $7E,$1E,$35,$8C,$27,$51,$07,$88 ; "~.5.'Q.." */
/* ED9F */	0x27,	0x51,	0x07,	0x88,	
/* EDA3 */	0x09,	0x8B,	0xFE,	0xE4,	/* .byte   $09,$8B,$FE,$E4,$AF,$AD,$F2,$AF ; "..~d/-r/" */
/* EDA7 */	0xAF,	0xAD,	0xF2,	0xAF,	
/* EDAB */	0xE4,	0xAE,	0xA1,	0xDC,	/* .byte   $E4,$AE,$A1,$DC,$DE,$9C,$DD,$9C ; "d.!\^.]." */
/* EDAF */	0xDE,	0x9C,	0xDD,	0x9C,	
/* EDB3 */	0xDE,	0xDD,	0x9E,	0xC3,	/* .byte   $DE,$DD,$9E,$C3,$DD,$CF,$CA,$CD ; "^].C]OJM" */
/* EDB7 */	0xDD,	0xCF,	0xCA,	0xCD,	
/* EDBB */	0xCB,	0x00,	0x47,	0x9D,	/* .byte   $CB,$00,$47,$9D,$AD,$A5,$AD,$AF ; "K.G.-%-/" */
/* EDBF */	0xAD,	0xA5,	0xAD,	0xAF,	
/* EDC3 */	0xAC,	0x76,	0x9D,	0xAD,	/* .byte   $AC,$76,$9D,$AD,$A5,$AD,$A9,$A8 ; ",v.-%-)(" */
/* EDC7 */	0xA5,	0xAD,	0xA9,	0xA8,	
/* EDCB */	0xE6,	0xA6,	0xAF,	0x60,	/* .byte   $E6,$A6,$AF,$60,$8C,$20,$AF,$B4 ; "f&/`. /4" */
/* EDCF */	0x8C,	0x20,	0xAF,	0xB4,	
/* EDD3 */	0xB5,	0xA1,	0xF2,	0xAC,	/* .byte   $B5,$A1,$F2,$AC,$A3,$F2,$A3,$B3 ; "5!r,#r#3" */
/* EDD7 */	0xA3,	0xF2,	0xA3,	0xB3,	
/* EDDB */	0x60,	0x8C,	0x20,	0xAC,	/* .byte   $60,$8C,$20,$AC,$A5,$A4,$EE,$B5 ; "`. ,%$n5" */
/* EDDF */	0xA5,	0xA4,	0xEE,	0xB5,	
/* EDE3 */	0xB2,	0x60,	0xAE,	0xB5,	/* .byte   $B2,$60,$AE,$B5,$B2,$F4,$B3,$A9 ; "2`.52t3)" */
/* EDE7 */	0xB2,	0xF4,	0xB3,	0xA9,	
/* EDEB */	0xAC,	0x60,	0x8C,	0x20,	/* .byte   $AC,$60,$8C,$20,$B4,$B3,$A9,$AC ; ",`. 43)," */
/* EDEF */	0xB4,	0xB3,	0xA9,	0xAC,	
/* EDF3 */	0x7A,	0x7E,	0x9A,	0x22,	/* .byte   $7A,$7E,$9A,$22,$20,$00,$60,$03 ; "z~." .`." */
/* EDF7 */	0x20,	0x00,	0x60,	0x03,	
/* EDFB */	0xBF,	0x60,	0x03,	0xBF,	/* .byte   $BF,$60,$03,$BF,$1F             ; "?`.?." */
/* EDFF */	0x1F,				
/* EE00 */					
/* EE00 */					/* ; token $48 - "," string output */
/* EE00 */	0x20,	0xB1,	0xE7,		/* print_str_comma:        JSR     tabout */
/* EE03 */					
/* EE03 */					/* ; token $45 - ";" string output */
/* EE03 */					/* ; token $61 - "PRINT" string */
/* EE03 */	0xE8,				/* print_str:      INX */
/* EE04 */	0xE8,				/* INX */
/* EE05 */	0xB5,	0x4F,			/* LDA     rnd+1,X */
/* EE07 */	0x85,	0xDA,			/* STA     aux */
/* EE09 */	0xB5,	0x77,			/* LDA     syn_stk_h+31,X */
/* EE0B */	0x85,	0xDB,			/* STA     aux+1 */
/* EE0D */	0xB4,	0x4E,			/* LDY     rnd,X */
/* EE0F */	0x98,				/* Lee0f:  TYA */
/* EE10 */	0xD5,	0x76,			/* CMP     syn_stk_h+30,X */
/* EE12 */	0xB0,	0x09,			/* BCS     Lee1d */
/* EE14 */	0xB1,	0xDA,			/* LDA     (aux),Y */
/* EE16 */	0x20,	0xC9,	0xE3,		/* JSR     cout */
/* EE19 */	0xC8,				/* INY */
/* EE1A */	0x4C,	0x0F,	0xEE,		/* JMP     Lee0f */
/* EE1D */	0xA9,	0xFF,			/* Lee1d:  LDA     #$FF */
/* EE1F */	0x85,	0xD5,			/* STA     cr_flag */
/* EE21 */	0x60,				/* RTS */
/* EE22 */					
/* EE22 */					/* ; token $3B - "LEN(" function */
/* EE22 */	0xE8,				/* len_fn: INX */
/* EE23 */	0xA9,	0x00,			/* LDA     #$00 */
/* EE25 */	0x95,	0x78,			/* STA     noun_stk_h_str,X */
/* EE27 */	0x95,	0xA0,			/* STA     noun_stk_h_int,X */
/* EE29 */	0xB5,	0x77,			/* LDA     syn_stk_h+31,X */
/* EE2B */	0x38,				/* SEC */
/* EE2C */	0xF5,	0x4F,			/* SBC     rnd+1,X */
/* EE2E */	0x95,	0x50,			/* STA     noun_stk_l,X */
/* EE30 */	0x4C,	0x23,	0xE8,		/* JMP     left_paren */
/* EE33 */					
/* EE33 */	0xFF,				/* .byte   $FF */
/* EE34 */					
/* EE34 */	0x20,	0x15,	0xE7,		/* getbyte:        JSR     get16bit */
/* EE37 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* EE39 */	0xD0,	0x28,			/* BNE     gr_255_err */
/* EE3B */	0xA5,	0xCE,			/* LDA     acc */
/* EE3D */	0x60,				/* RTS */
/* EE3E */					
/* EE3E */					/* ; token $68 - "," for PLOT statement (???) */
/* EE3E */	0x20,	0x34,	0xEE,		/* plot_comma:     JSR     getbyte */
/* EE41 */	0xA4,	0xC8,			/* LDY     text_index */
/* EE43 */	0xC9,	0x30,			/* CMP     #$30 */
/* EE45 */	0xB0,	0x21,			/* BCS     range_err */
/* EE47 */	0xC0,	0x28,			/* CPY     #$28 */
/* EE49 */	0xB0,	0x1D,			/* BCS     range_err */
/* EE4B */	0x60,				/* RTS */
/* EE4C */	0xEA,				/* NOP */
/* EE4D */	0xEA,				/* NOP */
/* EE4E */					
/* EE4E */	0x20,	0x34,	0xEE,		/* Tee4e:  JSR     getbyte */
/* EE51 */	0x60,				/* RTS */
/* EE52 */	0xEA,				/* NOP */
/* EE53 */	0x8A,				/* Tee5e:  TXA */
/* EE54 */	0xA2,	0x01,			/* LDX     #$01 */
/* EE56 */	0xB4,	0xCE,			/* l123:   LDY     acc,X */
/* EE58 */	0x94,	0x4C,			/* STY     himem,X */
/* EE5A */	0xB4,	0x48,			/* LDY     var,X */
/* EE5C */	0x94,	0xCA,			/* STY     pp,X */
/* EE5E */	0xCA,				/* DEX */
/* EE5F */	0xF0,	0xF5,			/* BEQ     l123 */
/* EE61 */	0xAA,				/* TAX */
/* EE62 */	0x60,				/* RTS */
/* EE63 */	0xA0,	0x77,			/* gr_255_err:     LDY     #$77            ; > 255 error */
/* EE65 */	0x4C,	0xE0,	0xE3,		/* go_errmess_5:   JMP     print_err_msg */
/* EE68 */	0xA0,	0x7B,			/* range_err:      LDY     #$7B            ; range error */
/* EE6A */	0xD0,	0xF9,			/* BNE     go_errmess_5 */
/* EE6C */					
/* EE6C */	0x20,	0x54,	0xE2,		/* See6c:  JSR     Se254 */
/* EE6F */	0xA5,	0xDA,			/* LDA     aux */
/* EE71 */	0xD0,	0x07,			/* BNE     Lee7a */
/* EE73 */	0xA5,	0xDB,			/* LDA     aux+1 */
/* EE75 */	0xD0,	0x03,			/* BNE     Lee7a */
/* EE77 */	0x4C,	0x7E,	0xE7,		/* JMP     Le77e */
/* EE7A */	0x06,	0xCE,			/* Lee7a:  ASL     acc */
/* EE7C */	0x26,	0xCF,			/* ROL     acc+1 */
/* EE7E */	0x26,	0xE6,			/* ROL     p3 */
/* EE80 */	0x26,	0xE7,			/* ROL     p3+1 */
/* EE82 */	0xA5,	0xE6,			/* LDA     p3 */
/* EE84 */	0xC5,	0xDA,			/* CMP     aux */
/* EE86 */	0xA5,	0xE7,			/* LDA     p3+1 */
/* EE88 */	0xE5,	0xDB,			/* SBC     aux+1 */
/* EE8A */	0x90,	0x0A,			/* BCC     Lee96 */
/* EE8C */	0x85,	0xE7,			/* STA     p3+1 */
/* EE8E */	0xA5,	0xE6,			/* LDA     p3 */
/* EE90 */	0xE5,	0xDA,			/* SBC     aux */
/* EE92 */	0x85,	0xE6,			/* STA     p3 */
/* EE94 */	0xE6,	0xCE,			/* INC     acc */
/* EE96 */	0x88,				/* Lee96:  DEY */
/* EE97 */	0xD0,	0xE1,			/* BNE     Lee7a */
/* EE99 */	0x60,				/* RTS */
/* EE9A */					
/* EE9A */	0xFF,	0xFF,	0xFF,	0xFF,	/* .byte   $FF,$FF,$FF,$FF,$FF,$FF */
/* EE9E */	0xFF,	0xFF,			
/* EEA0 */					
/* EEA0 */					/* ; token $4D - "CALL" statement */
/* EEA0 */	0x20,	0x15,	0xE7,		/* call_stmt:      JSR     get16bit */
/* EEA3 */	0x6C,	0xCE,	0x00,		/* JMP     (acc) */
/* EEA6 */	0xA5,	0x4C,			/* l1233:  LDA     himem */
/* EEA8 */	0xD0,	0x02,			/* BNE     l1235 */
/* EEAA */	0xC6,	0x4D,			/* DEC     himem+1 */
/* EEAC */	0xC6,	0x4C,			/* l1235:  DEC     himem */
/* EEAE */	0xA5,	0x48,			/* LDA     var */
/* EEB0 */	0xD0,	0x02,			/* BNE     l1236 */
/* EEB2 */	0xC6,	0x49,			/* DEC     var+1 */
/* EEB4 */	0xC6,	0x48,			/* l1236:  DEC     var */
/* EEB6 */	0xA0,	0x00,			/* l1237:  LDY     #$00 */
/* EEB8 */	0xB1,	0x4C,			/* LDA     (himem),Y */
/* EEBA */	0x91,	0x48,			/* STA     (var),Y */
/* EEBC */	0xA5,	0xCA,			/* LDA     pp */
/* EEBE */	0xC5,	0x4C,			/* CMP     himem */
/* EEC0 */	0xA5,	0xCB,			/* LDA     pp+1 */
/* EEC2 */	0xE5,	0x4D,			/* SBC     himem+1 */
/* EEC4 */	0x90,	0xE0,			/* BCC     l1233 */
/* EEC6 */	0x4C,	0x53,	0xEE,		/* JMP     Tee5e */
/* EEC9 */	0xC9,	0x28,			/* CMP     #$28 */
/* EECB */	0xB0,	0x9B,			/* Leecb:  BCS     range_err */
/* EECD */	0xA8,				/* TAY */
/* EECE */	0xA5,	0xC8,			/* LDA     text_index */
/* EED0 */	0x60,				/* RTS */
/* EED1 */	0xEA,				/* NOP */
/* EED2 */	0xEA,				/* NOP */
/* EED3 */					
/* EED3 */					/* print_err_msg1: */
/* EED3 */	0x98,				/* TYA */
/* EED4 */	0xAA,				/* TAX */
/* EED5 */	0xA0,	0x6E,			/* LDY     #$6E */
/* EED7 */	0x20,	0xC4,	0xE3,		/* JSR     Se3c4 */
/* EEDA */	0x8A,				/* TXA */
/* EEDB */	0xA8,				/* TAY */
/* EEDC */	0x20,	0xC4,	0xE3,		/* JSR     Se3c4 */
/* EEDF */	0xA0,	0x72,			/* LDY     #$72 */
/* EEE1 */	0x4C,	0xC4,	0xE3,		/* JMP     Se3c4 */
/* EEE4 */					
/* EEE4 */	0x20,	0x15,	0xE7,		/* Seee4:  JSR     get16bit */
/* EEE7 */	0x06,	0xCE,			/* Leee7:  ASL     acc */
/* EEE9 */	0x26,	0xCF,			/* ROL     acc+1 */
/* EEEB */	0x30,	0xFA,			/* BMI     Leee7 */
/* EEED */	0xB0,	0xDC,			/* BCS     Leecb */
/* EEEF */	0xD0,	0x04,			/* BNE     Leef5 */
/* EEF1 */	0xC5,	0xCE,			/* CMP     acc */
/* EEF3 */	0xB0,	0xD6,			/* BCS     Leecb */
/* EEF5 */	0x60,				/* Leef5:  RTS */
/* EEF6 */					
/* EEF6 */					/* ; token $2E - "PEEK" fn (uses $3F left paren) */
/* EEF6 */	0x20,	0x15,	0xE7,		/* peek_fn:        JSR     get16bit */
/* EEF9 */	0xB1,	0xCE,			/* LDA     (acc),Y */
/* EEFB */	0x94,	0x9F,			/* STY     syn_stk_l+31,X */
/* EEFD */	0x4C,	0x08,	0xE7,		/* JMP     push_ya_noun_stk */
/* EF00 */					
/* EF00 */					/* ; token $65 - "," for POKE statement */
/* EF00 */	0x20,	0x34,	0xEE,		/* poke_stmt:      JSR     getbyte */
/* EF03 */	0xA5,	0xCE,			/* LDA     acc */
/* EF05 */	0x48,				/* PHA */
/* EF06 */	0x20,	0x15,	0xE7,		/* JSR     get16bit */
/* EF09 */	0x68,				/* PLA */
/* EF0A */	0x91,	0xCE,			/* STA     (acc),Y */
/* EF0C */					
/* EF0C */	0x60,				/* Tef0c:  RTS */
/* EF0D */					
/* EF0D */	0xFF,	0xFF,	0xFF,		/* .byte   $FF,$FF,$FF */
/* EF10 */					
/* EF10 */					/* ; token $15 - "/" for numeric division */
/* EF10 */	0x20,	0x6C,	0xEE,		/* divide: JSR     See6c */
/* EF13 */	0xA5,	0xCE,			/* LDA     acc */
/* EF15 */	0x85,	0xE6,			/* STA     p3 */
/* EF17 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* EF19 */	0x85,	0xE7,			/* STA     p3+1 */
/* EF1B */	0x4C,	0x44,	0xE2,		/* JMP     Le244 */
/* EF1E */					
/* EF1E */					/* ; token $44 - "," next var in DIM statement is numeric */
/* EF1E */					/* ; token $4F - "DIM", next var is numeric */
/* EF1E */	0x20,	0xE4,	0xEE,		/* dim_num:        JSR     Seee4 */
/* EF21 */	0x4C,	0x34,	0xE1,		/* JMP     Le134 */
/* EF24 */					
/* EF24 */					/* ; token $2D - "(" for numeric array subscript */
/* EF24 */	0x20,	0xE4,	0xEE,		/* num_array_subs: JSR     Seee4 */
/* EF27 */	0xB4,	0x78,			/* LDY     noun_stk_h_str,X */
/* EF29 */	0xB5,	0x50,			/* LDA     noun_stk_l,X */
/* EF2B */	0x69,	0xFE,			/* ADC     #$FE */
/* EF2D */	0xB0,	0x01,			/* BCS     Lef30 */
/* EF2F */	0x88,				/* DEY */
/* EF30 */	0x85,	0xDA,			/* Lef30:  STA     aux */
/* EF32 */	0x84,	0xDB,			/* STY     aux+1 */
/* EF34 */	0x18,				/* CLC */
/* EF35 */	0x65,	0xCE,			/* ADC     acc */
/* EF37 */	0x95,	0x50,			/* STA     noun_stk_l,X */
/* EF39 */	0x98,				/* TYA */
/* EF3A */	0x65,	0xCF,			/* ADC     acc+1 */
/* EF3C */	0x95,	0x78,			/* STA     noun_stk_h_str,X */
/* EF3E */	0xA0,	0x00,			/* LDY     #$00 */
/* EF40 */	0xB5,	0x50,			/* LDA     noun_stk_l,X */
/* EF42 */	0xD1,	0xDA,			/* CMP     (aux),Y */
/* EF44 */	0xC8,				/* INY */
/* EF45 */	0xB5,	0x78,			/* LDA     noun_stk_h_str,X */
/* EF47 */	0xF1,	0xDA,			/* SBC     (aux),Y */
/* EF49 */	0xB0,	0x80,			/* BCS     Leecb */
/* EF4B */	0x4C,	0x23,	0xE8,		/* JMP     left_paren */
/* EF4E */					
/* EF4E */					/* ; token $2F - "RND" fn (uses $3F left paren) */
/* EF4E */	0x20,	0x15,	0xE7,		/* rnd_fn: JSR     get16bit */
/* EF51 */	0xA5,	0x4E,			/* LDA     rnd */
/* EF53 */	0x20,	0x08,	0xE7,		/* JSR     push_ya_noun_stk */
/* EF56 */	0xA5,	0x4F,			/* LDA     rnd+1 */
/* EF58 */	0xD0,	0x04,			/* BNE     Lef5e */
/* EF5A */	0xC5,	0x4E,			/* CMP     rnd */
/* EF5C */	0x69,	0x00,			/* ADC     #$00 */
/* EF5E */	0x29,	0x7F,			/* Lef5e:  AND     #$7F */
/* EF60 */	0x85,	0x4F,			/* STA     rnd+1 */
/* EF62 */	0x95,	0xA0,			/* STA     noun_stk_h_int,X */
/* EF64 */	0xA0,	0x11,			/* LDY     #$11 */
/* EF66 */	0xA5,	0x4F,			/* Lef66:  LDA     rnd+1 */
/* EF68 */	0x0A,				/* ASL */
/* EF69 */	0x18,				/* CLC */
/* EF6A */	0x69,	0x40,			/* ADC     #$40 */
/* EF6C */	0x0A,				/* ASL */
/* EF6D */	0x26,	0x4E,			/* ROL     rnd */
/* EF6F */	0x26,	0x4F,			/* ROL     rnd+1 */
/* EF71 */	0x88,				/* DEY */
/* EF72 */	0xD0,	0xF2,			/* BNE     Lef66 */
/* EF74 */	0xA5,	0xCE,			/* LDA     acc */
/* EF76 */	0x20,	0x08,	0xE7,		/* JSR     push_ya_noun_stk */
/* EF79 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* EF7B */	0x95,	0xA0,			/* STA     noun_stk_h_int,X */
/* EF7D */	0x4C,	0x7A,	0xE2,		/* JMP     mod_op */
/* EF80 */					
/* EF80 */	0x20,	0x15,	0xE7,		/* Tef80:  JSR     get16bit */
/* EF83 */	0xA4,	0xCE,			/* LDY     acc */
/* EF85 */	0xC4,	0x4C,			/* CPY     himem */
/* EF87 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* EF89 */	0xE5,	0x4D,			/* SBC     himem+1 */
/* EF8B */	0x90,	0x1F,			/* BCC     Lefab */
/* EF8D */	0x84,	0x48,			/* STY     var */
/* EF8F */	0xA5,	0xCF,			/* LDA     acc+1 */
/* EF91 */	0x85,	0x49,			/* STA     var+1 */
/* EF93 */	0x4C,	0xB6,	0xEE,		/* Lef93:  JMP     l1237 */
/* EF96 */					
/* EF96 */	0x20,	0x15,	0xE7,		/* Tef96:  JSR     get16bit */
/* EF99 */	0xA4,	0xCE,			/* LDY     acc */
/* EF9B */	0xC4,	0xCA,			/* CPY     pp */
/* EF9D */	0xA5,	0xCF,			/* LDA     acc+1 */
/* EF9F */	0xE5,	0xCB,			/* SBC     pp+1 */
/* EFA1 */	0xB0,	0x09,			/* BCS     Lefab */
/* EFA3 */	0x84,	0x4A,			/* STY     lomem */
/* EFA5 */	0xA5,	0xCF,			/* LDA     acc+1 */
/* EFA7 */	0x85,	0x4B,			/* STA     lomem+1 */
/* EFA9 */	0x4C,	0xB7,	0xE5,		/* JMP     clr */
/* EFAC */	0x4C,	0xCB,	0xEE,		/* Lefab:  JMP     Leecb */
/* EFAF */	0xEA,				/* NOP */
/* EFB0 */	0xEA,				/* NOP */
/* EFB1 */	0xEA,				/* NOP */
/* EFB2 */	0xEA,				/* NOP */
/* EFB3 */	0x20,	0xC9,	0xEF,		/* Lefb3:  JSR     Sefc9 */
/* EFB6 */					
/* EFB6 */					/* ; token $26 - "," for string input */
/* EFB6 */					/* ; token $52 - "INPUT" statement for string */
/* EFB6 */	0x20,	0x71,	0xE1,		/* string_input:   JSR     input_str */
/* EFB9 */	0x4C,	0xBF,	0xEF,		/* JMP     Lefbf */
/* EFBC */					
/* EFBC */					/* ; token $53 - "INPUT" with literal string prompt */
/* EFBC */	0x20,	0x03,	0xEE,		/* input_prompt:   JSR     print_str */
/* EFBF */	0xA9,	0xFF,			/* Lefbf:  LDA     #$FF */
/* EFC1 */	0x85,	0xC8,			/* STA     text_index */
/* EFC3 */	0xA9,	0x74,			/* LDA     #$74 */
/* EFC5 */	0x8D,	0x00,	0x02,		/* STA     buffer */
/* EFC8 */	0x60,				/* RTS */
/* EFC9 */					
/* EFC9 */	0x20,	0x36,	0xE7,		/* Sefc9:  JSR     not_op */
/* EFCC */	0xE8,				/* INX */
/* EFCD */					
/* EFCD */	0x20,	0x36,	0xE7,		/* Sefcd:  JSR     not_op */
/* EFD0 */	0xB5,	0x50,			/* LDA     noun_stk_l,X */
/* EFD2 */	0x60,				/* RTS */
/* EFD3 */					
/* EFD3 */					/* ; memory initialization for 4K RAM */
/* EFD3 */	0xA9,	0x00,			/* mem_init_4k:    LDA     #$00 */
/* EFD5 */	0x85,	0x4A,			/* STA     lomem */
/* EFD7 */	0x85,	0x4C,			/* STA     himem */
/* EFD9 */	0xA9,	0x08,			/* LDA     #$08 */
/* EFDB */	0x85,	0x4B,			/* STA     lomem+1         ; LOMEM defaults to $0800 */
/* EFDD */	0xA9,	0x10,			/* LDA     #$10 */
/* EFDF */	0x85,	0x4D,			/* STA     himem+1         ; HIMEM defaults to $1000 */
/* EFE1 */	0x4C,	0xAD,	0xE5,		/* JMP     new_cmd */
/* EFE4 */					
/* EFE4 */	0xD5,	0x78,			/* Sefe4:  CMP     noun_stk_h_str,X */
/* EFE6 */	0xD0,	0x01,			/* BNE     Lefe9 */
/* EFE8 */	0x18,				/* CLC */
/* EFE9 */	0x4C,	0x02,	0xE1,		/* Lefe9:  JMP     Le102 */
/* EFEC */					
/* EFEC */	0x20,	0xB7,	0xE5,		/* Tefec:  JSR     clr */
/* EFEF */	0x4C,	0x36,	0xE8,		/* JMP     run_warm */
/* EFF2 */					
/* EFF2 */	0x20,	0xB7,	0xE5,		/* Teff2:  JSR     clr */
/* EFF5 */	0x4C,	0x5B,	0xE8,		/* JMP     goto_stmt */
/* EFF8 */					
/* EFF8 */	0xE0,	0x80,			/* Seff8:  CPX     #$80 */
/* EFFA */	0xD0,	0x01,			/* BNE     Leffd */
/* EFFC */	0x88,				/* DEY */
/* EFFD */	0x4C,	0x0C,	0xE0,		/* Leffd:  JMP     Se00c */
/* EFFD */					
};
#endif





// ============================================================================
// ============================================================================
// Apple-1 ACI CASSETTE INTERFACE ROM
// F100 version
// ============================================================================
// ============================================================================
//
#ifdef _INCL_ACI
  #if _TARGET == PERSONAL_COMPUTER
    unsigned char aciROM[256] = {
  #else
    const unsigned char aciROM[256] PROGMEM = {
  #endif
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
};
#endif





// ============================================================================
// ============================================================================
// BAUM/WOZNIAK MINI-ASSEMBLER & WOZMON
// FBAE version
// ============================================================================
// ============================================================================
//
// http://forum.6502.org/viewtopic.php?f=2&t=5884
// The Apple II mini-assembler with WozMon, reconstructed by Mike Barry
// In honour of Marcel van Kervinck, who led the way with the Gigatron Apple-1 mode
//
#ifdef _INCL_MINIASM
  #if _TARGET == PERSONAL_COMPUTER
    unsigned char asmROM[1106] = {
  #else
    const unsigned char asmROM[1106] PROGMEM = {
  #endif
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
/* 00fff8 */ 0xff, 0xff, 0x00, 0x0f  /* .byte $FF ,$FF ,$00 ,$0F ,$00 ,$FF ,$00 ,$00 */
/* 00fffc */ /*0x00, 0xff, 0x00, 0x00*/  /*  */
/* 010000 */                          /*  */
/* 010000 */                          /*  */
/* 010000 */                          /*  */
};
#endif





// ============================================================================
// ============================================================================
// Apple-1 ACI CASSETTE INTERFACE ROM
// F100 version
// ============================================================================
// ============================================================================
//




#ifdef _INCL_LIFE
  #if _TARGET == PERSONAL_COMPUTER
    unsigned char lifeROM[440] = {
  #else
    const unsigned char lifeROM[440] PROGMEM = {
  #endif
0xA2, 	0x00, 	0xA9, 	0xA0, 	0x9D, 	0x00, 	0x24, 	0x9D, 	0x00, 	0x25, 	0x9D, 	0x00, 	0x26, 	0x9D, 	0x00, 	0x27, 	0xE8, 	0xD0, 	0xF1, 	0xBD, 	0x80, 	0x21, 	0x20, 	0xEF, 	0xFF, 	0xE8, 	0xE0, 	0x38, 	0xD0, 	0xF5, 	0x20, 	0x02, 	
0x21, 	0xC9, 	0x8D, 	0xD0, 	0xF9, 	0x20, 	0x3F, 	0x21, 	0x29, 	0x3F, 	0x69, 	0x20, 	0x85, 	0xF9, 	0xA0, 	0x00, 	0x20, 	0x3F, 	0x21, 	0x29, 	0x03, 	0x18, 	0x69, 	0x24, 	0x85, 	0xF1, 	0xA5, 	0xFA, 	0x85, 	0xF0, 	0xA9, 	0xAA, 	
0x91, 	0xF0, 	0xC6, 	0xF9, 	0xD0, 	0xEA, 	0xA9, 	0x24, 	0x85, 	0xF9, 	0xA9, 	0x40, 	0x85, 	0xF8, 	0xA2, 	0x27, 	0xA9, 	0xAD, 	0x9D, 	0xD8, 	0x27, 	0x9D, 	0x40, 	0x24, 	0xCA, 	0x10, 	0xF7, 	0xB1, 	0xF8, 	0x20, 	0xEF, 	0xFF, 	
0xE6, 	0xF8, 	0xD0, 	0xF7, 	0xE6, 	0xF9, 	0xA5, 	0xF9, 	0xC9, 	0x28, 	0xD0, 	0xEF, 	0x20, 	0x02, 	0x21, 	0xA0, 	0x00, 	0x20, 	0xF3, 	0x20, 	0xB1, 	0xF0, 	0x91, 	0xF2, 	0xE6, 	0xF0, 	0xE6, 	0xF2, 	0xD0, 	0xF6, 	0xE6, 	0xF1, 	
0xE6, 	0xF3, 	0xA5, 	0xF1, 	0xC9, 	0x28, 	0xD0, 	0xEC, 	0x20, 	0x61, 	0x21, 	0xA0, 	0x00, 	0xB1, 	0xF2, 	0xC9, 	0xAA, 	0xD0, 	0x0A, 	0xA0, 	0x27, 	0x20, 	0x14, 	0x21, 	0xA0, 	0x01, 	0x20, 	0x1D, 	0x21, 	0xE6, 	0xF0, 	0xE6, 	
0xF2, 	0xD0, 	0xE8, 	0xE6, 	0xF1, 	0xE6, 	0xF3, 	0xA5, 	0xF1, 	0xC9, 	0x28, 	0xD0, 	0xDE, 	0x18, 	0xA2, 	0x27, 	0xBD, 	0xD8, 	0x27, 	0x7D, 	0x68, 	0x24, 	0x9D, 	0x68, 	0x24, 	0xBD, 	0x40, 	0x24, 	0x7D, 	0xB0, 	0x27, 	0x9D, 	
0xB0, 	0x27, 	0xCA, 	0x10, 	0xEB, 	0x20, 	0xF3, 	0x20, 	0xA0, 	0x00, 	0xB1, 	0xF0, 	0xC9, 	0x02, 	0xD0, 	0x04, 	0xB1, 	0xF2, 	0xD0, 	0x0A, 	0xC9, 	0x03, 	0xD0, 	0x04, 	0xA9, 	0xAA, 	0xD0, 	0x02, 	0xA9, 	0xA0, 	0x91, 	0xF0, 	
0xE6, 	0xF0, 	0xE6, 	0xF2, 	0xD0, 	0xE2, 	0xE6, 	0xF1, 	0xE6, 	0xF3, 	0xA5, 	0xF3, 	0xC9, 	0x2C, 	0xD0, 	0xD8, 	0x4C, 	0x46, 	0x20, 	0xA9, 	0x24, 	0x85, 	0xF1, 	0xA9, 	0x28, 	0x85, 	0xF3, 	0xA9, 	0x40, 	0x85, 	0xF0, 	0x85, 	
0xF2, 	0x60, 	0xE6, 	0xFA, 	0xD0, 	0x02, 	0xE6, 	0xFB, 	0xAD, 	0x11, 	0xD0, 	0x10, 	0xF5, 	0xAD, 	0x10, 	0xD0, 	0x20, 	0xEF, 	0xFF, 	0x60, 	0x20, 	0x1D, 	0x21, 	0xC8, 	0xC0, 	0x2A, 	0xD0, 	0xF8, 	0x60, 	0x18, 	0xB1, 	0xF0, 	
0x69, 	0x01, 	0x91, 	0xF0, 	0x84, 	0xF9, 	0x38, 	0xA5, 	0xF0, 	0xE5, 	0xF9, 	0x85, 	0xF6, 	0xA5, 	0xF1, 	0xE9, 	0x00, 	0x85, 	0xF7, 	0xA0, 	0x00, 	0xB1, 	0xF6, 	0x18, 	0x69, 	0x01, 	0x91, 	0xF6, 	0xA4, 	0xF9, 	0x60, 	0xA5, 	
0xFB, 	0x85, 	0xF6, 	0xA5, 	0xFA, 	0x0A, 	0x26, 	0xF6, 	0x0A, 	0x26, 	0xF6, 	0x18, 	0x65, 	0xFA, 	0x48, 	0xA5, 	0xF6, 	0x65, 	0xFB, 	0x85, 	0xFB, 	0x68, 	0x69, 	0x11, 	0x85, 	0xFA, 	0xA5, 	0xFB, 	0x69, 	0x36, 	0x85, 	0xFB, 	
0x60, 	0x20, 	0xF3, 	0x20, 	0xA9, 	0x00, 	0xA8, 	0x91, 	0xF0, 	0xE6, 	0xF0, 	0xD0, 	0xFA, 	0xE6, 	0xF1, 	0xA6, 	0xF1, 	0xE0, 	0x28, 	0xD0, 	0xF2, 	0x20, 	0xF3, 	0x20, 	0x60, 	0xA0, 	0xA0, 	0xA0, 	0xA0, 	0xA0, 	0xA0, 	0xA0, 	
0x8D, 	0x8D, 	0x8D, 	0x8D, 	0xC3, 	0xCF, 	0xCE, 	0xD7, 	0xC1, 	0xD9, 	0xA7, 	0xD3, 	0xA0, 	0xC7, 	0xC1, 	0xCD, 	0xC5, 	0xA0, 	0xCF, 	0xC6, 	0xA0, 	0xCC, 	0xC9, 	0xC6, 	0xC5, 	0x8D, 	0x8D, 	0xD0, 	0xCC, 	0xC5, 	0xC1, 	0xD3, 	
0xC5, 	0xA0, 	0xD4, 	0xD9, 	0xD0, 	0xC5, 	0xA0, 	0xD9, 	0xCF, 	0xD5, 	0xD2, 	0xA0, 	0xC6, 	0xD5, 	0xCC, 	0xCC, 	0xA0, 	0xCE, 	0xC1, 	0xCD, 	0xC5, 	0xAE, 	0x8D, 	0x8D
}; 									
#endif
