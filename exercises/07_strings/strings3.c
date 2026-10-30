// Building strings safely: implement `make_greeting` so it writes
// "Hello, <name>!" into `out`.
//
// `out_size` is the size of the `out` buffer. The result must never be
// longer than that (including the '\0'). If it doesn't fit, cut it short,
// but it must still be a valid string!
//
// snprintf from <stdio.h> does all of this for you.

#include "clings.h"

void make_greeting(char *out, size_t out_size, const char *name) {
    // TODO
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
