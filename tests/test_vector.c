#include "junk/vector.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


void test_vector_single_item() {
    struct junk_vector* vec;
    junk_vector_init(&vec);
    int item1 = 10;
    JUNK_VECTOR_INSERT(vec, &item1);
    int* ret1 = junk_vector_get(vec, 0);
    assert(item1 == *ret1);
}

void test_vector_multiple_items() {
    struct junk_vector* vec;
    junk_vector_init(&vec);
    int item1 = 10;
    int item2 = 20;
    int item3 = 30;
    JUNK_VECTOR_INSERT(vec, &item1);
    JUNK_VECTOR_INSERT(vec, &item2);
    JUNK_VECTOR_INSERT(vec, &item3);
    int* ret1 = junk_vector_get(vec, 0);
    int* ret2 = junk_vector_get(vec, 1);
    int* ret3 = junk_vector_get(vec, 2);
    assert(item1 == *ret1);
    assert(item2 == *ret2);
    assert(item3 == *ret3);
}

void test_vector_multiple_items_char() {
    struct junk_vector* vec;
    junk_vector_init(&vec);
    char* item1 = "Hello";
    char* item2 = "World";
    char* item3 = "!";
    JUNK_VECTOR_INSERT(vec, item1);
    JUNK_VECTOR_INSERT(vec, item2);
    JUNK_VECTOR_INSERT(vec, item3);
    char* ret1 = junk_vector_get(vec, 0);
    char* ret2 = junk_vector_get(vec, 1);
    char* ret3 = junk_vector_get(vec, 2);
    assert(strcmp(item1, ret1) == 0);
    assert(strcmp(item2, ret2) == 0);
    assert(strcmp(item3, ret3) == 0);
}
void test_vector_multiple_items_complex_data() {
    struct junk_vector* vec;
    struct item {
        float x;
        float y;
        char* name;
    };
    junk_vector_init(&vec);
    struct item item1 = { .x = 1.0f, .y = 0.0f, "coord1" };
    struct item item2 = { .x = 0.0f, .y = 1.0f, "coord2" };
    struct item item3 = { .x = 0.0f, .y = 0.0f, "coord3" };
    JUNK_VECTOR_INSERT(vec, &item1);
    JUNK_VECTOR_INSERT(vec, &item2);
    JUNK_VECTOR_INSERT(vec, &item3);
    struct item* ret1 = junk_vector_get(vec, 0);
    struct item* ret2 = junk_vector_get(vec, 1);
    struct item* ret3 = junk_vector_get(vec, 2);

    assert(junk_vector_length(vec) == 3);

    assert(item1.x == ret1->x);
    assert(item1.y == ret1->y);
    assert(strcmp(item1.name, ret1->name) == 0);

    assert(item2.x == ret2->x);
    assert(item2.y == ret2->y);
    assert(strcmp(item2.name, ret2->name) == 0);

    assert(item3.x == ret3->x);
    assert(item3.y == ret3->y);
    assert(strcmp(item3.name, ret3->name) == 0);
}

void test_vector_multiple_items_complex_data_pop() {
    struct junk_vector* vec;
    struct item {
        float x;
        float y;
        char* name;
    };
    junk_vector_init(&vec);
    struct item item1 = { .x = 1.0f, .y = 0.0f, "coord1" };
    struct item item2 = { .x = 0.0f, .y = 1.0f, "coord2" };
    struct item item3 = { .x = 0.0f, .y = 0.0f, "coord3" };
    JUNK_VECTOR_INSERT(vec, &item1);
    JUNK_VECTOR_INSERT(vec, &item2);
    JUNK_VECTOR_INSERT(vec, &item3);

    // Pop item2
    junk_vector_pop(vec, 1);
    struct item* ret1 = junk_vector_get(vec, 0);
    struct item* ret3 = junk_vector_get(vec, 1);

    assert(junk_vector_length(vec) == 2);

    assert(item1.x == ret1->x);
    assert(item1.y == ret1->y);
    assert(strcmp(item1.name, ret1->name) == 0);

    assert(item3.x == ret3->x);
    assert(item3.y == ret3->y);
    assert(strcmp(item3.name, ret3->name) == 0);

    // Put it back
    junk_vector_insert(vec, &item2, 1);

    assert(junk_vector_length(vec) == 3);
    struct item* ret2 = junk_vector_get(vec, 1);
    assert(item2.x == ret2->x);
    assert(item2.y == ret2->y);
    assert(strcmp(item2.name, ret2->name) == 0);
}

int main() {
    test_vector_single_item();
    test_vector_multiple_items();
    test_vector_multiple_items_char();
    test_vector_multiple_items_complex_data();
    test_vector_multiple_items_complex_data_pop();
    return 0;
}
