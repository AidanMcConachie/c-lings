// An array of structs. Implement the functions below.

#include "clings.h"

typedef struct {
    const char *name;
    double price;
    int quantity;
} Item;

// Total cost of all items (price * quantity, summed).
double cart_total(const Item *items, size_t len) {
    // TODO
    return 0;
}

// Returns a pointer to the item with the highest price, or NULL if the
// cart is empty.
const Item *most_expensive(const Item *items, size_t len) {
    // TODO
    return NULL;
}

TEST(totals) {
    Item cart[] = {
        {.name = "apple", .price = 0.5, .quantity = 6},
        {.name = "bread", .price = 2.25, .quantity = 1},
        {.name = "cheese", .price = 7.0, .quantity = 2},
    };
    ASSERT_NEAR(cart_total(cart, 3), 19.25, 1e-9);
    ASSERT_NEAR(cart_total(cart, 0), 0.0, 1e-9);
}

TEST(finds_most_expensive) {
    Item cart[] = {
        {.name = "apple", .price = 0.5, .quantity = 6},
        {.name = "cheese", .price = 7.0, .quantity = 2},
        {.name = "bread", .price = 2.25, .quantity = 1},
    };
    const Item *best = most_expensive(cart, 3);
    ASSERT_PTR_EQ(best, &cart[1]);
    ASSERT_STR_EQ(best->name, "cheese");
}

TEST(empty_cart) {
    ASSERT_NULL(most_expensive(NULL, 0));
}
