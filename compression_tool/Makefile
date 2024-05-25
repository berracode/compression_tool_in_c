# Nombre del compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -Wextra -g
CFLAGS = -g

# Archivos de encabezado
HEADERS = ktmem.h heap_priority_queue.h

# Archivos fuente
SRCS = ktmem.c heap_priority_queue.c main.c

# Archivos objeto generados
OBJS = $(SRCS:.c=.o)

# Nombre del ejecutable
EXEC = cpress

# Reglas de compilación
all: $(EXEC) clean_objs

# Regla para compilar el ejecutable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regla para compilar archivos .c a archivos .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos objeto
clean_objs:
	rm -f $(OBJS)

# Limpiar archivos generados
clean:
	rm -f $(OBJS) $(EXEC)

# Recompilar todo desde cero
rebuild: clean all

.PHONY: all clean clean_objs rebuild
