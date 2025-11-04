#include "vec.h"

#define VEC3_FUNCTION_DEFINATION(PREFIX, TYPE) \
    void PREFIX##vec3##_fill(PREFIX##vec3* restrict vec, TYPE value) { \
        vec->x = value; \
        vec->y = value; \
        vec->z = value; \
    } \
    void PREFIX##vec3##_add(const PREFIX##vec3* restrict a, const PREFIX##vec3* restrict b, PREFIX##vec3* restrict dest) { \
        dest->x = a->x + b->x; \
        dest->y = a->y + b->y; \
        dest->z = a->z + b->z; \
    } \
    void PREFIX##vec3##_sub(const PREFIX##vec3* restrict a, const PREFIX##vec3* restrict b, PREFIX##vec3* restrict dest) { \
        dest->x = a->x - b->x; \
        dest->y = a->y - b->y; \
        dest->z = a->z - b->z; \
    } \
    void PREFIX##vec3##_mul(const PREFIX##vec3* restrict a, const PREFIX##vec3* restrict b, PREFIX##vec3* restrict dest) { \
        dest->x = a->x * b->x; \
        dest->y = a->y * b->y; \
        dest->z = a->z * b->z; \
    } \
    void PREFIX##vec3##_div(const PREFIX##vec3* restrict a, const PREFIX##vec3* restrict b, PREFIX##vec3* restrict dest) { \
        dest->x = a->x / b->x; \
        dest->y = a->y / b->y; \
        dest->z = a->z / b->z; \
    } \
    int64_t PREFIX##vec3##_dot(const PREFIX##vec3* restrict a, const PREFIX##vec3* restrict b) { \
        return (int64_t)a->x * (int64_t)b->x + (int64_t)a->y * (int64_t)b->y + (int64_t)a->z * (int64_t)b->z; \
    } \
    void PREFIX##vec3##_scale(const PREFIX##vec3* restrict src, TYPE value, PREFIX##vec3* restrict dest) { \
        dest->x = src->x * value; \
        dest->y = src->y * value; \
        dest->z = src->z * value; \
    } \
    void PREFIX##vec3##_copy(const PREFIX##vec3* restrict src, PREFIX##vec3* restrict dest) { \
        dest->x = src->x; \
        dest->y = src->y; \
        dest->z = src->z; \
    } \
    void PREFIX##vec3##_cross(const PREFIX##vec3* restrict a, const PREFIX##vec3* restrict b, PREFIX##vec3* restrict dest) { \
        dest->x = a->y * b->z - a->z * b->y; \
        dest->y = a->z * b->x - a->x * b->z; \
        dest->z = a->x * b->y - a->y * b->x; \
    }

SDS_FOR_EACH_TYPE(VEC3_FUNCTION_DEFINATION)