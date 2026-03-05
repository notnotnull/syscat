TARGET = syscat
SRCS = src/main.c
BIN_DIR = bin
CC=gcc
RM =rm -f

# Compiler and linker flags
# -Wall: Enable all warnings
# -Wextra: Enable extra warnings
# ---
# -nostdlib: Do not link against the standard library
CFLAGS = -Wall -Wextra
LINK_FLAGS = -nostdlib

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^ $(LINK_FLAGS)

clean:
	$(RM) $(BIN_DIR)/$(TARGET)