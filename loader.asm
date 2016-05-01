global loader	; the entry symbol for ELF
global mboot	; will be passed to the kernel


MBOOT_MAGIC_NUMBER  equ 0x1BADB002 	; multiboot's file signature/magic number
MBOOT_PAGE_ALIGN    equ 1<<0    	; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    	; Provide kernel with memory info
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_MAGIC_NUMBER + MBOOT_HEADER_FLAGS)
								; (magic number + checksum + flags should equal 0)
KERNEL_STACK_SIZE	equ 16384

section .bootstrap, nobits
align 4
kern_stack:					; label will point to the reserved memory
	resb KERNEL_STACK_SIZE	; NASM-specific pseudo-instruction to reserve memory

section .text

mboot:
	align 4					; must be 4 byte aligned
	dd MBOOT_MAGIC_NUMBER	; dd will write the constant to the binary
	dd MBOOT_HEADER_FLAGS
	dd MBOOT_CHECKSUM
	dd mboot
	dd loader

extern kmain
loader:							; defined as entry point in linker script
	push ebx					; load multiboot header location
	mov  esp, kern_stack		; point to top of stack (grows downward)
	cli
	call kmain
	jmp $
