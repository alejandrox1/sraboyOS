# Intro

This is a learning project in kernel and driver development. I'm using the [Little OS Book](https://littleosbook.github.io) as a basis but also heavily relying on the [OSDev Wiki](http://wiki.osdev.org/). I prefer QEMU over Bochs so see below (or the Makefile) for the differences in setup.

# Environment

I use VirtualBox and QEMU:
- Host: Win10 on amd64
- Guest: Debian Jessie 8.4 (w/ Guest Additions)
  - Nested guest: qemu-system-i386


**NOTE:** Be sure to check your host keys if using a nested VM setup like mine. I like to use CTRL+ALT with VirtualBox, to match VMWare since I use that too, but that's also QEMU's default. If you end up clicking inside the QEMU window, it will capture your mouse and QEMU's default host key is also CTRL+ALT. VirtualBox will not send CTRL+ALT to the guest so you won't be able to get out unless you have Guest Additions installed, which will let your mouse leave the Debian guest (but can't help you the QEMU guest). You can change the host key at runtime too so it's not a big deal, as long as you can get your mouse back to the host.

# Build

You'll need a GCC cross-compilation environment setup; there's a handy tutorial [on OSDEV's wiki](http://wiki.osdev.org/GCC_Cross-Compiler). You should just use the Makefile but below are some quick and dirty examples for building. Note that these examples are not complete since I won't be updating them every time I add a new file or change something so check out the Makefile.

The Little OS Book has a slightly different setup, which can be found [here](https://littleosbook.github.io/#build-system).

Assemble: `nasm -felf32 loader.asm`

Link: `ld -T link.ld -melf_i386 loader.o -o kernel.elf` or, if you've set up a cross-compilation environment like [this OSDev tutorial](http://wiki.osdev.org/GCC_Cross-Compiler), `i686-elf-gcc -T link.ld -o kernel.elf -ffreestanding -O2 -nostdlib loader.o kmain.o -lgcc`

Compile: `i686-elf-gcc -Iinclude -T link.ld -o kernel.elf -ffreestanding -O2 -nostdlib loader.o kmain.o -lgcc`

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

Run QEMU:
 - `qemu-system-i386 -kernel kernel.elf [options]`
 - `-s` tells QEMU to listen for a debugger on port 1234
 - `-serial stdio` QEMU will dump serial output to the terminal
 - `-S` will immediately halt upon opening so you can, for example, debug the loader

Run GDB:
 - `gdbtui` I like the TUI
 - `target remote localhost:1234`
 - `file kernel.elf` loads debugging symbols from the file
 - `set disassembly-flavor intel` (if you're so inclined, you can put this in .gdbinit)
