// Two more pointer ideas:
//
// 1. A pointer to a local variable dangles once the function returns: the
//    variable doesn't exist anymore! Fix `get_counter` so the pointer it
//    returns stays valid. (Without malloc, we'll get to that later.)
//
// 2. A pointer to a pointer lets a function change *which* address the
//    caller's pointer holds. Implement `skip_spaces`.

#include "clings.h"

int *get_counter(void) {
    int counter = 0;
    return &counter;
}

// Moves *s forward past any leading spaces.
void skip_spaces(const char **s) {
    // TODO
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
