// C passes arguments by value: the function works on a copy.
// `doubled` is supposed to give back twice its argument. Fix it!

#include "clings.h"

void doubled(int x) {
    x = x * 2;
}

TEST(doubles_numbers) {
    int n = 21;
    int result = doubled(n);
    ASSERT_EQ(result, 42);
    ASSERT_EQ(n, 21); // the caller's variable is untouched
    ASSERT_EQ(doubled(-4), -8);
}
