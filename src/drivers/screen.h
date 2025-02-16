// src/drivers/screen.h
#ifndef SCREEN_H
#define SCREEN_H

#include "../include/types.h"

// Screen dimensions
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// Colors
#define VGA_BLACK 0
#define VGA_BLUE 1
#define VGA_GREEN 2
#define VGA_CYAN 3
#define VGA_RED 4
#define VGA_MAGENTA 5
#define VGA_BROWN 6
#define VGA_LIGHT_GREY 7
#define VGA_DARK_GREY 8
#define VGA_LIGHT_BLUE 9
#define VGA_LIGHT_GREEN 10
#define VGA_LIGHT_CYAN 11
#define VGA_LIGHT_RED 12
#define VGA_LIGHT_MAGENTA 13
#define VGA_LIGHT_BROWN 14
#define VGA_WHITE 15

// Screen functions
void init_screen();
void clear_screen();
void print_char(char c, uint8_t color);
void print_string(const char* str);
void print_string_colored(const char* str, uint8_t color);
void set_cursor(int x, int y);
void backspace();
void newline();

#endif

