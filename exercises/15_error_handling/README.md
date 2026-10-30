# Error handling

C has no exceptions. Functions report errors through their return value,
and it's the caller's job to check it:

- Return a status code (`0`/`-1`, `bool`, or an `enum` of error codes) and
  hand back results through pointer "out parameters".
- Return a pointer that is `NULL` on failure (like `malloc` and `fopen`).
- Many standard library functions also set the global `errno` (from
  `<errno.h>`) to say *what* went wrong.

When a function acquires several resources, a common pattern is a single
cleanup section at the end that every error path jumps to with `goto`. It's
one of the few good uses of `goto`!

## Further information

- [cppreference: errno](https://en.cppreference.com/w/c/error/errno)
- [cppreference: strtol](https://en.cppreference.com/w/c/string/byte/strtol)
- [cppreference: goto](https://en.cppreference.com/w/c/language/goto)
