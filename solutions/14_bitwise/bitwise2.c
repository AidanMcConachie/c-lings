#include <stdbool.h>

#include "clings.h"

// Writing flags as shifts makes it obvious each one has its own bit.
enum Permission {
    PERM_READ = 1 << 0,
    PERM_WRITE = 1 << 1,
    PERM_EXEC = 1 << 2,
};

bool has_permission(unsigned perms, enum Permission p) {
    // && is a logical AND of "is it non-zero?", & combines the bits.
    return (perms & p) != 0;
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
