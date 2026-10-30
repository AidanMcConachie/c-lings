// A growable array of ints, like a mini std::vector / Vec. When it runs out
// of room, it grows its buffer with realloc.
//
// Implement `vec_push` and `vec_free`.

#include <stdlib.h>

#include "clings.h"

typedef struct {
    int *data;
    size_t len; // number of elements in use
    size_t cap; // number of elements allocated
} IntVec;

// Appends `value`. Grows the buffer (doubling its capacity, starting at 4)
// when it's full. Returns false if memory couldn't be allocated.
bool vec_push(IntVec *v, int value) {
    // TODO
    return false;
}

// Frees the buffer and resets the vector to empty.
void vec_free(IntVec *v) {
    // TODO
}

TEST(push_a_few) {
    IntVec v = {0};
    ASSERT(vec_push(&v, 10));
    ASSERT(vec_push(&v, 20));
    ASSERT(vec_push(&v, 30));
    ASSERT_EQ(v.len, 3);
    ASSERT_EQ(v.cap, 4);
    ASSERT_EQ(v.data[0], 10);
    ASSERT_EQ(v.data[2], 30);
    vec_free(&v);
    ASSERT_NULL(v.data);
    ASSERT_EQ(v.len, 0);
    ASSERT_EQ(v.cap, 0);
}

TEST(grows) {
    IntVec v = {0};
    for (int i = 0; i < 1000; i++) {
        ASSERT(vec_push(&v, i * i));
    }
    ASSERT_EQ(v.len, 1000);
    ASSERT_EQ(v.cap, 1024);
    ASSERT_EQ(v.data[999], 999 * 999);
    vec_free(&v);
}
