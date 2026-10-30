#include "clings.h"

int collatz_steps(long n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        steps++;
    }
    return steps;
}

TEST(one_needs_no_steps) {
    ASSERT_EQ(collatz_steps(1), 0);
}

TEST(powers_of_two) {
    ASSERT_EQ(collatz_steps(2), 1);
    ASSERT_EQ(collatz_steps(16), 4);
}

TEST(odd_numbers) {
    ASSERT_EQ(collatz_steps(3), 7); // 3 10 5 16 8 4 2 1
    ASSERT_EQ(collatz_steps(27), 111);
}
