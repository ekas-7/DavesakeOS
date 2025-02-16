// src/memory/memory.c
#include "memory.h"

// Start of heap
static uint32_t heap_start = 0x100000;  // 1MB mark
static uint32_t heap_end = 0x200000;    // 2MB mark

// Memory block header
typedef struct block_header {
    size_t size;
    uint8_t is_free;
    struct block_header* next;
} block_header_t;

static block_header_t* first_block = NULL;

void memory_init() {
    first_block = (block_header_t*)heap_start;
    first_block->size = heap_end - heap_start - sizeof(block_header_t);
    first_block->is_free = 1;
    first_block->next = NULL;
}

void* memset(void* dest, uint8_t val, size_t count) {
    uint8_t* temp = (uint8_t*)dest;
    for (; count != 0; count--) *temp++ = val;
    return dest;
}

void* memcpy(void* dest, const void* src, size_t count) {
    const uint8_t* sp = (const uint8_t*)src;
    uint8_t* dp = (uint8_t*)dest;
    for (; count != 0; count--) *dp++ = *sp++;
    return dest;
}

uint32_t* malloc(size_t size) {
    block_header_t* current = first_block;
    
    while (current) {
        if (current->is_free && current->size >= size) {
            // Split block if too large
            if (current->size > size + sizeof(block_header_t) + 4) {
                block_header_t* new_block = (block_header_t*)((uint32_t)current + sizeof(block_header_t) + size);
                new_block->size = current->size - size - sizeof(block_header_t);
                new_block->is_free = 1;
                new_block->next = current->next;
                
                current->size = size;
                current->next = new_block;
            }
            
            current->is_free = 0;
            return (uint32_t*)((uint32_t)current + sizeof(block_header_t));
        }
        current = current->next;
    }
    
    return NULL;  // No free block found
}

void free(void* ptr) {
    if (!ptr) return;
    
    block_header_t* header = (block_header_t*)((uint32_t)ptr - sizeof(block_header_t));
    header->is_free = 1;
    
    // Merge with next block if it's free
    if (header->next && header->next->is_free) {
        header->size += sizeof(block_header_t) + header->next->size;
        header->next = header->next->next;
    }
}