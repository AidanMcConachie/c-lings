#include "clings.h"

int bigger(int a, int b) {
    return a > b ? a : b;
}

TEST(ten_is_bigger_than_eight) {
    ASSERT_EQ(bigger(10, 8), 10);
}

TEST(fortytwo_is_bigger_than_thirtytwo) {
    ASSERT_EQ(bigger(32, 42), 42);
}

TEST(equal_numbers) {
    ASSERT_EQ(bigger(42, 42), 42);
}

TEST(negative_numbers) {
    ASSERT_EQ(bigger(-5, -9), -5);
}
