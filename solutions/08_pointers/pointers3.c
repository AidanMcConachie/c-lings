#include "clings.h"

int sum_range(const int *start, const int *end) {
    int total = 0;
    for (const int *p = start; p != end; p++) {
        total += *p;
    }
    return total;
}

const char *find_char(const char *s, char c) {
    for (; *s != '\0'; s++) {
        if (*s == c) {
            return s;
        }
    }
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
