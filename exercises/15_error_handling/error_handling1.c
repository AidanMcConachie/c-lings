// `safe_divide` is supposed to report problems instead of crashing, but it
// doesn't check for any. Right now it crashes the whole test program!
//
// Two things can go wrong when dividing ints:
// - dividing by zero
// - INT_MIN / -1, whose result (INT_MAX + 1) doesn't fit in an int

#include <limits.h>

#include "clings.h"

typedef enum {
    DIV_OK,
    DIV_BY_ZERO,
    DIV_OVERFLOW,
} DivStatus;

DivStatus safe_divide(int a, int b, int *result) {
    *result = a / b;
    return DIV_OK;
}

TEST(normal_division) {
    int r = 0;
    ASSERT_EQ(safe_divide(42, 6, &r), DIV_OK);
    ASSERT_EQ(r, 7);
    ASSERT_EQ(safe_divide(-9, 2, &r), DIV_OK);
    ASSERT_EQ(r, -4);
}

TEST(division_by_zero) {
    int r = 123;
    ASSERT_EQ(safe_divide(1, 0, &r), DIV_BY_ZERO);
    ASSERT_EQ(r, 123); // untouched on error
}

TEST(overflow) {
    int r = 123;
    ASSERT_EQ(safe_divide(INT_MIN, -1, &r), DIV_OVERFLOW);
    ASSERT_EQ(r, 123);
    ASSERT_EQ(safe_divide(INT_MIN, 1, &r), DIV_OK);
    ASSERT_EQ(r, INT_MIN);
}
