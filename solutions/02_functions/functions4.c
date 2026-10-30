#include "clings.h"

// Changing a parameter only changes the function's copy, so return the
// result instead.
int doubled(int x) {
    return x * 2;
}

TEST(doubles_numbers) {
    int n = 21;
    int result = doubled(n);
    ASSERT_EQ(result, 42);
    ASSERT_EQ(n, 21); // the caller's variable is untouched
    ASSERT_EQ(doubled(-4), -8);
}
