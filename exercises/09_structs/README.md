# Structs

A struct groups several values (its *members*) into one type:

```c
struct Point {
    int x;
    int y;
};

struct Point p = {.x = 1, .y = 2}; // designated initializer
p.x = 5;                           // member access
```

`typedef struct { ... } Point;` gives the type a shorter name, so you can
write `Point` instead of `struct Point`.

Structs are copied when assigned or passed to a function, just like ints.
To let a function modify a struct, pass a pointer; `p->x` is shorthand for
`(*p).x`.

## Further information

- [cppreference: Struct declaration](https://en.cppreference.com/w/c/language/struct)
- [cppreference: Struct and union initialization](https://en.cppreference.com/w/c/language/struct_initialization)
- [cppreference: typedef](https://en.cppreference.com/w/c/language/typedef)
