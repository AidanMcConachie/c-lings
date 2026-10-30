#include <stdbool.h>
#include <stdint.h>

#include "clings.h"

int popcount(uint32_t x) {
    int count = 0;
    while (x != 0) {
        x &= x - 1; // clears the lowest set bit
        count++;
    }
    return count;
}

bool is_power_of_two(uint32_t x) {
    // x - 1 flips the lowest set bit and every bit below it, so x & (x - 1)
    // is 0 exactly when x had a single bit set.
    return x != 0 && (x & (x - 1)) == 0;
}

uint8_t byte_at(uint32_t x, int n) {
    return (uint8_t)((x >> (8 * n)) & 0xFF);
}

TEST(counting_bits) {
    ASSERT_EQ(popcount(0), 0);
    ASSERT_EQ(popcount(1), 1);
    ASSERT_EQ(popcount(0xFF), 8);
    ASSERT_EQ(popcount(0x80000001u), 2);
    ASSERT_EQ(popcount(0xFFFFFFFFu), 32);
}

TEST(powers_of_two) {
    ASSERT(is_power_of_two(1));
    ASSERT(is_power_of_two(64));
    ASSERT(is_power_of_two(0x80000000u));
    ASSERT_FALSE(is_power_of_two(0));
    ASSERT_FALSE(is_power_of_two(6));
    ASSERT_FALSE(is_power_of_two(0xFFFFFFFFu));
}

TEST(bytes) {
    ASSERT_EQ(byte_at(0x11223344u, 0), 0x44);
    ASSERT_EQ(byte_at(0x11223344u, 1), 0x33);
    ASSERT_EQ(byte_at(0x11223344u, 3), 0x11);
}
