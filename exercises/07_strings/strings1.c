// Every C string ends with a NUL character '\0'.
//
// 1. `make_word` writes "cat" into `out`, but the result isn't a proper
//    string. Fix it.
// 2. Implement `my_strlen` yourself (no <string.h>!): count the characters
//    before the terminating '\0'.

#include "clings.h"

void make_word(char *out) {
    out[0] = 'c';
    out[1] = 'a';
    out[2] = 't';
}

size_t my_strlen(const char *s) {
    // TODO
    return 0;
}

TEST(make_word_makes_a_string) {
    char buf[8];
    memset(buf, 'X', sizeof buf); // fill the buffer with junk first
    make_word(buf);
    ASSERT_STR_EQ(buf, "cat");
}

TEST(lengths) {
    ASSERT_EQ(my_strlen(""), 0);
    ASSERT_EQ(my_strlen("a"), 1);
    ASSERT_EQ(my_strlen("hello, world"), 12);
}

TEST(array_holds_terminator) {
    char word[] = "dog";
    ASSERT_EQ(my_strlen(word), 3);
    ASSERT_EQ(sizeof(word), 4); // 3 letters + '\0'
    ASSERT_EQ(word[3], '\0');
}
