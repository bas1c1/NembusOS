bits 32

; constants for multiboot header
_MEMINFO     equ  1<<0
_BOOTDEVICE  equ  1<<1
_CMDLINE     equ  1<<2
_MODULECOUNT equ  1<<3
_SYMT        equ  48 ; bits 4 & 5
_MEMMAP      equ  1<<6
_DRIVE       equ  1<<7
_CONFIGT     equ  1<<8
_BOOTLDNAME  equ  1<<9
_APMT        equ  1<<10
_VIDEO       equ  1<<11
_VIDEO_FRAMEBUF equ  1<<12
_FLAGS       equ  _MEMINFO | _BOOTDEVICE | _CMDLINE | _MODULECOUNT | _SYMT | _MEMMAP | _DRIVE | _CONFIGT | _BOOTLDNAME | _VIDEO_FRAMEBUF
_MAGIC_HEADER       equ  0x1BADB002
_CHECKSUM    equ -(_MAGIC_HEADER + _FLAGS)
_BOOTLOADER_MAGIC  equ  0x2BADB002

section .initial_stack, nobits
    align 4
section .text
global start
extern _kmain

; set multiboot section
section .multiboot_header
    align 4
    dd _MAGIC_HEADER
    dd _FLAGS
    dd _CHECKSUM

start:
	mov esp, stack_space
	mov eax, _BOOTLOADER_MAGIC
	push ebx
    push eax
	call _kmain

section .bss
resb 8192; 8KB for stack
stack_space: