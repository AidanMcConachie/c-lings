// qsort sorts any array, given a function that compares two elements.
// The comparator gets pointers to the elements (as `const void *`) and
// returns a negative number, zero or a positive number.
//
// 1. `compare_ints` has a subtle bug with very big and very small numbers.
// 2. Implement `compare_strings` so arrays of strings sort alphabetically.

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "clings.h"

int compare_ints(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return x - y;
}

// The array elements are `const char *`, so a and b point to those.
int compare_strings(const void *a, const void *b) {
    // TODO
    return 0;
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
