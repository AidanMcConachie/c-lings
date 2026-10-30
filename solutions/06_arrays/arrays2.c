#include "clings.h"

// Inside the function `values` is really an `int *`, so sizeof(values) is
// the size of a pointer. The caller has to tell us the length.
int sum(const int values[], size_t len) {
    int total = 0;
    for (size_t i = 0; i < len; i++) {
        total += values[i];
    }
    return total;
}

int max_of(const int values[], size_t len) {
    int best = values[0];
    for (size_t i = 1; i < len; i++) {
        if (values[i] > best) {
            best = values[i];
        }
    }
    return best;
}

TEST(sums) {
    int nums[] = {1, 2, 3, 4, 5, 6};
    ASSERT_EQ(sum(nums, 6), 21);
    ASSERT_EQ(sum(nums, 3), 6);
}

TEST(maximum) {
    int nums[] = {3, 9, -2, 7};
    ASSERT_EQ(max_of(nums, 4), 9);

    int negative[] = {-8, -3, -5};
    ASSERT_EQ(max_of(negative, 3), -3);
}
