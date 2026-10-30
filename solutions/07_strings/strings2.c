#include <stdbool.h>
#include <string.h>

#include "clings.h"

bool same_text(const char *a, const char *b) {
    // `a == b` compares addresses. strcmp compares the characters and
    // returns 0 when they're all equal.
    return strcmp(a, b) == 0;
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
