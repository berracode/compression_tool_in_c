#ifndef KTHASH_TABLE_H
#define KTHASH_TABLE_H
#include <stdlib.h>


typedef struct ktprefix_code_t {
    char *character;
    char *bits;
    struct ktprefix_code_t *next;
} ktprefix_code_t;

typedef struct kthash_table_t{
    ktprefix_code_t **prefix_codes;
    size_t size;
    size_t capacity;
    size_t max_bits_len;
} kthash_table_t;

kthash_table_t* create_prefix_table(size_t initial_capacity);

void insert_prefix_code(kthash_table_t *table, char *character, int frequency, char *bits);

void free_prefix_table(kthash_table_t *table);

void print_prefix_table(const kthash_table_t *table);

ktprefix_code_t* find_prefix_code(kthash_table_t *table, const char *character);


#endif