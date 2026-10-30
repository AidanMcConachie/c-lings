#include <stdio.h>

#include "clings.h"

typedef enum { VAL_INT, VAL_DOUBLE, VAL_TEXT } ValueKind;

typedef struct {
    ValueKind kind;
    union {
        int i;
        double d;
        const char *text;
    } as;
} Value;

void value_format(const Value *v, char *buf, size_t size) {
    // The tag tells us which union member is valid to read.
    switch (v->kind) {
    case VAL_INT:
        snprintf(buf, size, "%d", v->as.i);
        break;
    case VAL_DOUBLE:
        snprintf(buf, size, "%.2f", v->as.d);
        break;
    case VAL_TEXT:
        snprintf(buf, size, "\"%s\"", v->as.text);
        break;
    }
}

TEST(formats_ints) {
    char buf[32];
    Value v = {.kind = VAL_INT, .as.i = 42};
    value_format(&v, buf, sizeof buf);
    ASSERT_STR_EQ(buf, "42");
}

TEST(formats_doubles) {
    char buf[32];
    Value v = {.kind = VAL_DOUBLE, .as.d = 2.5};
    value_format(&v, buf, sizeof buf);
    ASSERT_STR_EQ(buf, "2.50");
}

TEST(formats_text) {
    char buf[32];
    Value v = {.kind = VAL_TEXT, .as.text = "hi"};
    value_format(&v, buf, sizeof buf);
    ASSERT_STR_EQ(buf, "\"hi\"");
}

TEST(union_members_share_memory) {
    Value v;
    // A union is only as big as its biggest member...
    ASSERT(sizeof(v.as) == sizeof(double) || sizeof(v.as) == sizeof(const char *));
    // ...because every member starts at the same address.
    ASSERT_PTR_EQ(&v.as.i, &v.as.d);
}
