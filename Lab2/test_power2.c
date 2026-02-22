#include <stdio.h>

int isPowerOfTwo(int n) {
    if (n <= 0 || (n & (n - 1)) != 0) {
        return 0;
    }
    
    int exponent = 0;
    while (n > 1) {
        n = n >> 1;  // Right shift is divide by 2
        exponent++;
    }
    return exponent;
}

int main() {
    printf("Testing isPowerOfTwo function:\n\n");
    
    // Test powers of 2
    printf("1: %d\n", isPowerOfTwo(1));
    printf("2: %d)\n", isPowerOfTwo(2));
    printf("4: %d)\n", isPowerOfTwo(4));
    printf("8: %d)\n", isPowerOfTwo(8));
    printf("16: %d)\n", isPowerOfTwo(16));
    printf("32: %d)\n", isPowerOfTwo(32));
    printf("1024: %d)\n", isPowerOfTwo(1024));
    
    // Test non-powers of 2
    printf("\n");
    printf("0: %d\n", isPowerOfTwo(0));
    printf("3: %d\n", isPowerOfTwo(3));
    printf("5: %d\n", isPowerOfTwo(5));
    printf("6: %d\n", isPowerOfTwo(6));
    printf("100: %d\n", isPowerOfTwo(100));
    printf("-4: %d\n", isPowerOfTwo(-4));
    
    return 0;
}