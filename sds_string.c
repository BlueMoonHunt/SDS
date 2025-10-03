#include "sds.h"
#include <string.h>
#include <wchar.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct ustring ustring;
struct ustring {
    size_t size;
    size_t capacity;
    char data[];
};

typedef struct wstring wstring;
struct wstring {
    size_t size;
    size_t capacity;
    wchar_t data[];
};

ustring* ustring_create(const char* text, Arena* arena) {
    if (!text)
        text = "";
    size_t size_in_bytes = strlen(text);
    size_t capacity_in_bytes = alignUpPow2(size_in_bytes + 1, STRING_BLOCK_SIZE);

    ustring* str = (ustring*)arena_alloc(arena, sizeof(ustring) + capacity_in_bytes, 1);
    if (!str)
        return NULL;

    str->size = size_in_bytes;
    str->capacity = capacity_in_bytes;
    strcpy(str->data, text);
    return str;
}

const char* ustring_to_cstr(const ustring* text) {
    return text ? text->data : NULL;
}

size_t ustring_size(const ustring* text) {
    return (text) ? text->size : 0;
}

size_t ustring_capacity(const ustring* text) {
    return (text) ? text->capacity : 0;
}

ustring* ustring_concat(const ustring* text1, const ustring* text2, Arena* arena) {
    if (!text1 || !text2)
        return ustring_create("", arena);
    size_t size = text1->size + text2->size + 1;
    char* str = (char*)arena_alloc(arena, size, 1);

    memcpy(str, text1->data, text1->size);
    memcpy(str, text2->data, text2->size);
    str[size] = '\0';

    return ustring_create(str, arena);
}

_Bool ustring_equals(const ustring* text1, const ustring* text2) {
    return (text1 && text2 && text1->size == text2->size && strcmp(text1->data, text2->data) == 0);
}

wstring* wstring_create(const wchar_t* text, Arena* arena) {
    if (!text)
        text = L"";
    size_t len = wcslen(text);
    size_t size_in_bytes = len * sizeof(wchar_t);
    size_t capacity_in_bytes = alignUpPow2(size_in_bytes + sizeof(wchar_t), STRING_BLOCK_SIZE);
#ifdef _WIN32
    wstring* str = (wstring*)arena_alloc(arena, sizeof(wstring) + capacity_in_bytes, 2);
#else
    wstring* str = (wstring*)arena_alloc(arena, sizeof(wstring) + capacity_in_bytes, 4);
#endif
    if (!str)
        return NULL;

    str->size = size_in_bytes;
    str->capacity = capacity_in_bytes;
    wcscpy(str->data, text);
    return str;
}

const wchar_t* wstring_to_wstr(const wstring* text) {
    return text ? text->data : NULL;
}

size_t wstring_size(const wstring* text) {
    return (text) ? text->size : 0;
}

size_t wstring_capacity(const wstring* text) {
    return (text) ? text->capacity : 0;
}

wstring* wstring_concat(const wstring* text1, const wstring* text2, Arena* arena) {
    if (!text1 || !text2)
        return wstring_create(L"", arena);
    size_t size = text1->size + text2->size + 1;
    wchar_t* str = (wchar_t*)arena_alloc(arena, size * sizeof(wchar_t), 2);

    memcpy(str, text1->data, text1->size * sizeof(wchar_t));
    memcpy(str, text2->data, text2->size * sizeof(wchar_t));
    str[size] = L'\0';

    return wstring_create(str, arena);
}

_Bool wstring_equals(const wstring* text1, const wstring* text2) {
    return (text1 && text2 && text1->size == text2->size && wcscmp(text1->data, text2->data) == 0);
}
