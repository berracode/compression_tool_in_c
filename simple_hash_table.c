#include <stdio.h>
#include <string.h>
#include "simple_hash_table.h"
#include "ktmem.h"

unsigned int hash_function2(const char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

hash_table_t* create_table() {
    hash_table_t *hashTable = (hash_table_t *)ktmalloc(sizeof(hash_table_t));
    hashTable->table = (hash_node_t **)calloc(TABLE_SIZE, sizeof(hash_node_t *));
    for (int i = 0; i < TABLE_SIZE; i++){
        hashTable->table[i] = NULL;
    }

    hashTable->size = 0;
    hashTable->capacity = TABLE_SIZE;
    
    return hashTable;
}

hash_node_t* create_node(const char *key, int frequency) {
    hash_node_t *newNode = malloc(sizeof(hash_node_t));
    newNode->key = strdup(key);
    newNode->frequency = frequency;
    newNode->next = NULL;
    return newNode;
}

void insert(hash_table_t *hashTable, const char *key) {
    unsigned int index = hash_function2(key);
    hash_node_t *newNode = create_node(key, 1);

    if (hashTable->table[index] == NULL) {
        hashTable->table[index] = newNode;
        hashTable->size++;
    } else {
        hash_node_t *current = hashTable->table[index];
        hash_node_t *prev = NULL;
        while (current != NULL && strcmp(current->key, key) != 0) {
            prev = current;
            current = current->next;
        }
        if (current == NULL) {
            prev->next = newNode;
            hashTable->size++;
        } else {
            current->frequency++;
            free(newNode->key);
            free(newNode);
        }
    }
}

int get_frequency(hash_table_t *hashTable, const char *key) {
    unsigned int index = hash_function2(key);
    hash_node_t *current = hashTable->table[index];
    while (current != NULL && strcmp(current->key, key) != 0) {
        current = current->next;
    }
    return current ? current->frequency : 0;
}

void free_table(hash_table_t *hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_node_t *current = hashTable->table[i];
        while (current != NULL) {
            hash_node_t *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(hashTable->table);
    free(hashTable);
}

void print_frequencies(hash_table_t *hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_node_t *current = hashTable->table[i];
        while (current != NULL) {
            printf("Character: %s, Frequency: %d\n", current->key, current->frequency);
            current = current->next;
        }
    }
}
