#ifndef VEC_IMPLEMENTATION
#define VEC_IMPLEMENTATION

#include "types.h"

#define VEC_FUNCTION_DECLARATION(PREFIX, TYPE, N) \
    void PREFIX##vec##N##_fill(PREFIX##vec##N* restrict vec, TYPE value); \
    void PREFIX##vec##N##_add(const PREFIX##vec##N* restrict a, const PREFIX##vec##N* restrict b, PREFIX##vec##N* restrict dest); \
    void PREFIX##vec##N##_sub(const PREFIX##vec##N* restrict a, const PREFIX##vec##N* restrict b, PREFIX##vec##N* restrict dest); \
    void PREFIX##vec##N##_mul(const PREFIX##vec##N* restrict a, const PREFIX##vec##N* restrict b, PREFIX##vec##N* restrict dest); \
    void PREFIX##vec##N##_div(const PREFIX##vec##N* restrict a, const PREFIX##vec##N* restrict b, PREFIX##vec##N* restrict dest); \
    int64_t PREFIX##vec##N##_dot(const PREFIX##vec##N* restrict a, const PREFIX##vec##N* restrict b); \
    void PREFIX##vec##N##_scale(const PREFIX##vec##N* restrict src, TYPE value, PREFIX##vec##N* restrict dest); \
    void PREFIX##vec##N##_copy(const PREFIX##vec##N* restrict src, PREFIX##vec##N* restrict dest);

#define VEC3_EX_FUNCTION_DECLARATION(PREFIX, TYPE, N) \
        void PREFIX##vec##N##_cross(const PREFIX##vec##N* restrict a, const PREFIX##vec##N* restrict b, PREFIX##vec##N* restrict dest);

SDS_FOR_EACH_TYPE_EX(VEC_FUNCTION_DECLARATION, 2)
SDS_FOR_EACH_TYPE_EX(VEC_FUNCTION_DECLARATION, 3)
SDS_FOR_EACH_TYPE_EX(VEC3_EX_FUNCTION_DECLARATION, 3)
SDS_FOR_EACH_TYPE_EX(VEC_FUNCTION_DECLARATION, 4)

#endif // VEC_IMPLEMENTATION