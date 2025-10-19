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


typedef struct _gvec _gvec;
_gvec* _gvec_create(size_t element_size, size_t alignment, Arena* arena);
void _gvec_reserve(_gvec* vec, size_t new_capacity, Arena* arena);
void _gvec_emplace_back(_gvec* vec, const uint8_t* data, Arena* arena);
void _gvec_pop_back(_gvec* vec);
void _gvec_clear(_gvec* vec);
uint8_t* _gvec_data(_gvec* vec);
size_t _gvec_size(_gvec* vec);
size_t _gvec_capacity(_gvec* vec);
uint8_t* _gvec_front(const _gvec* vec);
uint8_t* _gvec_back(const _gvec* vec);

#define dynVecGen(type, prefix) \
    typedef _gvec prefix##vec; \
    static inline prefix##vec* prefix##vec_create(Arena* arena) { return (_gvec*)_gvec_create(sizeof(type), alignof(type), arena); } \
    static inline void prefix##vec_reserve(prefix##vec* vec, size_t size, Arena* arena) { _gvec_reserve((_gvec*)vec, size, arena); } \
    static inline void prefix##vec_push(prefix##vec* vec, type data, Arena* arena) { _gvec_emplace_back((_gvec*)vec, (uint8_t*)&data, arena); } \
    static inline void prefix##vec_emplace_back(prefix##vec* vec, type* data_ptr, Arena* arena) { _gvec_emplace_back((_gvec*)vec, (uint8_t*)data_ptr, arena); } \
    static inline void prefix##vec_pop_back(prefix##vec* vec) { _gvec_pop_back((_gvec*)vec); } \
    static inline void prefix##vec_clear(prefix##vec* vec) { _gvec_clear((_gvec*)vec); } \
    static inline type* prefix##vec_data(prefix##vec* vec) { return (type*)_gvec_data((_gvec*)vec); } \
    static inline size_t prefix##vec_size(prefix##vec* vec) { return _gvec_size((_gvec*)vec); } \
    static inline size_t prefix##vec_capacity(prefix##vec* vec) { return _gvec_capacity((_gvec*)vec); } \
    static inline type* prefix##vec_front(prefix##vec* vec) { return (type*)_gvec_front((_gvec*)vec); } \
    static inline type* prefix##vec_back(prefix##vec* vec) { return (type*)_gvec_back((_gvec*)vec); } \
    static inline type* prefix##vec_at(prefix##vec* vec, size_t index) { \
        assert(index < _gvec_size(vec) && "Vector access out of bounds");\
         return &(((type*)_gvec_data(vec))[index]); } \
    static inline type* prefix##vec_idx(prefix##vec* vec, size_t index) { return &(((type*)_gvec_data(vec))[index]); }


dynVecGen(int8_t, i8)
dynVecGen(int16_t, i16)
dynVecGen(int32_t, i32)
dynVecGen(int64_t, i64)
dynVecGen(uint8_t, u8)
dynVecGen(uint16_t, u16)
dynVecGen(uint32_t, u32)
dynVecGen(uint64_t, u64)
dynVecGen(float, f32)
dynVecGen(double, f64)
dynVecGen(ustring*, ustr)
dynVecGen(wstring*, wstr)

#endif // SDS_IMPLEMENTATION