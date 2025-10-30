#include "sds.h"
#include <string.h>
#include <wchar.h>
#include <stddef.h>

#define STRING_BLOCK_SIZE 32

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

// --- ustring Implementation ---

ustring* ustring_create_with_capacity(const size_t size, Arena* arena) {
    size_t allocated_elements = align_up_pow2(size + 1, STRING_BLOCK_SIZE);
    ustring* str = (ustring*)arena_alloc(arena, sizeof(ustring) + allocated_elements * sizeof(char), alignof(ustring));

    str->size = size;
    str->capacity = allocated_elements - 1;
    return str;
}

ustring* ustring_create(const char* text, Arena* arena) {
    if (!text)
        text = "";
    size_t size = strlen(text);
    ustring* str = ustring_create_with_capacity(size, arena);
    memcpy(str->data, text, size + 1);
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
    if (!text1)
        return ustring_create(ustring_to_cstr(text2), arena);
    if (!text2)
        return ustring_create(ustring_to_cstr(text1), arena);

    size_t new_size = text1->size + text2->size;
    ustring* new_str = ustring_create_with_capacity(new_size, arena);

    memcpy(new_str->data, text1->data, text1->size);
    memcpy(new_str->data + text1->size, text2->data, text2->size);
    new_str->data[new_size] = '\0';

    return new_str;
}

void ustring_append(ustring** destination, const ustring* source, Arena* arena) {
    if (!destination || !(*destination) || !source)
        return;
    ustring* destination_str = *destination;
    size_t new_size = destination_str->size + source->size;

    if (destination_str->capacity >= new_size) {
        memcpy(destination_str->data + destination_str->size, source->data, source->size + 1);
        destination_str->size = new_size;
    }
    else {
        *destination = ustring_concat(*destination, source, arena);
    }
}

void ustring_append_cstr(ustring** destination, const char* cstr, Arena* arena) {
    if (!cstr)
        return;
    ustring* temp = ustring_create(cstr, arena);
    ustring_append(destination, temp, arena);
}

_Bool ustring_equals(const ustring* text1, const ustring* text2) {
    if (!text1 || !text2)
        return text1 == text2;
    if (text1->size != text2->size)
        return 0;
    return memcmp(text1->data, text2->data, text1->size) == 0;
}

// --- wstring Implementation ---

wstring* wstring_create_with_capacity(const size_t size, Arena* arena) {
    size_t allocated_elements = align_up_pow2(size + 1, STRING_BLOCK_SIZE);

    wstring* wide_str = (wstring*)arena_alloc(arena, sizeof(wstring) + allocated_elements * sizeof(wchar_t), alignof(wstring));

    wide_str->size = size;
    wide_str->capacity = allocated_elements - 1;
    return wide_str;
}

wstring* wstring_create(const wchar_t* text, Arena* arena) {
    if (!text)
        text = L"";
    size_t size = wcslen(text);
    wstring* wide_str = wstring_create_with_capacity(size, arena);

    memcpy(wide_str->data, text, size * sizeof(wchar_t));
    wide_str->data[wide_str->size] = L'\0';
    return wide_str;
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
    if (!text1)
        return wstring_create(wstring_to_wstr(text2), arena);
    if (!text2)
        return wstring_create(wstring_to_wstr(text1), arena);

    size_t new_size = text1->size + text2->size;
    wstring* wide_str = wstring_create_with_capacity(new_size, arena);

    memcpy(wide_str->data, text1->data, text1->size * sizeof(wchar_t));
    memcpy(wide_str->data + text1->size, text2->data, text2->size * sizeof(wchar_t));
    wide_str->data[new_size] = L'\0';

    return wide_str;
}

void wstring_append(wstring** destination, const wstring* source, Arena* arena) {
    if (!destination || !(*destination) || !source)
        return;
    wstring* destination_str = *destination;
    size_t new_size = destination_str->size + source->size;

    if (destination_str->capacity >= new_size) {
        memcpy(destination_str->data + destination_str->size, source->data, (source->size + 1) * sizeof(wchar_t));
        destination_str->size = new_size;
    }
    else {
        *destination = wstring_concat(*destination, source, arena);
    }
}

void wstring_append_wstr(wstring** destination, const wchar_t* wstr, Arena* arena) {
    if (!wstr)
        return;
    wstring* temp = wstring_create(wstr, arena);
    wstring_append(destination, temp, arena);
}

_Bool wstring_equals(const wstring* text1, const wstring* text2) {
    if (!text1 || !text2)
        return text1 == text2;
    if (text1->size != text2->size)
        return 0;

    return wmemcmp(text1->data, text2->data, text1->size) == 0;
}