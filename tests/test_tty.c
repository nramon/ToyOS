/* Host-side unit tests for the terminal emulator driver.

   tty.c is included directly, with VGA_MEMORY pointing to a buffer in host
   memory. The buffer is surrounded by guard cells to detect out-of-bounds
   writes. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define GUARD     (2 * 80)
#define GUARD_VAL 0xDEAD

static volatile uint16_t vga_buf[GUARD + 80 * 25 + GUARD];
#define VGA_MEMORY (vga_buf + GUARD)

#include "../toy_os/arch/i386/tty.c"

static int failures;

#define CHECK(cond)                                                                                \
	do {                                                                                       \
		if (!(cond)) {                                                                     \
			printf("  FAILED %s:%d: %s\n", __FILE__, __LINE__, #cond);                 \
			failures++;                                                                \
		}                                                                                  \
	} while (0)

#define CELL(row, col) (vga_buf[GUARD + (row)*80 + (col)])
#define CH(row, col)   ((char)(CELL(row, col) & 0xFF))
#define ATTR(row, col) ((uint8_t)(CELL(row, col) >> 8))

static void reset(void)
{
	for (size_t i = 0; i < GUARD + 80 * 25 + GUARD; i++) {
		vga_buf[i] = GUARD_VAL;
	}
	tty_init();
}

static int guards_intact(void)
{
	for (size_t i = 0; i < GUARD; i++) {
		if (vga_buf[i] != GUARD_VAL || vga_buf[GUARD + 80 * 25 + i] != GUARD_VAL) {
			return 0;
		}
	}
	return 1;
}

static int row_is(size_t row, const char *s)
{
	size_t len = strlen(s);

	for (size_t col = 0; col < 80; col++) {
		if (CH(row, col) != (col < len ? s[col] : ' ')) {
			return 0;
		}
	}
	return 1;
}

static void test_init_clears_screen(void)
{
	reset();
	for (size_t row = 0; row < 25; row++) {
		CHECK(row_is(row, ""));
	}
	CHECK(ATTR(0, 0) == 0x07); /* Light grey on black. */
	CHECK(guards_intact());
}

static void test_puts(void)
{
	reset();
	tty_puts("Welcome to Toy OS!");
	CHECK(row_is(0, "Welcome to Toy OS!"));
	CHECK(row_is(1, ""));
	CHECK(ATTR(0, 0) == 0x07);
}

static void test_newline_and_carriage_return(void)
{
	reset();
	tty_puts("ab\ncd\rX");
	CHECK(row_is(0, "ab"));
	CHECK(row_is(1, "Xd"));
}

static void test_line_wrap(void)
{
	reset();
	for (int i = 0; i < 80; i++) {
		tty_putc('a');
	}
	/* The 80th character must stay on the first row... */
	CHECK(CH(0, 79) == 'a');
	CHECK(row_is(1, ""));
	/* ...and the 81st goes to the start of the second one. */
	tty_putc('b');
	CHECK(CH(1, 0) == 'b');
	CHECK(CH(0, 0) == 'a');
	CHECK(guards_intact());
}

static void test_fill_screen(void)
{
	reset();
	for (int i = 0; i < 80 * 25; i++) {
		tty_putc('x');
	}
	/* Writing the last cell scrolls the screen once: 24 full rows and an empty one. */
	CHECK(CH(0, 0) == 'x');
	CHECK(CH(23, 79) == 'x');
	CHECK(row_is(24, ""));
	CHECK(guards_intact());
}

static void test_scroll(void)
{
	char line[8];

	reset();
	for (int i = 0; i < 30; i++) {
		snprintf(line, sizeof(line), "L%d\n", i);
		tty_puts(line);
	}
	/* 30 lines written, the last row is the (empty) 31st: lines 6-29 are visible. */
	CHECK(row_is(0, "L6"));
	CHECK(row_is(23, "L29"));
	CHECK(row_is(24, ""));
	CHECK(guards_intact());
}

static void test_scroll_keeps_cursor_on_last_row(void)
{
	reset();
	for (int i = 0; i < 40; i++) {
		tty_putc('\n');
	}
	tty_puts("end");
	CHECK(row_is(24, "end"));
	CHECK(row_is(23, ""));
	CHECK(guards_intact());
}

static void test_high_bit_characters(void)
{
	reset();
	tty_putc((char)0xC9); /* Box drawing character: must not clobber the attribute. */
	CHECK(CELL(0, 0) == (uint16_t)(0x0700 | 0xC9));
}

int main(void)
{
	static const struct {
		const char *name;
		void (*fn)(void);
	} tests[] = {
	    {"init clears screen", test_init_clears_screen},
	    {"puts", test_puts},
	    {"newline and carriage return", test_newline_and_carriage_return},
	    {"line wrap", test_line_wrap},
	    {"fill screen", test_fill_screen},
	    {"scroll", test_scroll},
	    {"scroll keeps cursor on last row", test_scroll_keeps_cursor_on_last_row},
	    {"high bit characters", test_high_bit_characters},
	};

	for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
		int before = failures;

		tests[i].fn();
		printf("%s: %s\n", failures == before ? "ok  " : "FAIL", tests[i].name);
	}
	printf("%s\n", failures ? "FAILED" : "PASS");
	return failures != 0;
}
