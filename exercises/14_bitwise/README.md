# Bitwise operations

C lets you work on the individual bits of an integer:

| Operator | Meaning     | Example (binary)        |
| -------- | ----------- | ----------------------- |
| `a & b`  | AND         | `1100 & 1010 == 1000`   |
| `a \| b` | OR          | `1100 \| 1010 == 1110`  |
| `a ^ b`  | XOR         | `1100 ^ 1010 == 0110`   |
| `~a`     | NOT         | `~1100 == ...0011`      |
| `a << n` | shift left  | `0011 << 2 == 1100`     |
| `a >> n` | shift right | `1100 >> 2 == 0011`     |

`1u << n` is a number with only bit n set, which is the building block for
setting (`x | mask`), clearing (`x & ~mask`), toggling (`x ^ mask`) and
testing (`x & mask`) bits.

Use unsigned types for bit manipulation: shifting into the sign bit of a
signed integer is undefined behavior.

Don't confuse `&` and `|` with the logical `&&` and `||`!

## Further information

- [cppreference: Bitwise operators](https://en.cppreference.com/w/c/language/operator_arithmetic#Bitwise_logic)
- [Bit Twiddling Hacks](https://graphics.stanford.edu/~seander/bithacks.html)
