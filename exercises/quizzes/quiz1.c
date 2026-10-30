// Quiz 1: variables, functions, if, types and loops.
//
// Implement the functions below so the tests pass.
//
// A year is a leap year if it's divisible by 4, except years divisible by
// 100, which are only leap years if they're also divisible by 400.
// (So 2024 and 2000 are leap years, but 1900 isn't.)

#include <stdbool.h>

#include "clings.h"

bool is_leap_year(int year) {
    // TODO
    return false;
}

// Months are numbered 1 (January) to 12 (December). February has 29 days in
// leap years. Return 0 for an invalid month.
int days_in_month(int month, int year) {
    // TODO
    return 0;
}

// Total number of days in all the years from `from` to `to`, inclusive.
int days_between_years(int from, int to) {
    // TODO
    return 0;
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
