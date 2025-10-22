#ifndef SDS_IMPLEMENTATION
#define SDS_IMPLEMENTATION

#include <stdint.h>
#include <stdalign.h>
#include <assert.h>

#define alignUpPow2(x,b)   (((x) + (b) - 1)&(~((b) - 1)))
#define alignDownPow2(x,b) ((x)&(~((b) - 1)))
#define alignUpPadPow2(x,b)  ((0-(x)) & ((b) - 1))
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

#define vecImplementation(type, prefix) \
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

vecImplementation(int8_t, i8)
vecImplementation(int16_t, i16)
vecImplementation(int32_t, i32)
vecImplementation(int64_t, i64)
vecImplementation(uint8_t, u8)
vecImplementation(uint16_t, u16)
vecImplementation(uint32_t, u32)
vecImplementation(uint64_t, u64)
vecImplementation(float, f32)
vecImplementation(double, f64)

typedef enum ArenaFlag ArenaFlag;
enum ArenaFlag {
    ArenaFlag_None,
    ArenaFlag_Resize_On_Overflow
};

typedef struct OverflowBlock OverflowBlock;
typedef struct Arena Arena;
typedef struct ArenaState ArenaState;

Arena* arena_create(size_t capacity, ArenaFlag flag);
void arena_destroy(Arena* arena);
void* arena_alloc(Arena* arena, size_t size, size_t alignment);
void arena_reset(Arena* arena);
ArenaState* arena_get_state(Arena* arena);
void arena_rewind(Arena* arena, ArenaState* previousState);

// STRING
typedef struct ustring ustring;
typedef struct wstring wstring;

ustring* ustring_create(const char* text, Arena* arena);
ustring* ustring_create_with_capacity(const size_t size, Arena* arena);
const char* ustring_to_cstr(const ustring* text);
size_t ustring_size(const ustring* text);
size_t ustring_capacity(const ustring* text);
ustring* ustring_concat(const ustring* text1, const ustring* text2, Arena* arena);
void ustring_append(ustring** destination, const ustring* source, Arena* arena);
void ustring_append_cstr(ustring** destination, const char* cstr, Arena* arena);
_Bool ustring_equals(const ustring* text1, const ustring* text2);

wstring* wstring_create(const wchar_t* text, Arena* arena);
wstring* wstring_create_with_capacity(const size_t size, Arena* arena);
const wchar_t* wstring_to_wstr(const wstring* text);
size_t wstring_size(const wstring* text);
size_t wstring_capacity(const wstring* text);
wstring* wstring_concat(const wstring* text1, const wstring* text2, Arena* arena);
void wstring_append(wstring** destination, const wstring* source, Arena* arena);
void wstring_append_wstr(wstring** destination, const wchar_t* wstr, Arena* arena);
_Bool wstring_equals(const wstring* text1, const wstring* text2);

//VECTOR
typedef struct vector vector;

vector* _vector_create(size_t element_size, size_t element_alignment, Arena* arena);
#define vector_create(element_type, arena) _vector_create(sizeof(element_type), alignof(element_type), arena);
void vector_reserve(vector* vec, size_t new_capacity);
void _vector_emplace_back(vector* vec, const void* element, size_t element_size);
#define vector_emplace_back(vector, element) _vector_emplace_back(vector, (element), sizeof(*(element)));
void vector_pop_back(vector* vec);
void vector_clear(vector* vec);
void* vector_data(vector* vec);
size_t vector_size(vector* vec);
size_t vector_capacity(vector* vec);
void* vector_front(const vector* vec);
void* vector_back(const vector* vec);


#endif // SDS_IMPLEMENTATION