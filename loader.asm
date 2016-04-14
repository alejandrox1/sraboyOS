global loader                   ; the entry symbol for ELF

; ************* Constants *************
KERNEL_STACK_SIZE equ 4096
MAGIC_NUMBER equ 0x1BADB002     ; multiboot's file signature/magic number
FLAGS        equ 0x0            ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
								; (magic number + checksum + flags should equal 0)

; ************* BSS *************
section .bss
align 4
; ************* kernel_stack *************
kernel_stack:					; label will point to the reserved memory
	resb KERNEL_STACK_SIZE		; NASM-specific pseudo-instruction to reserve memory

; ************* TEXT *************
section .text:
align 4                         ; must be 4 byte aligned
	dd MAGIC_NUMBER             ; writes the constant to the binary,
	dd FLAGS
	dd CHECKSUM

; ************* loader *************
loader:                         ; the loader label (defined as entry point in linker script)
	;mov eax, 0xCAFEBABE        ; place the number 0xCAFEBABE in the register eax
	mov esp, kernel_stack + KERNEL_STACK_SIZE	;point ESP to top of stack (since it grows downward)

	extern kmain
	call kmain
	cli
.loop:
	hlt
	jmp .loop                   ; loop forever
