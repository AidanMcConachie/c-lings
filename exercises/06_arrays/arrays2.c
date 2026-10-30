// `sum` tries to work out the length of its array argument with sizeof.
// The compiler warns that this doesn't do what it looks like.
//
// Fix `sum` by giving it a length parameter, then implement `max_of` too.

#include "clings.h"

int sum(int values[]) {
    int total = 0;
    size_t len = sizeof(values) / sizeof(values[0]);
    for (size_t i = 0; i < len; i++) {
        total += values[i];
    }
    return total;
}

// Returns the biggest element. `len` is at least 1.
int max_of(const int values[], size_t len) {
    // TODO
    return 0;
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
