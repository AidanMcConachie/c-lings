#include <stdio.h>

// A prototype tells the compiler about `square` before its first use.
int square(int x);

int main(void) {
    int result = square(7);
    printf("7 squared is %d\n", result);
    return result == 49 ? 0 : 1;
}

int square(int x) {
    return x * x;
}
