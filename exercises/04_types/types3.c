// How well do you know C's types? Replace each TODO with the right number.

#include <limits.h>
#include <stdint.h>

#include "clings.h"

TEST(fixed_width_sizes) {
    ASSERT_EQ(sizeof(int8_t), TODO);
    ASSERT_EQ(sizeof(uint16_t), TODO);
    ASSERT_EQ(sizeof(int32_t), TODO);
    ASSERT_EQ(sizeof(uint64_t), TODO);
}

TEST(unsigned_wraps_around) {
    uint8_t small = 255;
    small = small + 1;
    ASSERT_EQ(small, TODO);

    unsigned int u = 0;
    u = u - 1;
    ASSERT(u == TODO); // hint: <limits.h> has a name for this
}

TEST(conversions) {
    int truncated = (int)3.99;
    ASSERT_EQ(truncated, TODO);

    int negative = (int)-3.99;
    ASSERT_EQ(negative, TODO);

    char letter = 'A' + 2;
    ASSERT_EQ(letter, TODO); // a character literal works here too
}
