// Pointer arithmetic: adding 1 to a pointer moves it to the next element.
//
// Implement these WITHOUT using [] indexing. Walk a pointer instead!

#include "clings.h"

// Sums the ints from `start` up to (but not including) `end`.
int sum_range(const int *start, const int *end) {
    // TODO
    return 0;
}

// Returns a pointer to the first occurrence of `c` in `s`, or NULL if `c`
// isn't in the string.
const char *find_char(const char *s, char c) {
    // TODO
    return NULL;
}

TEST(sums_range) {
    int a[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(sum_range(a, a + 5), 15);
    ASSERT_EQ(sum_range(a + 1, a + 3), 5);
    ASSERT_EQ(sum_range(a, a), 0);
}

TEST(finds_char) {
    const char *s = "hello";
    ASSERT_PTR_EQ(find_char(s, 'h'), s);
    ASSERT_PTR_EQ(find_char(s, 'l'), s + 2);
    ASSERT_PTR_EQ(find_char(s, 'o'), s + 4);
}

TEST(missing_char) {
    ASSERT_NULL(find_char("hello", 'z'));
    ASSERT_NULL(find_char("", 'a'));
}
