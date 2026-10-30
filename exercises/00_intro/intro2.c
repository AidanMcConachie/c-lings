// printf() is how C programs print text. Its first argument is a "format
// string"; every conversion like %d or %s in it must be matched by another
// argument with the value to print.
//
// This program doesn't compile. Fix it so it prints a greeting!

#include <stdio.h>

int main(void) {
    printf("Hello %s!\n");
    return 0;
}
