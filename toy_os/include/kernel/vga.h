/* Header file for VGA compatible text mode.
   Author:   Ramon Novoa <ramonnovoa@gmail.com>
   See: https://en.wikipedia.org/wiki/VGA-compatible_text_mode
  
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
#ifndef _VGA_H_
#define _VGA_H_

#include <stdint.h>

/* Get a color byte given the foreground and background colors. */
#define VGA_COLOR(fg, bg) (fg | bg << 4)

/* Get a character word given the character and the color. */
#define VGA_CHAR(chr, col) (chr | col << 8)

/* VGA color palette. */
#define	COLOR_BLACK			0
#define	COLOR_BLUE			1
#define	COLOR_GREEN			2
#define	COLOR_CYAN			3
#define	COLOR_RED			4
#define	COLOR_MAGENTA		5
#define	COLOR_BROWN			6
#define	COLOR_LIGHT_GREY	7
#define	COLOR_DARK_GREY		8
#define	COLOR_LIGHT_BLUE	9
#define	COLOR_LIGHT_GREEN	10
#define	COLOR_LIGHT_CYAN	11
#define	COLOR_LIGHT_RED		12
#define	COLOR_LIGHT_MAGENTA	13
#define	COLOR_LIGHT_BROWN	14
#define	COLOR_WHITE			15

/* Screen size. */
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

/* Start address of the VGA text buffer */
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000; 

#endif
