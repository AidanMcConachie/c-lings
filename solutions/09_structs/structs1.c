#include "clings.h"

struct Point {
    int x;
    int y;
};

struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

TEST(points) {
    struct Point p = {.x = 3, .y = -4};
    ASSERT_EQ(p.x, 3);
    ASSERT_EQ(p.y, -4);
    p.x = 10;
    ASSERT_EQ(p.x, 10);
}

TEST(colors) {
    struct Color green = {.red = 0, .green = 255, .blue = 0};
    ASSERT_EQ(green.red, 0);
    ASSERT_EQ(green.green, 255);
    ASSERT_EQ(green.blue, 0);
    ASSERT_EQ(sizeof(struct Color), 3); // pick the smallest fitting type!
}

TEST(copies) {
    struct Point a = {.x = 1, .y = 2};
    struct Point b = a; // structs are copied by assignment
    b.x = 100;
    ASSERT_EQ(a.x, 1);
    ASSERT_EQ(b.x, 100);
}
