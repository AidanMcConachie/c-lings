// Bit flags pack several yes/no options into one integer. Each flag must
// have its own bit. There are two bugs here: find them!

#include <stdbool.h>

#include "clings.h"

enum Permission {
    PERM_READ = 1,
    PERM_WRITE = 2,
    PERM_EXEC = 3,
};

bool has_permission(unsigned perms, enum Permission p) {
    return perms && p;
}

unsigned grant(unsigned perms, enum Permission p) {
    return perms | p;
}

unsigned revoke(unsigned perms, enum Permission p) {
    return perms & ~p;
}

TEST(flags_dont_overlap) {
    ASSERT_EQ(PERM_READ & PERM_WRITE, 0);
    ASSERT_EQ(PERM_READ & PERM_EXEC, 0);
    ASSERT_EQ(PERM_WRITE & PERM_EXEC, 0);
}

TEST(checking) {
    unsigned perms = PERM_READ | PERM_WRITE;
    ASSERT(has_permission(perms, PERM_READ));
    ASSERT(has_permission(perms, PERM_WRITE));
    ASSERT_FALSE(has_permission(perms, PERM_EXEC));
}

TEST(granting_and_revoking) {
    unsigned perms = PERM_READ;
    perms = grant(perms, PERM_EXEC);
    ASSERT(has_permission(perms, PERM_EXEC));
    perms = revoke(perms, PERM_READ);
    ASSERT_FALSE(has_permission(perms, PERM_READ));
    ASSERT(has_permission(perms, PERM_EXEC));
}
