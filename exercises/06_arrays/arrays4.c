// `reverse` should reverse an array in place. It reads one element past the
// end of the array, which C won't stop you from doing. Fix it!

#include "clings.h"

void reverse(int values[], size_t len) {
    for (size_t i = 0; i < len / 2; i++) {
        int tmp = values[i];
        values[i] = values[len - i];
        values[len - i] = tmp;
    }
}

TEST(reverse_odd_length) {
    int a[] = {1, 2, 3, 4, 5};
    reverse(a, 5);
    ASSERT_EQ(a[0], 5);
    ASSERT_EQ(a[1], 4);
    ASSERT_EQ(a[2], 3);
    ASSERT_EQ(a[3], 2);
    ASSERT_EQ(a[4], 1);
}

TEST(reverse_even_length) {
    int a[] = {10, 20, 30, 40, 99};
    reverse(a, 4); // only the first 4 elements!
    ASSERT_EQ(a[0], 40);
    ASSERT_EQ(a[1], 30);
    ASSERT_EQ(a[2], 20);
    ASSERT_EQ(a[3], 10);
    ASSERT_EQ(a[4], 99);
}
