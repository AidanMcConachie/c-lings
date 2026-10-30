#include "clings.h"

// Conditions are checked from top to bottom and the first true one wins, so
// check the highest threshold first. `>=` makes the boundaries inclusive.
char grade(int score) {
    if (score >= 90) {
        return 'A';
    } else if (score >= 80) {
        return 'B';
    } else if (score >= 70) {
        return 'C';
    } else if (score >= 60) {
        return 'D';
    } else {
        return 'F';
    }
}

TEST(top_grades) {
    ASSERT_EQ(grade(100), 'A');
    ASSERT_EQ(grade(90), 'A');
    ASSERT_EQ(grade(89), 'B');
    ASSERT_EQ(grade(80), 'B');
}

TEST(middle_grades) {
    ASSERT_EQ(grade(75), 'C');
    ASSERT_EQ(grade(70), 'C');
    ASSERT_EQ(grade(60), 'D');
}

TEST(failing_grades) {
    ASSERT_EQ(grade(59), 'F');
    ASSERT_EQ(grade(0), 'F');
}
