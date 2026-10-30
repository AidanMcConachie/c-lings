// `switch` is handy for picking between many values. But beware of
// fall-through! Fix `day_name` so each day gets the right name.

#include "clings.h"

const char *day_name(int day) {
    const char *name = "unknown";
    switch (day) {
    case 1:
        name = "Monday";
    case 2:
        name = "Tuesday";
    case 3:
        name = "Wednesday";
    case 4:
        name = "Thursday";
    case 5:
        name = "Friday";
    case 6:
        name = "Saturday";
    case 7:
        name = "Sunday";
    }
    return name;
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
