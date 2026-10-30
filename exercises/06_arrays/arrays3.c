// A 2D array is an array of arrays: `int grid[3][3]` is 3 rows of 3 ints,
// and `grid[row][col]` picks one element.
//
// Implement the two functions below.

#include "clings.h"

// The trace of a square matrix is the sum of its diagonal:
// m[0][0] + m[1][1] + m[2][2].
int trace(int m[3][3]) {
    // TODO
    return 0;
}

// Transposing flips the matrix over its diagonal: rows become columns.
// Modify `m` in place.
void transpose(int m[3][3]) {
    // TODO
}

TEST(trace_of_matrix) {
    int m[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    ASSERT_EQ(trace(m), 15);
}

TEST(transpose_matrix) {
    int m[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    transpose(m);
    ASSERT_EQ(m[0][1], 4);
    ASSERT_EQ(m[1][0], 2);
    ASSERT_EQ(m[2][0], 3);
    ASSERT_EQ(m[0][2], 7);
    ASSERT_EQ(m[1][2], 8);
    ASSERT_EQ(m[1][1], 5);
}
