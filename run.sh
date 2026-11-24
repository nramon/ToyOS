#!/bin/bash
# Boot Toy OS in QEMU. Extra arguments are passed to QEMU.
exec qemu-system-i386 -cdrom "$(dirname "$0")/toy_os.iso" "$@"
