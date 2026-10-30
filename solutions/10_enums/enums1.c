#include "clings.h"

enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
};

// The values are 0 to 3 in clockwise order, so turning is just arithmetic.
enum Direction turn_right(enum Direction d) {
    return (enum Direction)((d + 1) % 4);
}

enum Direction opposite(enum Direction d) {
    return (enum Direction)((d + 2) % 4);
}

TEST(values) {
    ASSERT_EQ(NORTH, 0);
    ASSERT_EQ(WEST, 3);
}

TEST(turning_right) {
    ASSERT_EQ(turn_right(NORTH), EAST);
    ASSERT_EQ(turn_right(EAST), SOUTH);
    ASSERT_EQ(turn_right(WEST), NORTH);
}

TEST(opposites) {
    ASSERT_EQ(opposite(NORTH), SOUTH);
    ASSERT_EQ(opposite(EAST), WEST);
    ASSERT_EQ(opposite(WEST), EAST);
}
