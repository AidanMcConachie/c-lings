// The compiler has something to say about these functions.

#include "clings.h"

int is_even(int n) {
    n % 2 == 0;
}

int max3(int a, int b, int c) {
    int biggest = a;
    if (b > biggest) {
        biggest = b;
    }
    if (c > biggest) {
        biggest = c;
    }
}

TEST(is_even_works) {
    ASSERT(is_even(4));
    ASSERT(is_even(0));
    ASSERT_FALSE(is_even(7));
}

TEST(max3_works) {
    ASSERT_EQ(max3(1, 2, 3), 3);
    ASSERT_EQ(max3(9, 2, 3), 9);
    ASSERT_EQ(max3(1, 8, 3), 8);
    ASSERT_EQ(max3(-1, -2, -3), -1);
}
