#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "compress.h"
#include "ktmem.h"
#include "heap_priority_queue.h"
#include "binary_utils.h"


char* calculate_frequencies_table_with_hash_table(hash_table_t *hash_table, const char *filename, unsigned int *count_characters) {
    printf("Calculating frequencies table with hash table...\n");

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Can't open file");
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *content = ktmalloc(file_size+1);
    char *char_as_string = NULL;

    size_t bytes_read;

    while ((bytes_read = fread(content, 1, file_size, file)) > 0) {
        int i = 0;
        content[file_size] = '\0';
        while (i < bytes_read) {
            char_as_string = get_char_as_string(content, &i);
            insert(hash_table, char_as_string);
            *count_characters = *count_characters + 1;
            ktfree(char_as_string);
        }
    }
    fclose(file);
    return content;
}

void create_priority_queue(binary_heap_pq_t *heap, hash_table_t *hash_table) {
    printf("building priority queue...\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_node_t *current = hash_table->table[i];
        while (current != NULL) {
            insert_node(heap, current->key, current->frequency);
            current = current->next;
        }
    }
}

void create_tree(binary_heap_pq_t *heap){

    heap_pq_node_t *first_temp;
    heap_pq_node_t *second_temp;

    printf("Building binary tree...\n");
    while (!is_empty(heap)&& heap->size > 1) {
        remove_top_node(heap, &first_temp);
        remove_top_node(heap, &second_temp);

        int weight = first_temp->tree_node->data->weight + second_temp->tree_node->data->weight;
        heap_pq_node_t *root_node = create_new_node(NULL, weight);

        root_node->tree_node->left = first_temp->tree_node;
        root_node->tree_node->right = second_temp->tree_node;
        first_temp->tree_node = NULL;
        second_temp->tree_node = NULL;

        free_heap_pq_node(second_temp);
        free_heap_pq_node(first_temp);
        insert_heap_pq_node(heap, root_node);

    }
}

void create_huffman_tree(binary_heap_pq_t *heap, hash_table_t *hash_table) {

    create_priority_queue(heap, hash_table);

    create_tree(heap);
}

// FUNCTIONS FOR PREFIX CODE TABLE
int* increase(int *counter){
    *counter = *counter + 1;
    return counter;
}

void fill_with_blank_space(char *code, int len){
    for (int i = 0; i < len; i++){
        code[i] = 32;
    }
    code[len] = '\0';
}

void visit_tree_node_post_order(tree_node_t *node, int *edges, int *count_left, int *count_right, int is_left, char *code, kthash_table_t *prefix_code_table) {
    if (node == NULL) {
        if(is_left){
            *count_left = *count_left -1;

        } else {
            *count_right = *count_right -1;
        }
        return;
    }

    *edges = *count_left+(*count_right);
    if(*edges > 0){
        int len = *edges;
        if(code == NULL || len == 1){
            code = (char*)ktmalloc(len + 1);
            fill_with_blank_space(code, len);
        } else if(code != NULL) {
            char *new_code = (char*)ktmalloc(len + 1);
            strcpy(new_code, code);
            code = (char*)ktmalloc(len + 1);
            code[len] = '\0';
            strcpy(code, new_code);
            ktfree(new_code);
        }
        if(is_left){
            code[len-1] = '0';
        }else{
            code[len-1] = '1';
        }
    }
    visit_tree_node_post_order(node->left, edges, increase(count_left), count_right, 1, code, prefix_code_table);
    visit_tree_node_post_order(node->right, edges, count_left, increase(count_right), 0, code, prefix_code_table);
    if (node->data != NULL) {
        if(node->data->element !=NULL) {
            *edges = 0;
            if(*count_left>=1 && is_left){
                *count_left = *count_left -1;
            }
            if(*count_right>=1 && !is_left){
                *count_right = *count_right -1;
            }
            insert_prefix_code(prefix_code_table, node->data->element, node->data->weight, code);
        }else {

            if(*count_left>=1 && is_left){
                *count_left = *count_left -1;
            }
            if(*count_right>=1 && !is_left){
                *count_right = *count_right -1;
            }
        }
        ktfree(code);
    }
}

void build_prefix_code_table(binary_heap_pq_t *heap, kthash_table_t *prefix_code_table){

    heap_pq_node_t *head = heap->head;
    int i = 0, count_left = 0, count_right = 0;
    char *code = NULL;
    //TODO: refactor this function (it's work but can be more simple)
    visit_tree_node_post_order(head->tree_node, &i, &count_left, &count_right, 1, code, prefix_code_table);
    ktfree(code);
}

// FUNCTIONS TO WRITE DATA COMPRESSED
void write_headers_in_compressed_file(kthash_table_t *prefix_code_table, const char *output_filename, int padding){
    FILE *output = fopen(output_filename, "wb");

    if (!output) {
        perror("Error: opening file");
        exit(EXIT_FAILURE);
    }
    // write prefix code table len
    fwrite(&prefix_code_table->size, sizeof(uint8_t), 1, output); //save numbers of "characters"
    fwrite(&padding, sizeof(uint8_t), 1, output); //padding right

    for (size_t i = 0; i < prefix_code_table->size; i++) {
        ktprefix_code_t *current = prefix_code_table->prefix_codes[i];
        while (current != NULL) {
            unsigned int char_len = strlen(current->character);
            unsigned int bits_len = strlen(current->bits);

            fwrite(&char_len, sizeof(uint8_t), 1, output);
            fwrite(current->character, sizeof(char), char_len, output);
            
            fwrite(&bits_len, sizeof(uint8_t), 1, output);
            write_bits(output, current->bits);
            current = current->next;
        }

    }

    // delimiter used to separate compressed headers and compressed data
    char delimiter = '\n';
    fwrite(&delimiter, sizeof(char), 1, output);
    fclose(output);

}

void write_data_encoded_in_compressed_file(const char *content, const char *output_file_path, kthash_table_t *prefix_code_table){
    printf("Writing compressed data...\n");

    size_t file_size = strlen(content);
    size_t max_bits_len = prefix_code_table->max_bits_len;

    char *char_as_string;
    char *compress_data = ktmalloc(max_bits_len+1);
    int writter_index = 0;

    int index = 0;
    while (index < file_size){
        char_as_string = get_char_as_string(content, &index);

        ktprefix_code_t *found = find_prefix_code(prefix_code_table, char_as_string);
        int bits_len = strlen(found->bits);
        for (size_t i = 0; i < bits_len; i++){
            compress_data[writter_index] = found->bits[i];
            writter_index++;
        }
        ktfree(char_as_string);
    }

    compress_data[max_bits_len] = '\0';

    //write headers
    int padding = strlen(compress_data) % 8;
    write_headers_in_compressed_file(prefix_code_table, output_file_path, padding);

    FILE *output_file = fopen(output_file_path, "ab");
    if (!output_file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    //write all the compressed data
    write_bits(output_file, compress_data);

    ktfree(compress_data);
    fclose(output_file);

}
