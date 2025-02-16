// src/memory/memory.h
#ifndef MEMORY_H
#define MEMORY_H

#include "../include/types.h"

// Memory management functions
void* memcpy(void* dest, const void* src, size_t count);
void* memset(void* dest, uint8_t val, size_t count);
uint32_t* malloc(size_t size);
void free(void* ptr);

// Memory information
void memory_init();
uint32_t get_memory_size();
void memory_map();

#endif