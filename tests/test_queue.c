#include "junk/queue.h"
#include <assert.h>
#include <stdlib.h>


void test_queue_init() {
    struct junk_queue q;
    junk_queue_init(&q);
    assert(q.size == 0);
    assert(q.head == 0);
    assert(q.tail == 0);
    junk_queue_free(&q);
}

void test_queue_insert() {
    struct junk_queue q;
    junk_queue_init(&q);
    char* item1 = "item1";
    char* item2 = "item2";
    char* item3 = "item3";
    junk_queue_insert(&q, item1);
    junk_queue_insert(&q, item2);
    junk_queue_insert(&q, item3);
    assert(q.size == 3);
    assert(q.head->ptr == item1);
    assert(q.tail->ptr == item3);
    assert(q.head->next->ptr == item2);
    assert(q.head->next->next == q.tail);
    junk_queue_free(&q);
}

void test_queue_length() {
    struct junk_queue q;
    junk_queue_init(&q);
    char* item1 = "item1";
    char* item2 = "item2";
    char* item3 = "item3";
    junk_queue_insert(&q, item1);
    junk_queue_insert(&q, item2);
    junk_queue_insert(&q, item3);
    assert(junk_queue_length(&q) == 3);
    junk_queue_free(&q);
}

void test_queue_peek() {
    struct junk_queue q;
    junk_queue_init(&q);
    char* item1 = "item1";
    char* item2 = "item2";
    char* item3 = "item3";
    junk_queue_insert(&q, item1);
    junk_queue_insert(&q, item2);
    junk_queue_insert(&q, item3);
    assert(junk_queue_peek(&q) == item1);
    junk_queue_free(&q);
}

void test_queue_pop() {
    struct junk_queue q;
    junk_queue_init(&q);
    char* item1 = "item1";
    char* item2 = "item2";
    char* item3 = "item3";
    junk_queue_insert(&q, item1);
    junk_queue_insert(&q, item2);
    junk_queue_insert(&q, item3);

    assert(junk_queue_pop(&q) == item1);
    assert(junk_queue_length(&q) == 2);
    assert(q.head->ptr == item2);
    assert(q.tail->ptr == item3);


    assert(junk_queue_pop(&q) == item2);
    assert(junk_queue_length(&q) == 1);
    assert(q.head->ptr == item3);
    assert(q.tail->ptr == item3);

    assert(junk_queue_pop(&q) == item3);
    assert(junk_queue_length(&q) == 0);
    assert(q.head == NULL);
    assert(q.tail == NULL);

    junk_queue_free(&q);
}

int main() {
    test_queue_init();
    test_queue_insert();
    test_queue_length();
    test_queue_peek();
    test_queue_pop();
    return 0;
}
