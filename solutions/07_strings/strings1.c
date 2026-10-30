#include "clings.h"

void make_word(char *out) {
    out[0] = 'c';
    out[1] = 'a';
    out[2] = 't';
    out[3] = '\0'; // without this, nothing marks the end of the string
}

size_t my_strlen(const char *s) {
    size_t n = 0;
    while (s[n] != '\0') {
        n++;
    }
    return n;
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
