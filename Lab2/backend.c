#include <stdio.h>
#include <stdlib.h>
#include "build_tree.h"

int regtable[NUMREG];  // reg[i] contains current number of uses of register i
int vartable[NUMVAR];  // var[i] contains register to which var is assigned

void init_regtable(void) 
{
    for (int i = 0; i < NUMREG; i++)
        regtable[i] = 0;
}

void init_vartable(void)
{
    for (int i = 0; i < NUMVAR; i++)
        vartable[i] = -1;
}
    
int reuse_reg(int reg)
{
    if (regtable[reg] == 1)
        return(1);
    if (regtable[reg] > 1)
        return(0);
    printf("Error: called reuse_reg on unused register\n");
    return(-1);  // shouldn't happen
}

int assign_reg(int var)
{ 
        if ((var != -1) && (vartable[var] != -1)) {
            regtable[vartable[var]]++; // variable is already assigned a register
            return(vartable[var]);
        } else {
            for (int i = 5; i < NUMREG; i++)     // find unassigned register
                if (regtable[i] == 0) {
                    regtable[i]++;
                    if (var != -1) {
                        vartable[var] = i;
                    }
                    return(i);;
            }
            return(-1);  // out of registers
        }
}  

int release_reg(int reg)
{
    if (regtable[reg] > 0) {
        regtable[reg]--;
        return(0);
    } else
        return(-1);
}    

void printregtable(void)
{
    printf("register table -- number of uses of each register\n");
    for (int i = 0; i < NUMREG; i++)
      if (regtable[i])
        printf("register: x%d, uses: %d\n", i, regtable[i]);
}

void printvartable(void)
{
    printf("variable table -- register to which var is assigned\n");
    for (int i = 0; i < NUMVAR; i++)
      if (vartable[i] != -1)
        printf("variable: %c, register: x%d\n", 'a'+i, vartable[i]);
}

struct node *generate_code(struct node *root)
{
    struct node *left, *right;
    char instr[20];
    int destreg;

    if (root) {
        if (root->left)
            left = generate_code(root->left);
        if (root->right)
            right = generate_code(root->right);
        if (root->type == REG)
            return(root);
        else if (root->type == VAR) {
            root->type = REG;
            root->data = vartable[root->data];
            return(root);
        } else if (root->type == BINARYOP) {
            // Case 1: REG op REG
            if((left->type == REG) && (right->type == REG)) {
                if (reuse_reg(left->data) == 1) {
                    destreg = left->data;
                    release_reg(right->data);
                }
                else if (reuse_reg(right->data) == 1) {
                    destreg = right->data;
                    release_reg(left->data);
                }
                else {
                    destreg = assign_reg(-1);
                    if (destreg == -1) {
                        printf("Error: out of registers\n");
                        exit(-1); 
                    }
                    release_reg(left->data);
                    release_reg(right->data);
                }
                sprintf(instr, "%s  x%d, x%d, x%d", optable[root->data].instr, destreg, 
                        left->data, right->data);
                printf("%s\n", instr);
                free(left);
                free(right);
                root->type = REG;
                root->data = destreg;
            }
            // Case 2: CONST op REG (like 5 + a)
            else if ((left->type == CONST) && (right->type == REG)) {
                // Determine destination register
                if (reuse_reg(right->data)) {
                    destreg = right->data;
                } else {
                    destreg = assign_reg(-1);
                    if (destreg == -1) {
                        printf("Error: out of registers\n");
                        exit(-1);
                    }
                    release_reg(right->data);
                }
    
                // Check if operation is commutative
                if (root->data == ADD || root->data == MUL || 
                    root->data == AND || root->data == OR || root->data == XOR) {
                    
                    // Commutative: can swap operands
                    if (root->data == ADD) {
                        sprintf(instr, "addi  x%d, x%d, %d", destreg, right->data, left->data);
                        printf("%s\n", instr);
                    }
                    else if (root->data == XOR) {
                        sprintf(instr, "xori  x%d, x%d, %d", destreg, right->data, left->data);
                        printf("%s\n", instr);
                    }
                    else if (root->data == OR) {
                        sprintf(instr, "ori  x%d, x%d, %d", destreg, right->data, left->data);
                        printf("%s\n", instr);
                    }
                    else if (root->data == AND) {
                        sprintf(instr, "andi  x%d, x%d, %d", destreg, right->data, left->data);
                        printf("%s\n", instr);
                    }
                    else {  // MUL
                        int const_reg = assign_reg(-1);
                        if (const_reg == -1) {
                            printf("Error: out of registers\n");
                            exit(-1);
                        }
                        sprintf(instr, "li  x%d, %d", const_reg, left->data);
                        printf("%s\n", instr);
                        
                        sprintf(instr, "%s  x%d, x%d, x%d", optable[root->data].instr, 
                                destreg, const_reg, right->data);
                        printf("%s\n", instr);
                        release_reg(const_reg);
                    }
                }
                else {
                    // Non-commutative: must preserve order
                    int const_reg = assign_reg(-1);
                    if (const_reg == -1) {
                        printf("Error: out of registers\n");
                        exit(-1);
                    }
                    
                    sprintf(instr, "li  x%d, %d", const_reg, left->data);
                    printf("%s\n", instr);
                    
                    sprintf(instr, "%s  x%d, x%d, x%d", optable[root->data].instr, 
                            destreg, const_reg, right->data);
                    printf("%s\n", instr);
                    
                    release_reg(const_reg);
                }
                
                free(left);
                free(right);
                root->type = REG;
                root->data = destreg;
            }
            // Case 3: REG op CONST (like a + 5)
            else if ((left->type == REG) && (right->type == CONST)) {
                    // Determine destination register
                    if (reuse_reg(left->data)) {
                        destreg = left->data;
                    } else {
                        destreg = assign_reg(-1);
                        if (destreg == -1) {
                            printf("Error: out of registers\n");
                            exit(-1);
                        }
                        release_reg(left->data);
                    }
                    
                    // Generate instruction based on operator
                    if (root->data == ADD) {
                        // Check if immediate fits in 12 bits
                        if (right->data >= -2048 && right->data <= 2047) {
                            sprintf(instr, "addi  x%d, x%d, %d", destreg, left->data, right->data);
                            printf("%s\n", instr);
                        } else {
                            // Load constant, then use add
                            int const_reg = assign_reg(-1);
                            if (const_reg == -1) {
                                printf("Error: out of registers\n");
                                exit(-1);
                            }
                            sprintf(instr, "li  x%d, %d", const_reg, right->data);
                            printf("%s\n", instr);
                            
                            sprintf(instr, "add  x%d, x%d, x%d", destreg, left->data, const_reg);
                            printf("%s\n", instr);
                            
                            release_reg(const_reg);
                        }
                    }
                    else if (root->data == SUB) {
                        // Check if immediate fits in 12 bits
                        if (right->data >= -2048 && right->data <= 2047) {
                            // Use addi with negative (a - 5 = a + (-5))
                            sprintf(instr, "addi  x%d, x%d, %d", destreg, left->data, -(right->data));
                            printf("%s\n", instr);
                        } else {
                            // Load constant, then use sub
                            int const_reg = assign_reg(-1);
                            if (const_reg == -1) {
                                printf("Error: out of registers\n");
                                exit(-1);
                            }
                            sprintf(instr, "li  x%d, %d", const_reg, right->data);
                            printf("%s\n", instr);
                            
                            sprintf(instr, "sub  x%d, x%d, x%d", destreg, left->data, const_reg);
                            printf("%s\n", instr);
                            
                            release_reg(const_reg);
                        }
                    }
                    else if (root->data == XOR) {
                        if (right->data >= -2048 && right->data <= 2047) {
                            sprintf(instr, "xori  x%d, x%d, %d", destreg, left->data, right->data);
                            printf("%s\n", instr);
                        } else {
                            int const_reg = assign_reg(-1);
                            if (const_reg == -1) {
                                printf("Error: out of registers\n");
                                exit(-1);
                            }
                            sprintf(instr, "li  x%d, %d", const_reg, right->data);
                            printf("%s\n", instr);
                            
                            sprintf(instr, "xor  x%d, x%d, x%d", destreg, left->data, const_reg);
                            printf("%s\n", instr);
                            
                            release_reg(const_reg);
                        }
                    }
                    else if (root->data == OR) {
                        if (right->data >= -2048 && right->data <= 2047) {
                            sprintf(instr, "ori  x%d, x%d, %d", destreg, left->data, right->data);
                            printf("%s\n", instr);
                        } else {
                            int const_reg = assign_reg(-1);
                            if (const_reg == -1) {
                                printf("Error: out of registers\n");
                                exit(-1);
                            }
                            sprintf(instr, "li  x%d, %d", const_reg, right->data);
                            printf("%s\n", instr);
                            
                            sprintf(instr, "or  x%d, x%d, x%d", destreg, left->data, const_reg);
                            printf("%s\n", instr);
                            
                            release_reg(const_reg);
                        }
                    }
                    else if (root->data == AND) {
                        if (right->data >= -2048 && right->data <= 2047) {
                            sprintf(instr, "andi  x%d, x%d, %d", destreg, left->data, right->data);
                            printf("%s\n", instr);
                        } else {
                            int const_reg = assign_reg(-1);
                            if (const_reg == -1) {
                                printf("Error: out of registers\n");
                                exit(-1);
                            }
                            sprintf(instr, "li  x%d, %d", const_reg, right->data);
                            printf("%s\n", instr);
                            
                            sprintf(instr, "and  x%d, x%d, x%d", destreg, left->data, const_reg);
                            printf("%s\n", instr);
                            
                            release_reg(const_reg);
                        }
                    }
                    else if (root->data == SLL) {
                        sprintf(instr, "slli  x%d, x%d, %d", destreg, left->data, right->data);
                        printf("%s\n", instr);
                    }
                    else if (root->data == SRL) {
                        sprintf(instr, "srli  x%d, x%d, %d", destreg, left->data, right->data);
                        printf("%s\n", instr);
                    }
                    else {
                        // MUL, DIV - need to load constant first
                        int const_reg = assign_reg(-1);
                        if (const_reg == -1) {
                            printf("Error: out of registers\n");
                            exit(-1);
                        }
                        sprintf(instr, "li  x%d, %d", const_reg, right->data);
                        printf("%s\n", instr);
                        
                        sprintf(instr, "%s  x%d, x%d, x%d", optable[root->data].instr, 
                                destreg, left->data, const_reg);
                        printf("%s\n", instr);
                        release_reg(const_reg);
                    }
                    
                    free(left);
                    free(right);
                    root->type = REG;
                    root->data = destreg;
            }
            // Case 4: CONST op CONST (like 3 + 7)
            else if ((left->type == CONST) && (right->type == CONST)) {
                // Load left constant
                int left_reg = assign_reg(-1);
                if (left_reg == -1) {
                    printf("Error: out of registers\n");
                    exit(-1);
                }
                sprintf(instr, "li  x%d, %d", left_reg, left->data);
                printf("%s\n", instr);
                
                // Load right constant
                int right_reg = assign_reg(-1);
                if (right_reg == -1) {
                    printf("Error: out of registers\n");
                    exit(-1);
                }
                sprintf(instr, "li  x%d, %d", right_reg, right->data);
                printf("%s\n", instr);
                
                // Get result register
                destreg = assign_reg(-1);
                if (destreg == -1) {
                    printf("Error: out of registers\n");
                    exit(-1);
                }
                
                // Do the operation
                sprintf(instr, "%s  x%d, x%d, x%d", optable[root->data].instr, 
                        destreg, left_reg, right_reg);
                printf("%s\n", instr);
                
                // Clean up
                release_reg(left_reg);
                release_reg(right_reg);
                free(left);
                free(right);
                root->type = REG;
                root->data = destreg;
            }
        } else if (root->type == UNARYOP) {
            if (root->data == UMINUS) {
                if (left->type == REG) {
                    if (reuse_reg(left->data)) {
                        destreg = left->data;
                    } else {
                        destreg = assign_reg(-1);
                        if (destreg == -1) {
                            printf("Error: out of registers\n");
                            exit(-1);
                        }
                        release_reg(left->data);
                    }
                    sprintf(instr, "sub  x%d, x0, x%d", destreg, left->data);
                    printf("%s\n", instr);
                    free(left);
                    root->type = REG;
                    root->data = destreg;
                }
                else if (left->type == CONST) {
                    // Load the constant into a register
                    int const_reg = assign_reg(-1);
                    if (const_reg == -1) {
                        printf("Error: out of registers\n");
                        exit(-1);
                    }
                    sprintf(instr, "li  x%d, %d", const_reg, left->data);
                    printf("%s\n", instr);
                    
                    // Get a register for result
                    int result_reg = assign_reg(-1);
                    if (result_reg == -1) {
                        printf("Error: out of registers\n");
                        exit(-1);
                    }
                    
                    // Do subtraction: result = 0 - const
                    sprintf(instr, "sub  x%d, x0, x%d", result_reg, const_reg);
                    printf("%s\n", instr);
                    
                    release_reg(const_reg);
                    free(left);
                    
                    // Return result
                    root->type = REG;
                    root->data = result_reg;
                }
            }
            else if (root->data == NOT) {
                if (left->type == REG) {
                    // Figure out destination register
                    if (reuse_reg(left->data)) {
                        destreg = left->data;
                    } else {
                        destreg = assign_reg(-1);
                        if (destreg == -1) {
                            printf("Error: out of registers\n");
                            exit(-1);
                        }
                        release_reg(left->data);
                    }
                    // Create xori destreg, left, -1
                    sprintf(instr, "xori  x%d, x%d, -1", destreg, left->data);
                    printf("%s\n", instr);
                    
                    // Clean
                    free(left);
                    root->type = REG;
                    root->data = destreg;
                }
                else if (left->type == CONST) {
                    // Load the constant into a register
                    int const_reg = assign_reg(-1);
                    if (const_reg == -1) {
                        printf("Error: out of registers\n");
                        exit(-1);
                    }
                    sprintf(instr, "li  x%d, %d", const_reg, left->data);
                    printf("%s\n", instr);
                    
                    // Get a register for result
                    int result_reg = assign_reg(-1);
                    if (result_reg == -1) {
                        printf("Error: out of registers\n");
                        exit(-1);
                    }
                    
                    // Do XOR: result = const XOR -1
                    sprintf(instr, "xori  x%d, x%d, -1", result_reg, const_reg);
                    printf("%s\n", instr);
                    
                    release_reg(const_reg);
                    free(left);
                    
                    // Return result
                    root->type = REG;
                    root->data = result_reg;
                }
            }
        }
    }
    return(root);
}