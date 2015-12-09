/* Header file for the terminal emulator driver.
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
#ifndef _TTY_H_
#define _TTY_H_

#include <stddef.h>

void tty_init(void);
void tty_putc(char c);
void tty_puts(char* s);

#endif
