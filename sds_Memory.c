#include "sds.h"
#include "stdlib.h"

#define ARENA_ALLIGNMENT 16
typedef struct OverflowBlock OverflowBlock;
struct OverflowBlock {
    void* memory;
    OverflowBlock* next;
};

typedef struct Arena Arena;
struct Arena {
    uint8_t* memory;
    size_t size;
    size_t offset;
    OverflowBlock* overflowHead;
};

Arena* arena_create(size_t capacity) {
    size_t newCapacity = alignUpPow2(capacity, ARENA_ALLIGNMENT);
    Arena* arena = (Arena*)malloc(sizeof(Arena) + capacity);
    if (!arena)
        return NULL;;

    arena->size = newCapacity;
    arena->overflowHead = NULL;
    arena->offset = 0;
    arena->memory = (uint8_t*)arena + sizeof(Arena);
    return arena;
}

void arena_destroy(Arena* arena) {
    if (!arena)
        return;

    arena_reset(arena);
    free(arena);
}

void* arena_alloc(Arena* arena, size_t size, size_t alignment) {
    if (!arena)
        return NULL;

    if (!isPow2(alignment))
        alignment = 2;

    uintptr_t memory = (uintptr_t)(arena->memory + arena->offset);
    size_t padding = alignUpPadPow2(memory, alignment);
    size_t totalSize = padding + size;

    if (totalSize > arena->size - arena->offset) {
        size_t overflowPadding = alignUpPadPow2(sizeof(OverflowBlock), alignment);
        void* blockStart = malloc(sizeof(OverflowBlock) + overflowPadding + size);
        if (!blockStart)
            return NULL;
        void* newMemory = (uint8_t*)blockStart + sizeof(OverflowBlock) + overflowPadding;
        OverflowBlock* tracker = (OverflowBlock*)blockStart;
        tracker->memory = blockStart;
        tracker->next = arena->overflowHead;
        arena->overflowHead = tracker;
        return newMemory;
    }

    arena->offset += totalSize;
    return (void*)(memory + padding);
}

void arena_reset(Arena* arena) {
    if (!arena)
        return;

    OverflowBlock* current = arena->overflowHead;
    while (current) {
        OverflowBlock* next = current->next;
        free(current->memory);
        current = next;
    }
    arena->overflowHead = NULL;
    arena->offset = 0;
}

size_t arena_get_state(Arena* arena) {
    return (arena) ? arena->offset : 0;
}

void arena_rewind(Arena* arena, size_t previousState) {
    if (arena && previousState <= arena->offset)
        arena->offset = previousState;
}