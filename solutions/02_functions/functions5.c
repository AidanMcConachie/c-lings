#include "clings.h"

int next_ticket(void) {
    // A static local is initialized once and keeps its value between calls.
    static int last = 0;
    last = last + 1;
    return last;
}

TEST(tickets_count_up) {
    ASSERT_EQ(next_ticket(), 1);
    ASSERT_EQ(next_ticket(), 2);
    ASSERT_EQ(next_ticket(), 3);
}
