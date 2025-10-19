#include "sds.h"
#include <string.h>

#define VECTOR_BLOCK_SIZE 16

struct _gvec {
    size_t size;
    size_t capacity;
    size_t element_size;
    size_t element_alignment;
    uint8_t* data;
};

_gvec* _gvec_create(size_t element_size, size_t alignment, Arena* arena) {
    element_size = (element_size == 0) ? 1 : element_size;
    alignment = (alignment == 0) ? 1 : alignment;

    _gvec* vec = (_gvec*)arena_alloc(arena, sizeof(_gvec), alignof(_gvec));
    vec->capacity = VECTOR_BLOCK_SIZE;
    vec->size = 0;
    vec->element_size = element_size;
    vec->element_alignment = alignment;
    vec->data = (uint8_t*)arena_alloc(arena, vec->element_size * vec->capacity, vec->element_alignment);

    return vec;
}

void _gvec_reserve(_gvec* vec, size_t new_capacity, Arena* arena) {
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

void _gvec_emplace_back(_gvec* vec, const uint8_t* data, Arena* arena) {
    if (!vec || !data)
        return;

    if (vec->size == vec->capacity) {
        size_t new_cap = alignUpPow2(vec->capacity * 2, VECTOR_BLOCK_SIZE);
        _gvec_reserve(vec, new_cap, arena);
    }

    uint8_t* dest = vec->data + (vec->size * vec->element_size);
    memcpy(dest, data, vec->element_size);
    vec->size++;
}

void _gvec_pop_back(_gvec* vec) {
    if (vec && vec->size > 0)
        vec->size--;
}

void _gvec_clear(_gvec* vec) {
    if (vec)
        vec->size = 0;
}

uint8_t* _gvec_data(_gvec* vec) {
    return (vec) ? vec->data : NULL;
}

size_t _gvec_size(_gvec* vec) {
    return (vec) ? vec->size : 0;
}

size_t _gvec_capacity(_gvec* vec) {
    return (vec) ? vec->capacity : 0;
}

uint8_t* _gvec_front(const _gvec* vec) {
    if (vec->size == 0) return NULL;
    return vec->data;
}
uint8_t* _gvec_back(const _gvec* vec) {
    if (vec->size == 0) return NULL;
    return vec->data + ((vec->size - 1) * vec->element_size);
}