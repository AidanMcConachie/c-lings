// `my_strdup` makes a heap copy of a string. clings detects that it writes
// past the end of the block it allocates. Fix it, then implement
// `concat` too.

#include <stdlib.h>
#include <string.h>

#include "clings.h"

char *my_strdup(const char *s) {
    char *copy = malloc(strlen(s));
    if (copy == NULL) {
        return NULL;
    }
    strcpy(copy, s);
    return copy;
}

// Returns a new heap-allocated string: `a` followed by `b`.
char *concat(const char *a, const char *b) {
    // TODO
    return NULL;
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
