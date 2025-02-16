// src/kernel/kernel.c
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY (uint16_t*)0xB8000

void kmain(void) {
    uint16_t* const video_memory = VGA_MEMORY;
    const char* msg = "Welcome to DaveOS!";
    const uint8_t color = 0x0F; // White on black

    // Clear screen
    for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = (color << 8) | ' ';
    }

    // Print message
    int i = 0;
    while(msg[i] != '\0') {
        video_memory[i] = (color << 8) | msg[i];
        i++;
    }
}