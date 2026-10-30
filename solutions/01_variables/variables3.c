#include <stdio.h>

int main(void) {
    int hours = 3;
    // Declared outside the `if` block so it's still in scope below.
    int minutes = hours * 60;

    if (hours > 2) {
        printf("That's a long meeting.\n");
    }

    printf("%d hours is %d minutes\n", hours, minutes);
    return 0;
}
