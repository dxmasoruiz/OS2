CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread

# Source files
SRC_FILES = ipc_utils.c passenger.c launch.c
HDR_FILES = ipc_utils.h passenger.h

# Object files (corresponding to the source files)
OBJ_FILES = ipc_utils.o passenger.o launch.o

# Executable target
TARGET = exercise2

# Default target: Build the executable
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
    $(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
ipc_utils.o: ipc_utils.c ipc_utils.h
    $(CC) $(CFLAGS) -c -o $@ $<

passenger.o: passenger.c passenger.h ipc_utils.h
    $(CC) $(CFLAGS) -c -o $@ $<

launch.o: launch.c ipc_utils.h passenger.h
    $(CC) $(CFLAGS) -c -o $@ $<

# Clean up build artifacts
clean:
    rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean