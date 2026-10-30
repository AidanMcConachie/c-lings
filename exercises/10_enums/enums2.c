// A new kind of shape was added, but `shape_area` wasn't updated. Luckily
// the compiler noticed! Make it handle every kind of shape.

#include "clings.h"

typedef enum {
    SHAPE_CIRCLE,
    SHAPE_RECTANGLE,
    SHAPE_TRIANGLE,
} ShapeKind;

typedef struct {
    ShapeKind kind;
    double a; // radius, width or base
    double b; // height (unused for circles)
} Shape;

double shape_area(Shape s) {
    switch (s.kind) {
    case SHAPE_CIRCLE:
        return 3.14159265358979 * s.a * s.a;
    case SHAPE_RECTANGLE:
        return s.a * s.b;
    }
    return 0;
}

TEST(circle) {
    Shape s = {.kind = SHAPE_CIRCLE, .a = 2};
    ASSERT_NEAR(shape_area(s), 12.566370614, 1e-6);
}

TEST(rectangle) {
    Shape s = {.kind = SHAPE_RECTANGLE, .a = 3, .b = 4};
    ASSERT_NEAR(shape_area(s), 12, 1e-9);
}

TEST(triangle) {
    Shape s = {.kind = SHAPE_TRIANGLE, .a = 3, .b = 4};
    ASSERT_NEAR(shape_area(s), 6, 1e-9);
}
