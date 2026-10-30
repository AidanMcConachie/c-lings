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

// Same list, different X: this time each entry becomes a string.
static const char *color_names[] = {
#define X(name) #name,
    COLOR_LIST
#undef X
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
