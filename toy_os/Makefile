CROSS ?= i686-elf-
CC = $(CROSS)gcc
AS = $(CROSS)as

BIN = toy_os.bin
ISO = ../toy_os.iso
OBJS = kernel/kernel.o arch/i386/boot.o arch/i386/tty.o
DEPS = $(OBJS:.o=.d)

CFLAGS = -std=gnu99 -ffreestanding -fno-stack-protector -O2 -g -Wall -Wextra -Werror -Iinclude

.PHONY: all iso clean

all: $(BIN)

$(BIN): $(OBJS) kernel/linker.ld
	$(CC) -T kernel/linker.ld -o $@ -ffreestanding -O2 -nostdlib $(OBJS) -lgcc

# Create a bootable ISO with GRUB.
iso: $(ISO)

$(ISO): $(BIN) grub.cfg
	rm -rf isodir
	mkdir -p isodir/boot/grub
	cp $(BIN) isodir/boot/
	cp grub.cfg isodir/boot/grub/
	grub-mkrescue -o $@ isodir
	rm -rf isodir

arch/i386/boot.o: arch/i386/boot.s
	$(AS) $< -o $@

# -MMD -MP track header dependencies automatically.
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -MMD -MP

clean:
	$(RM) $(OBJS) $(DEPS) $(BIN) $(ISO)
	$(RM) -r isodir

-include $(DEPS)
