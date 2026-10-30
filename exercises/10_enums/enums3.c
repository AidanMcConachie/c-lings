// A tagged union: `Value` holds an int, a double or a string, and `kind`
// says which one. Implement `value_format`, which writes the value as text
// into `buf`:
//
//   VAL_INT    42        => "42"
//   VAL_DOUBLE 2.5       => "2.50"    (always 2 decimals)
//   VAL_TEXT   "hi"      => "\"hi\""  (wrapped in double quotes)

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
    // TODO
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
