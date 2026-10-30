#include "clings.h"

TEST(first_five_primes) {
    // `int primes[] = ...` would also work: the compiler counts for you.
    int primes[5] = {2, 3, 5, 7, 11};

    size_t count = sizeof(primes) / sizeof(primes[0]);
    ASSERT_EQ(count, 5);
    ASSERT_EQ(primes[0], 2);
    ASSERT_EQ(primes[4], 11);
}

TEST(partially_initialized) {
    int numbers[6] = {1, 2};
    ASSERT_EQ(numbers[1], 2);
    ASSERT_EQ(numbers[5], 0);
}
