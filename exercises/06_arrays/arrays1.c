// The compiler doesn't like this array. Fix it!

#include "clings.h"

TEST(first_five_primes) {
    int primes[4] = {2, 3, 5, 7, 11};

    size_t count = sizeof(primes) / sizeof(primes[0]);
    ASSERT_EQ(count, 5);
    ASSERT_EQ(primes[0], 2);
    ASSERT_EQ(primes[4], 11);
}

TEST(partially_initialized) {
    // Elements without an initializer are set to zero.
    int numbers[6] = {1, 2};
    ASSERT_EQ(numbers[1], 2);
    // TODO: what's in numbers[5]? Fix the assertion.
    ASSERT_EQ(numbers[5], 2);
}
