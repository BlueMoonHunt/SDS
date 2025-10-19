#ifndef SDS_IMPLEMENTATION
#define SDS_IMPLEMENTATION

#include <stdint.h>
#include <stdalign.h>
#include <assert.h>

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

vecGen(int8_t, i8)
vecGen(int16_t, i16)
vecGen(int32_t, i32)
vecGen(int64_t, i64)
vecGen(uint8_t, u8)
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

Arena* arena_create(size_t capacity, ArenaFlag flag);
void arena_destroy(Arena* arena);
void* arena_alloc(Arena* arena, size_t size, size_t alignment);
void arena_reset(Arena* arena);
size_t arena_get_state(Arena* arena);
void arena_rewind(Arena* arena, size_t previousState);


// DATA STRUCTURES BUILD TO WORK WITH ARENAS
// TODO : vector, map, unordered_map
// InProgress : string
// DONE :

// STRING
#define STRING_BLOCK_SIZE 32

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


typedef struct vector vector;
vector* vector_create(size_t element_size, size_t alignment, Arena* arena);
void vector_reserve(vector* vec, size_t new_capacity, Arena* arena);
void vector_emplace_back(vector* vec, const void* data, Arena* arena);
void vector_pop_back(vector* vec);
void vector_clear(vector* vec);
void* vector_data(vector* vec);
size_t vector_size(vector* vec);
size_t vector_capacity(vector* vec);
void* vector_front(const vector* vec);
void* vector_back(const vector* vec);

#define vectorGen(type, prefix) \
    typedef vector prefix##vector; \
    static inline prefix##vector* prefix##vector_create(Arena* arena) { return (vector*)vector_create(sizeof(type), alignof(type), arena); } \
    static inline void prefix##vector_reserve(prefix##vector* vec, size_t size, Arena* arena) { vector_reserve((vector*)vec, size, arena); } \
    static inline void prefix##vector_push(prefix##vector* vec, type data, Arena* arena) { vector_emplace_back((vector*)vec, &data, arena); } \
    static inline void prefix##vector_emplace_back(prefix##vector* vec, type* data_ptr, Arena* arena) { vector_emplace_back((vector*)vec, data_ptr, arena); } \
    static inline void prefix##vector_pop_back(prefix##vector* vec) { vector_pop_back((vector*)vec); } \
    static inline void prefix##vector_clear(prefix##vector* vec) { vector_clear((vector*)vec); } \
    static inline type* prefix##vector_data(prefix##vector* vec) { return (type*)vector_data((vector*)vec); } \
    static inline size_t prefix##vector_size(prefix##vector* vec) { return vector_size((vector*)vec); } \
    static inline size_t prefix##vector_capacity(prefix##vector* vec) { return vector_capacity((vector*)vec); } \
    static inline type* prefix##vector_front(prefix##vector* vec) { return (type*)vector_front((vector*)vec); } \
    static inline type* prefix##vector_back(prefix##vector* vec) { return (type*)vector_back((vector*)vec); } \
    static inline type* prefix##vector_at(prefix##vector* vec, size_t index) { \
        assert(index < vector_size(vec) && "Vector access out of bounds");\
         return &(((type*)vector_data(vec))[index]); } \
    static inline type* prefix##vector_idx(prefix##vector* vec, size_t index) { return &(((type*)vector_data(vec))[index]); }


vectorGen(int8_t, i8)
vectorGen(int16_t, i16)
vectorGen(int32_t, i32)
vectorGen(int64_t, i64)
vectorGen(uint8_t, u8)
vectorGen(uint16_t, u16)
vectorGen(uint32_t, u32)
vectorGen(uint64_t, u64)
vectorGen(float, f32)
vectorGen(double, f64)
vectorGen(ustring*, ustr)
vectorGen(wstring*, wstr)

#endif // SDS_IMPLEMENTATION