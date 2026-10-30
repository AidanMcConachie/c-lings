# Functions

A C function has a return type, a name, a parameter list and a body:

```c
int add(int a, int b) {
    return a + b;
}
```

`void` as the return type means "returns nothing"; `(void)` as the parameter
list means "takes no arguments".

C compiles a file from top to bottom, so a function must be declared before
it's called. If you want to define it later, add a *prototype* (the function
header followed by `;`) before the first call.

Arguments are passed *by value*: the function gets a copy, so assigning to a
parameter doesn't change the caller's variable.

A `static` local variable keeps its value between calls.

## Further information

- [cppreference: Function definitions](https://en.cppreference.com/w/c/language/function_definition)
- [cppreference: Function declarations](https://en.cppreference.com/w/c/language/function_declaration)
- [cppreference: Storage duration (static)](https://en.cppreference.com/w/c/language/storage_duration)
