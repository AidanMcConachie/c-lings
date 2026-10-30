#include "clings.h"

typedef int (*IntFn)(int);

int twice(int x) {
    return 2 * x;
}

int square(int x) {
    return x * x;
}

int apply_n(IntFn f, int n, int x) {
    for (int i = 0; i < n; i++) {
        x = f(x);
    }
    return x;
}

int add_three(int x) {
    return x + 3;
}

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
