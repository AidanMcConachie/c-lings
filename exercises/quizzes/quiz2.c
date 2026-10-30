// Quiz 2: arrays, strings and pointers.
//
// Implement the functions below. You may use <string.h>.

#include "clings.h"

// Reverses a string in place: "hello" becomes "olleh".
void str_reverse(char *s) {
    // TODO
}

// Counts the words in `s`. Words are separated by one or more spaces.
size_t word_count(const char *s) {
    // TODO
    return 0;
}

// Copies the words of `s` into `out` in reverse order, separated by single
// spaces: "the quick fox" => "fox quick the". `out` is big enough.
void reverse_words(const char *s, char *out) {
    // TODO
}

TEST(reverses_strings) {
    char a[] = "hello";
    str_reverse(a);
    ASSERT_STR_EQ(a, "olleh");

    char b[] = "ab";
    str_reverse(b);
    ASSERT_STR_EQ(b, "ba");

    char c[] = "";
    str_reverse(c);
    ASSERT_STR_EQ(c, "");
}

TEST(counts_words) {
    ASSERT_EQ(word_count("hello world"), 2);
    ASSERT_EQ(word_count("  lots   of   space  "), 3);
    ASSERT_EQ(word_count("single"), 1);
    ASSERT_EQ(word_count(""), 0);
    ASSERT_EQ(word_count("   "), 0);
}

TEST(reverses_words) {
    char out[64];
    reverse_words("the quick brown fox", out);
    ASSERT_STR_EQ(out, "fox brown quick the");

    reverse_words("  spaced   out ", out);
    ASSERT_STR_EQ(out, "out spaced");

    reverse_words("", out);
    ASSERT_STR_EQ(out, "");
}
