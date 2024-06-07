#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "compress.h"
#include "binary_utils.h"
#include "kthash_table.h"
#include "ktmem.h"

binary_heap_pq_t *heap;
hash_table_t *hash_table;
kthash_table_t *prefix_code_table;

void help(char *argv){
    printf("\n");
    printf("usage: %s <COMMAND> FILE_PATH\n\n", argv);
    printf("Commands:\n");
    printf("    compress        compress data\n");
    printf("    decompress      decompress data\n");
    printf("    help\n\n");

    printf("Options:\n\n");
    printf("    -h, --help      Print help\n");
    printf("    -v, --version   Print version\n");

}

void concatenate_bits(char **compress_data, const char *bits) {
    size_t old_len = *compress_data ? strlen(*compress_data) : 0;
    //printf("bits len %ld\n", strlen(bits));
    size_t new_len = old_len + strlen(bits) + 1;
    //printf("new_len %ld\n", new_len);

    char *new_data = ktmalloc(new_len);

    if (*compress_data) {
        strcpy(new_data, *compress_data);
        free(*compress_data);
    }else{
        new_data[0] = '\0';
        //printf("new data len: %ld, new_data:|%s|\n", strlen(new_data), new_data);
    }
    //printf("new data len: %ld\n", strlen(new_data));
    strcat(new_data, bits);

    *compress_data = new_data;
}

void concatenate_bits2(char **destination, const char *source) {
    size_t dest_len = *destination ? strlen(*destination) : 0;
    size_t src_len = strlen(source);

    char *new_data = realloc(*destination, dest_len + src_len + 1);
    if (!new_data) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    if (dest_len == 0) {
        strcpy(new_data, source);
    } else {
        strcat(new_data, source);
    }

    *destination = new_data;
}


void write_data_encoded_in_compressed_file(const char *content, const char *output_file_path){

    struct timespec start, end, start1, end1, start2, end2;
    clock_gettime(CLOCK_MONOTONIC, &start);

    FILE *output_file = fopen(output_file_path, "ab");
    if (!output_file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);

    }

    printf("---------- INICIA ESCRITURA DE DATA COMPRIMIDA---------\n");
    //recorrer el archivo plano y sacar los caracteres y buscarlos en la hashtable
    // TODO: (función hash para optimizar busqueda)
    // TODO: volver esto un metodo ya que se reutiliza

    size_t file_size = strlen(content);
    printf("#######################\n");
    printf("file size: %ld\n", file_size);

    char *char_as_string;
    char *compress_data = ktmalloc(file_size);

    int index = 0;
    printf("bytes_read_pf %ld\n", file_size);
    clock_gettime(CLOCK_MONOTONIC, &start2);
    while (index < file_size){
        char_as_string = get_char_as_string(content, &index);
        //printf("char_string %s | ht size: %zu| index=%d - bytes_read=%ld\n", char_as_string, prefix_code_table->size, index, bytes_read_pf);
        //buscar en la tabla hash el codigo binario de ese char_as_string
        size_t prefix_code_table_size = prefix_code_table->size;


        ktprefix_code_t *found = find_prefix_code(prefix_code_table, char_as_string);
        //concatenate_bits2(&compress_data, found->bits);
        strcat(compress_data, found->bits);
        ktfree(char_as_string);

    }
    
    clock_gettime(CLOCK_MONOTONIC, &end2);


    clock_gettime(CLOCK_MONOTONIC, &start1);
    write_bits(output_file, compress_data);
    clock_gettime(CLOCK_MONOTONIC, &end1);



    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_time = (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec) / 1e9;

    double elapsed_time1 = (end1.tv_sec - start1.tv_sec) +
                          (end1.tv_nsec - start1.tv_nsec) / 1e9;

    double elapsed_time2 = (end2.tv_sec - start2.tv_sec) +
                          (end2.tv_nsec - start2.tv_nsec) / 1e9;

    printf("Tiempo transcurrido: %f segundos\n", elapsed_time);
    printf("Tiempo transcurrido en write bits: %f segundos\n", elapsed_time1);
    printf("Tiempo transcurrido en while y concatenar: %f segundos\n", elapsed_time2);

    printf("---------- BITS CD:|%ld|\n", strlen(compress_data));
    printf("---------- FINALIZA ESCRITURA DE DATA COMPRIMIDA---------\n");
    ktfree(compress_data);
    fclose(output_file);

}


void compress_file(char *file_path) {
    unsigned int count_characters = 0;
    heap = create_binary_heap_pq();
    hash_table = create_table();


    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    char *content = calculate_frequencies_table_with_hash_table(hash_table, file_path, &count_characters);
    create_huffman_tree(heap, hash_table); //incluido build_tree();

    printf("---------- STARTING PREFIX HASH_TABLE with: %d---------\n", count_characters);

    prefix_code_table = create_prefix_table(count_characters);
    build_prefix_code_table(heap, prefix_code_table);

    print_prefix_table(prefix_code_table);

    write_headers_in_compressed_file(prefix_code_table,"./test_files/output.bin", count_characters);
    
    // TODO: escribir data del archivo plano en archivo de salida. MUY LENTO AÚN
    //write_data_encoded_in_compressed_file(content, "./test_files/output.bin");

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

int main(int argc, char *argv[]) {

    if (argc < 3) {
        if(argc ==2 && (strcmp(argv[1], "-v")==0 ||strcmp(argv[1], "--version")==0)){
            printf("%s version 0.0.1, build xxx\n", argv[0]);
            return 1;
        }
        help(argv[0]);
        return 1;
    }

    char *command = argv[1];
    char *file_path = argv[2];

    if(strcmp(command, "compress")==0){
        compress_file(file_path);
    } else if (strcmp(command, "decompress")==0){
        printf("Not implemented yet\n");
    } else {
        help(argv[0]);
    }

}

