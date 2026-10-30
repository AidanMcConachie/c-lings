// A C function can only return one value. To give back more, the caller
// passes pointers ("out parameters") that the function writes into.
//
// Implement the functions below.

#include <stdbool.h>

#include "clings.h"

// Finds the smallest and biggest element. `len` is at least 1.
void min_max(const int *values, size_t len, int *min, int *max) {
    // TODO
}

// Divides a by b, storing the quotient and remainder. Returns false (and
// doesn't touch *quotient or *remainder) if b is 0.
bool divide(int a, int b, int *quotient, int *remainder) {
    // TODO
    return false;
}

TEST(min_and_max) {
    int values[] = {4, -2, 9, 0, 7};
    int lo = 0, hi = 0;
    min_max(values, 5, &lo, &hi);
    ASSERT_EQ(lo, -2);
    ASSERT_EQ(hi, 9);
}

TEST(single_element) {
    int values[] = {3};
    int lo = 0, hi = 0;
    min_max(values, 1, &lo, &hi);
    ASSERT_EQ(lo, 3);
    ASSERT_EQ(hi, 3);
}

TEST(division) {
    int q = 0, r = 0;
    ASSERT(divide(17, 5, &q, &r));
    ASSERT_EQ(q, 3);
    ASSERT_EQ(r, 2);
}

TEST(division_by_zero) {
    int q = -1, r = -1;
    ASSERT_FALSE(divide(1, 0, &q, &r));
    ASSERT_EQ(q, -1);
    ASSERT_EQ(r, -1);
}
