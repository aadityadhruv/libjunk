#pragma once
/*
 * Queue
 *
 * A FIFO queue implementation
 *
 */

struct junk_queue_item {
    void* ptr;
    struct junk_queue_item* next;
};

/*
 * The items are stored as void pointers. Pointer data memory
 * is managed externally. The struct is only responsible for taking
 * care of junk_queue_item memory allocations
 *
 * The queue is implemented as a linked list of junk_queue_items
 *
 */
struct junk_queue {
    int size;
    struct junk_queue_item* head;
    struct junk_queue_item* tail;
};

/* junk_queue_init
 * Initalize a queue struct pointer to start storing data.
 * Returns 0 on success, non zero rc on error
 */
int junk_queue_init(struct junk_queue* queue);

/*
 * Insert an item in an initialized queue
 */
int junk_queue_insert(struct junk_queue* queue, void* element);

/*
 * Get the current size of a queue
 */
int junk_queue_length(struct junk_queue* queue);
/*
 * Get the pointer at the top (front) of the queue
 */
void* junk_queue_peek(struct junk_queue* queue); 

/*
 * Delete and return the first element
 */
void* junk_queue_pop(struct junk_queue* queue);

/*
 * Free data allocated by the queue. Note that this does
 * not touch the queue pointer itself
 */
void junk_queue_free(struct junk_queue* queue);
