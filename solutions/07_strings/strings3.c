#include <stdio.h>

#include "clings.h"

void make_greeting(char *out, size_t out_size, const char *name) {
    // snprintf never writes more than out_size bytes and always
    // NUL-terminates (when out_size > 0).
    snprintf(out, out_size, "Hello, %s!", name);
}

TEST(greets) {
    char buf[64];
    make_greeting(buf, sizeof buf, "Dennis");
    ASSERT_STR_EQ(buf, "Hello, Dennis!");

    make_greeting(buf, sizeof buf, "Ken");
    ASSERT_STR_EQ(buf, "Hello, Ken!");
}

TEST(truncates_without_overflowing) {
    char buf[16];
    memset(buf, 'X', sizeof buf);
    char *small = buf + 4; // only use the middle 8 bytes of buf
    make_greeting(small, 8, "Brian");
    ASSERT_STR_EQ(small, "Hello, ");
    ASSERT_EQ(buf[3], 'X');  // nothing written before...
    ASSERT_EQ(buf[12], 'X'); // ... or after the 8 bytes
}
