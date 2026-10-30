// `next_ticket` should hand out ticket numbers 1, 2, 3, ... one per call.
// Make it remember the last number it handed out. Don't use a global
// variable!

#include "clings.h"

int next_ticket(void) {
    int last = 0;
    last = last + 1;
    return last;
}

TEST(tickets_count_up) {
    ASSERT_EQ(next_ticket(), 1);
    ASSERT_EQ(next_ticket(), 2);
    ASSERT_EQ(next_ticket(), 3);
}
