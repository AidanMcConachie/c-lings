// `make_filled` should return a new heap-allocated array of `len` ints, all
// set to `value`. The caller frees it.

#include <stdlib.h>

#include "clings.h"

int *make_filled(size_t len, int value) {
    // TODO
    return NULL;
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
