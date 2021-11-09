display     .equ $d012
cls         .equ $d014
load        .org $0200

start        sta cls
             ldx #$00
print_char   lda hello,X
             cmp #$00
             beq exit
             sta display
             inx
             jmp print_char
exit         jmp exit

hello       .byte "  Hello, World!"
            .byte $00

