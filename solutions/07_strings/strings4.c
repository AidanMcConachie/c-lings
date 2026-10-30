#include <ctype.h>
#include <stdbool.h>

#include "clings.h"

size_t count_char(const char *s, char c) {
    size_t count = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] == c) {
            count++;
        }
    }
    return count;
}

void capitalize_words(char *s) {
    bool at_word_start = true;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] == ' ') {
            at_word_start = true;
        } else if (at_word_start) {
            // ctype functions take an unsigned char value (or EOF).
            s[i] = (char)toupper((unsigned char)s[i]);
            at_word_start = false;
        }
    }
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
