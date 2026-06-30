# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = sistema
OBJS = main.o usuarios.o productos.o
SRCS = main.c usuarios.c productos.c

# Reglas principales
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

# Compilar archivos objeto
main.o: main.c usuarios.h productos.h
	$(CC) $(CFLAGS) -c main.c

usuarios.o: usuarios.c usuarios.h
	$(CC) $(CFLAGS) -c usuarios.c

productos.o: productos.c productos.h
	$(CC) $(CFLAGS) -c productos.c

# Generar archivos ensamblador (.s) desde los fuentes
main.s: main.c
	$(CC) $(CFLAGS) -S main.c

usuarios.s: usuarios.c
	$(CC) $(CFLAGS) -S usuarios.c

productos.s: productos.c
	$(CC) $(CFLAGS) -S productos.c

asm: main.s usuarios.s productos.s

# Convertir archivos objeto (.o) a ensamblador (.s) con objdump
main.s.dump: main.o
	objdump -d -S main.o > main.s.dump

usuarios.s.dump: usuarios.o
	objdump -d -S usuarios.o > usuarios.s.dump

productos.s.dump: productos.o
	objdump -d -S productos.o > productos.s.dump

disasm: main.s.dump usuarios.s.dump productos.s.dump

# Limpiar
clean:
	rm -f $(TARGET) $(OBJS) main.s usuarios.s productos.s main.s.dump usuarios.s.dump productos.s.dump

# Limpiar todo incluyendo el ejecutable
distclean: clean

phony: all clean distclean asm disasm
