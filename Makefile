# Entry point for building, running, testing and linting Toy OS.
# Run `make help` to see the available targets.

SOURCES = $(shell find toy_os -name '*.c' -o -name '*.h')
SCRIPTS = build.sh run.sh test.sh

.PHONY: all run test lint format clean help

all: ## Build the kernel and the ISO (uses Docker if there is no local toolchain).
	./build.sh

run: all ## Build and boot Toy OS in QEMU.
	./run.sh

test: all ## Boot Toy OS headless in QEMU and check its output.
	./test.sh

lint: ## Run cppcheck, clang-format (check only) and shellcheck.
	cppcheck --enable=warning,style,performance,portability --error-exitcode=1 \
		--inline-suppr --suppress=missingIncludeSystem -Itoy_os/include -q toy_os
	clang-format --dry-run --Werror $(SOURCES)
	shellcheck $(SCRIPTS)

format: ## Format the C sources with clang-format.
	clang-format -i $(SOURCES)

clean: ## Remove build outputs.
	find toy_os \( -name '*.o' -o -name '*.d' -o -name 'toy_os.bin' \) -delete
	rm -rf toy_os/isodir toy_os.iso

help: ## Show this help.
	@grep -E '^[a-z]+:.*## ' $(MAKEFILE_LIST) | sed 's/:.*## /\t/'
