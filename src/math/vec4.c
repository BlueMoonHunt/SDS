#include "vec.h"

#define VEC4_FUNCTION_DEFINATION(PREFIX, TYPE) \
    void PREFIX##vec4##_fill(PREFIX##vec4* restrict vec, TYPE value) { \
        vec->x = value; \
        vec->y = value; \
        vec->z = value; \
        vec->w = value; \
    } \
    void PREFIX##vec4##_add(const PREFIX##vec4* restrict a, const PREFIX##vec4* restrict b, PREFIX##vec4* restrict dest) { \
        dest->x = a->x + b->x; \
        dest->y = a->y + b->y; \
        dest->z = a->z + b->z; \
        dest->w = a->w + b->w; \
    } \
    void PREFIX##vec4##_sub(const PREFIX##vec4* restrict a, const PREFIX##vec4* restrict b, PREFIX##vec4* restrict dest) { \
        dest->x = a->x - b->x; \
        dest->y = a->y - b->y; \
        dest->z = a->z - b->z; \
        dest->w = a->w - b->w; \
    } \
    void PREFIX##vec4##_mul(const PREFIX##vec4* restrict a, const PREFIX##vec4* restrict b, PREFIX##vec4* restrict dest) { \
        dest->x = a->x * b->x; \
        dest->y = a->y * b->y; \
        dest->z = a->z * b->z; \
        dest->w = a->w * b->w; \
    } \
    void PREFIX##vec4##_div(const PREFIX##vec4* restrict a, const PREFIX##vec4* restrict b, PREFIX##vec4* restrict dest) { \
        dest->x = a->x / b->x; \
        dest->y = a->y / b->y; \
        dest->z = a->z / b->z; \
        dest->w = a->w / b->w; \
    } \
    int64_t PREFIX##vec4##_dot(const PREFIX##vec4* restrict a, const PREFIX##vec4* restrict b) { \
        return (int64_t)a->x * (int64_t)b->x + (int64_t)a->y * (int64_t)b->y + (int64_t)a->z * (int64_t)b->z + (int64_t)a->w * (int64_t)b->w; \
    } \
    void PREFIX##vec4##_scale(const PREFIX##vec4* restrict src, TYPE value, PREFIX##vec4* restrict dest) { \
        dest->x = src->x * value; \
        dest->y = src->y * value; \
        dest->z = src->z * value; \
        dest->w = src->w * value; \
    } \
    void PREFIX##vec4##_copy(const PREFIX##vec4* restrict src, PREFIX##vec4* restrict dest) { \
        dest->x = src->x; \
        dest->y = src->y; \
        dest->z = src->z; \
        dest->w = src->w; \
    }

SDS_FOR_EACH_TYPE(VEC4_FUNCTION_DEFINATION)