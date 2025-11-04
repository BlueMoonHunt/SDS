#ifndef COMMON_IMPLEMENTATION
#define COMMON_IMPLEMENTATION
#include <math.h>
#include <stdint.h>

#define SDS_FOR_EACH_TYPE_EX(FUNCTION, ...) \
    FUNCTION(i8,  int8_t, __VA_ARGS__)   \
    FUNCTION(i16, int16_t, __VA_ARGS__)  \
    FUNCTION(i32, int32_t, __VA_ARGS__)  \
    FUNCTION(i64, int64_t, __VA_ARGS__)  \
    FUNCTION(u8,  uint8_t, __VA_ARGS__)  \
    FUNCTION(u16, uint16_t, __VA_ARGS__) \
    FUNCTION(u32, uint32_t, __VA_ARGS__) \
    FUNCTION(u64, uint64_t, __VA_ARGS__) \
    FUNCTION(f32, float, __VA_ARGS__)    \
    FUNCTION(f64, double, __VA_ARGS__)

#define SDS_FOR_EACH_TYPE(FUNCTION) \
    FUNCTION(i8,  int8_t)   \
    FUNCTION(i16, int16_t)  \
    FUNCTION(i32, int32_t)  \
    FUNCTION(i64, int64_t)  \
    FUNCTION(u8,  uint8_t)  \
    FUNCTION(u16, uint16_t) \
    FUNCTION(u32, uint32_t) \
    FUNCTION(u64, uint64_t) \
    FUNCTION(f32, float)    \
    FUNCTION(f64, double)


#endif // COMMON_IMPLEMENTATION