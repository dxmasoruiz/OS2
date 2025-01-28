CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread

# Archivos fuente
SRC_FILES = ipc_utils.c passenger.c launch.c
HDR_FILES = ipc_utils.h passenger.h

# Archivos objeto correspondientes a los archivos fuente
OBJ_FILES = ipc_utils.o passenger.o launch.o

# Objetivo final (ejecutable)
TARGET = exercise2

# Objetivo por defecto: construir el ejecutable
all: $(TARGET)

# Enlazar archivos objeto para crear el ejecutable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Compilar los archivos fuente en archivos objeto
ipc_utils.o: ipc_utils.c ipc_utils.h
	$(CC) $(CFLAGS) -c -o $@ $<

passenger.o: passenger.c passenger.h ipc_utils.h
	$(CC) $(CFLAGS) -c -o $@ $<

launch.o: launch.c ipc_utils.h passenger.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Limpiar los archivos generados
clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
