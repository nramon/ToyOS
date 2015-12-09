TARGET=i686-elf-
BIN=toy_os.bin

all: toy_os.bin

$(BIN): kernel/kernel.o arch/i386/boot.o arch/i386/tty.o
	$(TARGET)gcc -T kernel/linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

arch/i386/boot.o: arch/i386/boot.s
	$(TARGET)as $< -o $@

kernel/kernel.o: include/kernel/tty.h
arch/i386/tty.o: include/kernel/tty.h

clean:
	rm kernel/*.o arch/i386/*.o $(BIN)

%.o: %.c
	$(TARGET)gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude
