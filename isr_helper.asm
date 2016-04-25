%macro ISR_NOERRCODE 1
    global isr%1

    isr%1:
        cli
        push byte 0     ; push a dummy error code
        push byte %1    ; push the interrupt number
        jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        cli             ; error code is already on the stack
        push byte %1    ; push the interrupt number
        jmp isr_common_stub
%endmacro

%macro IRQ 2
    global irq%1
    irq%1:
        cli
        push byte 0     ; push a dummy error code
        push byte %2    ; push the IRQ number
        jmp  irq_common_stub
%endmacro

extern isr_handler  ; see isr.h

isr_common_stub:
    pusha           ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov ax, ds      ; Lower 16-bits of eax = ds.
    push eax        ; save the data segment descriptor
    mov ax, 0x10    ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call isr_handler

    pop eax        ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                     ; Pops edi,esi,ebp...
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    sti
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

extern irq_handler

irq_common_stub:
    pusha           ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov ax, ds      ; Lower 16-bits of eax = ds.
    push eax        ; save the data segment descriptor
    mov ax, 0x10    ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler

    pop ebx        ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa           ; Pops edi,esi,ebp,esp,ebx,edx,ecx,eax
    add esp, 8     ; Cleans up the pushed error code and pushed irq number
    sti            ; (re)Enable interrupts "set interrupt flag"
    iret           ; pops CS, EIP, EFLAGS, SS, and ESP


%assign i 0
%rep 8
ISR_NOERRCODE i
%assign i i+1
%endrep

ISR_NOERRCODE 9

%assign i 15
%rep 17
ISR_NOERRCODE i
%assign i i+1
%endrep

ISR_ERRCODE 8

%assign i 10
%rep 5
ISR_ERRCODE i
%assign i i+1
%endrep

%assign i 0
%rep 16
IRQ i, i+32
%assign i i+1
%endrep

