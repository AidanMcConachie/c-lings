# Arrays

An array is a fixed number of elements of the same type, stored next to
each other in memory: `int scores[5] = {90, 85, 70, 60, 100};`. Indexing
starts at 0, so the last element is `scores[4]`.

C does **not** check array bounds. Reading or writing past the end is
undefined behavior: it might crash, or silently corrupt other data.

When you pass an array to a function it *decays* into a pointer to its
first element, and the function has no idea how long the array is. That's
why C functions taking arrays almost always take a length too.

`sizeof(arr) / sizeof(arr[0])` gives the number of elements, but only where
`arr` is still an array (not a pointer!).

## Further information

- [cppreference: Array declaration](https://en.cppreference.com/w/c/language/array)
- [cppreference: Array to pointer conversion](https://en.cppreference.com/w/c/language/conversion#Array_to_pointer_conversion)
