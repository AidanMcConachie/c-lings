#include <stdbool.h>
#include <stdlib.h>

#include "clings.h"

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *list_reverse(Node *head) {
    Node *prev = NULL;
    while (head != NULL) {
        Node *next = head->next;
        head->next = prev; // point backwards
        prev = head;
        head = next;
    }
    return prev;
}

// Floyd's "tortoise and hare": the fast pointer moves two steps for every
// step of the slow one. If there's a cycle, fast eventually laps slow.
bool list_has_cycle(const Node *head) {
    const Node *slow = head, *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true;
        }
    }
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
