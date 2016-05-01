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

# References & Tutorials

There are a lot of great resources out there but not many good tutorials. Most of the tutorials gloss over the code or gloss over the contents, or both; it forces you to spend time reading about theory and finding other implementations of concepts but it can be a pain when you just want X to work so you can learn about Y.

This is just a list of some of the great stuff I've found online, including others' toy kernels. For better or worse, many of them are based on the same few tutorials found elsewhere so, often, the code is easy enough to follow.

 - [Little OS Book](https://littleosbook.github.io/) - May not be maintained anymore, as of late 2015. There are some bugs referenced on the [GitHub repo](https://github.com/littleosbook/littleosbook). It starts off with a lot of example code but then tapers off by only providing theory and leaving you to your own devices. It was great to get me started but I needed a bit more help on interrupts and memory management.

 - [aenix](https://github.com/helino/aenix) from the authors of the Little OS Book tutorial.

 - [OSDev.org](http://wiki.osdev.org/Main_Page) - The best resource on the net, with dozens of tutorials and even several pages (linked below) providing information on bugs in other tutorials.

 - [James Molloy's Tutorial](http://web.archive.org/web/20120223002615/http://www.jamesmolloy.co.uk/tutorial_html/1.-Environment%20setup.html) - [No longer maintained](http://forum.osdev.org/viewtopic.php?f=1&t=23072&start=0) but a great intro nonetheless. Newer, working code is in the [Google Code repo](https://code.google.com/archive/p/jamesm-tutorials/), but it doesn't match the tutorial. Since Google Code was decommissioned, you can only download a ZIP or export to GitHub; there's no online browsing of the SVN repo.

 - [Boom](https://github.com/BiggerOnTheInside/Boom) - A toy kernel based on JM's tutorial.

 - [Bare Metal Examples](https://github.com/cirosantilli/x86-bare-metal-examples) - Multiple toy OSes for reference.

 - [Bran's Kernel Dev Tutorial](http://www.osdever.net/bkerndev/Docs/title.htm) - Great tutorial, also with [known bugs](http://wiki.osdev.org/Bran's_Known_Bugs).

 - [MIT's Xv6](https://pdos.csail.mit.edu/6.828/2011/xv6.html) - Unix-like OS used to teach OS development at MIT.

 - [XOmB-barebones](https://github.com/xomboverlord/xomb-bare-bones) - Barebones x64 kernel written in D.

 - [ToaruOS](https://github.com/klange/toaruos) - Much more complex than all the others above but it's a genuinely functional Unix-like OS with a fantastic desktop environment and even some application ports, including GCC and Bochs, which means it can be self-hosting!

# Git Tags

See the [Releases tab](https://github.com/sraboy/sraboyOS/tags) for tagged commits that contain working code, to see how something was implemented at that point. Checkout the actual commits in a tag to see what had changed and why. Tags v0.01 - v0.04 were primarily based on the Little OS Book, James Molloy's tutorial and OSDev's Barebones Tutorial. After that, you'll notice a divergence as the more advanced topics started to come into play. Those previous tutorials were still useful but presented some problems due to architectural choices and bugs.
