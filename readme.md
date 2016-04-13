# Intro

This is just me following along with https://littleosbook.github.io.

# Environment

I use VirtualBox and QEMU:
- Host: Win10 on amd64
- Guest: Debian Jessie 8.4 (w/ Guest Additions)
  - Nested guest: qemu-system-i386

I did a couple things differently from the tutorial:

Primarily, I'm using a nested VM. Also, I already had QEMU installed and didn't want to also install Bochs so I use QEMU and **[gdbtui](https://sourceware.org/gdb/onlinedocs/gdb/TUI.html)**).

Until/unless you need a file system, you also don't need to build the ISO since you can just pass the `kernel` parameter and kernel image to QEMU. Once you do want to use an ISO, just use the `cdrom` parameter and the ISO file.

**NOTE:** Be sure to check your host keys. I like to use CTRL+ALT with VirtualBox, to match VMWare since I use that too, but that's also QEMU's default. If you end up clicking inside the QEMU window, it will capture your mouse and QEMU's default host key is also CTRL+ALT. VirtualBox will not send CTRL+ALT to the guest so you won't be able to get out unless you have Guest Additions installed, which will let your mouse leave the Debian guest (but can't help you the QEMU guest). You can change the host key at runtime too so it's not a big deal, as long as you can get your mouse back to the host.

# Build

Assemble: `nasm -felf32 loader.asm`

Link: `ld -T link.ld -melf_i386 loader.o -o kernel.elf` or, if you've set up a cross-compilation environment like [this OSDev tutorial](http://wiki.osdev.org/GCC_Cross-Compiler), `i686-elf-gcc -T link.ld -o kernel.elf -ffreestanding -O2 -nostdlib loader.o kmain.o -lgcc`

Compile: `i686-elf-gcc -T link.ld -o kernel.elf -ffreestanding -O2 -nostdlib loader.o kmain.o -lgcc`

Generate ISO:

    genisoimage -R                              \
                -b boot/grub/stage2_eltorito    \
                -no-emul-boot                   \
                -boot-load-size 4               \
                -A myos                         \
                -input-charset utf8             \
                -quiet                          \
                -boot-info-table                \
                -o myos.iso                     \
                isodir

Run QEMU: `qemu-system-i386 -kernel kernel.elf -s &` or `qemu-system-i386 -cdrom myos.iso -s &`. The `s` tells QEMU to listen for a debugger on port 1234.

Run GDB: `gdbtui` then `target remote localhost:1234`
