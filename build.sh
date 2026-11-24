#!/bin/bash
# Build the kernel image and ISO.
#
# If an i686-elf cross-compiler and grub-mkrescue are installed locally they are
# used directly. Otherwise the build runs inside the Docker image defined by the
# Dockerfile (built automatically the first time; this takes a while).
cd "$(dirname "$0")" || exit 1

IMAGE=toyos-build

if ! command -v i686-elf-gcc >/dev/null || ! command -v grub-mkrescue >/dev/null; then
	ENGINE=$(command -v docker || command -v podman) || {
		echo "Need i686-elf-gcc and grub-mkrescue, or docker/podman." >&2
		exit 1
	}
	if [ -z "$($ENGINE images -q $IMAGE)" ]; then
		echo "Building the $IMAGE image (compiles GCC, this takes a while)..."
		$ENGINE build -t $IMAGE . || exit 1
	fi
	exec $ENGINE run --rm -u "$(id -u):$(id -g)" -v "$PWD":/work -w /work \
		$IMAGE ./build.sh
fi

cd toy_os && make && cd .. || exit 1

# Create the ISO and boot Toy OS with GRUB.
mkdir -p isodir/boot/grub
cp toy_os/toy_os.bin isodir/boot/
cat << EOF2 > grub.cfg
menuentry "Toy OS" {
	multiboot /boot/toy_os.bin
}
EOF2
mv grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o toy_os.iso isodir

# Clean up.
rm -rf isodir
