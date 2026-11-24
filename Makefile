# Entry point for building, running, testing and linting Toy OS.
# Run `make help` to see the available targets.

SOURCES = $(shell find toy_os tests -name '*.c' -o -name '*.h')
SCRIPTS = build.sh run.sh test.sh

.PHONY: all run debug unit coverage test lint format clean help

all: ## Build the kernel and the ISO (uses Docker if there is no local toolchain).
	./build.sh

run: all ## Build and boot Toy OS in QEMU.
	./run.sh

debug: all ## Boot Toy OS in QEMU paused, waiting for GDB on port 1234.
	@echo "Attach with: gdb toy_os/toy_os.bin -ex 'target remote :1234'"
	./run.sh -s -S

unit: ## Build and run the host-side unit tests.
	$(CC) -std=gnu99 -Wall -Wextra -Wshadow -Wpedantic -Wconversion -Werror -g -fsanitize=address,undefined -Itoy_os/include \
		tests/test_tty.c -o tests/test_tty
	./tests/test_tty

coverage: ## Run the unit tests with coverage (needs gcovr); writes coverage/coverage.xml.
	mkdir -p coverage
	$(CC) -std=gnu99 -O0 -g --coverage -Itoy_os/include tests/test_tty.c -o coverage/test_tty
	./coverage/test_tty
	gcovr --root . --filter 'toy_os/' --object-directory coverage --print-summary \
		--xml coverage/coverage.xml

test: unit all ## Run the unit tests, then boot Toy OS headless in QEMU and check its output.
	./test.sh

lint: ## Run cppcheck, clang-format (check only) and shellcheck.
	cppcheck --enable=warning,style,performance,portability --error-exitcode=1 \
		--inline-suppr --suppress=missingIncludeSystem -Itoy_os/include -q toy_os tests
	clang-format --dry-run --Werror $(SOURCES)
	shellcheck $(SCRIPTS)

format: ## Format the C sources with clang-format.
	clang-format -i $(SOURCES)

clean: ## Remove build outputs.
	find toy_os \( -name '*.o' -o -name '*.d' -o -name 'toy_os.bin' \) -delete
	rm -rf toy_os/isodir toy_os.iso tests/test_tty coverage

help: ## Show this help.
	@grep -E '^[a-z]+:.*## ' $(MAKEFILE_LIST) | sed 's/:.*## /\t/'
