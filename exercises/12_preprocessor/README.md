# Preprocessor

Before compiling, C runs the *preprocessor*, which works on text:

- `#include "file.h"` pastes in another file.
- `#define NAME value` replaces NAME with value everywhere after it.
- `#define SQUARE(x) ((x) * (x))` defines a function-like macro. Its
  arguments are substituted as text, which is why macros need lots of
  parentheses.
- `#if`, `#ifdef`, `#ifndef`, `#else` and `#endif` include or skip code.
- `#x` turns a macro argument into a string literal, and `a##b` glues two
  tokens together into one.

Macros are powerful but easy to get wrong. Prefer functions and `const`
variables when they can do the job.

## Further information

- [cppreference: Preprocessor](https://en.cppreference.com/w/c/preprocessor)
- [cppreference: Replacing text macros](https://en.cppreference.com/w/c/preprocessor/replace)
- [Wikipedia: X macro](https://en.wikipedia.org/wiki/X_macro)
