#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disassemble.h"

#define MAXINSTRLENGTH 10

int get_str(char s[], int lim)
{
    int c, i;
    
    for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
        s[i] = c;
    s[i] = '\0';
    return i;
}
  
int main()
{
    char instrstr[MAXINSTRLENGTH+1];
    unsigned int minstr;

    while (get_str(instrstr, MAXINSTRLENGTH+1)) {   
        minstr = (unsigned int) strtoul(instrstr, NULL, 16);
#ifdef DEBUG
        printf("Instruction to disassemble: %08x\n", minstr);
#endif
        disassemble(minstr);
    }
    return 0;
}
