// `make_badge` allocates a few strings along the way. On some paths it
// forgets to free them, and clings notices the leaks.
//
// Fix it so every path frees what it allocated. A single `cleanup:` label
// at the end of the function that all paths `goto` makes this much easier.

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

// Returns a heap copy of `s` without leading and trailing spaces.
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

// Makes "LAST, First" from a first and last name, e.g. "  Ada " and
// "lovelace" become "LOVELACE, Ada". Both names must be non-empty after
// trimming. On success, *out is a new heap string the caller frees.
BadgeStatus make_badge(const char *first, const char *last, char **out) {
    char *f = dup_trimmed(first);
    if (f == NULL) return BADGE_NO_MEMORY;
    if (f[0] == '\0') return BADGE_EMPTY_NAME;

    char *l = dup_trimmed(last);
    if (l == NULL) return BADGE_NO_MEMORY;
    if (l[0] == '\0') return BADGE_EMPTY_NAME;
    for (char *p = l; *p; p++) *p = (char)toupper((unsigned char)*p);

    size_t size = strlen(f) + strlen(l) + 3;
    char *badge = malloc(size);
    if (badge == NULL) return BADGE_NO_MEMORY;
    snprintf(badge, size, "%s, %s", l, f);

    *out = badge;
    return BADGE_OK;
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
