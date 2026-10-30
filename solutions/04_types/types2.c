#include "clings.h"

char to_upper(char c) {
    if (c >= 'a' && c <= 'z') {
        // Letters are consecutive, so 'c' - 'a' is the letter's position.
        return (char)(c - 'a' + 'A');
    }
    return c;
}

int digit_value(char c) {
    return c - '0';
}

TEST(uppercases_letters) {
    ASSERT_EQ(to_upper('a'), 'A');
    ASSERT_EQ(to_upper('q'), 'Q');
    ASSERT_EQ(to_upper('z'), 'Z');
}

TEST(leaves_others_alone) {
    ASSERT_EQ(to_upper('A'), 'A');
    ASSERT_EQ(to_upper('7'), '7');
    ASSERT_EQ(to_upper('!'), '!');
}

TEST(digit_values) {
    ASSERT_EQ(digit_value('0'), 0);
    ASSERT_EQ(digit_value('4'), 4);
    ASSERT_EQ(digit_value('9'), 9);
}
