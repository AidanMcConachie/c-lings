// `square` is defined below main(), but it's used inside main().
// Make this compile WITHOUT moving the definition of `square`.

#include <stdio.h>

int main(void) {
    int result = square(7);
    printf("7 squared is %d\n", result);
    return result == 49 ? 0 : 1;
}

int square(int x) {
    return x * x;
}
