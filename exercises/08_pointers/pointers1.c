// To let a function change a variable of its caller, pass the variable's
// address and write through the pointer.
//
// Fix `set_to_42` so the tests compile and pass.

#include "clings.h"

void set_to_42(int p) {
    p = 42;
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
