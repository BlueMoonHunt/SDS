#include "sds.h"
#include <string.h>

#define VECTOR_BLOCK_SIZE 16

struct vector {
    size_t size;
    size_t capacity;
    size_t element_size;
    size_t element_alignment;
    uint8_t* data;
};

vector* vector_create(size_t element_size, size_t alignment, Arena* arena) {
    element_size = (element_size == 0) ? 1 : element_size;
    alignment = (alignment == 0) ? 1 : alignment;

    vector* vec = (vector*)arena_alloc(arena, sizeof(vector), alignof(vector));
    vec->capacity = VECTOR_BLOCK_SIZE;
    vec->size = 0;
    vec->element_size = element_size;
    vec->element_alignment = alignment;
    vec->data = (uint8_t*)arena_alloc(arena, vec->element_size * vec->capacity, vec->element_alignment);

    return vec;
}

void vector_reserve(vector* vec, size_t new_capacity, Arena* arena) {
    if (!vec)
        return;
    if (vec->capacity < new_capacity) {

        uint8_t* new_data = (uint8_t*)arena_alloc(arena, vec->element_size * new_capacity, vec->element_alignment);
        if (vec->size > 0)
            memcpy(new_data, vec->data, vec->size * vec->element_size);

        vec->data = new_data;
        vec->capacity = new_capacity;
    }
}

void vector_emplace_back(vector* vec, const uint8_t* data, Arena* arena) {
    if (!vec || !data)
        return;

    if (vec->size == vec->capacity) {
        size_t new_cap = alignUpPow2(vec->capacity * 2, VECTOR_BLOCK_SIZE);
        vector_reserve(vec, new_cap, arena);
    }

    uint8_t* dest = vec->data + (vec->size * vec->element_size);
    memcpy(dest, data, vec->element_size);
    vec->size++;
}

void vector_pop_back(vector* vec) {
    if (vec && vec->size > 0)
        vec->size--;
}

void vector_clear(vector* vec) {
    if (vec)
        vec->size = 0;
}

uint8_t* vector_data(vector* vec) {
    return (vec) ? vec->data : NULL;
}

size_t vector_size(vector* vec) {
    return (vec) ? vec->size : 0;
}

size_t vector_capacity(vector* vec) {
    return (vec) ? vec->capacity : 0;
}

uint8_t* vector_front(const vector* vec) {
    if (vec->size == 0) return NULL;
    return vec->data;
}
uint8_t* vector_back(const vector* vec) {
    if (vec->size == 0) return NULL;
    return vec->data + ((vec->size - 1) * vec->element_size);
}