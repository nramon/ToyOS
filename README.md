# What is Toy OS?

Toy OS is a perpetual work-in-progress 32-bit x86 protected mode operating system written for learning purposes.

## Building and running Toy OS on Linux

`build.sh` uses a local i686-elf cross-compiler and *grub-mkrescue* if you have them. Otherwise it builds and runs everything inside Docker (or Podman) using the provided *Dockerfile*, so the only requirements are Docker/Podman and [QEMU](https://www.qemu.org) for *run.sh*. The first run compiles the cross-compiler into the `toyos-build` image, which takes 15-25 minutes; later runs reuse it.

If you prefer a local toolchain, see the [OSDev wiki](http://wiki.osdev.org/GCC_Cross-Compiler), and install [GNU GRUB](https://www.gnu.org/software/grub/) for *grub-mkrescue*.

From a terminal simply run:

    make run

Other targets (see `make help`):

| Target | Description |
| --- | --- |
| `make` | Build the kernel and `toy_os.iso`. |
| `make test` | Boot the ISO headless in QEMU and check the serial and VGA output. |
| `make lint` | Run *cppcheck*, *clang-format* (check only) and *shellcheck*. |
| `make format` | Format the C sources with *clang-format*. |
| `make clean` | Remove build outputs. |

`./build.sh`, `./run.sh` and `./test.sh` can also be called directly.

Kernel messages are also written to the first serial port (COM1), which is how `make test` checks the boot.

