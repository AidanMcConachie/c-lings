#include "clings.h"

// Returning from each case means we can't fall through. (Adding `break;`
// after every assignment works too.)
const char *day_name(int day) {
    switch (day) {
    case 1: return "Monday";
    case 2: return "Tuesday";
    case 3: return "Wednesday";
    case 4: return "Thursday";
    case 5: return "Friday";
    case 6: return "Saturday";
    case 7: return "Sunday";
    default: return "unknown";
    }
}

TEST(weekdays) {
    ASSERT_STR_EQ(day_name(1), "Monday");
    ASSERT_STR_EQ(day_name(3), "Wednesday");
    ASSERT_STR_EQ(day_name(5), "Friday");
}

TEST(weekend) {
    ASSERT_STR_EQ(day_name(6), "Saturday");
    ASSERT_STR_EQ(day_name(7), "Sunday");
}

TEST(not_a_day) {
    ASSERT_STR_EQ(day_name(0), "unknown");
    ASSERT_STR_EQ(day_name(8), "unknown");
}
