#include <stdbool.h>

#include "clings.h"

bool is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int days_in_month(int month, int year) {
    switch (month) {
    case 2:
        return is_leap_year(year) ? 29 : 28;
    case 4: case 6: case 9: case 11:
        return 30;
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    default:
        return 0;
    }
}

int days_between_years(int from, int to) {
    int days = 0;
    for (int year = from; year <= to; year++) {
        days += is_leap_year(year) ? 366 : 365;
    }
    return days;
}

TEST(leap_years) {
    ASSERT(is_leap_year(2024));
    ASSERT(is_leap_year(2000));
    ASSERT_FALSE(is_leap_year(1900));
    ASSERT_FALSE(is_leap_year(2023));
}

TEST(month_lengths) {
    ASSERT_EQ(days_in_month(1, 2023), 31);
    ASSERT_EQ(days_in_month(4, 2023), 30);
    ASSERT_EQ(days_in_month(2, 2023), 28);
    ASSERT_EQ(days_in_month(2, 2024), 29);
    ASSERT_EQ(days_in_month(12, 2023), 31);
}

TEST(invalid_months) {
    ASSERT_EQ(days_in_month(0, 2023), 0);
    ASSERT_EQ(days_in_month(13, 2023), 0);
}

TEST(year_ranges) {
    ASSERT_EQ(days_between_years(2023, 2023), 365);
    ASSERT_EQ(days_between_years(2023, 2024), 365 + 366);
    ASSERT_EQ(days_between_years(1900, 1999), 36524);
}
