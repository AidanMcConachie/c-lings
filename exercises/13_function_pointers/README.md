# Function pointers

Functions have addresses too, and you can store them in variables:

```c
int twice(int x) { return 2 * x; }

int (*f)(int) = twice;  // f points to a function taking and returning int
int y = f(21);          // calls twice(21)
```

The declaration syntax is famously awkward, so a `typedef` helps:

```c
typedef int (*IntFn)(int);
IntFn f = twice;
```

Function pointers let you pass behavior around: callbacks, comparison
functions for `qsort`, tables of operations, and so on.

## Further information

- [cppreference: Pointers to functions](https://en.cppreference.com/w/c/language/pointer#Pointers_to_functions)
- [cppreference: qsort](https://en.cppreference.com/w/c/algorithm/qsort)
- [cdecl.org](https://cdecl.org/): translates C declarations to English
