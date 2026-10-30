// A `char` is a small integer, and character literals like 'a' are numbers.
// That means you can do arithmetic with them!
//
// Implement these functions WITHOUT using <ctype.h>.

#include "clings.h"

// Turns 'a'..'z' into 'A'..'Z'. Any other character is returned unchanged.
char to_upper(char c) {
    // TODO
    return c;
}

// Turns a digit character '0'..'9' into its value 0..9.
int digit_value(char c) {
    // TODO
    return 0;
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
