// The preprocessor can turn code into strings with `#`, and glue tokens
// together with `##`. Define the three macros so the tests pass.

#include "clings.h"

// STRINGIFY(hello) => "hello"
// TODO: #define STRINGIFY(x) ...

// ARRAY_LEN(arr) => the number of elements in an array
// TODO: #define ARRAY_LEN(arr) ...

// DEFINE_GETTER(name) defines a function `int get_<name>(void)` that
// returns the variable `name`.
// TODO: #define DEFINE_GETTER(name) ...

static int width = 640;
static int height = 480;

DEFINE_GETTER(width)
DEFINE_GETTER(height)

TEST(stringify) {
    ASSERT_STR_EQ(STRINGIFY(hello), "hello");
    ASSERT_STR_EQ(STRINGIFY(1 + 2), "1 + 2");
}

TEST(array_len) {
    int a[7];
    double b[] = {1.0, 2.0, 3.0};
    ASSERT_EQ(ARRAY_LEN(a), 7);
    ASSERT_EQ(ARRAY_LEN(b), 3);
}

TEST(getters) {
    ASSERT_EQ(get_width(), 640);
    ASSERT_EQ(get_height(), 480);
}
