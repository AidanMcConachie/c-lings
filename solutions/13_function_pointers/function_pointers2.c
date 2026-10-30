#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "clings.h"

int compare_ints(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    // x - y can overflow (e.g. INT_MAX - INT_MIN), which is undefined
    // behavior. Comparisons can't overflow: this gives -1, 0 or 1.
    return (x > y) - (x < y);
}

int compare_strings(const void *a, const void *b) {
    // Each element is a `const char *`, so a and b are pointers to those.
    const char *x = *(const char *const *)a;
    const char *y = *(const char *const *)b;
    return strcmp(x, y);
}

TEST(sort_small_ints) {
    int a[] = {5, 2, 9, 1, 7};
    qsort(a, 5, sizeof a[0], compare_ints);
    ASSERT_EQ(a[0], 1);
    ASSERT_EQ(a[1], 2);
    ASSERT_EQ(a[4], 9);
}

TEST(sort_extreme_ints) {
    int a[] = {INT_MAX, -5, INT_MIN, 0};
    qsort(a, 4, sizeof a[0], compare_ints);
    ASSERT_EQ(a[0], INT_MIN);
    ASSERT_EQ(a[1], -5);
    ASSERT_EQ(a[2], 0);
    ASSERT_EQ(a[3], INT_MAX);
}

TEST(sort_strings) {
    const char *words[] = {"pear", "apple", "fig", "banana"};
    qsort(words, 4, sizeof words[0], compare_strings);
    ASSERT_STR_EQ(words[0], "apple");
    ASSERT_STR_EQ(words[1], "banana");
    ASSERT_STR_EQ(words[2], "fig");
    ASSERT_STR_EQ(words[3], "pear");
}
