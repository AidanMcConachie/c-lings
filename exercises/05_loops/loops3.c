// `count_digits` counts the decimal digits of a number. It works for most
// numbers, but not all of them. Fix it! (A do-while loop might help.)

#include "clings.h"

int count_digits(int n) {
    int digits = 0;
    while (n != 0) {
        n /= 10;
        digits++;
    }
    return digits;
}

TEST(positive_numbers) {
    ASSERT_EQ(count_digits(7), 1);
    ASSERT_EQ(count_digits(42), 2);
    ASSERT_EQ(count_digits(1000), 4);
}

TEST(negative_numbers) {
    ASSERT_EQ(count_digits(-123), 3);
}

TEST(zero_has_one_digit) {
    ASSERT_EQ(count_digits(0), 1);
}
