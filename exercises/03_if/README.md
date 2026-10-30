# If

`if (condition) { ... } else { ... }` runs one of two blocks. In C, any
non-zero value counts as true and zero counts as false.

The *ternary operator* `cond ? a : b` is an expression that evaluates to `a`
or `b`.

`switch` jumps to the `case` label that matches a value. Execution then
continues ("falls through") into the next case unless you `break` (or
`return`). Forgetting a `break` is a classic C bug, which is why `-Wextra`
warns about it.

## Further information

- [cppreference: if](https://en.cppreference.com/w/c/language/if)
- [cppreference: switch](https://en.cppreference.com/w/c/language/switch)
- [cppreference: Conditional operator](https://en.cppreference.com/w/c/language/operator_other#Conditional_operator)
