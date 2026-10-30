#include <stdbool.h>
#include <stdint.h>

#include "clings.h"

// 1u (unsigned) matters: 1 << 31 on a signed int is undefined behavior.

uint32_t set_bit(uint32_t x, int n) {
    return x | (1u << n);
}

uint32_t clear_bit(uint32_t x, int n) {
    return x & ~(1u << n);
}

uint32_t toggle_bit(uint32_t x, int n) {
    return x ^ (1u << n);
}

bool test_bit(uint32_t x, int n) {
    return (x >> n) & 1u;
}

TEST(setting) {
    ASSERT_EQ(set_bit(0, 0), 1);
    ASSERT_EQ(set_bit(0, 3), 8);
    ASSERT_EQ(set_bit(8, 3), 8);
    ASSERT_EQ(set_bit(0, 31), 0x80000000u);
}

TEST(clearing) {
    ASSERT_EQ(clear_bit(15, 0), 14);
    ASSERT_EQ(clear_bit(15, 3), 7);
    ASSERT_EQ(clear_bit(0, 5), 0);
}

TEST(toggling) {
    ASSERT_EQ(toggle_bit(5, 1), 7);
    ASSERT_EQ(toggle_bit(7, 1), 5);
}

TEST(testing) {
    ASSERT(test_bit(4, 2));
    ASSERT_FALSE(test_bit(4, 1));
    ASSERT(test_bit(0x80000000u, 31));
}
