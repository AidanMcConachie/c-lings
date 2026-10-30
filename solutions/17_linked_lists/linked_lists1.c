#include <stdlib.h>

#include "clings.h"

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *push_front(Node *head, int value) {
    Node *n = malloc(sizeof *n);
    if (n == NULL) {
        return NULL;
    }
    n->value = value;
    n->next = head;
    return n;
}

size_t list_len(const Node *head) {
    size_t len = 0;
    for (const Node *n = head; n != NULL; n = n->next) {
        len++;
    }
    return len;
}

int list_sum(const Node *head) {
    int sum = 0;
    for (const Node *n = head; n != NULL; n = n->next) {
        sum += n->value;
    }
    return sum;
}

void list_free(Node *head) {
    while (head != NULL) {
        // Grab `next` before freeing: afterwards head->next is off limits.
        Node *next = head->next;
        free(head);
        head = next;
    }
}

TEST(empty_list) {
    ASSERT_EQ(list_len(NULL), 0);
    ASSERT_EQ(list_sum(NULL), 0);
    list_free(NULL);
}

TEST(build_and_walk) {
    Node *list = NULL;
    for (int i = 1; i <= 4; i++) {
        list = push_front(list, i);
    }
    // the list is now 4 -> 3 -> 2 -> 1
    ASSERT_EQ(list->value, 4);
    ASSERT_EQ(list_len(list), 4);
    ASSERT_EQ(list_sum(list), 10);
    list_free(list);
}
