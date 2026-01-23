#pragma once
/*
 * Simple Vector Implementation
 *
 * A dynamically sized array with useful helper functions
 *
 */


/*
 * The main vector struct.
 * The items are stored as void pointers. Pointer data memory
 * is managed externally. The struct is only responsible for taking
 * care of the pointer memory allocation, i.e the array itself
 *
 */
struct vector {
    int size;
    int len;
    void** array;
};

/* vector_init
 * Initalize a vector struct pointer to start storing data.
 * The vec parameter will be initialized in the function itself.
 * Returns 0 on success, non zero rc on error
 */
int vector_init(struct vector** vec);

/*
 * Insert an item in an initialized vector at a given position pos
 */
int vector_insert(struct vector* vec, void* element, int pos);

/*
 * Get the current size of a vector
 */
int vector_length(struct vector* vec);
/*
 * Get the pointer stored in the vector at index from the vector vec
 */
void* vector_get(struct vector* vec, int index); 

/*
 * Delete and return the element at index from the vector vec
 */
void* vector_pop(struct vector* vec, int index);

#define VECTOR_INSERT(vec, element) vector_insert(vec, element, vec->len)
#define VECTOR_POP(vec) vector_pop(vec, vec->len)mutt_generate_attachment_commands
