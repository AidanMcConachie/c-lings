// `const` variables can't be assigned to after they are initialized.
// Make this program compile and exit successfully, but keep `answer` const!

#include <stdio.h>

int main(void) {
    const int answer = 41;

    answer = 42;

    printf("The answer is %d\n", answer);
    return answer == 42 ? 0 : 1;
}
