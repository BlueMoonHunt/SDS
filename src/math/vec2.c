#include "vec.h"

#define VEC2_FUNCTION_DEFINATION(PREFIX, TYPE) \
    void PREFIX##vec2##_fill(PREFIX##vec2* restrict vec, TYPE value) { \
        vec->x = value; \
        vec->y = value; \
    } \
    void PREFIX##vec2##_add(const PREFIX##vec2* restrict a, const PREFIX##vec2* restrict b, PREFIX##vec2* restrict dest) { \
        dest->x = a->x + b->x; \
        dest->y = a->y + b->y; \
    } \
    void PREFIX##vec2##_sub(const PREFIX##vec2* restrict a, const PREFIX##vec2* restrict b, PREFIX##vec2* restrict dest) { \
        dest->x = a->x - b->x; \
        dest->y = a->y - b->y; \
    } \
    void PREFIX##vec2##_mul(const PREFIX##vec2* restrict a, const PREFIX##vec2* restrict b, PREFIX##vec2* restrict dest) { \
        dest->x = a->x * b->x; \
        dest->y = a->y * b->y; \
    } \
    void PREFIX##vec2##_div(const PREFIX##vec2* restrict a, const PREFIX##vec2* restrict b, PREFIX##vec2* restrict dest) { \
        dest->x = a->x / b->x; \
        dest->y = a->y / b->y; \
    } \
    int64_t PREFIX##vec2##_dot(const PREFIX##vec2* restrict a, const PREFIX##vec2* restrict b) { \
        return (int64_t)a->x * (int64_t)b->x + (int64_t)a->y * (int64_t)b->y;\
    } \
    void PREFIX##vec2##_scale(const PREFIX##vec2* restrict src, TYPE value, PREFIX##vec2* restrict dest) { \
        dest->x = src->x * value; \
        dest->y = src->y * value; \
    } \
    void PREFIX##vec2##_copy(const PREFIX##vec2* restrict src, PREFIX##vec2* restrict dest) { \
        dest->x = src->x; \
        dest->y = src->y; \
    }

SDS_FOR_EACH_TYPE(VEC2_FUNCTION_DEFINATION)