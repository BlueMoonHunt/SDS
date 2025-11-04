#include "sds.h"
#include <string.h>

#define VECTOR_BLOCK_SIZE 8

struct vector {
    size_t size;
    size_t capacity;
    size_t element_size;
    size_t element_alignment;
    Arena* arena;
    void* data;
};

vector* _vector_create(size_t element_size, size_t element_alignment, Arena* arena) {
    element_size = (element_size == 0) ? 1 : element_size;
    element_alignment = (element_alignment == 0) ? 1 : element_alignment;

    vector* vec = (vector*)arena_alloc(arena, sizeof(vector), alignof(vector));
    vec->capacity = VECTOR_BLOCK_SIZE;
    vec->size = 0;
    vec->element_size = element_size;
    vec->element_alignment = element_alignment;
    vec->arena = arena;
    vec->data = (void*)arena_alloc(arena, vec->element_size * vec->capacity, vec->element_alignment);

    return vec;
}

void vector_reserve(vector* vec, size_t new_capacity) {
    if (!vec)
        return;
    if (vec->capacity < new_capacity) {

        void* new_data = (void*)arena_alloc(vec->arena, vec->element_size * new_capacity, vec->element_alignment);
        if (vec->size > 0)
            memmove(new_data, vec->data, vec->size * vec->element_size);

        vec->data = new_data;
        vec->capacity = new_capacity;
    }
}

void vector_emplace_back(vector* vec, const void* element) {
    if (!vec || !element)
        return;

    if (vec->size == vec->capacity) {
        size_t new_cap = align_up_pow2(vec->capacity * 2, VECTOR_BLOCK_SIZE);
        vector_reserve(vec, new_cap);
    }

    void* dest = (uint8_t*)vec->data + (vec->size * vec->element_size);
    memcpy(dest, element, vec->element_size);
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

void* vector_data(const vector* vec) {
    return (vec) ? vec->data : NULL;
}

size_t vector_size(const vector* vec) {
    return (vec) ? vec->size : 0;
}

size_t vector_capacity(const vector* vec) {
    return (vec) ? vec->capacity : 0;
}

void* vector_front(const vector* vec) {
    if (vec->size == 0) return NULL;
    return vec->data;
}
void* vector_back(const vector* vec) {
    if (vec->size == 0) return NULL;
    return (uint8_t*)vec->data + ((vec->size - 1) * vec->element_size);
}