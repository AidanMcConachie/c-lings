// Floating point numbers can't represent most decimals exactly, so
// comparing them with == is usually a bug.
//
// Fix `nearly_equal` so it treats numbers that are within 1e-9 of each
// other as equal.

#include <math.h>
#include <stdbool.h>

#include "clings.h"

bool nearly_equal(double a, double b) {
    return a == b;
}

TEST(exact_numbers) {
    ASSERT(nearly_equal(1.0, 1.0));
    ASSERT(nearly_equal(0.5 + 0.25, 0.75));
}

TEST(inexact_numbers) {
    ASSERT(nearly_equal(0.1 + 0.2, 0.3));
    ASSERT(nearly_equal(0.1 * 3, 0.3));
}

TEST(different_numbers) {
    ASSERT_FALSE(nearly_equal(0.1, 0.2));
    ASSERT_FALSE(nearly_equal(1.0, 1.001));
    ASSERT_FALSE(nearly_equal(-1.0, 1.0));
}
