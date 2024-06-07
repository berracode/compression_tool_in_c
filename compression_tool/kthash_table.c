#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kthash_table.h"
#include "ktmem.h"


kthash_table_t* create_prefix_table(size_t initial_capacity) {
    kthash_table_t *table = (kthash_table_t *)ktmalloc(sizeof(kthash_table_t));
    if (!table) return NULL;

    printf("size hash table %zu\n", initial_capacity);
    table->prefix_codes = (ktprefix_code_t **)calloc(initial_capacity , sizeof(ktprefix_code_t *));
    if (!table->prefix_codes) {
        ktfree(table);
        return NULL;
    }
    table->size = 0;
    table->capacity = initial_capacity;
    return table;
}

unsigned int hash_function(const size_t hash_table_size, const char *str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    //printf("hash=%d, ht size %ld\n", hash, hash_table_size);
    unsigned int n_hash = hash % hash_table_size;

    return n_hash;
}

ktprefix_code_t* find_prefix_code(kthash_table_t *table, const char *character) {
    unsigned int index = hash_function(table->capacity, character);
    ktprefix_code_t *current = table->prefix_codes[index];

    while (current != NULL) {
        if (strcmp(current->character, character) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL; // No se encontró el carácter
}


void insert_prefix_code(kthash_table_t *table, char *character, char *bits) {
    if(character != NULL && bits != NULL){
        unsigned int index= hash_function(table->capacity, character);
        printf("INDEX HT %d\n", index);

        ktprefix_code_t *new_node = (ktprefix_code_t *)ktmalloc(sizeof(ktprefix_code_t));
        new_node->character = (char*)ktmalloc(strlen(character) + 1);
        strcpy(new_node->character, character);

        new_node->bits = (char *)ktmalloc(strlen(bits) + 1);
        strcpy(new_node->bits, bits);

        new_node->next = NULL;

        // Insertar en la lista enlazada del bucket correspondiente
        if (table->prefix_codes[index] == NULL) {
            table->prefix_codes[index] = new_node;
        } else {
            ktprefix_code_t *current = table->prefix_codes[index];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
        }


        //table->prefix_codes[table->size].character = 
        //strcpy(table->prefix_codes[table->size].character, character);

        //table->prefix_codes[table->size].bits = (char*)ktmalloc(strlen(bits) + 1);
        //strcpy(table->prefix_codes[table->size].bits, bits);

        table->size++;
    }

}

void free_prefix_table(kthash_table_t *table) {
    for (size_t i = 0; i < table->capacity; i++) {
        ktprefix_code_t *current = table->prefix_codes[i];
        while (current != NULL) {
            ktprefix_code_t *temp = current;
            current = current->next;
            ktfree(temp->character);
            ktfree(temp->bits);
            ktfree(temp);
        }
    }
    ktfree(table->prefix_codes);
    ktfree(table);
}



void print_prefix_table(const kthash_table_t *table) {
    printf("capacity: %ld - size: %ld\n", table->capacity, table->size);
    for (size_t i = 0; i < table->capacity; i++) {
        ktprefix_code_t *current = table->prefix_codes[i];
        if(current != NULL) {
            while (current != NULL){
                printf("Bucket %ld: %s\n", i, current->character);
                current = current->next;
            }
            
            
        } else{
            printf("Bucket %ld VACIO\n", i);
        }

        //printf("Character: %s, Prefix: %s\n", table->prefix_codes[i].character, table->prefix_codes[i].bits);
    }
}

