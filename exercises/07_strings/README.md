# Strings

C has no string type. A string is an array of `char` that ends with a NUL
character (`'\0'`, the byte with value 0). Every function that works with
strings relies on that terminator to know where the string ends, so a
string of 5 letters needs an array of at least 6 chars.

`"hello"` is a *string literal*: a read-only array of 6 chars. You can use it
to initialize an array you *can* modify: `char s[] = "hello";`.

`<string.h>` has the classics: `strlen`, `strcmp`, `strcpy`, `strncpy`,
`strchr`, ... and `<stdio.h>` has `snprintf`, which is the safest way to
build a string into a buffer.

`==` on two strings compares their *addresses*, not their contents. Use
`strcmp(a, b) == 0` instead.

## Further information

- [cppreference: Null-terminated byte strings](https://en.cppreference.com/w/c/string/byte)
- [cppreference: String literals](https://en.cppreference.com/w/c/language/string_literal)
- [cppreference: snprintf](https://en.cppreference.com/w/c/io/fprintf)
