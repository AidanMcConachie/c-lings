// `grade` turns a score from 0 to 100 into a letter:
//   90-100 => 'A', 80-89 => 'B', 70-79 => 'C', 60-69 => 'D', below 60 => 'F'
// Fix the bugs in the else-if chain.

#include "clings.h"

char grade(int score) {
    if (score > 60) {
        return 'D';
    } else if (score > 70) {
        return 'C';
    } else if (score > 80) {
        return 'B';
    } else if (score > 90) {
        return 'A';
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
