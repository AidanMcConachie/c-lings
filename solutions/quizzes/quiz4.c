#include <errno.h>
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

typedef RpnStatus (*RpnOp)(long a, long b, long *out);

static RpnStatus op_add(long a, long b, long *out) { *out = a + b; return RPN_OK; }
static RpnStatus op_sub(long a, long b, long *out) { *out = a - b; return RPN_OK; }
static RpnStatus op_mul(long a, long b, long *out) { *out = a * b; return RPN_OK; }

static RpnStatus op_div(long a, long b, long *out) {
    if (b == 0) return RPN_DIV_BY_ZERO;
    *out = a / b;
    return RPN_OK;
}

static RpnOp lookup_op(const char *tok, size_t len) {
    if (len != 1) return NULL;
    switch (tok[0]) {
    case '+': return op_add;
    case '-': return op_sub;
    case '*': return op_mul;
    case '/': return op_div;
    default: return NULL;
    }
}

RpnStatus rpn_eval(const char *expr, long *result) {
    long stack[RPN_MAX_STACK];
    size_t top = 0; // number of elements on the stack

    const char *p = expr;
    for (;;) {
        while (*p == ' ') p++;
        if (*p == '\0') break;
        const char *tok = p;
        while (*p != ' ' && *p != '\0') p++;
        size_t len = (size_t)(p - tok);

        RpnOp op = lookup_op(tok, len);
        if (op != NULL) {
            if (top < 2) return RPN_STACK_UNDERFLOW;
            long b = stack[--top];
            long a = stack[--top];
            long value;
            RpnStatus st = op(a, b, &value);
            if (st != RPN_OK) return st;
            stack[top++] = value;
            continue;
        }

        char *end;
        errno = 0;
        long value = strtol(tok, &end, 10);
        if (end == tok || end != p || errno == ERANGE) return RPN_BAD_TOKEN;
        if (top == RPN_MAX_STACK) return RPN_STACK_OVERFLOW;
        stack[top++] = value;
    }

    if (top != 1) return RPN_LEFTOVERS;
    *result = stack[0];
    return RPN_OK;
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
