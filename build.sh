#!/bin/bash
# Build the kernel image and ISO.
#
# If an i686-elf cross-compiler and grub-mkrescue are installed locally they are
# used directly. Otherwise the build runs inside the Docker image defined by the
# Dockerfile (built automatically the first time; this takes a while).
set -euo pipefail
cd "$(dirname "$0")"

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

cd toy_os
make iso
