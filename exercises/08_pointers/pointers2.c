// `swap` should exchange the values of two ints. It compiles, but it
// doesn't swap anything. Fix it!

#include "clings.h"

void swap(int *a, int *b) {
    int *tmp = a;
    a = b;
    b = tmp;
}

TEST(swaps_values) {
    int x = 1, y = 2;
    swap(&x, &y);
    ASSERT_EQ(x, 2);
    ASSERT_EQ(y, 1);
}

TEST(swaps_array_elements) {
    int a[] = {10, 20, 30};
    swap(&a[0], &a[2]);
    ASSERT_EQ(a[0], 30);
    ASSERT_EQ(a[1], 20);
    ASSERT_EQ(a[2], 10);
}
