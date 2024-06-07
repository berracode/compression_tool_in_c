#ifndef COMPRESS_H
#define COMPRESS_H
#include <time.h>
#include "heap_priority_queue.h"
#include "simple_hash_table.h"

void calculate_frequencies_table(binary_heap_pq_t *heap, const char *filename, unsigned int *count_characters);
char* calculate_frequencies_table_with_hash_table(hash_table_t *hash_table, const char *filename, unsigned int *count_characters);
void create_huffman_tree(binary_heap_pq_t *heap, hash_table_t *hash_table);

#endif