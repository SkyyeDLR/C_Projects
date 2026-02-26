/* add definitions of masks below */

#define NINSTRUCTIONS 17

enum instruction{ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, ADDI, ANDI, ORI, XORI, SLLI, SRLI, SRAI, LW, SW};

extern int disassemble(unsigned int minstr);
