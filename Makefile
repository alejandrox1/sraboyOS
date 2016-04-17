# Modified from original to make use of cross-dev environment

OBJECTS = loader.o kmain.o io.o vga.o
INC = include
CC = i686-elf-gcc
CFLAGS = -ffreestanding -O2 -Wall -Wextra -c
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

run: makeiso
	qemu-system-i386 -cdrom $(ISONAME) -s

%.o: %.c
	$(CC) -I$(INC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf $(ISONAME)
