# Bootstrap the kernel.
# Author:   Ramon Novoa <ramonnovoa@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

.code32

.set MAGIC,    0x1BADB002       # Magic number. Let's the bootloader find the header.
.set FLAGS,    0x00000001 		# Set bit 0 (page alignment).
.set CHECKSUM, -(MAGIC + FLAGS) # When added to magic and flags must have a 32-bit unsigned sum of zero.

# Multiboot header. See: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Create a stack for the kernel.
.section .kernel_stack
	.skip 16384
stack_top:

# Main.
.section .text
	.global _start			# _start will be the entry point.

_start:
	mov $stack_top, %esp	# Initilize the stack.
	call kernel_main		# Run the kernel.

	# Do not leave the CPU executing whatever is in memory if the kernel exits.
	cli
	hlt
.hang:
	jmp .hang
	
