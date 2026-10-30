# Types

C has several integer types (`char`, `short`, `int`, `long`, `long long`,
each `signed` or `unsigned`) and floating point types (`float`, `double`).
Their sizes depend on the platform, so `<stdint.h>` provides fixed-width
types like `int32_t` and `uint8_t` when you need exact sizes.

Some things that surprise newcomers:

- Dividing two integers gives an integer: `7 / 2 == 3`.
- Unsigned integers wrap around: `(uint8_t)255 + 1` is `0`.
  (Signed overflow is *undefined behavior*: don't do it!)
- `char` is just a small integer. `'A'` is the number 65.
- Floating point numbers are approximations, `0.1 + 0.2 != 0.3`.
- `bool`, `true` and `false` come from `<stdbool.h>`.

`sizeof(x)` tells you how many bytes `x` (or a type) takes up.

## Further information

- [cppreference: Arithmetic types](https://en.cppreference.com/w/c/language/arithmetic_types)
- [cppreference: Fixed width integer types](https://en.cppreference.com/w/c/types/integer)
- [What Every Programmer Should Know About Floating-Point](https://floating-point-gui.de/)
