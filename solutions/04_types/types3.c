#include <limits.h>
#include <stdint.h>

#include "clings.h"

TEST(fixed_width_sizes) {
    // sizeof counts bytes, and the number in the name counts bits.
    ASSERT_EQ(sizeof(int8_t), 1);
    ASSERT_EQ(sizeof(uint16_t), 2);
    ASSERT_EQ(sizeof(int32_t), 4);
    ASSERT_EQ(sizeof(uint64_t), 8);
}

TEST(unsigned_wraps_around) {
    uint8_t small = 255;
    small = small + 1;
    ASSERT_EQ(small, 0);

    unsigned int u = 0;
    u = u - 1;
    ASSERT(u == UINT_MAX);
}

TEST(conversions) {
    // Converting to an integer truncates towards zero.
    int truncated = (int)3.99;
    ASSERT_EQ(truncated, 3);

    int negative = (int)-3.99;
    ASSERT_EQ(negative, -3);

    char letter = 'A' + 2;
    ASSERT_EQ(letter, 'C');
}
