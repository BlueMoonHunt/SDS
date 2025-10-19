#ifndef SDS_IMPLEMENTATION
#define SDS_IMPLEMENTATION

#include "stdint.h"
#include "stdalign.h"

#define alignUpPow2(x,b)   (((x) + (b) - 1)&(~((b) - 1)))
#define alignDownPow2(x,b) ((x)&(~((b) - 1)))
#define alignUpPadPow2(x,b)  ((-(intptr_t)(x)) & ((b) - 1))
#define isPow2(x)          ((x)!=0 && ((x)&((x)-1))==0)
#define isPow2OrZero(x)    ((((x) - 1)&(x)) == 0)

#define extractBit(word, idx) (((word) >> (idx)) & 1)
#define extract8(word, pos)   (((word) >> ((pos) << 3)) & UINT8_MAX)
#define extract16(word, pos)  (((word) >> ((pos) << 4)) & UINT16_MAX)
#define extract32(word, pos)  (((word) >> ((pos) << 5)) & UINT32_MAX)

#define kb(value) ((value) * 1024)
#define mb(value) ((value) * 1024 * kb(1))
#define gb(value) ((value) * 1024 * mb(1))
#define tb(value) ((value) * 1024 * gb(1))

#define vecGen(type, prefix) \
    typedef struct { \
        union { \
            struct { type x, y; }; \
            struct { type r, g; }; \
        }; \
    } prefix##vec2; \
\
    typedef struct { \
        union { \
            struct { type x, y, z; }; \
            struct { type r, g, b; }; \
        }; \
    } prefix##vec3; \
\
    typedef struct { \
        union { \
            struct { type x, y, z, w; }; \
            struct { type r, g, b, a; }; \
        }; \
    } prefix##vec4;

vecGen(int32_t, i8)
vecGen(int16_t, i16)
vecGen(int32_t, i32)
vecGen(int64_t, i64)
vecGen(uint32_t, u8)
vecGen(uint16_t, u16)
vecGen(uint32_t, u32)
vecGen(uint64_t, u64)
vecGen(float, f32)
vecGen(double, f64)

typedef enum ArenaFlag ArenaFlag;
enum ArenaFlag {
    ArenaFlag_none,
    ArenaFlag_resize_on_overflow
};

typedef struct OverflowBlock OverflowBlock;
typedef struct Arena Arena;

extern Arena* arena_create(size_t capacity, ArenaFlag flag);
extern void arena_destroy(Arena* arena);
extern void* arena_alloc(Arena* arena, size_t size, size_t alignment);
extern void arena_reset(Arena* arena);
extern size_t arena_get_state(Arena* arena);
extern void arena_rewind(Arena* arena, size_t previousState);


// DATA STRUCTURES BUILD TO WORK WITH ARENAS
// TODO : vector, map, unordered_map
// InProgress : string
// DONE :

// STRING
#define STRING_BLOCK_SIZE 32

typedef struct ustring ustring;
typedef struct wstring wstring;

extern ustring* ustring_create(const char* text, Arena* arena);
extern ustring* ustring_create_with_capacity(const size_t size, Arena* arena);
extern const char* ustring_to_cstr(const ustring* text);
extern size_t ustring_size(const ustring* text);
extern size_t ustring_capacity(const ustring* text);
extern ustring* ustring_concat(const ustring* text1, const ustring* text2, Arena* arena);
extern void ustring_append(ustring** destination, const ustring* source, Arena* arena);
extern void ustring_append_cstr(ustring** destination, const char* cstr, Arena* arena);
extern _Bool ustring_equals(const ustring* text1, const ustring* text2);

extern wstring* wstring_create(const wchar_t* text, Arena* arena);
extern wstring* wstring_create_with_capacity(const size_t size, Arena* arena);
extern const wchar_t* wstring_to_wstr(const wstring* text);
extern size_t wstring_size(const wstring* text);
extern size_t wstring_capacity(const wstring* text);
extern wstring* wstring_concat(const wstring* text1, const wstring* text2, Arena* arena);
extern void wstring_append(wstring** destination, const wstring* source, Arena* arena);
extern void wstring_append_wstr(wstring** destination, const wchar_t* wstr, Arena* arena);
extern _Bool wstring_equals(const wstring* text1, const wstring* text2);


typedef struct _gvec _gvec;
_gvec* _gvec_create(size_t element_size, Arena* arena);
void _gvec_reserve(_gvec** vec, size_t size, size_t element_size, Arena* arena);


#define vectorGen(type,prefix) \
    typedef _gvec prefix##vec; \
    static inline prefix##vec* prefix##vec_create(Arena* arena) { \
        return (prefix##vec*)_gvec_create(sizeof(type), Arena* arena); \
    } \
    static inline void prefix##vec_resize(prefix##vec** vec, size_t size, Arena* arena) {\
        return _gvec_resize((_gvec**)vec, size, sizeof(type), arena); \
    }

#endif // SDS_IMPLEMENTATION