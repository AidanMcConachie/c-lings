// atol() can't report errors: atol("hello") is just 0. strtol() can, if
// you check its results carefully.
//
// Rewrite `parse_long` with strtol so that it returns false for anything
// that isn't a whole number that fits in a long.

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "clings.h"

bool parse_long(const char *s, long *out) {
    *out = atol(s);
    return true;
}

TEST(valid_numbers) {
    long n = 0;
    ASSERT(parse_long("42", &n));
    ASSERT_EQ(n, 42);
    ASSERT(parse_long("-17", &n));
    ASSERT_EQ(n, -17);
    ASSERT(parse_long("  7", &n)); // leading whitespace is fine
    ASSERT_EQ(n, 7);
}

TEST(not_numbers) {
    long n = 0;
    ASSERT_FALSE(parse_long("", &n));
    ASSERT_FALSE(parse_long("hello", &n));
    ASSERT_FALSE(parse_long("12abc", &n));
}

TEST(out_of_range) {
    long n = 0;
    ASSERT_FALSE(parse_long("99999999999999999999999", &n));
    ASSERT_FALSE(parse_long("-99999999999999999999999", &n));
}
