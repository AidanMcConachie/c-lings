#include "clings.h"

#define STRINGIFY(x) #x
#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define DEFINE_GETTER(name) \
    int get_##name(void) {  \
        return name;        \
    }

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
