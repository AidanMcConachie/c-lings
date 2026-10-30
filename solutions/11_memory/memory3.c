#include <stdlib.h>
#include <string.h>

#include "clings.h"

char *my_strdup(const char *s) {
    // strlen doesn't count the '\0', but strcpy copies it, so we need one
    // more byte.
    char *copy = malloc(strlen(s) + 1);
    if (copy == NULL) {
        return NULL;
    }
    strcpy(copy, s);
    return copy;
}

char *concat(const char *a, const char *b) {
    size_t len_a = strlen(a), len_b = strlen(b);
    char *s = malloc(len_a + len_b + 1);
    if (s == NULL) {
        return NULL;
    }
    memcpy(s, a, len_a);
    memcpy(s + len_a, b, len_b + 1); // includes b's '\0'
    return s;
}

TEST(duplicates) {
    char *s = my_strdup("hello");
    ASSERT_STR_EQ(s, "hello");
    free(s);
}

TEST(copy_is_independent) {
    char original[] = "abc";
    char *copy = my_strdup(original);
    original[0] = 'X';
    ASSERT_STR_EQ(copy, "abc");
    free(copy);
}

TEST(concatenates) {
    char *s = concat("foo", "bar");
    ASSERT_STR_EQ(s, "foobar");
    free(s);

    s = concat("", "");
    ASSERT_STR_EQ(s, "");
    free(s);
}
