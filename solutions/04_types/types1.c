#include "clings.h"

double average(int a, int b) {
    // int / int is integer division. Make one side a double first.
    return (a + b) / 2.0;
}

TEST(average_of_whole_result) {
    ASSERT_NEAR(average(2, 4), 3.0, 1e-9);
}

TEST(average_with_fraction) {
    ASSERT_NEAR(average(2, 3), 2.5, 1e-9);
    ASSERT_NEAR(average(-1, 0), -0.5, 1e-9);
}
