/* Serial port (COM1) driver, used to log from the kernel to the host.
   Author:   Ramon Novoa <ramonnovoa@gmail.com>
   See: https://wiki.osdev.org/Serial_Ports

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.*/
#include <kernel/io.h>
#include <kernel/serial.h>

#define COM1 0x3F8

/* Register offsets from the base port. */
#define SERIAL_DATA		0	/* Data (DLAB=0) or divisor low byte (DLAB=1). */
#define SERIAL_IER		1	/* Interrupt enable (DLAB=0) or divisor high byte (DLAB=1). */
#define SERIAL_FCR		2	/* FIFO control. */
#define SERIAL_LCR		3	/* Line control. */
#define SERIAL_MCR		4	/* Modem control. */
#define SERIAL_LSR		5	/* Line status. */

#define SERIAL_LSR_TX_EMPTY	0x20

/* Initialize COM1 at 115200 baud, 8 data bits, no parity, one stop bit. */
void serial_init(void)
{
	outb(COM1 + SERIAL_IER, 0x00);	/* Disable interrupts. */
	outb(COM1 + SERIAL_LCR, 0x80);	/* Enable DLAB to set the divisor. */
	outb(COM1 + SERIAL_DATA, 0x01);	/* Divisor 1 (low byte). */
	outb(COM1 + SERIAL_IER, 0x00);	/* Divisor 1 (high byte). */
	outb(COM1 + SERIAL_LCR, 0x03);	/* 8N1, DLAB off. */
	outb(COM1 + SERIAL_FCR, 0xC7);	/* Enable and clear FIFOs. */
	outb(COM1 + SERIAL_MCR, 0x03);	/* DTR and RTS. */
}

/* Write a character to the serial port. */
void serial_putc(char c)
{
	while ((inb(COM1 + SERIAL_LSR) & SERIAL_LSR_TX_EMPTY) == 0) {
	}
	outb(COM1 + SERIAL_DATA, (uint8_t)c);
}

/* Write a string to the serial port. */
void serial_puts(const char *s)
{
	while (*s != 0) {
		serial_putc(*(s++));
	}
}
