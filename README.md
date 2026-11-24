# What is Toy OS?

Toy OS is a perpetual work-in-progress 32-bit x86 protected mode operating system written for learning purposes.

## Building and running Toy OS on Linux

`build.sh` uses a local i686-elf cross-compiler and *grub-mkrescue* if you have them. Otherwise it builds and runs everything inside Docker (or Podman) using the provided *Dockerfile*, so the only requirements are Docker/Podman and [QEMU](https://www.qemu.org) for *run.sh*. The first run compiles the cross-compiler into the `toyos-build` image, which takes 15-25 minutes; later runs reuse it.

If you prefer a local toolchain, see the [OSDev wiki](http://wiki.osdev.org/GCC_Cross-Compiler), and install [GNU GRUB](https://www.gnu.org/software/grub/) for *grub-mkrescue*.

From a terminal simply run:

    ./build.sh && ./run.sh

