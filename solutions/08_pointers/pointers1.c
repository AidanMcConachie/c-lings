#include "clings.h"

void set_to_42(int *p) {
    // Assigning to `p` would just change where the local pointer points.
    // `*p` is the int it points to.
    *p = 42;
}

TEST(sets_value) {
    int x = 0;
    set_to_42(&x);
    ASSERT_EQ(x, 42);
}

TEST(pointer_basics) {
    int y = 7;
    int *p = &y;
    *p = *p + 1;
    ASSERT_EQ(y, 8);
    set_to_42(p);
    ASSERT_EQ(y, 42);
}
