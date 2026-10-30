// Two classic list puzzles:
//
// 1. Reverse a list in place, by re-pointing the `next` pointers (no new
//    nodes, no arrays!).
// 2. Detect whether a list has a cycle: a node whose `next` points back to
//    an earlier node, so walking the list never ends. Do it without
//    modifying the list and without extra memory.

#include <stdbool.h>
#include <stdlib.h>

#include "clings.h"

typedef struct Node {
    int value;
    struct Node *next;
} Node;

// Returns the new head.
Node *list_reverse(Node *head) {
    // TODO
    return head;
}

bool list_has_cycle(const Node *head) {
    // TODO
    return false;
}

TEST(reverse) {
    Node c = {3, NULL};
    Node b = {2, &c};
    Node a = {1, &b};
    Node *head = list_reverse(&a);
    ASSERT_PTR_EQ(head, &c);
    ASSERT_PTR_EQ(c.next, &b);
    ASSERT_PTR_EQ(b.next, &a);
    ASSERT_NULL(a.next);
}

TEST(reverse_tiny_lists) {
    ASSERT_NULL(list_reverse(NULL));
    Node only = {1, NULL};
    ASSERT_PTR_EQ(list_reverse(&only), &only);
    ASSERT_NULL(only.next);
}

TEST(no_cycle) {
    Node c = {3, NULL};
    Node b = {2, &c};
    Node a = {1, &b};
    ASSERT_FALSE(list_has_cycle(&a));
    ASSERT_FALSE(list_has_cycle(NULL));
}

TEST(cycles) {
    Node d = {4, NULL};
    Node c = {3, &d};
    Node b = {2, &c};
    Node a = {1, &b};
    d.next = &b; // 1 -> 2 -> 3 -> 4 -> back to 2 ...
    ASSERT(list_has_cycle(&a));

    Node self = {0, NULL};
    self.next = &self;
    ASSERT(list_has_cycle(&self));
}
