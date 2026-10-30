# Memory

Local variables live on the *stack* and disappear when their function
returns. When you need memory that outlives a function, or whose size you
only know at runtime, you allocate it on the *heap*:

```c
int *numbers = malloc(count * sizeof *numbers); // uninitialized
int *zeros = calloc(count, sizeof *zeros);      // zero-filled
numbers = realloc(numbers, 2 * count * sizeof *numbers); // resize
free(numbers);                                  // give it back
```

You're responsible for calling `free` exactly once on every block:

- Forgetting to free is a *memory leak*.
- Using a block after freeing it is a *use-after-free*.
- Freeing it twice is a *double free*.
- Writing past the end of it is a *heap buffer overflow*.

All of these except leaks are undefined behavior. clings.h tracks your
allocations during tests and fails the test on leaks and (most) overflows.
For real projects, use `-fsanitize=address` or valgrind!

malloc returns `NULL` if it can't allocate memory; real code checks for that.

## Further information

- [cppreference: malloc](https://en.cppreference.com/w/c/memory/malloc)
- [cppreference: realloc](https://en.cppreference.com/w/c/memory/realloc)
- [cppreference: free](https://en.cppreference.com/w/c/memory/free)
