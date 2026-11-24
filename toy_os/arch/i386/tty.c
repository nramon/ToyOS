/* Terminal emulator driver.
   Author:   Ramon Novoa <ramonnovoa@gmail.com>
   See: https://en.wikipedia.org/wiki/Terminal_emulator

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
#include <kernel/tty.h>
#include <kernel/vga.h>

static size_t tty_row;
static size_t tty_column;
static uint8_t tty_color;
static volatile uint16_t *tty_screen;

/* Fill a row with blanks. */
static void tty_clear_row(size_t row)
{
	for (size_t col = 0; col < VGA_WIDTH; col++) {
		tty_screen[col + row * VGA_WIDTH] = VGA_CHAR(' ', tty_color);
	}
}

/* Scroll the screen up by one row. */
static void tty_scroll(void)
{
	for (size_t i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
		tty_screen[i] = tty_screen[i + VGA_WIDTH];
	}
	tty_clear_row(VGA_HEIGHT - 1);
}

/* Move to the start of the next row, scrolling if needed. */
static void tty_newline(void)
{
	tty_column = 0;
	if (++tty_row >= VGA_HEIGHT) {
		tty_row = VGA_HEIGHT - 1;
		tty_scroll();
	}
}

/* Initialize the terminal emulator. */
void tty_init(void)
{
	tty_row = 0;
	tty_column = 0;
	tty_color = VGA_COLOR(COLOR_LIGHT_GREY, COLOR_BLACK);
	tty_screen = VGA_MEMORY;

	for (size_t row = 0; row < VGA_HEIGHT; row++) {
		tty_clear_row(row);
	}
}

/* Print a character to the screen. */
void tty_putc(char c)
{
	if (c == '\n') {
		tty_newline();
		return;
	}
	if (c == '\r') {
		tty_column = 0;
		return;
	}

	tty_screen[tty_column + tty_row * VGA_WIDTH] = VGA_CHAR(c, tty_color);

	/* Move to the next row. */
	if (++tty_column >= VGA_WIDTH) {
		tty_newline();
	}
}

/* Print a string to the screen. */
void tty_puts(const char *s)
{
	while (*s != 0) {
		tty_putc(*(s++));
	}
}
