#ifndef DECOMPRESS_H
#define DECOMPRESS_H
#include "kthash_table.h"


void read_headers_from_compressed_file(char *compressed_file, char* output, kthash_table_t **prefix_code_table);

#endif