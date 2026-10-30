// Ownership: a Buffer owns its `data` block, so destroying a Buffer must
// free both. And after it's destroyed, the caller's pointer should be set
// to NULL so it can't be used (or freed!) again by accident.
//
// Fix `buffer_destroy`.

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

void buffer_destroy(Buffer *b) {
    free(b);
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
