global outb

; outb - send a byte to an I/O port
; stack: [esp + 8] data
;		 [esp + 4] I/O port
;		 [esp    ] return addr
;
; void outb(uint16_t port, uint8_t data)
outb:
	mov al, [esp + 8]
	mov dx, [esp + 4]
	out dx, al
	ret

; inb - returns a byte from the specified port
; stack: [esp + 4] addr of the port
;		 [esp    ] return addr
; void inb(uint16_t port)
global inb
inb:
	mov dx, [esp + 4]
	in al, dx	; stores read byte in al
	ret
