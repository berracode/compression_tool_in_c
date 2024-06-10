#ifndef COMPRESS_H
#define COMPRESS_H
#include <time.h>
#include "heap_priority_queue.h"
#include "kthash_table.h"
#include "simple_hash_table.h"

char* get_char_as_string(const char *str, int *index);

void calculate_frequencies_table(binary_heap_pq_t *heap, const char *filename, unsigned int *count_characters);
char* calculate_frequencies_table_with_hash_table(hash_table_t *hash_table, const char *filename, unsigned int *count_characters);
void create_huffman_tree(binary_heap_pq_t *heap, hash_table_t *hash_table);
void build_prefix_code_table(binary_heap_pq_t *heap, kthash_table_t *prefix_code_table);
void write_headers_in_compressed_file(kthash_table_t *prefix_code_table, const char *output_filename, int padding);
void write_data_encoded_in_compressed_file(const char *content, const char *output_file_path, kthash_table_t *prefix_code_table);

#endif