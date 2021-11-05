/**********************************************************************************
 * dcc6502.c -> Main module of:                                                   *
 * Disassembler and Cycle Counter for the 6502 microprocessor                     *
 *                                                                                *
 * This code is offered under the MIT License (MIT)                               *
 *                                                                                *
 * Copyright (c) 1998-2014 Tennessee Carmel-Veilleux <veilleux@tentech.ca>        *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 **********************************************************************************/

// 20200608 hacked version of dcc6502.c by OV. 
// For any other purpose than my own, please use the original dcc6502!
// https://github.com/tcarmelveilleux/dcc6502

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

#define VERSION_INFO "v2.1"
#define NUMBER_OPCODES 151

/* Exceptions for cycle counting */
#define CYCLES_CROSS_PAGE_ADDS_ONE      (1 << 0)
#define CYCLES_BRANCH_TAKEN_ADDS_ONE    (1 << 1)

/* The 6502's 13 addressing modes */
typedef enum {
    IMMED = 0, /* Immediate */
    ABSOL, /* Absolute */
    ZEROP, /* Zero Page */
    IMPLI, /* Implied */
    INDIA, /* Indirect Absolute */
    ABSIX, /* Absolute indexed with X */
    ABSIY, /* Absolute indexed with Y */
    ZEPIX, /* Zero page indexed with X */
    ZEPIY, /* Zero page indexed with Y */
    INDIN, /* Indexed indirect (with X) */
    ININD, /* Indirect indexed (with Y) */
    RELAT, /* Relative */
    ACCUM /* Accumulator */
} addressing_mode_e;

/** Some compilers don't have EOK in errno.h */
#ifndef EOK
#define EOK 0
#endif

typedef struct opcode_s {
    uint8_t number; /* Number of the opcode */
    const char *mnemonic; /* Index in the name table */
    addressing_mode_e addressing; /* Addressing mode */
    unsigned int cycles; /* Number of cycles */
    unsigned int cycles_exceptions; /* Mask of cycle-counting exceptions */
} opcode_t;

/* Opcode table */
static opcode_t g_opcode_table[NUMBER_OPCODES] = {
    {0x69, "ADC", IMMED, 2, 0}, /* ADC */
    {0x65, "ADC", ZEROP, 3, 0},
    {0x75, "ADC", ZEPIX, 4, 0},
    {0x6D, "ADC", ABSOL, 4, 0},
    {0x7D, "ADC", ABSIX, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0x79, "ADC", ABSIY, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0x61, "ADC", INDIN, 6, 0},
    {0x71, "ADC", ININD, 5, CYCLES_CROSS_PAGE_ADDS_ONE},

    {0x29, "AND", IMMED, 2, 0}, /* AND */
    {0x25, "AND", ZEROP, 3, 0},
    {0x35, "AND", ZEPIX, 4, 0},
    {0x2D, "AND", ABSOL, 4, 0},
    {0x3D, "AND", ABSIX, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0x39, "AND", ABSIY, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0x21, "AND", INDIN, 6, 0},
    {0x31, "AND", ININD, 5, CYCLES_CROSS_PAGE_ADDS_ONE},

    {0x0A, "ASL", ACCUM, 2, 0}, /* ASL */
    {0x06, "ASL", ZEROP, 5, 0},
    {0x16, "ASL", ZEPIX, 6, 0},
    {0x0E, "ASL", ABSOL, 6, 0},
    {0x1E, "ASL", ABSIX, 7, 0},

    {0x90, "BCC", RELAT, 2, CYCLES_CROSS_PAGE_ADDS_ONE | CYCLES_BRANCH_TAKEN_ADDS_ONE}, /* BCC */

    {0xB0, "BCS", RELAT, 2, CYCLES_CROSS_PAGE_ADDS_ONE | CYCLES_BRANCH_TAKEN_ADDS_ONE}, /* BCS */

    {0xF0, "BEQ", RELAT, 2, CYCLES_CROSS_PAGE_ADDS_ONE | CYCLES_BRANCH_TAKEN_ADDS_ONE}, /* BEQ */

    {0x24, "BIT", ZEROP, 3, 0}, /* BIT */
    {0x2C, "BIT", ABSOL, 4, 0},

    {0x30, "BMI", RELAT, 2, CYCLES_CROSS_PAGE_ADDS_ONE | CYCLES_BRANCH_TAKEN_ADDS_ONE}, /* BMI */

    {0xD0, "BNE", RELAT, 2, CYCLES_CROSS_PAGE_ADDS_ONE | CYCLES_BRANCH_TAKEN_ADDS_ONE}, /* BNE */

    {0x10, "BPL", RELAT, 2, CYCLES_CROSS_PAGE_ADDS_ONE | CYCLES_BRANCH_TAKEN_ADDS_ONE}, /* BPL */

    {0x00, "BRK", IMPLI, 7, 0}, /* BRK */

    {0x50, "BVC", RELAT, 2, CYCLES_CROSS_PAGE_ADDS_ONE | CYCLES_BRANCH_TAKEN_ADDS_ONE}, /* BVC */

    {0x70, "BVS", RELAT, 2, CYCLES_CROSS_PAGE_ADDS_ONE | CYCLES_BRANCH_TAKEN_ADDS_ONE}, /* BVS */

    {0x18, "CLC", IMPLI, 2, 0}, /* CLC */

    {0xD8, "CLD", IMPLI, 2, 0}, /* CLD */

    {0x58, "CLI", IMPLI, 2, 0}, /* CLI */

    {0xB8, "CLV", IMPLI, 2, 0}, /* CLV */

    {0xC9, "CMP", IMMED, 2, 0}, /* CMP */
    {0xC5, "CMP", ZEROP, 3, 0},
    {0xD5, "CMP", ZEPIX, 4, 0},
    {0xCD, "CMP", ABSOL, 4, 0},
    {0xDD, "CMP", ABSIX, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0xD9, "CMP", ABSIY, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0xC1, "CMP", INDIN, 6, 0},
    {0xD1, "CMP", ININD, 5, CYCLES_CROSS_PAGE_ADDS_ONE},

    {0xE0, "CPX", IMMED, 2, 0}, /* CPX */
    {0xE4, "CPX", ZEROP, 3, 0},
    {0xEC, "CPX", ABSOL, 4, 0},

    {0xC0, "CPY", IMMED, 2, 0}, /* CPY */
    {0xC4, "CPY", ZEROP, 3, 0},
    {0xCC, "CPY", ABSOL, 4, 0},

    {0xC6, "DEC", ZEROP, 5, 0}, /* DEC */
    {0xD6, "DEC", ZEPIX, 6, 0},
    {0xCE, "DEC", ABSOL, 6, 0},
    {0xDE, "DEC", ABSIX, 7, 0},

    {0xCA, "DEX", IMPLI, 2, 0}, /* DEX */

    {0x88, "DEY", IMPLI, 2, 0}, /* DEY */

    {0x49, "EOR", IMMED, 2, 0}, /* EOR */
    {0x45, "EOR", ZEROP, 3, 0},
    {0x55, "EOR", ZEPIX, 4, 0},
    {0x4D, "EOR", ABSOL, 4, 0},
    {0x5D, "EOR", ABSIX, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0x59, "EOR", ABSIY, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0x41, "EOR", INDIN, 6, 1},
    {0x51, "EOR", ININD, 5, CYCLES_CROSS_PAGE_ADDS_ONE},

    {0xE6, "INC", ZEROP, 5, 0}, /* INC */
    {0xF6, "INC", ZEPIX, 6, 0},
    {0xEE, "INC", ABSOL, 6, 0},
    {0xFE, "INC", ABSIX, 7, 0},

    {0xE8, "INX", IMPLI, 2, 0}, /* INX */

    {0xC8, "INY", IMPLI, 2, 0}, /* INY */

    {0x4C, "JMP", ABSOL, 3, 0}, /* JMP */
    {0x6C, "JMP", INDIA, 5, 0},

    {0x20, "JSR", ABSOL, 6, 0}, /* JSR */

    {0xA9, "LDA", IMMED, 2, 0}, /* LDA */
    {0xA5, "LDA", ZEROP, 3, 0},
    {0xB5, "LDA", ZEPIX, 4, 0},
    {0xAD, "LDA", ABSOL, 4, 0},
    {0xBD, "LDA", ABSIX, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0xB9, "LDA", ABSIY, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0xA1, "LDA", INDIN, 6, 0},
    {0xB1, "LDA", ININD, 5, CYCLES_CROSS_PAGE_ADDS_ONE},

    {0xA2, "LDX", IMMED, 2, 0}, /* LDX */
    {0xA6, "LDX", ZEROP, 3, 0},
    {0xB6, "LDX", ZEPIY, 4, 0},
    {0xAE, "LDX", ABSOL, 4, 0},
    {0xBE, "LDX", ABSIY, 4, CYCLES_CROSS_PAGE_ADDS_ONE},

    {0xA0, "LDY", IMMED, 2, 0}, /* LDY */
    {0xA4, "LDY", ZEROP, 3, 0},
    {0xB4, "LDY", ZEPIX, 4, 0},
    {0xAC, "LDY", ABSOL, 4, 0},
    {0xBC, "LDY", ABSIX, 4, CYCLES_CROSS_PAGE_ADDS_ONE},

    {0x4A, "LSR", ACCUM, 2, 0}, /* LSR */
    {0x46, "LSR", ZEROP, 5, 0},
    {0x56, "LSR", ZEPIX, 6, 0},
    {0x4E, "LSR", ABSOL, 6, 0},
    {0x5E, "LSR", ABSIX, 7, 0},

    {0xEA, "NOP", IMPLI, 2, 0}, /* NOP */

    {0x09, "ORA", IMMED, 2, 0}, /* ORA */
    {0x05, "ORA", ZEROP, 3, 0},
    {0x15, "ORA", ZEPIX, 4, 0},
    {0x0D, "ORA", ABSOL, 4, 0},
    {0x1D, "ORA", ABSIX, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0x19, "ORA", ABSIY, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0x01, "ORA", INDIN, 6, 0},
    {0x11, "ORA", ININD, 5, CYCLES_CROSS_PAGE_ADDS_ONE},

    {0x48, "PHA", IMPLI, 3, 0}, /* PHA */

    {0x08, "PHP", IMPLI, 3, 0}, /* PHP */

    {0x68, "PLA", IMPLI, 4, 0}, /* PLA */

    {0x28, "PLP", IMPLI, 4, 0}, /* PLP */

    {0x2A, "ROL", ACCUM, 2, 0}, /* ROL */
    {0x26, "ROL", ZEROP, 5, 0},
    {0x36, "ROL", ZEPIX, 6, 0},
    {0x2E, "ROL", ABSOL, 6, 0},
    {0x3E, "ROL", ABSIX, 7, 0},

    {0x6A, "ROR", ACCUM, 2, 0}, /* ROR */
    {0x66, "ROR", ZEROP, 5, 0},
    {0x76, "ROR", ZEPIX, 6, 0},
    {0x6E, "ROR", ABSOL, 6, 0},
    {0x7E, "ROR", ABSIX, 7, 0},

    {0x40, "RTI", IMPLI, 6, 0}, /* RTI */

    {0x60, "RTS", IMPLI, 6, 0}, /* RTS */

    {0xE9, "SBC", IMMED, 2, 0}, /* SBC */
    {0xE5, "SBC", ZEROP, 3, 0},
    {0xF5, "SBC", ZEPIX, 4, 0},
    {0xED, "SBC", ABSOL, 4, 0},
    {0xFD, "SBC", ABSIX, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0xF9, "SBC", ABSIY, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0xE1, "SBC", INDIN, 6, 0},
    {0xF1, "SBC", ININD, 5, CYCLES_CROSS_PAGE_ADDS_ONE},

    {0x38, "SEC", IMPLI, 2, 0}, /* SEC */

    {0xF8, "SED", IMPLI, 2, 0}, /* SED */

    {0x78, "SEI", IMPLI, 2, 0}, /* SEI */

    {0x85, "STA", ZEROP, 3, 0}, /* STA */
    {0x95, "STA", ZEPIX, 4, 0},
    {0x8D, "STA", ABSOL, 4, 0},
    {0x9D, "STA", ABSIX, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0x99, "STA", ABSIY, 4, CYCLES_CROSS_PAGE_ADDS_ONE},
    {0x81, "STA", INDIN, 6, 0},
    {0x91, "STA", ININD, 5, CYCLES_CROSS_PAGE_ADDS_ONE},

    {0x86, "STX", ZEROP, 3, 0}, /* STX */
    {0x96, "STX", ZEPIY, 4, 0},
    {0x8E, "STX", ABSOL, 4, 0},

    {0x84, "STY", ZEROP, 3, 0}, /* STY */
    {0x94, "STY", ZEPIX, 4, 0},
    {0x8C, "STY", ABSOL, 4, 0},

    {0xAA, "TAX", IMPLI, 2, 0}, /* TAX */

    {0xA8, "TAY", IMPLI, 2, 0}, /* TAY */

    {0xBA, "TSX", IMPLI, 2, 0}, /* TSX */

    {0x8A, "TXA", IMPLI, 2, 0}, /* TXA */

    {0x9A, "TXS", IMPLI, 2, 0}, /* TXS */

    {0x98, "TYA", IMPLI, 2, 0} /* TYA */
};


/* This function appends cycle counting to the comment block. See following
 * for methods used:
 * "Nick Bensema's Guide to Cycle Counting on the Atari 2600"
 * http://www.alienbill.com/2600/cookbook/cycles/nickb.txt
 */
static char *append_cycle(char *input, uint8_t entry, uint16_t pc, uint16_t new_pc) {
    char tmpstr[256];
    int cycles = g_opcode_table[entry].cycles;
    int exceptions = g_opcode_table[entry].cycles_exceptions;
    int crosses_page = ((pc & 0xff00u) != (new_pc & 0xff00u)) ? 1 : 0;

    // On some exceptional conditions, instruction will take an extra cycle, or even two
    if (exceptions != 0) {
        if ((exceptions & CYCLES_BRANCH_TAKEN_ADDS_ONE) && (exceptions & CYCLES_CROSS_PAGE_ADDS_ONE)) {
            /* Branch case: check for page crossing, since it can be determined
             * statically from the relative offset and current PC.
             */
            if (crosses_page) {
                /* Crosses page, always at least 1 extra cycle, two times */
                sprintf(tmpstr, " Cycles: %d/%d", cycles + 1, cycles + 2);
            } else {
                /* Does not cross page, maybe one extra cycle if branch taken */
                sprintf(tmpstr, " Cycles: %d/%d", cycles, cycles + 1);
            }
        } else {
            /* One exception: two times, can't tell in advance whether page crossing occurs */
            sprintf(tmpstr, " Cycles: %d/%d", cycles, cycles + 1);
        }
    } else {
        /* No exceptions, no extra time */
        sprintf(tmpstr, " Cycles: %d", cycles);
    }

    strcat(input, tmpstr);
    return (input + strlen(input));
}

/* Helper macros for disassemble() function */
#define DUMP_FORMAT ("%-16s%-16s;")
#define HIGH_PART(val) (((val) >> 8) & 0xFFu)
#define LOW_PART(val) ((val) & 0xFFu)
#define LOAD_WORD(buffer, current_pc) ((uint16_t)buffer[(current_pc) + 1] | (((uint16_t)buffer[(current_pc) + 2]) << 8))

/* This function disassembles the opcode at the PC and outputs it in *output */
void disassemble(char *output, uint8_t *buffer, uint16_t *pc) {
    char opcode_repr[256], hex_dump[256];
    int opcode_idx;
    int len = 0;
    int entry = 0;
    int found = 0;
    uint8_t byte_operand;
    uint16_t word_operand = 0;
    uint16_t current_addr;
    uint8_t opcode;
    const char *mnemonic;

    current_addr = *pc;
    *pc = 0;
    opcode_repr[0] = '\0';
    hex_dump[0] = '\0';
    opcode = buffer[0];
    // Linear search for opcode
    for (opcode_idx = 0; opcode_idx < NUMBER_OPCODES; opcode_idx++) {
        if (opcode == g_opcode_table[opcode_idx].number) {
            /* Found the opcode, record its table index */
            found = 1;
            entry = opcode_idx;
        }
    }

    // For opcode not found, terminate early
    if (!found) {
        sprintf(opcode_repr, ".byte $%02X", opcode);
            sprintf(hex_dump, "$%04X> %02X:", current_addr, opcode);
            sprintf(output, "%-16s%-16s; INVALID OPCODE !!!\n", hex_dump, opcode_repr);
        return;
    }

    // Opcode found in table: disassemble properly according to addressing mode
    mnemonic = g_opcode_table[entry].mnemonic;

    // Set hex dump to default single address format. Will be overwritten
    // by more complex output in case of hex dump mode enabled
    sprintf(hex_dump, "$%04X", current_addr);

    switch (g_opcode_table[entry].addressing) {
        case IMMED:
            /* Get immediate value operand */
            byte_operand = buffer[*pc + 1];
            *pc += 1;
            sprintf(opcode_repr, "%s #$%02X", mnemonic, byte_operand);
                sprintf(hex_dump, "$%04X> %02X %02X:", current_addr, opcode, byte_operand);

            break;
        case ABSOL:
            /* Get absolute address operand */
            word_operand = LOAD_WORD(buffer, *pc);
            *pc += 2;

            sprintf(opcode_repr, "%s $%02X%02X", mnemonic, HIGH_PART(word_operand), LOW_PART(word_operand));
                sprintf(hex_dump, "$%04X> %02X %02X%02X:", current_addr, opcode, LOW_PART(word_operand), HIGH_PART(word_operand));

            break;
        case ZEROP:
            /* Get zero page address */
            byte_operand = buffer[*pc + 1];
            *pc += 1;

            sprintf(opcode_repr, "%s $%02X", mnemonic, byte_operand);
                sprintf(hex_dump, "$%04X> %02X %02X:", current_addr, opcode, byte_operand);

            break;
        case IMPLI:
            sprintf(opcode_repr, "%s", mnemonic);
                sprintf(hex_dump, "$%04X> %02X:", current_addr, opcode);

            break;
        case INDIA:
            /* Get indirection address */
            word_operand = LOAD_WORD(buffer, *pc);
            *pc += 2;

            sprintf(opcode_repr, "%s ($%02X%02X)", mnemonic, HIGH_PART(word_operand), LOW_PART(word_operand));
                sprintf(hex_dump, "$%04X> %02X %02X%02X:", current_addr, opcode, LOW_PART(word_operand), HIGH_PART(word_operand));

            break;
        case ABSIX:
            /* Get base address */
            word_operand = LOAD_WORD(buffer, *pc);
            *pc += 2;

            sprintf(opcode_repr, "%s $%02X%02X,X", mnemonic, HIGH_PART(word_operand), LOW_PART(word_operand));
                sprintf(hex_dump, "$%04X> %02X %02X%02X:", current_addr, opcode, LOW_PART(word_operand), HIGH_PART(word_operand));

            break;
        case ABSIY:
            /* Get baser address */
            word_operand = LOAD_WORD(buffer, *pc);
            *pc += 2;

            sprintf(opcode_repr, "%s $%02X%02X,Y", mnemonic, HIGH_PART(word_operand), LOW_PART(word_operand));
                sprintf(hex_dump, "$%04X> %02X %02X%02X:", current_addr, opcode, LOW_PART(word_operand), HIGH_PART(word_operand));

            break;
        case ZEPIX:
            /* Get zero-page base address */
            byte_operand = buffer[*pc + 1];
            *pc += 1;

            sprintf(opcode_repr, "%s $%02X,X", mnemonic, byte_operand);
                sprintf(hex_dump, "$%04X> %02X %02X:", current_addr, opcode, byte_operand);

            break;
        case ZEPIY:
            /* Get zero-page base address */
            byte_operand = buffer[*pc + 1];
            *pc += 1;

            sprintf(opcode_repr, "%s $%02X,Y", mnemonic, byte_operand);
                sprintf(hex_dump, "$%04X> %02X %02X:", current_addr, opcode, byte_operand);

            break;
        case INDIN:
            /* Get zero-page base address */
            byte_operand = buffer[*pc + 1];
            *pc += 1;

            sprintf(opcode_repr, "%s ($%02X,X)", mnemonic, byte_operand);
                sprintf(hex_dump, "$%04X> %02X %02X:", current_addr, opcode, byte_operand);

            break;
        case ININD:
            /* Get zero-page base address */
            byte_operand = buffer[*pc + 1];
            *pc += 1;

            sprintf(opcode_repr, "%s ($%02X),Y", mnemonic, byte_operand);
                sprintf(hex_dump, "$%04X> %02X %02X:", current_addr, opcode, byte_operand);

            break;
        case RELAT:
            /* Get relative modifier */
            byte_operand = buffer[*pc + 1];
            *pc += 1;

            // Compute displacement from first byte after full instruction.
            word_operand = current_addr + 2;
            if (byte_operand > 0x7Fu) {
                word_operand -= ((~byte_operand & 0x7Fu) + 1);
            } else {
                word_operand += byte_operand & 0x7Fu;
            }

            sprintf(opcode_repr, "%s $%04X", mnemonic, word_operand);
                sprintf(hex_dump, "$%04X> %02X %02X:", current_addr, opcode, byte_operand);

            break;
        case ACCUM:
            sprintf(opcode_repr, "%s A", mnemonic);
                sprintf(hex_dump, "$%04X> %02X:", current_addr, opcode);

            break;
        default:
            // Will not happen since each entry in opcode_table has address mode set
            break;
    }

    // Emit disassembly line content, prior to annotation comments
    len = sprintf(output, DUMP_FORMAT, hex_dump, opcode_repr);
    output += len;

    /* Add cycle count if necessary */
    output = append_cycle(output, entry, *pc + 1, word_operand);
}

int dismain(void) {
    char tmpstr[512];
    uint8_t buffer[4]; /* Memory buffer */
    uint16_t pcx; /* Program counter */

	buffer[0]=0xD0; buffer[1]=0xd1; // copy 1 more byte if you're unsure of instruction length
	pcx = 0x3DC6;
        
	disassemble(tmpstr, buffer, &pcx);
        
	fprintf(stdout, "%s\n", tmpstr);
        pcx++;	// might be useful, points to next instruction

    return 0;
}

