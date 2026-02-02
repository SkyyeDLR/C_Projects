#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "build_tree.h"

int get_str(char s[], int lim) {
    int c, i;
    
    i = 0;
    for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
        s[i] = c;
    s[i] = '\0';
    return i;
}

void postorder(struct node *root) {
    if (root == NULL) {
        return;
    }

    postorder(root->left);
    postorder(root->right);

    // If root is a VAR add it to the char 'a' to get another char
    if (root->type == VAR) {
        printf("%c", 'a' + root->data);
    } else if (root->data == CONST) {    // If root is a number, just print it.
        printf("%d", root->data);
    } else if (root->type == UNARYOP || root->type == BINARYOP) {   // If the root is UNARYOP or BINARYOP use the optable from build.c
        printf("%s", optable[root->data].symbol);
    }
}

int main() {
    char expr1[MAXEXPRLENGTH];
    struct node *root;

    while (get_str(expr1, MAXEXPRLENGTH)) {   
        printf("Expression to parse: %s\n", expr1);
        root = build_tree(expr1);
        printf("Postfix: ");
        postorder(root);
        printf("\n");
        printf("\n");
    }
    return 0;
}