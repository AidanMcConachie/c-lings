# Variables

In C every variable has a type that is fixed when you declare it:
`int x = 5;` declares an `int` named `x` and initializes it to 5.

A variable is only visible inside the block (`{ ... }`) where it was
declared, from the point of declaration onwards. A declaration in an inner
block with the same name *shadows* the outer one.

`const` makes a variable read-only after initialization.

Note that local variables that aren't initialized hold garbage, not zero.
Reading them is undefined behavior, so always initialize your variables!

## Further information

- [cppreference: Declarations](https://en.cppreference.com/w/c/language/declarations)
- [cppreference: Scope](https://en.cppreference.com/w/c/language/scope)
- [cppreference: const](https://en.cppreference.com/w/c/language/const)
