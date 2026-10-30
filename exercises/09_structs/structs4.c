// Structs can contain other structs. Implement the functions below.

#include <stdbool.h>

#include "clings.h"

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point top_left;     // smallest x and y
    Point bottom_right; // biggest x and y
} Rect;

int rect_area(Rect r) {
    // TODO
    return 0;
}

// Returns a copy of `r` moved by dx and dy. `r` itself must not change.
Rect rect_translate(Rect r, int dx, int dy) {
    // TODO
    return r;
}

// Is `p` inside `r`? Points on the edges count as inside.
bool rect_contains(const Rect *r, Point p) {
    // TODO
    return false;
}

TEST(area) {
    Rect r = {.top_left = {0, 0}, .bottom_right = {4, 3}};
    ASSERT_EQ(rect_area(r), 12);
}

TEST(translate) {
    Rect r = {.top_left = {1, 1}, .bottom_right = {2, 2}};
    Rect moved = rect_translate(r, 10, -1);
    ASSERT_EQ(moved.top_left.x, 11);
    ASSERT_EQ(moved.top_left.y, 0);
    ASSERT_EQ(moved.bottom_right.x, 12);
    ASSERT_EQ(moved.bottom_right.y, 1);
    ASSERT_EQ(r.top_left.x, 1); // the original is unchanged
}

TEST(contains) {
    Rect r = {.top_left = {0, 0}, .bottom_right = {10, 5}};
    ASSERT(rect_contains(&r, (Point){5, 2}));
    ASSERT(rect_contains(&r, (Point){0, 0}));
    ASSERT(rect_contains(&r, (Point){10, 5}));
    ASSERT_FALSE(rect_contains(&r, (Point){11, 2}));
    ASSERT_FALSE(rect_contains(&r, (Point){5, -1}));
}
