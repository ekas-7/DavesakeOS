// src/drivers/screen.c
#include "screen.h"

// Current cursor position
static int cursor_x = 0;
static int cursor_y = 0;

// VGA memory pointer
static uint16_t* video_memory = (uint16_t*)VGA_MEMORY;

// Port I/O functions
static void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static uint8_t inb(uint16_t port) {
    uint8_t value;
    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// Update hardware cursor
static void update_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

void init_screen() {
    cursor_x = 0;
    cursor_y = 0;
    clear_screen();
    update_cursor();
}

void clear_screen() {
    uint16_t blank = 0x20 | (VGA_LIGHT_GREY << 8);  // Space character with light grey
    
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = blank;
    }
    
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

// Scroll the screen up one line
static void scroll() {
    if (cursor_y >= VGA_HEIGHT) {
        // Move each line up
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            video_memory[i] = video_memory[i + VGA_WIDTH];
        }
        
        // Clear the last line
        uint16_t blank = 0x20 | (VGA_LIGHT_GREY << 8);
        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            video_memory[i] = blank;
        }
        
        cursor_y = VGA_HEIGHT - 1;
    }
}

void print_char(char c, uint8_t color) {
    uint16_t attrib = color << 8;
    
    if (c == '\n' || c == '\r') {
        newline();
    }
    else if (c == '\b') {
        backspace();
    }
    else {
        video_memory[cursor_y * VGA_WIDTH + cursor_x] = c | attrib;
        cursor_x++;
        
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    
    scroll();
    update_cursor();
}

void print_string(const char* str) {
    print_string_colored(str, VGA_LIGHT_GREY);
}

void print_string_colored(const char* str, uint8_t color) {
    while (*str) {
        print_char(*str++, color);
    }
}

void set_cursor(int x, int y) {
    cursor_x = (x >= VGA_WIDTH) ? VGA_WIDTH - 1 : x;
    cursor_y = (y >= VGA_HEIGHT) ? VGA_HEIGHT - 1 : y;
    update_cursor();
}

void backspace() {
    if (cursor_x > 0) {
        cursor_x--;
    } else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    }
    
    video_memory[cursor_y * VGA_WIDTH + cursor_x] = 0x20 | (VGA_LIGHT_GREY << 8);
    update_cursor();
}

void newline() {
    cursor_x = 0;
    cursor_y++;
    scroll();
    update_cursor();
}