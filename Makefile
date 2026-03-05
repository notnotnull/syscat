TARGET = syscat
VERSION = 0.1.0
RELEASE = 1

# Packaging
# Debian
DEBIAN_PACKAGE = $(TARGET)_$(VERSION)_amd64.deb
DEBIAN_CONTROL = dist/debian/DEBIAN/control

SRCS = src/main.c
BIN_DIR = bin
CC=gcc
LINT=clang-format
RM =rm -f
VALGRIND_FLAGS = --leak-check=full --track-origins=yes --show-leak-kinds=all

# Compiler and linker flags
# -Wall: Enable all warnings
# -Wextra: Enable extra warnings
# ---
# -nostdlib: Do not link against the standard library
CFLAGS = -Wall -Wextra
LINK_FLAGS = -nostdlib

.PHONY: all clean check lint valgrind dist debian

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^ $(LINK_FLAGS)

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

check: lint	valgrind
	@echo "All checks passed!"

lint:
	$(LINT) -i $(SRCS)

valgrind: $(TARGET)
	valgrind $(VALGRIND_FLAGS) $(BIN_DIR)/$(TARGET) .gitignore 1>/dev/null

distclean:
	$(RM) -rf dist $(PACKAGES)

clean:
	$(RM) $(BIN_DIR)/$(TARGET)