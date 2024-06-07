#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "compress.h"
#include "ktmem.h"
#include "heap_priority_queue.h"

char* get_char_as_string(const char *str, int *index) {
    char c = str[*index];
    int length = 1;
    //print_binary(c, 8);
    if ((c & 0x80) == 0x00) {
        length = 1; // 1 byte UTF-8 (ASCII)
    } else if ((c & 0xE0) == 0xC0) {
        length = 2; // 2 bytes
    } else if ((c & 0xF0) == 0xE0) {
        length = 3; // 3 bytes
    } else if ((c & 0xF8) == 0xF0) {
        length = 4; // 4 bytes
    }

    char* utf8_char = (char*)ktmalloc(length + 1);

    for (int i = 0; i < length; i++) {
        utf8_char[i] = str[*index + i];
    }
    utf8_char[length] = '\0';

    *index += length;
    return utf8_char;
}

/**
 * calcula la tabla de frencuencias (nada más) y retorna un hash table con esa
 * información
 * HACER: usar la hash_table paa construir una cola de prioridad y el huffman tree 
 * (usar el binary_heap_pq_t)
*/
char* calculate_frequencies_table_with_hash_table(hash_table_t *hash_table, const char *filename, unsigned int *count_characters) {
    printf("Calculate frequencies table with hash table\n");

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Can't open file");
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *content = ktmalloc(file_size);
    char *char_as_string;

    size_t bytes_read;
    

    while ((bytes_read = fread(content, 1, file_size, file)) > 0) {
        int i = 0;
        while (i < bytes_read) {
            char_as_string = get_char_as_string(content, &i);
            insert(hash_table, char_as_string);
            *count_characters = *count_characters + 1;
            ktfree(char_as_string);
        }
    }
    *count_characters = (int)hash_table->size;
    printf("\ncount_characters %d, - %ld\n", *count_characters, hash_table->size);

    printf("Frequencies table\n");
    //traverse_list_and_trees(heap, print_heap_post_order);
    //print_frequencies(hash_table);

    fclose(file);
    return content;
}


void create_priority_queue(binary_heap_pq_t *heap, hash_table_t *hash_table) {
    printf("Construyendo priority_queue...\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_node_t *current = hash_table->table[i];
        while (current != NULL) {
            insert_node(heap, current->key, current->frequency);
            //printf("Character: %s, Frequency: %d\n", current->key, current->frequency);
            current = current->next;
        }
    }
}



void create_tree(binary_heap_pq_t *heap){
    // leer los dos primeros nodos y removerlos
    // sumar los pesos
    // crear un nuevo tree_nodo_t para el peso, y ponerlo a apuntar a los dos nodos leidos por left y right
    // insertarlo en el heap nuevamente.
    // repetir hasta que solo haya 1 nodo en el heap

    heap_pq_node_t *first_temp;
    heap_pq_node_t *second_temp;

    printf("---------------Building binary tree---------------------\n");
    while (!is_empty(heap)&& heap->size > 1) {
        remove_top_node(heap, &first_temp);
        remove_top_node(heap, &second_temp);

        int weight = first_temp->tree_node->data->weight + second_temp->tree_node->data->weight;
        heap_pq_node_t *root_node = create_new_node(NULL, weight);
        //printf("## ROOT NODE: %d\n", root_node->tree_node->data->weight);
        //print_heap_pq_node(root_node);

        root_node->tree_node->left = first_temp->tree_node;
        root_node->tree_node->right = second_temp->tree_node;
        first_temp->tree_node = NULL;
        second_temp->tree_node = NULL;

        free_heap_pq_node(second_temp);
        free_heap_pq_node(first_temp);
        insert_heap_pq_node(heap, root_node);

    }

    //traverse_list_and_trees(heap, print_heap_post_order);

}


void create_huffman_tree(binary_heap_pq_t *heap, hash_table_t *hash_table) {

    create_priority_queue(heap, hash_table);

    create_tree(heap);

    traverse_list_and_trees(heap, print_heap_post_order);

}

