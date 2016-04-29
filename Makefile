# Modified from original to make use of cross-dev environment

OBJECTS = loader.o idt_gdt_loader.o isr_helper.o io.o \
		  kmain.o kheap.o isr.o descriptor_tables.o paging.o \
		  string.o stdlib.o \
		  kbd.o vga.o serial.o timer.o
INC = include
CC = i686-elf-gcc
CFLAGS = -ffreestanding -O2 -Wall -Wextra -c -ggdb -funsigned-char
LDFLAGS = -T link.ld -ffreestanding -O2 -nostdlib -lgcc
AS = nasm
ASFLAGS = -felf32
ISONAME = sraboyos.iso

all: kernel.elf

kernel.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o kernel.elf

makeiso: kernel.elf
	cp kernel.elf isodir/boot/kernel.elf
	genisoimage -R                          \
			-b boot/grub/stage2_eltorito    \
			-no-emul-boot                   \
			-boot-load-size 4               \
			-A myos                         \
			-input-charset utf8             \
			-quiet                          \
			-boot-info-table                \
			-o $(ISONAME)                   \
			isodir

# -s = allow remote GDB on port 1234
# -serial = output serial contents to [file:filename.txt | stdio]
run: makeiso
	qemu-system-i386 -cdrom $(ISONAME) -s -serial stdio


%.o: %.c
	$(CC) -I$(INC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf $(ISONAME)
