# Enums

An enum defines a set of named integer constants:

```c
enum Direction { NORTH, EAST, SOUTH, WEST }; // 0, 1, 2, 3
```

Enums pair naturally with `switch`. With `-Wall`, the compiler warns when a
`switch` on an enum doesn't handle every value, which is a great way to
catch the places you forgot to update after adding a new one.

A *tagged union* combines an enum (the tag) with a `union` (whose members
all share the same memory) to make a value that can be one of several
types. Only the member that matches the tag is valid to read.

## Further information

- [cppreference: Enumerations](https://en.cppreference.com/w/c/language/enum)
- [cppreference: Union declaration](https://en.cppreference.com/w/c/language/union)
