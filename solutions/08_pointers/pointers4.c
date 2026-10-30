#include <stdbool.h>

#include "clings.h"

void min_max(const int *values, size_t len, int *min, int *max) {
    *min = values[0];
    *max = values[0];
    for (size_t i = 1; i < len; i++) {
        if (values[i] < *min) *min = values[i];
        if (values[i] > *max) *max = values[i];
    }
}

bool divide(int a, int b, int *quotient, int *remainder) {
    if (b == 0) {
        return false;
    }
    *quotient = a / b;
    *remainder = a % b;
    return true;
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
