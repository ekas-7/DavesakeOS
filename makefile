CC = i686-elf-gcc
ASM = nasm

CFLAGS = -ffreestanding -O2 -nostdlib -nostartfiles -Wall -Wextra
ASMFLAGS = -f bin

SRC_DIR = src
BUILD_DIR = build
OS_IMAGE = $(BUILD_DIR)/os.img

$(shell mkdir -p $(BUILD_DIR))

all: $(OS_IMAGE)

$(BUILD_DIR)/bootloader.bin: $(SRC_DIR)/boot/bootloader.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/kernel.o: $(SRC_DIR)/kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OS_IMAGE): $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.o
	cat $^ > $@

clean:
	rm -f $(BUILD_DIR)/*

run: $(OS_IMAGE)
	qemu-system-i386 -nographic -fda $(OS_IMAGE)

run-sdl: $(OS_IMAGE)
	qemu-system-i386 -display sdl -fda $(OS_IMAGE)

run-curses: $(OS_IMAGE)
	qemu-system-i386 -display curses -fda $(OS_IMAGE)

.PHONY: all clean run run-sdl run-curses