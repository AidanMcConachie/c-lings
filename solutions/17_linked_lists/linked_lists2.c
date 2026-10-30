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

bool push_back(Node **head, int value) {
    // `link` points at the pointer we may need to change: first the head,
    // then each node's `next`. Walk until it points at a NULL pointer.
    Node **link = head;
    while (*link != NULL) {
        link = &(*link)->next;
    }
    Node *n = malloc(sizeof *n);
    if (n == NULL) {
        return false;
    }
    n->value = value;
    n->next = NULL;
    *link = n;
    return true;
}

bool remove_value(Node **head, int value) {
    // No special case for the head: it's just the first link.
    for (Node **link = head; *link != NULL; link = &(*link)->next) {
        if ((*link)->value == value) {
            Node *doomed = *link;
            *link = doomed->next;
            free(doomed);
            return true;
        }
    }
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
