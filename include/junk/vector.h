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
struct junk_vector {
    int size;
    int len;
    void** array;
};

/* junk_vector_init
 * Initalize a vector struct pointer to start storing data.
 * The vec parameter will be initialized in the function itself.
 * Returns 0 on success, non zero rc on error
 */
int junk_vector_init(struct junk_vector** vec);

/*
 * Insert an item in an initialized vector at a given position pos
 */
int junk_vector_insert(struct junk_vector* vec, void* element, int pos);

/*
 * Get the current size of a vector
 */
int junk_vector_length(struct junk_vector* vec);
/*
 * Get the pointer stored in the vector at index from the vector vec
 */
void* junk_vector_get(struct junk_vector* vec, int index); 

/*
 * Delete and return the element at index from the vector vec
 */
void* junk_vector_pop(struct junk_vector* vec, int index);

#define JUNK_VECTOR_INSERT(vec, element) junk_vector_insert(vec, element, vec->len)
#define JUNK_VECTOR_POP(vec) junk_vector_pop(vec, vec->len)mutt_generate_attachment_commands
