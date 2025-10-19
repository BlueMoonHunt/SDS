#include "sds.h"
#include <stdlib.h>

#ifndef ARENA_ALLIGNMENT
#define ARENA_ALLIGNMENT 16
#endif

typedef struct OverflowBlock OverflowBlock;
struct OverflowBlock {
    OverflowBlock* next;
};

typedef struct Arena Arena;
struct Arena {
    ArenaFlag flags;

    uint8_t* memory;
    size_t size;
    size_t offset;
    OverflowBlock* overflow_head;
    size_t overflow_size;
};

Arena* arena_create(size_t capacity, ArenaFlag flags) {
    size_t new_capacity = alignUpPow2(max(capacity, kb(1)), ARENA_ALLIGNMENT);
    Arena* arena = (Arena*)malloc(sizeof(Arena));
    if (!arena)
        return NULL;

    uint8_t* memory = (uint8_t*)malloc(new_capacity);
    if (!memory) {
        free(arena);
        return NULL;
    }

    arena->flags = flags;
    arena->memory = memory;
    arena->size = new_capacity;
    arena->offset = 0;
    arena->overflow_head = NULL;
    arena->overflow_size = 0;

    return arena;
}

void arena_destroy(Arena* arena) {
    if (!arena)
        return;

    arena_reset(arena);
    free(arena->memory);
    free(arena);
}

void* arena_alloc(Arena* arena, size_t size, size_t alignment) {
    if (!arena || size == 0)
        return NULL;

    if (!isPow2(alignment))
        alignment = ARENA_ALLIGNMENT;

    uintptr_t current_ptr = (uintptr_t)(arena->memory + arena->offset);
    size_t padding = alignUpPadPow2(current_ptr, alignment);
    size_t required_size = padding + size;

    if (arena->offset + required_size <= arena->size) {
        void* user_ptr = (void*)(current_ptr + padding);
        arena->offset += required_size;
        return user_ptr;
    }

    size_t total_alloc_size = sizeof(OverflowBlock) + size + alignment - 1;
    void* block_start = malloc(total_alloc_size);
    if (!block_start)
        return NULL;

    void* user_memory_unaligned = (uint8_t*)block_start + sizeof(OverflowBlock);
    size_t overflow_padding = alignUpPadPow2((uintptr_t)user_memory_unaligned, alignment);
    void* user_memory = (uint8_t*)user_memory_unaligned + overflow_padding;
    OverflowBlock* tracker = (OverflowBlock*)block_start;
    tracker->next = arena->overflow_head;
    arena->overflow_head = tracker;
    arena->overflow_size += size;

    return user_memory;
}

void arena_reset(Arena* arena) {
    if (!arena)
        return;

    OverflowBlock* current = arena->overflow_head;
    while (current) {
        OverflowBlock* to_free = current;
        current = current->next;
        free(to_free);
    }
    arena->overflow_head = NULL;

    arena->offset = 0;

    if (arena->flags & ArenaFlag_resize_on_overflow && arena->overflow_size > 0) {
        size_t new_size = arena->size + arena->overflow_size;
        uint8_t* memory = (uint8_t*)realloc(arena->memory, new_size);

        if (memory) {
            arena->memory = memory;
            arena->size = new_size;
        }
    }

    arena->overflow_size = 0;
}

size_t arena_get_state(Arena* arena) {
    return (arena) ? arena->offset : 0;
}

void arena_rewind(Arena* arena, size_t previous_state) {
    if (arena && previous_state <= arena->offset)
        arena->offset = previous_state;
}