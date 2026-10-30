// Quiz 4: the final quiz! Strings, pointers, structs, enums, function
// pointers and error handling, all together.
//
// Implement a Reverse Polish Notation calculator. In RPN the operator comes
// after its operands, so "3 4 +" means 3 + 4, and "5 1 2 + 4 * + 3 -" means
// 5 + (1 + 2) * 4 - 3. No parentheses needed!
//
// The algorithm uses a stack:
// - Numbers are pushed onto the stack.
// - An operator pops two numbers (the top one is the RIGHT operand), applies
//   the operation and pushes the result.
// - At the end, exactly one number must be left: that's the answer.
//
// Tokens are separated by spaces. Supported operators: + - * /
// Numbers can be negative ("-3"); a lone "-" is the operator.

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "clings.h"

typedef enum {
    RPN_OK,
    RPN_BAD_TOKEN,       // something that's neither a number nor an operator
    RPN_STACK_UNDERFLOW, // an operator without two operands
    RPN_STACK_OVERFLOW,  // more than RPN_MAX_STACK numbers on the stack
    RPN_DIV_BY_ZERO,
    RPN_LEFTOVERS,       // not exactly one number left at the end
} RpnStatus;

#define RPN_MAX_STACK 32

RpnStatus rpn_eval(const char *expr, long *result) {
    // TODO
    return RPN_BAD_TOKEN;
}

TEST(simple) {
    long r = 0;
    ASSERT_EQ(rpn_eval("3 4 +", &r), RPN_OK);
    ASSERT_EQ(r, 7);
    ASSERT_EQ(rpn_eval("42", &r), RPN_OK);
    ASSERT_EQ(r, 42);
}

TEST(operand_order) {
    long r = 0;
    ASSERT_EQ(rpn_eval("10 4 -", &r), RPN_OK);
    ASSERT_EQ(r, 6);
    ASSERT_EQ(rpn_eval("20 5 /", &r), RPN_OK);
    ASSERT_EQ(r, 4);
}

TEST(complex) {
    long r = 0;
    ASSERT_EQ(rpn_eval("5 1 2 + 4 * + 3 -", &r), RPN_OK);
    ASSERT_EQ(r, 14);
    ASSERT_EQ(rpn_eval("  2   -3  *  ", &r), RPN_OK);
    ASSERT_EQ(r, -6);
}

TEST(errors) {
    long r = 0;
    ASSERT_EQ(rpn_eval("1 +", &r), RPN_STACK_UNDERFLOW);
    ASSERT_EQ(rpn_eval("1 2", &r), RPN_LEFTOVERS);
    ASSERT_EQ(rpn_eval("", &r), RPN_LEFTOVERS);
    ASSERT_EQ(rpn_eval("1 0 /", &r), RPN_DIV_BY_ZERO);
    ASSERT_EQ(rpn_eval("1 2 ^", &r), RPN_BAD_TOKEN);
    ASSERT_EQ(rpn_eval("1 2x +", &r), RPN_BAD_TOKEN);
}

TEST(stack_limit) {
    char expr[256] = "";
    for (int i = 0; i < RPN_MAX_STACK + 1; i++) {
        strcat(expr, "1 ");
    }
    long r = 0;
    ASSERT_EQ(rpn_eval(expr, &r), RPN_STACK_OVERFLOW);
}
