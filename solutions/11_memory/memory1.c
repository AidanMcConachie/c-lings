#include <stdlib.h>

#include "clings.h"

int *make_filled(size_t len, int value) {
    // `sizeof *a` is the size of one element, whatever its type.
    int *a = malloc(len * sizeof *a);
    if (a == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        a[i] = value;
    }
    return a;
}

TEST(allocates_and_fills) {
    int *a = make_filled(5, 7);
    ASSERT_NOT_NULL(a);
    for (size_t i = 0; i < 5; i++) {
        ASSERT_EQ(a[i], 7);
    }
    free(a);
}

TEST(survives_the_function) {
    // This is what heap memory is for: a local array would be gone by now.
    int *a = make_filled(1000, -1);
    ASSERT_NOT_NULL(a);
    ASSERT_EQ(a[999], -1);
    free(a);
}
