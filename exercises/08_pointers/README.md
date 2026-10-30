# Pointers

A pointer holds the address of another object.

```c
int x = 10;
int *p = &x;  // p points to x ("&" = address of)
*p = 20;      // writes through the pointer ("*" = dereference): x is now 20
```

Pointers are how C functions modify their caller's variables, how arrays
are passed around, and how you build data structures.

- `NULL` is a pointer that points to nothing. Dereferencing it crashes.
- Adding 1 to a pointer moves it to the next *element*, not the next byte:
  `p + 1` is `&p[1]`. In fact `p[i]` is just `*(p + i)`.
- `const int *p` is a pointer to an int you can't modify through `p`.
- A pointer to a local variable is *dangling* once that function returns.

## Further information

- [cppreference: Pointer declaration](https://en.cppreference.com/w/c/language/pointer)
- [cppreference: Member access operators (*, &, [])](https://en.cppreference.com/w/c/language/operator_member_access)
- [Beej's Guide to C: Pointers](https://beej.us/guide/bgc/html/split/pointers.html)
