// Implement `apply_n`, which calls `f` on `x` n times:
// apply_n(f, 3, x) == f(f(f(x))). Then write `add_three` so the last test
// passes.

#include "clings.h"

typedef int (*IntFn)(int);

int twice(int x) {
    return 2 * x;
}

int square(int x) {
    return x * x;
}

int apply_n(IntFn f, int n, int x) {
    // TODO
    return x;
}

// TODO: define add_three

TEST(apply_zero_times) {
    ASSERT_EQ(apply_n(twice, 0, 5), 5);
}

TEST(apply_several_times) {
    ASSERT_EQ(apply_n(twice, 3, 1), 8);
    ASSERT_EQ(apply_n(square, 2, 3), 81);
}

TEST(your_own_function) {
    ASSERT_EQ(apply_n(add_three, 4, 0), 12);
}
