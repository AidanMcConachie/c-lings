#include <stdlib.h>

#include "clings.h"

typedef struct {
    char *data;
    size_t size;
} Buffer;

Buffer *buffer_create(size_t size) {
    Buffer *b = malloc(sizeof *b);
    if (b == NULL) {
        return NULL;
    }
    b->data = calloc(size, 1);
    if (b->data == NULL) {
        free(b);
        return NULL;
    }
    b->size = size;
    return b;
}

void buffer_destroy(Buffer **b) {
    if (*b == NULL) {
        return;
    }
    // Free what the Buffer owns first: after free(*b), reading (*b)->data
    // would be a use-after-free.
    free((*b)->data);
    free(*b);
    *b = NULL;
}

TEST(create_and_destroy) {
    Buffer *b = buffer_create(64);
    ASSERT_NOT_NULL(b);
    ASSERT_EQ(b->size, 64);
    ASSERT_EQ(b->data[63], 0);
    buffer_destroy(&b);
    ASSERT_NULL(b);
}

TEST(destroying_twice_is_harmless) {
    Buffer *b = buffer_create(8);
    buffer_destroy(&b);
    buffer_destroy(&b); // b is NULL now, so this must do nothing
    ASSERT_NULL(b);
}
