# Makefile for the rescue-operation project

# Compiler
CC = gcc

# Directories
SRC_DIR = ../src
OBJ_DIR = ../obj
BIN_DIR = ../bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Executable
TARGET = $(BIN_DIR)/exercise2

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Linker flags
LDFLAGS =

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

# Build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean