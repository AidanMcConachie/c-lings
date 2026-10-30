#include "clings.h"

int count_digits(int n) {
    int digits = 0;
    // A do-while loop runs its body at least once, so 0 counts as one digit.
    // Dividing a negative number by 10 also moves it towards 0, so negative
    // numbers work too.
    do {
        n /= 10;
        digits++;
    } while (n != 0);
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
