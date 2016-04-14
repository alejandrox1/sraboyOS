global outb

; outb - send a byte to an I/O port
; stack: [esp + 8] data
;		 [esp + 4] I/O port
;		 [esp    ] return addr
outb:
	mov al, [esp + 8]
	mov dx, [esp + 4]
	out dx, al
	ret
