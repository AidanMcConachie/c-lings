# Loops

C has three loops:

```c
for (int i = 0; i < 10; i++) { ... }  // init; condition; step
while (condition) { ... }             // checks first
do { ... } while (condition);         // runs at least once
```

`break` leaves the innermost loop and `continue` skips to its next
iteration.

Off-by-one errors (`<` vs `<=`, starting at 0 vs 1) are the most common loop
bug. When writing a loop, check it by hand against the smallest inputs!

## Further information

- [cppreference: for](https://en.cppreference.com/w/c/language/for)
- [cppreference: while](https://en.cppreference.com/w/c/language/while)
- [cppreference: do-while](https://en.cppreference.com/w/c/language/do)
