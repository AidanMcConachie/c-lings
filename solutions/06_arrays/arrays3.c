#include "clings.h"

int trace(int m[3][3]) {
    int total = 0;
    for (int i = 0; i < 3; i++) {
        total += m[i][i];
    }
    return total;
}

void transpose(int m[3][3]) {
    // Only swap the elements above the diagonal with those below it,
    // otherwise everything gets swapped twice.
    for (int row = 0; row < 3; row++) {
        for (int col = row + 1; col < 3; col++) {
            int tmp = m[row][col];
            m[row][col] = m[col][row];
            m[col][row] = tmp;
        }
    }
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
