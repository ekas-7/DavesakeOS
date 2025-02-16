// src/kernel/kernel.c
#include "../memory/memory.h"
#include "../drivers/screen.h"

// Global variables for screen management
static uint16_t* video_memory = (uint16_t*)0xB8000;
static int cursor_x = 0;
static int cursor_y = 0;

// Function prototypes
void test_memory();
void test_screen();
void print_memory_info();
void print_centered(const char* str, uint8_t color);
void update_cursor();

// Port I/O functions
static void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

void kmain(void) {
    // Initialize systems
    memory_init();
    init_screen();
    
    // Welcome message
    clear_screen();
    print_centered("Welcome to DaveOS!", VGA_GREEN);
    print_string("\n\n");
    
    // Test memory allocation and screen output
    test_memory();
    print_string("\n");
    test_screen();
    
    // Print memory information
    print_memory_info();
    
    // Position cursor at bottom
    set_cursor(0, VGA_HEIGHT - 2);
    print_string_colored("Press any key to start...", VGA_LIGHT_GREY);
}

void test_memory() {
    print_string_colored("Testing Memory Allocation:\n", VGA_CYAN);
    
    // Allocate and test multiple memory blocks
    char* str1 = (char*)malloc(20);
    char* str2 = (char*)malloc(30);
    char* str3 = (char*)malloc(25);
    
    if (str1 && str2 && str3) {
        memcpy(str1, "First Block", 11);
        memcpy(str2, "Second Memory Block", 18);
        memcpy(str3, "Third Test Block", 15);
        
        print_string("  - ");
        print_string(str1);
        print_string("\n  - ");
        print_string(str2);
        print_string("\n  - ");
        print_string(str3);
        print_string("\n");
        
        // Free memory
        free(str1);
        free(str2);
        free(str3);
        
        print_string_colored("Memory test completed successfully!\n", VGA_GREEN);
    } else {
        print_string_colored("Memory allocation failed!\n", VGA_RED);
    }
}

void test_screen() {
    print_string_colored("\nTesting Screen Functions:\n", VGA_CYAN);
    
    // Test different colors
    print_string_colored("Color Test:\n", VGA_WHITE);
    print_string_colored("  Red Text\n", VGA_RED);
    print_string_colored("  Blue Text\n", VGA_BLUE);
    print_string_colored("  Green Text\n", VGA_GREEN);
    print_string_colored("  Yellow Text\n", VGA_LIGHT_BROWN);
    
    // Test cursor positioning
    set_cursor(40, 12);
    print_string_colored("Positioned Text\n", VGA_MAGENTA);
}

void print_memory_info() {
    print_string_colored("\nMemory Information:\n", VGA_CYAN);
    print_string("  Total Memory: XXXXX bytes\n");
    print_string("  Free Memory: XXXXX bytes\n");
    print_string("  Used Memory: XXXXX bytes\n");
}

void print_centered(const char* str, uint8_t color) {
    int len = 0;
    const char* temp = str;
    while (*temp != 0) {
        len++;
        temp++;
    }
    
    int x = (VGA_WIDTH - len) / 2;
    set_cursor(x, cursor_y);
    print_string_colored(str, color);
}

// Update hardware cursor
void update_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

void clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            uint16_t* location = video_memory + (y * VGA_WIDTH + x);
            *location = (VGA_BLACK << 4) | (VGA_LIGHT_GREY & 0x0F) << 8 | ' ';
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}





// v2 of kernel 

// // src/kernel/kernel.c
// #include "../include/types.h"

// // VGA hardware addresses and constants
// static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;
// #define VGA_WIDTH 80
// #define VGA_HEIGHT 25

// // Colors
// #define BLACK 0
// #define WHITE 15
// #define DEFAULT_COLOR ((BLACK << 4) | WHITE)

// // Cursor position
// static int cursor_x = 0;
// static int cursor_y = 0;

// // Port I/O functions without using push/pop
// static inline void outb(uint16_t port, uint8_t val) {
//     __asm__ __volatile__("outb %b0, %w1" :: "a"(val), "Nd"(port));
// }

// static inline uint8_t inb(uint16_t port) {
//     uint8_t ret;
//     __asm__ __volatile__("inb %w1, %b0" : "=a"(ret) : "Nd"(port));
//     return ret;
// }

// // Update hardware cursor
// void update_cursor() {
//     uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
//     outb(0x3D4, 14);
//     outb(0x3D5, (pos >> 8) & 0xFF);
//     outb(0x3D4, 15);
//     outb(0x3D5, pos & 0xFF);
// }

// // Screen functions
// void clear_screen() {
//     uint16_t blank = (DEFAULT_COLOR << 8) | ' ';
//     for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
//         VGA_MEMORY[i] = blank;
//     }
//     cursor_x = 0;
//     cursor_y = 0;
//     update_cursor();
// }

// void write_char(char c, uint8_t color, int x, int y) {
//     if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
//         const int index = y * VGA_WIDTH + x;
//         VGA_MEMORY[index] = ((uint16_t)color << 8) | (uint8_t)c;
//     }
// }

// void scroll() {
//     // Move each line up
//     for (int y = 0; y < VGA_HEIGHT - 1; y++) {
//         for (int x = 0; x < VGA_WIDTH; x++) {
//             VGA_MEMORY[y * VGA_WIDTH + x] = VGA_MEMORY[(y + 1) * VGA_WIDTH + x];
//         }
//     }

//     // Clear last line
//     uint16_t blank = (DEFAULT_COLOR << 8) | ' ';
//     for (int x = 0; x < VGA_WIDTH; x++) {
//         VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = blank;
//     }
// }

// void print_char(char c) {
//     if (c == '\n') {
//         cursor_x = 0;
//         cursor_y++;
//     } else {
//         write_char(c, DEFAULT_COLOR, cursor_x, cursor_y);
//         cursor_x++;
//     }

//     if (cursor_x >= VGA_WIDTH) {
//         cursor_x = 0;
//         cursor_y++;
//     }

//     if (cursor_y >= VGA_HEIGHT) {
//         scroll();
//         cursor_y = VGA_HEIGHT - 1;
//     }

//     update_cursor();
// }

// void print_string(const char* str) {
//     for (int i = 0; str[i] != '\0'; i++) {
//         print_char(str[i]);
//     }
// }

// // Kernel entry point
// void kmain(void) {
//     // Initialize screen
//     clear_screen();
    
//     // Print welcome message
//     print_string("DaveOS Kernel Loaded Successfully!\n");
//     print_string("================================\n");
//     print_string("Welcome to DaveOS!\n");
// }