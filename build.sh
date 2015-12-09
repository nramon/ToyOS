#!/bin/bash
# Build the kernel image.
cd toy_os && make && cd .. || exit 1

# Create the ISO and boot Toy OS with GRUB.
mkdir -p isodir/boot/grub
cp toy_os/toy_os.bin isodir/boot/
cat << EOF > grub.cfg
menuentry "Toy OS" {
	multiboot /boot/toy_os.bin
}
EOF
mv grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o toy_os.iso isodir

# Clean up.
rm -rf isodir

