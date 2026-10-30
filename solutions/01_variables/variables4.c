#include "clings.h"

int count_even(int limit) {
    int count = 0;
    for (int i = 0; i < limit; i++) {
        if (i % 2 == 0) {
            // Re-declaring `count` here created a new variable that shadowed
            // the outer one. We want to update the outer one instead.
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
