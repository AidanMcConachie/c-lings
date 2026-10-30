#include "clings.h"

int *get_counter(void) {
    // A static local lives for the whole program, so its address stays
    // valid after the function returns.
    static int counter = 0;
    return &counter;
}

void skip_spaces(const char **s) {
    // *s is the caller's pointer; we move it forward.
    while (**s == ' ') {
        (*s)++;
    }
}

TEST(counter_survives) {
    int *c = get_counter();
    ASSERT_NOT_NULL(c);
    *c += 5;
    ASSERT_EQ(*get_counter(), 5); // same counter every time
}

TEST(skips_spaces) {
    const char *text = "   hello";
    const char *p = text;
    skip_spaces(&p);
    ASSERT_PTR_EQ(p, text + 3);
    ASSERT_STR_EQ(p, "hello");
}

TEST(nothing_to_skip) {
    const char *text = "hi there";
    const char *p = text;
    skip_spaces(&p);
    ASSERT_PTR_EQ(p, text);
}
