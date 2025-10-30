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
    size_t overflow_count;
};

Arena* arena_create(size_t capacity, ArenaFlag flags) {
    size_t new_capacity = align_up_pow2(max(capacity, kb(1)), ARENA_ALLIGNMENT);
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

    if (!is_pow2(alignment))
        alignment = ARENA_ALLIGNMENT;

    uintptr_t current_ptr = (uintptr_t)(arena->memory + arena->offset);
    size_t padding = align_up_pad_pow2(current_ptr, alignment);
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
    size_t overflow_padding = align_up_pad_pow2((uintptr_t)user_memory_unaligned, alignment);
    void* user_memory = (uint8_t*)user_memory_unaligned + overflow_padding;
    OverflowBlock* tracker = (OverflowBlock*)block_start;
    tracker->next = arena->overflow_head;
    arena->overflow_head = tracker;
    arena->overflow_size += size;
    arena->overflow_count += 1;

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
    arena->overflow_count = 0;

    arena->offset = 0;

    if (arena->flags & ArenaFlag_Resize_On_Overflow && arena->overflow_size > 0) {
        size_t new_size = arena->size + arena->overflow_size;
        uint8_t* memory = (uint8_t*)realloc(arena->memory, new_size);

        if (memory) {
            arena->memory = memory;
            arena->size = new_size;
        }
    }

    arena->overflow_size = 0;
}

typedef struct ArenaState ArenaState;
struct ArenaState {
    size_t arena_offset;
    OverflowBlock* overflow_head;
    size_t overflow_count;
};

ArenaState* arena_get_state(Arena* arena) {
    size_t offset = arena->offset;
    void* head = arena->overflow_head;
    size_t count = arena->overflow_count;

    ArenaState* state = (ArenaState*)arena_alloc(arena, sizeof(ArenaState), alignof(ArenaState));
    state->arena_offset = offset;
    state->overflow_head = head;
    state->overflow_count = count;

    return state;
}

void arena_rewind(Arena* arena, ArenaState* previous_state) {
    arena->offset = previous_state->arena_offset;
    while (previous_state->overflow_count < arena->overflow_count) {
        OverflowBlock* to_free = arena->overflow_head;
        arena->overflow_head = arena->overflow_head->next;
        free(to_free);
        previous_state->overflow_count--;
    }
}