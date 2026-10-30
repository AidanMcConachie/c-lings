# Linked lists

Time to build a data structure! A singly linked list is a chain of nodes,
each holding a value and a pointer to the next node. The last node's `next`
is `NULL`, and an empty list is just a `NULL` head pointer.

```c
typedef struct Node {
    int value;
    struct Node *next;
} Node;
```

(Inside the struct the typedef name `Node` doesn't exist yet, which is why
`next` is declared as `struct Node *`.)

Functions that may change the head of the list (inserting at the front,
removing the first node) either return the new head, or take a pointer to
the head pointer: `Node **head`.

## Further information

- [Wikipedia: Linked list](https://en.wikipedia.org/wiki/Linked_list)
- [Linus Torvalds on "good taste" and pointers to pointers](https://github.com/mkirchner/linked-list-good-taste)
