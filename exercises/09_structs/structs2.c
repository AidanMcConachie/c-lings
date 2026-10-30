// `birthday` should make a person one year older, but the tests pass it a
// pointer. Fix it, and implement `make_person` too.

#include "clings.h"

typedef struct {
    char name[32];
    int age;
} Person;

void birthday(Person p) {
    p.age++;
}

// Creates a Person. Names longer than 31 characters get cut short.
Person make_person(const char *name, int age) {
    Person p = {0};
    // TODO
    return p;
}

TEST(make_person_works) {
    Person p = make_person("Grace", 85);
    ASSERT_STR_EQ(p.name, "Grace");
    ASSERT_EQ(p.age, 85);
}

TEST(long_names_are_truncated) {
    Person p = make_person("Somebody With A Really Very Long Name", 30);
    ASSERT_EQ(strlen(p.name), 31);
}

TEST(birthday_adds_a_year) {
    Person p = make_person("Ada", 36);
    birthday(&p);
    ASSERT_EQ(p.age, 37);
}
