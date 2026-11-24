#!/bin/bash
# Smoke test: boot the ISO headless in QEMU and check that the kernel prints
# its boot message to the serial port and to the VGA text buffer.
set -euo pipefail
cd "$(dirname "$0")"

TIMEOUT=${TIMEOUT:-10}
[ -f toy_os.iso ] || ./build.sh >/dev/null

OUT=$(mktemp -d)
trap 'rm -rf "$OUT"' EXIT

# Run QEMU in the background; the monitor reads from a FIFO so that VGA memory
# can be dumped once the kernel has booted.
mkfifo "$OUT/mon"
qemu-system-i386 -cdrom toy_os.iso -display none -no-reboot \
	-serial "file:$OUT/serial.log" -monitor "stdio" \
	< "$OUT/mon" > "$OUT/monitor.log" 2>&1 &
QEMU=$!
exec 3> "$OUT/mon"
trap 'kill $QEMU 2>/dev/null || true; rm -rf "$OUT"' EXIT

fail() {
	echo "FAIL: $1" >&2
	echo "--- serial output ---" >&2
	cat "$OUT/serial.log" >&2 || true
	exit 1
}

# Wait for the boot message on the serial port.
for _ in $(seq $((TIMEOUT * 5))); do
	grep -q "Toy OS booted." "$OUT/serial.log" 2>/dev/null && break
	sleep 0.2
done
grep -q "Welcome to Toy OS!" "$OUT/serial.log" || fail "welcome message not seen on the serial port"
grep -q "Toy OS booted." "$OUT/serial.log" || fail "boot message not seen on the serial port"

# Dump the first 18 VGA cells: "Welcome to Toy OS!" in light grey on black.
echo "xp /18xh 0xb8000" >&3
sleep 1
echo "quit" >&3
wait $QEMU 2>/dev/null || true

EXPECTED="0757 0765 076c 0763 076f 076d 0765 0720 0774 076f 0720 0754 076f 0779 0720 074f 0753 0721"
ACTUAL=$(grep -a '^[0-9a-f]\{16\}:' "$OUT/monitor.log" | grep -o '0x[0-9a-f]\{4\}' | sed 's/^0x//' | head -18 | tr '\n' ' ' | sed 's/ $//')
[ "$ACTUAL" = "$EXPECTED" ] || fail "unexpected VGA contents: $ACTUAL"

echo "PASS"
