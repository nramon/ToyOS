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

size_t tty_row;
size_t tty_column;
size_t tty_color;
uint16_t *tty_screen;

/* Initialize the terminal emulator. */
void tty_init(void)
{
	tty_row = 0;
	tty_column = 0;
	tty_color = VGA_COLOR(COLOR_LIGHT_GREY, COLOR_BLACK);
	tty_screen = VGA_MEMORY;
}

/* Print a character to the screen. */
void tty_putc(char c)
{
	tty_screen[(tty_column++) + tty_row * VGA_WIDTH] = VGA_CHAR(c, tty_color);

	/* Move to the next row. */
	if (tty_column > VGA_WIDTH) {
		tty_column = 0;
		tty_row++;
	}

	/* Wrap the screen. */
	if (tty_row > VGA_HEIGHT) {
		tty_row = 0;
	}
}

/* Print a string to the screen. */
void tty_puts(char* s)
{
	while(*s != 0) {
		tty_putc(*(s++));
	}
}

