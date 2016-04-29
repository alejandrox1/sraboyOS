global load_gdt

; void load_gdt(struct gdt_ptr_t * gdt_ptr_t);
load_gdt:
    mov	  eax, [esp + 4]	;struct gdt *
    lgdt  [eax]

; All these registers can be adjusted with a MOV. They will now
; point to our new kernel data segment which is 8 bytes past the
; code segment we have CS point to
;
; The GDT must have at least three entries:
; idx   offset  name					  addr range	   type priv
;	0	0x00	null descriptor
;	1	0x08	kernel code segment	0x00000000 - 0xFFFFFFFF	RX	PL0
;	2	0x10	kernel data segment	0x00000000 - 0xFFFFFFFF	RW	PL0
   mov	ax, 0x10 ; 0x10 = new data selector
   mov	ds, ax
   mov	es, ax
   mov	fs, ax
   mov	gs, ax
   mov	ss, ax
; CS can't be changed with MOV so we use a "far jump" which
; results in 0x08 being loaded into CS. This means the segment
; selector contains 0000 0000 0000 1 000. The first 15 bits
; represent the index to our location and the last three bits
; are settings: first bit is 0 for GDT or 1 for LDT. The last
; two bits are the privilege level: 00 = highest, 11 = lowest.
.reload_cs:
   jmp   0x08:.flush ; 0x08 = new code selector
.flush:
   ret


global load_idt


; void load_idt(struct idt_ptr * idt_ptr_t, uint32_t size);
load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret
