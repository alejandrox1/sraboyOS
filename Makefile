# Modified from original to make use of cross-dev environment

OBJECTS = loader.o kmain.o io.o vga.o
CC = i686-elf-gcc
CFLAGS = -ffreestanding -O2 -Wall -Wextra -c
LDFLAGS = -T link.ld -ffreestanding -O2 -nostdlib -lgcc
AS = nasm
ASFLAGS = -felf32

all: kernel.elf

kernel.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o kernel.elf

myos.iso: kernel.elf
	cp kernel.elf isodir/boot/kernel.elf
	genisoimage -R                          \
			-b boot/grub/stage2_eltorito    \
			-no-emul-boot                   \
			-boot-load-size 4               \
			-A myos                         \
			-input-charset utf8             \
			-quiet                          \
			-boot-info-table                \
			-o myos.iso                     \
			isodir

run: myos.iso
	qemu-system-i386 -cdrom myos.iso -s

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf myos.iso
