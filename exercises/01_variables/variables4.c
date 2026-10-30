// This one compiles, but the test fails. Find the bug!
//
// (From here on most exercises use TEST blocks from clings.h. A test passes
// when all of the ASSERTs in it hold.)

#include "clings.h"

int count_even(int limit) {
    int count = 0;
    for (int i = 0; i < limit; i++) {
        if (i % 2 == 0) {
            int count = 0;
            count = count + 1;
        }
    }
    return count;
}

TEST(counts_even_numbers_below_limit) {
    ASSERT_EQ(count_even(1), 1);  // 0
    ASSERT_EQ(count_even(6), 3);  // 0, 2, 4
    ASSERT_EQ(count_even(10), 5); // 0, 2, 4, 6, 8
}
