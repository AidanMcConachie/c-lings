#include "clings.h"

int sum_to(int n) {
    int total = 0;
    // Start at 1 and include n itself.
    for (int i = 1; i <= n; i++) {
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
