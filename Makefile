# Nombre del compilador
CC = gcc

# Opciones de compilación para desarrollo
CFLAGS_DEV = -Wall -Wextra -g

# Opciones de compilación para producción
CFLAGS_PROD = -O3 -flto -DNDEBUG -march=native

# Archivos de encabezado
HEADERS = ktmem.h heap_priority_queue.h kthash_table.h compress.h simple_hash_table.h binary_utils.h decompress.h

# Archivos fuente
SRCS = ktmem.c heap_priority_queue.c kthash_table.c compress.c simple_hash_table.c binary_utils.c decompress.c main.c

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

# Tareas para compilación de desarrollo y producción
dev: CFLAGS = $(CFLAGS_DEV)
dev: rebuild

prod: CFLAGS = $(CFLAGS_PROD)
prod: rebuild
	strip $(EXEC)

.PHONY: all clean clean_objs rebuild dev prod
