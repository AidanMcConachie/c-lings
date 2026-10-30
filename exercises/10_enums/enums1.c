// Add the missing direction, then implement the functions.

#include "clings.h"

enum Direction {
    NORTH,
    EAST,
    SOUTH,
};

// Turning right from NORTH gives EAST, from WEST gives NORTH, etc.
enum Direction turn_right(enum Direction d) {
    // TODO
    return d;
}

enum Direction opposite(enum Direction d) {
    // TODO
    return d;
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
