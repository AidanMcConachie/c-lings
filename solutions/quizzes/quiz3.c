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

SbStatus sb_init(StrBuf *sb) {
    sb->data = malloc(16);
    if (sb->data == NULL) {
        return SB_NO_MEMORY;
    }
    sb->data[0] = '\0';
    sb->len = 0;
    sb->cap = 16;
    return SB_OK;
}

// Makes sure there's room for `extra` more characters plus the '\0'.
static SbStatus sb_reserve(StrBuf *sb, size_t extra) {
    size_t needed = sb->len + extra + 1;
    if (needed <= sb->cap) {
        return SB_OK;
    }
    size_t new_cap = sb->cap ? sb->cap : 16;
    while (new_cap < needed) {
        new_cap *= 2;
    }
    char *p = realloc(sb->data, new_cap);
    if (p == NULL) {
        return SB_NO_MEMORY;
    }
    sb->data = p;
    sb->cap = new_cap;
    return SB_OK;
}

SbStatus sb_append(StrBuf *sb, const char *s) {
    size_t n = strlen(s);
    if (sb_reserve(sb, n) != SB_OK) {
        return SB_NO_MEMORY;
    }
    memcpy(sb->data + sb->len, s, n + 1);
    sb->len += n;
    return SB_OK;
}

SbStatus sb_append_char(StrBuf *sb, char c) {
    char s[2] = {c, '\0'};
    return sb_append(sb, s);
}

char *sb_take(StrBuf *sb) {
    char *s = sb->data;
    sb->data = NULL;
    sb->len = 0;
    sb->cap = 0;
    return s;
}

void sb_free(StrBuf *sb) {
    free(sb->data); // free(NULL) is allowed and does nothing
    sb->data = NULL;
    sb->len = 0;
    sb->cap = 0;
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
