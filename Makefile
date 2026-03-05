TARGET = syscat
VERSION = 0.1.0

# Packaging
# Debian
DEBIAN_PACKAGE = $(TARGET)_$(VERSION)_amd64.deb
DEBIAN_CONTROL = dist/debian/DEBIAN/control

SRCS = src/utils.c src/main.c
TEST_SRCS = tests/test_utils.c src/utils.c
INCLUDES = include
BIN_DIR = bin
CC=gcc
LINT=clang-format
RM =rm -rf
VALGRIND_FLAGS = --leak-check=full --track-origins=yes --show-leak-kinds=all

# Compiler and linker flags
# -Wall: Enable all warnings
# -Wextra: Enable extra warnings
# ---
# -nostdlib: Do not link against the standard library
CFLAGS = -Wall -Wextra
LINK_FLAGS = -nostdlib

.PHONY: all clean check lint valgrind dist debian test

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -I$(INCLUDES) $^ -o $(BIN_DIR)/$@ $(LINK_FLAGS)

test: $(TEST_SRCS)
	mkdir -p $(BIN_DIR)
	$(CC) -I./$(INCLUDES) $^ -o $(BIN_DIR)/test_utils -lcunit

dist: debian

debian: $(TARGET)
	@echo "Creating Debian package..."
	mkdir -p dist/debian/usr/bin
	mkdir -p dist/debian/DEBIAN
	cp $(BIN_DIR)/$(TARGET) dist/debian/usr/bin/
	echo "Package: $(TARGET)" > $(DEBIAN_CONTROL)
	echo "Version: $(VERSION)" >> $(DEBIAN_CONTROL)
	echo "Priority: optional" >> $(DEBIAN_CONTROL)
	echo "Architecture: amd64" >> $(DEBIAN_CONTROL)
	echo "Maintainer: Alex Campbell alex.m.campbell@hotmail.com" >> $(DEBIAN_CONTROL)
	echo "Description: Lightweight file concatenation tool." >> $(DEBIAN_CONTROL)
	dpkg-deb --build dist/debian $(DEBIAN_PACKAGE)
	@echo "Debian package created: $(DEBIAN_PACKAGE)"

check: lint	valgrind cunit
	@echo "All checks passed!"

checklint:
	$(LINT) --dry-run -Werror $(SRCS) $(INCLUDES)/*

lint:
	$(LINT) -i $(SRCS) $(INCLUDES)/*

valgrind: $(TARGET)
	valgrind $(VALGRIND_FLAGS) $(BIN_DIR)/$(TARGET) README.md 1>/dev/null

cunit: test
	$(BIN_DIR)/test_utils

distclean:
	$(RM) -rf dist $(PACKAGES)

clean:
	$(RM) $(BIN_DIR)/*
