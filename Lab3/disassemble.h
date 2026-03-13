/* Bit field masks */
#define OPCODEMASK   0x0000007F
#define RDMASK       0x00000F80
#define FUNCT3MASK   0x00007000
#define RS1MASK      0x000F8000
#define RS2MASK      0x01F00000
#define FUNCT7MASK   0xFE000000

#define NINSTRUCTIONS 17

enum instruction{ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, ADDI, ANDI, ORI, XORI, SLLI, SRLI, SRAI, LW, SW};

extern int disassemble(unsigned int minstr);