#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "clings.h"

bool parse_long(const char *s, long *out) {
    char *end;
    errno = 0; // strtol only sets errno on error, so clear it first
    long value = strtol(s, &end, 10);
    if (end == s) {
        return false; // no digits at all
    }
    if (*end != '\0') {
        return false; // junk after the number
    }
    if (errno == ERANGE) {
        return false; // too big or too small for a long
    }
    *out = value;
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
