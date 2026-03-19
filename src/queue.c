#include "junk/queue.h"
#include <stdio.h>
#include <stdlib.h>

int junk_queue_init(struct junk_queue* queue) {
    // Set size to 0 and head/tail to NULL
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return 0;
}

int junk_queue_insert(struct junk_queue* queue, void* element) {
    struct junk_queue_item* next = malloc(sizeof(struct junk_queue_item));
    next->next = NULL;
    next->ptr = element;
    // List is empty
    if (queue->head == NULL) {
        if (queue->size != 0) {
            fprintf(stderr, "junk_queue_insert: queue head null but size non zero (%d)!", queue->size);
            return -1;
        }
        queue->head = next;
        queue->tail = next;
        queue->size = 1;
    } else {
        if (queue->tail == NULL) {
            fprintf(stderr, "junk_queue_insert: queue tail is null but size non zero (%d)!", queue->size);
            return -1;
        }
        queue->tail->next = next;
        queue->tail = next;
        queue->size += 1;
    }
    return 0;
}

/*
 * Get the current size of a queue
 */
int junk_queue_length(struct junk_queue* queue) {
    return queue->size;
}
/*
 * Get the pointer at the top (front) of the queue
 */
void* junk_queue_peek(struct junk_queue* queue) {
    return queue->head->ptr;
}

/*
 * Delete and return the first element
 */
void* junk_queue_pop(struct junk_queue* queue) {
    // Queue of size 1
    if (queue->head == NULL) {
        if (queue->tail != NULL || queue->size != 0) {
            fprintf(stderr, "junk_queue_pop: head is NULL but tail or queue size isn't (%d)", queue->size);
            return NULL;
        }
    }
    if (queue->head == queue->tail) {
        if (queue->size != 1) {
            fprintf(stderr, "junk_queue_pop: queue head == queue tail but size not one (%d)!", queue->size);
            return NULL;
        }
        queue->tail = NULL;
    }
    void* ptr = queue->head->ptr;
    struct junk_queue_item* head = queue->head;
    queue->head = queue->head->next;
    queue->size -= 1;
    free(head);
    return ptr;
}

/*
 * Free data allocated by the queue. Note that this does
 * not touch the queue pointer itself
 */
void junk_queue_free(struct junk_queue* queue) {
    while (queue->head) {
        struct junk_queue_item* item = queue->head;
        queue->head = queue->head->next;
        free(item);
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}
