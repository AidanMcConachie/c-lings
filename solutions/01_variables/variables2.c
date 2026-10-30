#include <stdio.h>

int main(void) {
    // A const variable gets its value once, when it's initialized.
    const int answer = 42;

    printf("The answer is %d\n", answer);
    return answer == 42 ? 0 : 1;
}
