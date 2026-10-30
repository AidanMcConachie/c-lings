// `same_text` should tell whether two strings contain the same characters.
// It works for some strings but not others. Why?

#include <stdbool.h>

#include "clings.h"

bool same_text(const char *a, const char *b) {
    return a == b;
}

TEST(same_pointer) {
    const char *s = "hello";
    ASSERT(same_text(s, s));
}

TEST(same_contents_different_arrays) {
    char a[] = "hello";
    char b[] = "hello";
    ASSERT(same_text(a, b));
}

TEST(different_contents) {
    ASSERT_FALSE(same_text("hello", "help"));
    ASSERT_FALSE(same_text("hello", "hello!"));
}
