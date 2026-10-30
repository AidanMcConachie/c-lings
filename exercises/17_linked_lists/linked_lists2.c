// Pointers to pointers make list surgery easier: with `Node **link` you
// can modify the head pointer and any node's `next` pointer the same way.
//
// Implement `push_back` and `remove_value`.

#include <stdbool.h>
#include <stdlib.h>

#include "clings.h"

typedef struct Node {
    int value;
    struct Node *next;
} Node;

void list_free(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

// Appends a value at the end of the list. Returns false if out of memory.
bool push_back(Node **head, int value) {
    // TODO
    return false;
}

// Removes (and frees) the first node holding `value`. Returns whether a
// node was removed.
bool remove_value(Node **head, int value) {
    // TODO
    return false;
}

TEST(push_back_builds_in_order) {
    Node *list = NULL;
    ASSERT(push_back(&list, 1));
    ASSERT(push_back(&list, 2));
    ASSERT(push_back(&list, 3));
    ASSERT_NOT_NULL(list);
    ASSERT_EQ(list->value, 1);
    ASSERT_EQ(list->next->value, 2);
    ASSERT_EQ(list->next->next->value, 3);
    ASSERT_NULL(list->next->next->next);
    list_free(list);
}

TEST(remove_from_middle_and_ends) {
    Node *list = NULL;
    for (int i = 1; i <= 5; i++) {
        push_back(&list, i);
    }
    ASSERT(remove_value(&list, 3)); // middle
    ASSERT(remove_value(&list, 1)); // head
    ASSERT(remove_value(&list, 5)); // tail
    ASSERT_FALSE(remove_value(&list, 42));
    ASSERT_EQ(list->value, 2);
    ASSERT_EQ(list->next->value, 4);
    ASSERT_NULL(list->next->next);
    list_free(list);
}

TEST(remove_last_node) {
    Node *list = NULL;
    push_back(&list, 7);
    ASSERT(remove_value(&list, 7));
    ASSERT_NULL(list);
    ASSERT_FALSE(remove_value(&list, 7));
}
