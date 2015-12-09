# What is Toy OS?

Toy OS is a perpetual work-in-progress 16-bit x86 real mode operating system written for learning purposes.

## Building and running Toy OS on Linux

To build Toy OS you will need an i686-elf cross-compiler. Go to the [OSDev wiki](http://wiki.osdev.org/GCC_Cross-Compiler) if you need help setting one up.

To use the provided *build.sh* and *run.sh* scripts you will also need *grub-mkrescue*, which is part of [GNU GRUB](https://www.gnu.org/software/grub/), and [QEMU](www.qemu.org).

From a terminal simply run:

    ./build.sh && ./run.sh

