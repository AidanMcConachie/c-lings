// A dispatch table maps names (here: operator characters) to functions.
// The calculator only knows + and -. Teach it * and / as well.
// Division by zero must fail (return false) instead of crashing.

#include <stdbool.h>

#include "clings.h"

typedef bool (*BinOp)(int a, int b, int *result);

static bool add(int a, int b, int *result) {
    *result = a + b;
    return true;
}

static bool subtract(int a, int b, int *result) {
    *result = a - b;
    return true;
}

static const struct {
    char symbol;
    BinOp fn;
} operations[] = {
    {'+', add},
    {'-', subtract},
};

// Looks up `op` in the table and applies it. Returns false for unknown
// operators or if the operation fails.
bool calculate(char op, int a, int b, int *result) {
    for (size_t i = 0; i < sizeof operations / sizeof operations[0]; i++) {
        if (operations[i].symbol == op) {
            return operations[i].fn(a, b, result);
        }
    }
    return false;
}

TEST(add_and_subtract) {
    int r = 0;
    ASSERT(calculate('+', 2, 3, &r));
    ASSERT_EQ(r, 5);
    ASSERT(calculate('-', 2, 3, &r));
    ASSERT_EQ(r, -1);
}

TEST(multiply_and_divide) {
    int r = 0;
    ASSERT(calculate('*', 6, 7, &r));
    ASSERT_EQ(r, 42);
    ASSERT(calculate('/', 42, 5, &r));
    ASSERT_EQ(r, 8);
}

TEST(errors) {
    int r = 0;
    ASSERT_FALSE(calculate('/', 1, 0, &r));
    ASSERT_FALSE(calculate('^', 2, 3, &r));
}
