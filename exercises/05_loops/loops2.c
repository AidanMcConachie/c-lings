// The Collatz conjecture: start with any positive number n. If n is even,
// halve it. If it's odd, replace it with 3n + 1. Repeat. It seems you always
// end up at 1 eventually, but nobody has managed to prove it!
//
// Implement `collatz_steps`: return how many steps it takes to reach 1.
// A `while` loop fits nicely here.

#include "clings.h"

int collatz_steps(long n) {
    // TODO
    return -1;
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
