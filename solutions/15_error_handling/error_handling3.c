#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clings.h"

typedef enum {
    BADGE_OK,
    BADGE_EMPTY_NAME,
    BADGE_NO_MEMORY,
} BadgeStatus;

static char *dup_trimmed(const char *s) {
    while (*s == ' ') s++;
    size_t len = strlen(s);
    while (len > 0 && s[len - 1] == ' ') len--;
    char *copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}

BadgeStatus make_badge(const char *first, const char *last, char **out) {
    // Start everything as NULL so the cleanup code can free it all
    // unconditionally (free(NULL) does nothing).
    BadgeStatus status = BADGE_OK;
    char *f = NULL, *l = NULL;

    f = dup_trimmed(first);
    if (f == NULL) { status = BADGE_NO_MEMORY; goto cleanup; }
    if (f[0] == '\0') { status = BADGE_EMPTY_NAME; goto cleanup; }

    l = dup_trimmed(last);
    if (l == NULL) { status = BADGE_NO_MEMORY; goto cleanup; }
    if (l[0] == '\0') { status = BADGE_EMPTY_NAME; goto cleanup; }
    for (char *p = l; *p; p++) *p = (char)toupper((unsigned char)*p);

    size_t size = strlen(f) + strlen(l) + 3;
    char *badge = malloc(size);
    if (badge == NULL) { status = BADGE_NO_MEMORY; goto cleanup; }
    snprintf(badge, size, "%s, %s", l, f);
    *out = badge; // the caller owns badge now; we must not free it

cleanup:
    free(f);
    free(l);
    return status;
}

TEST(makes_badge) {
    char *badge = NULL;
    ASSERT_EQ(make_badge("  Ada ", "lovelace", &badge), BADGE_OK);
    ASSERT_STR_EQ(badge, "LOVELACE, Ada");
    free(badge);
}

TEST(empty_first_name) {
    char *badge = NULL;
    ASSERT_EQ(make_badge("   ", "Hopper", &badge), BADGE_EMPTY_NAME);
    ASSERT_NULL(badge);
}

TEST(empty_last_name) {
    char *badge = NULL;
    ASSERT_EQ(make_badge("Grace", "", &badge), BADGE_EMPTY_NAME);
    ASSERT_NULL(badge);
}
