// Strings are just arrays, so you can walk through them with a loop and
// modify them in place (as long as they're not string literals!).
//
// Implement the functions below.

#include "clings.h"

// Returns how many times `c` appears in `s`.
size_t count_char(const char *s, char c) {
    // TODO
    return 0;
}

// Uppercases the first letter of every word in `s`, in place. Words are
// separated by spaces. Use toupper() from <ctype.h>.
void capitalize_words(char *s) {
    // TODO
}

TEST(counting) {
    ASSERT_EQ(count_char("banana", 'a'), 3);
    ASSERT_EQ(count_char("banana", 'n'), 2);
    ASSERT_EQ(count_char("banana", 'z'), 0);
    ASSERT_EQ(count_char("", 'a'), 0);
}

TEST(capitalizing) {
    char s[] = "the c programming language";
    capitalize_words(s);
    ASSERT_STR_EQ(s, "The C Programming Language");
}

TEST(capitalizing_extra_spaces) {
    char s[] = "  hello   world ";
    capitalize_words(s);
    ASSERT_STR_EQ(s, "  Hello   World ");
}
