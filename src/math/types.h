#ifndef SDS_MATH_TYPES_IMPLEMENTATION
#define SDS_MATH_TYPES_IMPLEMENTATION
#include <stdint.h>
#include "common.h"

#define VEC(PREFIX, TYPE) \
    typedef TYPE PREFIX##vec2##_t[2]; \
    typedef union PREFIX##vec2 PREFIX##vec2; \
    union PREFIX##vec2 { \
        PREFIX##vec2##_t raw; \
        struct { TYPE x, y; }; \
        struct { TYPE r, g; }; \
        struct { TYPE u, v; }; \
    }; \
    typedef TYPE PREFIX##vec3##_t[3]; \
    typedef union PREFIX##vec3 PREFIX##vec3; \
    union PREFIX##vec3 { \
        PREFIX##vec3##_t raw; \
        struct { TYPE x, y, z; }; \
        struct { TYPE r, g, b; }; \
    }; \
    typedef TYPE PREFIX##vec4##_t[4]; \
    typedef union PREFIX##vec4 PREFIX##vec4; \
    union PREFIX##vec4 { \
        PREFIX##vec4##_t raw; \
        struct { TYPE x, y, z, w; }; \
        struct { TYPE r, g, b, a; }; \
    };

SDS_FOR_EACH_TYPE(VEC)


#define MATRIX(PREFIX, TYPE, ROWS, COLS) \
typedef PREFIX##vec##COLS##_t PREFIX##mat##ROWS##x##COLS##_t[ROWS]; \
typedef union PREFIX##mat##ROWS##x##COLS PREFIX##mat##ROWS##x##COLS; \
union PREFIX##mat##ROWS##x##COLS { \
    PREFIX##mat##ROWS##x##COLS##_t raw; \
    PREFIX##vec##ROWS column[COLS]; \
};

SDS_FOR_EACH_TYPE_EX(MATRIX, 2, 2)
SDS_FOR_EACH_TYPE_EX(MATRIX, 2, 3)
SDS_FOR_EACH_TYPE_EX(MATRIX, 2, 4)
SDS_FOR_EACH_TYPE_EX(MATRIX, 3, 2)
SDS_FOR_EACH_TYPE_EX(MATRIX, 3, 3)
SDS_FOR_EACH_TYPE_EX(MATRIX, 3, 4)
SDS_FOR_EACH_TYPE_EX(MATRIX, 4, 2)
SDS_FOR_EACH_TYPE_EX(MATRIX, 4, 3)
SDS_FOR_EACH_TYPE_EX(MATRIX, 4, 4)

#endif // SDS_MATH_TYPES_IMPLEMENTATION