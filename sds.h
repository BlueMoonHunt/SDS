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

#define max(a,b) ((a) > (b) ? (a) : (b))

#define VEC_GEN(type, prefix) \
    typedef struct { type x; type y; } prefix##vec2; \
    typedef struct { type x; type y; type z; } prefix##vec3; \
    typedef struct { type x; type y; type z; type w; } prefix##vec4;

VEC_GEN(int32_t, i8);
VEC_GEN(int16_t, i16);
VEC_GEN(int32_t, i32);
VEC_GEN(int64_t, i64);

VEC_GEN(uint32_t, u8);
VEC_GEN(uint16_t, u16);
VEC_GEN(uint32_t, u32);
VEC_GEN(uint64_t, u64);

VEC_GEN(float, f32);
VEC_GEN(double, f64);

typedef struct OverflowBlock OverflowBlock;
typedef struct Arena Arena;

// Creation & destinationruction
extern Arena* arena_create(size_t capacity);
extern void arena_destinationroy(Arena* arena);
// Allocation
extern void* arena_alloc(Arena* arena, size_t size, size_t alignment);
// Reset
extern void arena_reset(Arena* arena);
// TODO : STATUS OF ARENA
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
extern const char* ustring_to_cstr(const ustring* text);
extern size_t ustring_size(const ustring* text);
extern size_t ustring_capacity(const ustring* text);
extern ustring* ustring_concat(const ustring* text1, const ustring* text2, Arena* arena);
extern void ustring_append(ustring* destination, const ustring* source, Arena* arena);
extern void ustring_append_cstr(ustring* destination, const char* cstr, Arena* arena);
extern _Bool ustring_equals(const ustring* text1, const ustring* text2);

extern wstring* wstring_create(const wchar_t* text, Arena* arena);
extern const wchar_t* wstring_to_wstr(const wstring* text);
extern size_t wstring_size(const wstring* text);
extern size_t wstring_capacity(const wstring* text);
extern wstring* wstring_concat(const wstring* text1, const wstring* text2, Arena* arena);
extern void wstring_append(wstring* destination, const wstring* source, Arena* arena);
extern void wstring_append_wstr(wstring* destination, const wchar_t* wstr, Arena* arena);
extern _Bool wstring_equals(const wstring* text1, const wstring* text2);


#endif // SDS_IMPLEMENTATION