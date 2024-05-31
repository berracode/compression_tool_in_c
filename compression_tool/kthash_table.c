#include <stdio.h>
#include <string.h>
#include "kthash_table.h"
#include "ktmem.h"


kthash_table_t* create_prefix_table(size_t initial_capacity) {
    kthash_table_t *table = (kthash_table_t *)ktmalloc(sizeof(kthash_table_t));
    if (!table) return NULL;

    printf("size hash table %zu\n", initial_capacity);
    table->prefix_codes = (ktprefix_code_t *)ktmalloc(initial_capacity * sizeof(ktprefix_code_t));
    if (!table->prefix_codes) {
        ktfree(table);
        return NULL;
    }
    table->size = 0;
    return table;
}

void insert_prefix_code(kthash_table_t *table, char *character, char *bits) {
    if(character != NULL && bits != NULL){
        table->prefix_codes[table->size].character = (char*)ktmalloc(strlen(character) + 1);
        strcpy(table->prefix_codes[table->size].character, character);

        table->prefix_codes[table->size].bits = (char*)ktmalloc(strlen(bits) + 1);
        strcpy(table->prefix_codes[table->size].bits, bits);
        table->size++;
    }

}

void free_prefix_table(kthash_table_t *table) {
    if (!table) return;

    printf("----------Liberando hash table\n");
    for (size_t i = 0; i < table->size; i++) {
        printf("2 Liberando hash table\n");

        ktfree(table->prefix_codes[i].character);
                printf("3 Liberando hash table\n");

        ktfree(table->prefix_codes[i].bits);
    }
            printf("4 Liberando hash table\n");

    ktfree(table->prefix_codes);
            printf("5 Liberando hash table\n");

    ktfree(table);
            printf("6 Liberando hash table\n");

}


unsigned int hash_function(const size_t hash_table_size, const char *str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash % hash_table_size;
}


void print_prefix_table(const kthash_table_t *table) {
    for (size_t i = 0; i < table->size; i++) {
        printf("Character: %s, Prefix: %s\n", table->prefix_codes[i].character, table->prefix_codes[i].bits);
    }
}

