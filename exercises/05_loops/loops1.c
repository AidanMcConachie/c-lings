// `sum_to(n)` should add up all the numbers from 1 to n (inclusive).
// It's close... but not quite right.

#include "clings.h"

int sum_to(int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += i;
    }
    return total;
}

TEST(small_sums) {
    ASSERT_EQ(sum_to(1), 1);
    ASSERT_EQ(sum_to(3), 6); // 1 + 2 + 3
}

TEST(bigger_sum) {
    ASSERT_EQ(sum_to(100), 5050);
}

TEST(empty_sum) {
    ASSERT_EQ(sum_to(0), 0);
}
