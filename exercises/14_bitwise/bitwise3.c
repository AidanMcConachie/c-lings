// Some classic bit tricks. Implement them!

#include <stdbool.h>
#include <stdint.h>

#include "clings.h"

// Counts how many bits are 1.
int popcount(uint32_t x) {
    // TODO
    return 0;
}

// Powers of two have exactly one bit set. 0 is not a power of two.
bool is_power_of_two(uint32_t x) {
    // TODO
    return false;
}

// Returns byte number `n` (0 = least significant) of x.
// byte_at(0x11223344, 0) == 0x44, byte_at(0x11223344, 3) == 0x11
uint8_t byte_at(uint32_t x, int n) {
    // TODO
    return 0;
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
