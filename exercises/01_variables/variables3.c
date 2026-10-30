// A variable only exists inside the block where it's declared.
// Make this compile without changing what the program prints.

#include <stdio.h>

int main(void) {
    int hours = 3;

    if (hours > 2) {
        int minutes = hours * 60;
        printf("That's a long meeting.\n");
    }

    printf("%d hours is %d minutes\n", hours, minutes);
    return 0;
}
