#include <stdio.h>
#include "disassemble.h"

char mnemonics[NINSTRUCTIONS][5] = {
    "add ",
    "sub ",
    "and ",
    "or  ",
    "xor ",
    "sll ",
    "srl ",
    "sra ",
    "addi",
    "andi",
    "ori ",
    "xori",
    "slli",
    "srli",
    "srai",
    "lw  ",
    "sw  "
};

int disassemble(unsigned int minstr) {
    // Extract all fields
    unsigned int opcode = minstr & OPCODEMASK;
    unsigned int rd = (minstr & RDMASK) >> 7;
    unsigned int funct3 = (minstr & FUNCT3MASK) >> 12;
    unsigned int rs1 = (minstr & RS1MASK) >> 15;
    unsigned int rs2 = (minstr & RS2MASK) >> 20;
    unsigned int funct7 = (minstr & FUNCT7MASK) >> 25;

    // Extract immediates

    // Cast minstr to signed int
    int imm_i = (int)minstr >> 20;
    
    // I-format immediate signed, 12 bits
    int imm_upper = (minstr & 0xFE000000) >> 20;
    int imm_lower = (minstr & 0x00000F80) >> 7;
    int imm_s = imm_upper | imm_lower;
    if (imm_s & 0x800) {  // If negative (bit 11 set)
        imm_s = imm_s | 0xFFFFF000;  // Sign extend
    }
    
    // Debug print for Task 2
    printf("Debug: opcode = 0x%x, funct3 = 0x%x, funct7 = 0x%x, rd = x%d, rs1 = x%d, rs2 = x%d\n", opcode, funct3, funct7, rd, rs1, rs2);
    
    // Debug print for Task 3
    printf("imm_i = %d, imm_s = %d\n", imm_i, imm_s);

    switch (opcode) {
        case 0x33:  // R-format
            switch (funct3) {
                case 0x0:  // ADD or SUB
                    if (funct7 == 0x00) {
                        printf("%sx%d, x%d, x%d\n", mnemonics[ADD], rd, rs1, rs2);
                    } else if (funct7 == 0x20) {
                        printf("%sx%d, x%d, x%d\n", mnemonics[SUB], rd, rs1, rs2);
                    }
                    break;
                
                case 0x1:  // SLL
                    printf("%sx%d, x%d, x%d\n", mnemonics[SLL], rd, rs1, rs2);
                    break;
                
                case 0x4:  // XOR
                    printf("%sx%d, x%d, x%d\n", mnemonics[XOR], rd, rs1, rs2);
                    break;
                    
                case 0x5:  // SRL or SRA
                    if (funct7 = 0x00) {
                        printf("%sx%d, x%d, x%d\n", mnemonics[SRL], rd, rs1, rs2);
                    } else if (funct7 = 0x20) {
                        printf("%sx%d, x%d, x%d\n", mnemonics[SRA], rd, rs1, rs2);
                    }
                    break;
                    
                case 0x6:  // OR
                    printf("%sx%d, x%d, x%d\n", mnemonics[OR], rd, rs1, rs2);
                    break;
                    
                case 0x7:  // AND
                    printf("%sx%d, x%d, x%d\n", mnemonics[AND], rd, rs1, rs2);
                    break;
            }
            break;

            case 0x13:  // I-format
                switch (funct3) {
                    case 0x0:  // ADDI
                        printf("%s x%d, x%d, %d\n", mnemonics[ADDI], rd, rs1, imm_i);
                        break;
                    
                    case 0x1:  // SLLI
                        printf("%s x%d, x%d, %d\n", mnemonics[SLLI], rd, rs1, imm_i);
                        break;
                    
                    case 0x4:  // XORI
                        printf("%s x%d, x%d, %d\n", mnemonics[XORI], rd, rs1, imm_i);
                        break;
                        
                    case 0x5:  // SRLI or SRAI
                        if (funct7 == 0x00) {
                            printf("%s x%d, x%d, %d\n", mnemonics[SRLI], rd, rs1, imm_i);
                        } else if (funct7 == 0x20) {
                            printf("%s x%d, x%d, %d\n", mnemonics[SRAI], rd, rs1, imm_i);
                        }
                        break;
                        
                    case 0x6:  // ORI
                        printf("%s x%d, x%d, %d\n", mnemonics[ORI], rd, rs1, imm_i);
                        break;
                        
                    case 0x7:  // ANDI
                        printf("%s x%d, x%d, %d\n", mnemonics[ANDI], rd, rs1, imm_i);
                        break;
                }
                break;

                case 0x03:  // Load instructions
                    if (funct3 == 0x2) {  // lw
                        printf("%sx%d, %d(x%d)\n", mnemonics[LW], rd, imm_i, rs1);
                    }
                break;
            
                case 0x23:  // Store instructions
                    if (funct3 == 0x2) {  // sw
                        printf("%sx%d, %d(x%d)\n", mnemonics[SW], rs2, imm_s, rs1);
                    }
                break;
    }
    return 0;
}