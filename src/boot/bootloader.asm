[org 0x7c00]
[bits 16]

; Set up segments and stack
cli                     
mov ax, 0x0000
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00
sti                     

; Initialize video mode
mov ah, 0x00
mov al, 0x03    ; 80x25 text mode
int 0x10

; Clear screen first
call clear_screen

; Print welcome message
mov si, WELCOME_MSG
call print_string

; Main command loop
command_loop:
    mov si, PROMPT
    call print_string
    
    ; Read command into buffer
    mov di, command_buffer
    call read_line
    
    ; Parse command
    mov si, command_buffer
    
    ; Check commands
    mov di, cmd_help
    call strcmp
    jc do_help
    
    mov di, cmd_clear
    call strcmp
    jc do_clear
    
    mov di, cmd_reboot
    call strcmp
    jc do_reboot
    
    ; Unknown command
    mov si, UNKNOWN_CMD
    call print_string
    jmp command_loop

; Command handlers
do_help:
    mov si, HELP_MSG
    call print_string
    jmp command_loop

do_clear:
    call clear_screen
    jmp command_loop

do_reboot:
    int 0x19        ; Reboot

; Function to compare strings (SI and DI)
strcmp:
    pusha
.loop:
    mov al, [si]
    mov bl, [di]
    cmp al, bl
    jne .not_equal
    cmp al, 0
    je .equal
    inc si
    inc di
    jmp .loop
.not_equal:
    popa
    clc
    ret
.equal:
    popa
    stc
    ret

; Function to read a line
read_line:
    xor cx, cx          ; Clear counter
.loop:
    mov ah, 0
    int 0x16
    
    cmp al, 0x0D        ; Enter
    je .done
    
    cmp al, 0x08        ; Backspace
    je .backspace
    
    cmp cx, 62          ; Buffer limit
    je .loop
    
    mov [di], al
    inc di
    inc cx
    
    mov ah, 0x0E        ; Echo character
    int 0x10
    jmp .loop

.backspace:
    test cx, cx         ; If cx is 0, ignore backspace
    jz .loop
    
    mov ah, 0x0E
    mov al, 0x08
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 0x08
    int 0x10
    
    dec di
    dec cx
    jmp .loop

.done:
    mov byte [di], 0
    mov ah, 0x0E
    mov al, 0x0D
    int 0x10
    mov al, 0x0A
    int 0x10
    ret

; Function to clear screen
clear_screen:
    mov ah, 0x00
    mov al, 0x03
    int 0x10
    ret

; Function to print string (SI = string pointer)
print_string:
    mov ah, 0x0E
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

; Data section
WELCOME_MSG db 'DaveOS v0.1', 0x0D, 0x0A, 'Type "help" for commands', 0x0D, 0x0A, 0
PROMPT db '> ', 0
UNKNOWN_CMD db 'Unknown command', 0x0D, 0x0A, 0
HELP_MSG db 'Commands:', 0x0D, 0x0A
         db 'help   - Show this help', 0x0D, 0x0A
         db 'clear  - Clear screen', 0x0D, 0x0A
         db 'reboot - Restart system', 0x0D, 0x0A, 0

; Command strings
cmd_help db 'help', 0
cmd_clear db 'clear', 0
cmd_reboot db 'reboot', 0

; Buffer for command input
command_buffer times 64 db 0

; Boot sector padding
times 510-($-$$) db 0
dw 0xaa55