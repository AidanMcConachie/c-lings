#include <stdlib.h>

#include "clings.h"

long sum_of_squares(int n) {
    long *squares = malloc((size_t)n * sizeof *squares);
    if (squares == NULL) {
        return -1;
    }
    for (int i = 0; i < n; i++) {
        squares[i] = (long)(i + 1) * (i + 1);
    }

    long total = 0;
    for (int i = 0; i < n; i++) {
        total += squares[i];
    }
    free(squares); // we're done with it, so give it back
    return total;
}

TEST(small) {
    ASSERT_EQ(sum_of_squares(1), 1);
    ASSERT_EQ(sum_of_squares(3), 14);
}

TEST(large) {
    ASSERT_EQ(sum_of_squares(100), 338350);
}
