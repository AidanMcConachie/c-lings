#include <stdlib.h>

#include "clings.h"

typedef struct {
    int *data;
    size_t len; // number of elements in use
    size_t cap; // number of elements allocated
} IntVec;

bool vec_push(IntVec *v, int value) {
    if (v->len == v->cap) {
        size_t new_cap = v->cap ? v->cap * 2 : 4;
        // realloc(NULL, n) behaves like malloc(n). Assign to a temporary:
        // if realloc fails, v->data is still valid and must not be lost.
        int *new_data = realloc(v->data, new_cap * sizeof *new_data);
        if (new_data == NULL) {
            return false;
        }
        v->data = new_data;
        v->cap = new_cap;
    }
    v->data[v->len++] = value;
    return true;
}

void vec_free(IntVec *v) {
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
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
