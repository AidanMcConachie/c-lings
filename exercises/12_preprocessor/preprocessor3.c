// "X macros" keep several lists in sync from a single definition.
// COLOR_LIST is expanded once to build the enum. It's also supposed to
// build the `color_names` table, but someone wrote that table by hand and
// it's already out of date.
//
// Generate `color_names` from COLOR_LIST too, so it can't go stale again.

#include "clings.h"

#define COLOR_LIST \
    X(RED)         \
    X(GREEN)       \
    X(BLUE)        \
    X(YELLOW)

enum Color {
#define X(name) COLOR_##name,
    COLOR_LIST
#undef X
    COLOR_COUNT
};

static const char *color_names[] = {
    "RED",
    "GREEN",
    "BLUE",
};

const char *color_name(enum Color c) {
    if (c >= sizeof color_names / sizeof color_names[0]) {
        return NULL;
    }
    return color_names[c];
}

TEST(enum_values) {
    ASSERT_EQ(COLOR_RED, 0);
    ASSERT_EQ(COLOR_YELLOW, 3);
    ASSERT_EQ(COLOR_COUNT, 4);
}

TEST(names) {
    ASSERT_STR_EQ(color_name(COLOR_RED), "RED");
    ASSERT_STR_EQ(color_name(COLOR_BLUE), "BLUE");
    ASSERT_STR_EQ(color_name(COLOR_YELLOW), "YELLOW");
}

TEST(one_name_per_color) {
    ASSERT_EQ(sizeof color_names / sizeof color_names[0], COLOR_COUNT);
}
