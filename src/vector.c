#include "junk/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int vector_init_size = 16;
static int scale_factor = 2;

int junk_vector_init(struct junk_vector** vec) {
    *vec = malloc(sizeof(struct junk_vector));
    memset(*vec, 0, sizeof(struct junk_vector));
    if (*vec == NULL) {
        perror("vec: Could not alloc memory for vec");
        return -1;
    }
    (*vec)->array = malloc(sizeof(void*) * vector_init_size);
    memset((*vec)->array, 0, sizeof(void*) * vector_init_size);
    if ((*vec)->array == NULL) {
        perror("vec: Could not alloc memory for vec array");
        return -1;
    }
    (*vec)->size = vector_init_size;
    (*vec)->len = 0;
    return 0;
}
int junk_vector_insert(struct junk_vector* vec, void* element, int pos) {
    if (pos > vec->len) {
        return -1;
    }
    if (vec->len + 1 > vec->size / scale_factor) {
        void* ret = realloc(vec->array, sizeof(void*) * vec->size * scale_factor);
        if (ret == NULL) {
            perror("vec: Could not realloc on insert");
            return -1;
        }
        vec->array = ret;
        memset(vec->array + vec->size, 0, sizeof(void*) * vec->size);
        vec->size *= scale_factor;
    }
    void** ptr = vec->array + pos;
    void* buf[vec->len - pos];
    memcpy(buf, ptr, vec->len - pos);
    memcpy(ptr + 1, buf, vec->len - pos);
    *ptr = element;
    vec->len += 1;
    return 0;
}

int junk_vector_length(struct junk_vector *vec) {
    return vec->len;
}

void* junk_vector_get(struct junk_vector* vec, int index) {
    if (index > vec->len) {
        return NULL;
    }
    void** ptr = vec->array + index;
    return *ptr;
} 
void* junk_vector_pop(struct junk_vector* vec, int index) {
    if (index > vec->len) {
        return NULL;
    }
    void** ptr = vec->array + index;
    void* buf[vec->len - index];
    // TODO: This +1 *should* be safe if last element since 
    // we allocate more than needed, so there will be 0s
    // Also we exploit this fact by not doing a -1 on the size
    // Copying 1 more blank byte just to make it easier to write
    memcpy(buf, ptr + 1, vec->len - index);
    memcpy(ptr, buf, vec->len - index);
    vec->len -= 1;
    return *ptr;
}
