// Quiz 3: structs, enums and memory.
//
// Build a tiny string builder: a heap buffer that grows as you append text.
// `data` must always be a valid NUL-terminated string once the builder has
// been initialized.

#include <stdlib.h>
#include <string.h>

#include "clings.h"

typedef enum {
    SB_OK,
    SB_NO_MEMORY,
} SbStatus;

typedef struct {
    char *data;
    size_t len; // characters in use, not counting the '\0'
    size_t cap; // bytes allocated
} StrBuf;

// Allocates an initial 16-byte buffer holding the empty string.
SbStatus sb_init(StrBuf *sb) {
    // TODO
    return SB_NO_MEMORY;
}

// Appends `s`, growing the buffer (at least doubling it) when needed.
SbStatus sb_append(StrBuf *sb, const char *s) {
    // TODO
    return SB_NO_MEMORY;
}

// Appends a single character.
SbStatus sb_append_char(StrBuf *sb, char c) {
    // TODO
    return SB_NO_MEMORY;
}

// Hands the string over to the caller (who must free it) and leaves the
// builder empty, with data == NULL.
char *sb_take(StrBuf *sb) {
    // TODO
    return NULL;
}

void sb_free(StrBuf *sb) {
    // TODO
}

TEST(starts_empty) {
    StrBuf sb;
    ASSERT_EQ(sb_init(&sb), SB_OK);
    ASSERT_STR_EQ(sb.data, "");
    ASSERT_EQ(sb.len, 0);
    ASSERT_EQ(sb.cap, 16);
    sb_free(&sb);
}

TEST(appends) {
    StrBuf sb;
    ASSERT_EQ(sb_init(&sb), SB_OK);
    ASSERT_EQ(sb_append(&sb, "Hello"), SB_OK);
    ASSERT_EQ(sb_append_char(&sb, ','), SB_OK);
    ASSERT_EQ(sb_append(&sb, " world"), SB_OK);
    ASSERT_STR_EQ(sb.data, "Hello, world");
    ASSERT_EQ(sb.len, 12);
    sb_free(&sb);
}

TEST(grows) {
    StrBuf sb;
    ASSERT_EQ(sb_init(&sb), SB_OK);
    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(sb_append(&sb, "abc"), SB_OK);
    }
    ASSERT_EQ(sb.len, 300);
    ASSERT(sb.cap > 300);
    ASSERT_EQ(strlen(sb.data), 300);
    sb_free(&sb);
}

TEST(take_ownership) {
    StrBuf sb;
    ASSERT_EQ(sb_init(&sb), SB_OK);
    sb_append(&sb, "mine now");
    char *s = sb_take(&sb);
    ASSERT_STR_EQ(s, "mine now");
    ASSERT_NULL(sb.data);
    ASSERT_EQ(sb.len, 0);
    sb_free(&sb); // must be safe to call after sb_take
    free(s);
}
