// Macros substitute text, not values. These macros give surprising
// results. Fix the macro definitions (not the tests!).

#include "clings.h"

#define SQUARE(x) x * x
#define DOUBLE(x) (x) + (x)
#define BUFFER_SIZE 64

TEST(square) {
    ASSERT_EQ(SQUARE(3), 9);
    ASSERT_EQ(SQUARE(1 + 2), 9);
    ASSERT_EQ(100 / SQUARE(5), 4);
}

TEST(double_it) {
    ASSERT_EQ(DOUBLE(4), 8);
    ASSERT_EQ(DOUBLE(4) * 10, 80);
}

TEST(constant) {
    char buf[BUFFER_SIZE];
    ASSERT_EQ(sizeof buf, 64);
}
