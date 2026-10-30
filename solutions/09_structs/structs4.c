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
    int width = r.bottom_right.x - r.top_left.x;
    int height = r.bottom_right.y - r.top_left.y;
    return width * height;
}

Rect rect_translate(Rect r, int dx, int dy) {
    // `r` is already a copy, so we can modify and return it.
    r.top_left.x += dx;
    r.top_left.y += dy;
    r.bottom_right.x += dx;
    r.bottom_right.y += dy;
    return r;
}

bool rect_contains(const Rect *r, Point p) {
    return p.x >= r->top_left.x && p.x <= r->bottom_right.x &&
           p.y >= r->top_left.y && p.y <= r->bottom_right.y;
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
