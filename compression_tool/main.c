#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "compress.h"
#include "decompress.h"
#include "binary_utils.h"
#include "kthash_table.h"
#include "ktmem.h"

binary_heap_pq_t *heap;
hash_table_t *hash_table;
kthash_table_t *prefix_code_table;

void help(char *argv){
    printf("\n");
    printf("usage: %s <COMMAND> <SRC_FILE> <DST_FILE> \n\n", argv);
    printf("Commands:\n");
    printf("    compress        compress text file\n");
    printf("    decompress      decompress data\n");
    printf("    help\n\n");

    printf("Options:\n\n");
    printf("    -h, --help      Print help\n");
    printf("    -v, --version   Print version\n");

}

void compress_file(char *file_path, char *dst_file_path) {
    unsigned int count_characters = 0;
    heap = create_binary_heap_pq();
    hash_table = create_table();


    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    char *content = calculate_frequencies_table_with_hash_table(hash_table, file_path, &count_characters);
    create_huffman_tree(heap, hash_table); //incluido build_tree();

    printf("---------- STARTING PREFIX HASH_TABLE with: %ld---------\n", hash_table->size);

    prefix_code_table = create_prefix_table(hash_table->size);
    build_prefix_code_table(heap, prefix_code_table);

    print_prefix_table(prefix_code_table);

    // TODO: escribir data del archivo plano en archivo de salida. MUY LENTO AÚN
    write_data_encoded_in_compressed_file(content, dst_file_path, prefix_code_table);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Tiempo transcurrido: %f segundos con hash table, pq, huffman_tree, prefix_code table\n", elapsed_time);

    ktfree(content);
    free_prefix_table(prefix_code_table);
    free_table(hash_table);
    free_binary_heap_pq(heap);

    printf("End by happy path\n");
}

void decompress_file(char *compressed_file, char *decompressed_file){
    read_headers_from_compressed_file(compressed_file, decompressed_file, &prefix_code_table);
    //print_prefix_table(prefix_code_table);

    free_prefix_table(prefix_code_table);

     printf("End by happy path\n");

}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        if(argc ==2 && (strcmp(argv[1], "-v")==0 ||strcmp(argv[1], "--version")==0)){
            printf("%s version 0.0.1, build xxx\n", argv[0]);
            return 0;
        }
        help(argv[0]);
        return 1;
    }

    char *command = argv[1];
    char *file_path = argv[2];
    char *dst_file_path = argv[3];

    if(strcmp(command, "compress")==0){
        compress_file(file_path, dst_file_path);
    } else if (strcmp(command, "decompress")==0){
        decompress_file(file_path, dst_file_path);
    } else {
        help(argv[0]);
    }

}

