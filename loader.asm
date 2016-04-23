global loader                   ; the entry symbol for ELF

; ************* Constants *************
;KERNEL_STACK_SIZE   equ 4096
MBOOT_MAGIC_NUMBER  equ 0x1BADB002 	; multiboot's file signature/magic number
MBOOT_PAGE_ALIGN    equ 1<<0    	; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    	; Provide kernel with memory info
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_MAGIC_NUMBER + MBOOT_HEADER_FLAGS)
								; (magic number + checksum + flags should equal 0)

; ************* BSS *************
;section .bss
;align 4
; ************* kernel_stack *************
;kernel_stack:					; label will point to the reserved memory
;	resb KERNEL_STACK_SIZE		; NASM-specific pseudo-instruction to reserve memory

; ************* TEXT *************
global mboot

mboot:
	;align 4                         ; must be 4 byte aligned
	dd MBOOT_MAGIC_NUMBER             ; writes the constant to the binary,
	dd MBOOT_HEADER_FLAGS
	dd MBOOT_CHECKSUM
	dd mboot
	dd loader

extern kmain
loader:                         ; the loader label (defined as entry point in linker script)
	;mov eax, 0xCAFEBABE        ; place the number 0xCAFEBABE in the register eax
	;mov esp, kernel_stack + KERNEL_STACK_SIZE	;point ESP to top of stack (since it grows downward)
	push ebx	;load multiboot header location
	cli
	call kmain
	jmp $
