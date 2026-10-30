#include <limits.h>

#include "clings.h"

typedef enum {
    DIV_OK,
    DIV_BY_ZERO,
    DIV_OVERFLOW,
} DivStatus;

DivStatus safe_divide(int a, int b, int *result) {
    // Check *before* dividing: both cases are undefined behavior, and on
    // most machines they crash the program.
    if (b == 0) {
        return DIV_BY_ZERO;
    }
    if (a == INT_MIN && b == -1) {
        return DIV_OVERFLOW;
    }
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
