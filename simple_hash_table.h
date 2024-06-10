#ifndef SIMPLE_HASH_TABLE_H
#define SIMPLE_HASH_TABLE_H
#include <stdlib.h>

#define TABLE_SIZE 200

typedef struct hash_node_t {
    char *key;
    int frequency;
    struct hash_node_t *next;
} hash_node_t;

typedef struct hash_table_t {
    hash_node_t **table;
    size_t size;
    size_t capacity;
} hash_table_t;


unsigned int hash_function2(const char *str);
hash_table_t* create_table();
hash_node_t* create_node(const char *key, int frequency);
void insert(hash_table_t *hash_table, const char *key);
int get_frequency(hash_table_t *hash_table, const char *key);
void free_table(hash_table_t *hash_table);


#endif