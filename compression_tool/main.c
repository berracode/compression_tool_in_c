#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "compress.h"
#include "kthash_table.h"
#include "ktmem.h"

binary_heap_pq_t *heap;
hash_table_t *hash_table;
kthash_table_t *prefix_hash_table;

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


void print_binary(unsigned int value, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        unsigned int mask = 1 << i;
        putchar((value & mask) ? '1' : '0');
    }
    putchar('\n');
}

int* increase(int *counter){
    *counter = *counter + 1;
    return counter;
}

void fill_with_blank_space(char *code, int len){
    //printf("ESPACIO EN BLANCO: %d\n", len);
    for (int i = 0; i < len; i++)
    {
        code[i] = 32;
    }
    code[len] = '\0';
}


void visit_tree_node_post_order(tree_node_t *node, int *edges, int *count_left, int *count_right, int is_left, char *code) {
    if (node == NULL) {
        if(is_left){
            *count_left = *count_left -1;

        } else {
            *count_right = *count_right -1;
        }
        return;
    }

    *edges = *count_left+(*count_right);
    //printf("---------------------------\n");
    //printf("CL %d | CR %d\n", *count_left, *count_right);
    //printf("### peso: %d\n",node->data->weight);
    if(*edges > 0){
        int len = *edges;
        //printf("len %d | code %s |\n\n", len, code);
        if(code == NULL || len == 1){
            code = (char*)ktmalloc(len + 1);
            fill_with_blank_space(code, len);
        } else if(code != NULL) {
            char *new_code = (char*)ktmalloc(len + 1);
            strcpy(new_code, code);
            code = (char*)ktmalloc(len + 1);
            code[len] = '\0';
            //fill_with_blank_space(code, len);
            strcpy(code, new_code);
            //printf("new code sin modificar: %s\n", new_code);
            ktfree(new_code);
        }
        if(is_left){
            code[len-1] = '0';
            //printf("#### Cadena %s\n", code);
        }else{
            code[len-1] = '1';
            //printf("#### Cadena %s\n", code);
        }
    }
    visit_tree_node_post_order(node->left, edges, increase(count_left), count_right, 1, code);
    visit_tree_node_post_order(node->right, edges, count_left, increase(count_right), 0, code);
    if (node->data != NULL) {
        if(node->data->element !=NULL) {
            //printf("(2) CL %d | CR %d | Is_left %d\n", *count_left, *count_right, is_left);

            *edges = 0;
            if(*count_left>=1 && is_left){
                *count_left = *count_left -1;
            }
            if(*count_right>=1 && !is_left){
                *count_right = *count_right -1;
            }
            //printf("(3) CL %d | CR %d | Is_left %d\n", *count_left, *count_right, is_left);
            insert_prefix_code(prefix_hash_table, node->data->element, code);
            printf("|\t\t%s \t\t|\t\t %d\t\t|\t\t%s\t\t|\t\t%ld\t\t|\n",node->data->element, node->data->weight, code, strlen(code));

        }else {
              //          printf("(NODO ROOT 1) CL %d | CR %d | Is_left %d\n", *count_left, *count_right, is_left);

            if(*count_left>=1 && is_left){
                *count_left = *count_left -1;
            }
            if(*count_right>=1 && !is_left){
                *count_right = *count_right -1;
            }
            //printf("(NODO ROOT 2) CL %d | CR %d | Is_left %d\n", *count_left, *count_right, is_left);
        }
        ktfree(code);
    }
}

void build_prefix_code_table(){
    /*
    recorrer hasta las hojas
    si es un nodo a la izquierda ponerle 0
    */

    heap_pq_node_t *head = heap->head;
    int i = 0, count_left = 0, count_right = 0;
    char *code = NULL;
    printf("|\t\tCHAR\t\t|\t\tFREQ\t\t|\t\tCODE\t\t|\t\tBITS\t\t|\n");
    visit_tree_node_post_order(head->tree_node, &i, &count_left, &count_right, 1, code);
    ktfree(code);
}

void print_string_as_bits(const char *bits){

        //print_binary(c, 8);
    //printf("BINARIO\n");
    int len = strlen(bits);
    for (int i = 0; i < len; i++){
        print_binary(bits[i], 8);
    }
    

}

// Función para convertir una cadena de bits a un byte
uint8_t bits_to_byte(const char *bits) {
    //printf("bits %s\n", bits); //110110110 - 10
    //print_string_as_bits(bits);
    uint8_t byte = 0;
    /*while (*bits) {
        byte = (byte << 1) | (*bits++ - '0');
    }*/

    // Iterar sobre la cadena de bits hasta que se alcance el final ('\0')
    // 110110110
    // 10110110  byte
    // 00000001 bit_value
    //B11011011 byte 
    int iter = 0;
    while (*bits != '\0' && iter < 8) {
        //printf("byte antes %d\n", byte);
        // Desplazar el byte un lugar a la izquierda
        byte <<= 1;
        //printf("byte despues %d\n", byte);

        // Convertir el carácter binario actual en un valor numérico (0 o 1)
        uint8_t bit_value = (*bits == '1') ? 1 : 0;
        
        // Aplicar la operación OR para establecer el bit en la posición actual
        byte |= bit_value;
        //printf("byte final %d\n", byte);

        
        // Avanzar al siguiente carácter en la cadena de bits
        bits++;
        iter++;
    }
    return byte;
}

void write_bits(FILE *file, const char *bits) {
    size_t len = strlen(bits);
    size_t full_bytes = len / 8;
    size_t remaining_bits = len % 8;

    //printf("len %zu, full %zu, r %zu\n", len, full_bytes, remaining_bits);

    // Escribir bytes completos
    for (size_t i = 0; i < full_bytes; i++) {
        //printf("QQQQQQQQQQQQQQQQQQQQQQ ocupa más de un byte\n");
        uint8_t byte = bits_to_byte(bits + i * 8);
        //printf("byte: %d\n", byte);
        fwrite(&byte, sizeof(uint8_t), 1, file);
    }

    // Escribir bits restantes
    if (remaining_bits > 0) {
        //printf("QQQQQQQQQQQQQQQQQQQQQQ NO ocupa más de un byte\n");

        char last_byte_str[9] = {0};  // Un byte más el terminador nulo
        strncpy(last_byte_str, bits + full_bytes * 8, remaining_bits);
        //printf("last byte str %s\n", last_byte_str);
        uint8_t last_byte = bits_to_byte(last_byte_str);
        fwrite(&last_byte, sizeof(uint8_t), 1, file);
    }
}

void write_headers_in_compressed_file(const char *output_filename, unsigned int count_characters){
    FILE *output = fopen(output_filename, "wb");

    if (!output) {
        perror("Error: opening file");
        exit(EXIT_FAILURE);
    }
    // Escribir el tamaño de la tabla de prefijos
    printf("tabla size: %ld, sizeof: %zu\n", prefix_hash_table->size, sizeof(prefix_hash_table->size));
    printf("count_characters size of: %zu\n", sizeof(count_characters));
    fwrite(&prefix_hash_table->size, sizeof(uint8_t), 1, output);

    // Escribir cada prefijo en el encabezado
    for (size_t i = 0; i < prefix_hash_table->size; i++) {
        ktprefix_code_t *current = prefix_hash_table->prefix_codes[i];
        while (current != NULL) {
            //printf("escribiendo header char %s bits %s\n",current->character, current->bits);
            unsigned int char_len = strlen(current->character);
            unsigned int bits_len = strlen(current->bits);

            //printf("char_len %u y sizeof %zu\n", char_len, sizeof(char_len));// 1 - 4
            //printf("bits_len %u y sizeof %zu\n--------\n", bits_len, sizeof(bits_len));

            
            // Escribir la longitud del carácter y la cadena del carácter
            fwrite(&char_len, sizeof(uint8_t), 1, output);
            fwrite(current->character, sizeof(char), char_len, output);
            
            // Escribir la longitud de los bits y la cadena de bits
            fwrite(&bits_len, sizeof(uint8_t), 1, output);
            //fwrite(prefix_hash_table->prefix_codes[i].bits, sizeof(char), bits_len, output);
            write_bits(output, current->bits);

            current = current->next;
        }

    }

    // Escribir un delimitador (un salto de línea) para separar el encabezado del contenido
    char delimiter = '\n';
    fwrite(&delimiter, sizeof(char), 1, output);

    fclose(output);

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

/*
void write_data_encoded_in_compressed_file(const char *plain_file_path, const char *output_file_path ){

    FILE *output_file = fopen(output_file_path, "ab");
    if (!output_file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);

    }


    printf("---------- INICIA ESCRITURA DE DATA COMPRIMIDA---------\n");
    //recorrer el archivo plano y sacar los caracteres y buscarlos en la hashtable
    // TODO: (función hash para optimizar busqueda)
    // TODO: volver esto un metodo ya que se reutiliza
    long file_size = get_file_size(plain_file_path);

    FILE *plain_file = fopen(plain_file_path, "rb");
    if (plain_file == NULL) {
        perror("Can't open file");
        exit(EXIT_FAILURE);
    }

    unsigned char *buffer_pf = ktmalloc(file_size);
    char *char_as_string;

    char *compress_data = ktmalloc(3282838);

    size_t bytes_read_pf;

    struct timespec start, end, start1, end1, start2, end2;

    clock_gettime(CLOCK_MONOTONIC, &start);

    while ((bytes_read_pf = fread(buffer_pf, 1, file_size, plain_file)) > 0) {
        int index = 0;
        printf("bytes_read_pf %ld\n", bytes_read_pf);
        clock_gettime(CLOCK_MONOTONIC, &start2);
        while (index < bytes_read_pf)
        {
            if(!(buffer_pf[index] >= 0 && buffer_pf[index] <=31) && buffer_pf[index]!=0x7F){
                char_as_string = get_utf8_string(buffer_pf, &index);
                //printf("char_string %s | ht size: %zu| index=%d - bytes_read=%ld\n", char_as_string, prefix_hash_table->size, index, bytes_read_pf);
                //buscar en la tabla hash el codigo binario de ese char_as_string
                size_t prefix_hash_table_size = prefix_hash_table->size;


                ktprefix_code_t *found = find_prefix_code(prefix_hash_table, char_as_string);
                //concatenate_bits2(&compress_data, found->bits);
                strcat(compress_data, found->bits);
                ktfree(char_as_string);

            } else {
                index++;
            }

        }
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
    ktfree(buffer_pf);
    fclose(plain_file);
    fclose(output_file);

}
*/


void free_priority_queue(){
    printf("---------------Liberando Priority queue---------------------\n");
    heap_pq_node_t *topNode;
    while (!is_empty(heap)) {
        remove_top_node(heap, &topNode);
        //printf("aqui para: \n");
        //printf("$ en remove %s: %d\n", topNode->tree_node->data->element, topNode->tree_node->data->weight);
        //printf("aqui para: \n");

        free_heap_pq_node(topNode);
    }
    printf("priority queue size: %d\n", heap->size);
    if(is_empty(heap)){
        printf("NO vacia\n");
        free_heap_priority_queue(heap);
    }
}

void compress_file(char *file_path) {
    unsigned int count_characters = 0;
    heap = create_binary_heap_pq();
    //binary_heap_pq_t *heap2 = create_binary_heap_pq();
    hash_table = create_table();

    //calculate_frequencies_table(heap, file_path, &count_characters);
    //count_characters = 0;

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    char *content = calculate_frequencies_table_with_hash_table(hash_table, file_path, &count_characters);
    create_huffman_tree(heap, hash_table); //incluido build_tree();

    //free_priority_queue();
    //read_file_with_buffer(file_path, &count_characters);
    //build_tree();
    //printf("---------------Ending binary tree build---------------------\n");

    printf("---------- STARTING PREFIX HASH_TABLE with: %d---------\n", count_characters);

    prefix_hash_table = create_prefix_table(count_characters);
    build_prefix_code_table();

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_time = (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Tiempo transcurrido: %f segundos con hash table, pq, huffman_tree, prefix_code table\n", elapsed_time);



    //print_prefix_table(prefix_hash_table);
    //printf("---------------ENDING PREFIX HASH_TABLE---------------------\n");
    // for (size_t i = 0; i < prefix_hash_table->size; i++) {
    //     printf("Character: %s, Bits: %s\n", prefix_hash_table->prefix_codes[i].character, prefix_hash_table->prefix_codes[i].bits);
    // }
    //write_headers_in_compressed_file("./test_files/output.bin",count_characters);
    // escribir data del archivo plano en archivo de salida.
    //write_data_encoded_in_compressed_file(file_path, "./test_files/output.bin");

    //free_prefix_table(prefix_hash_table);

    //traverse_list_and_trees(heap, print_heap_post_order);

    
            // ERROR AQUI: Parece que se daña el arbol traverse_list_and_trees(heap, print_heap_post_order);

    // printf("------------ Destroying binary tree -----\n");
    // if (!is_empty(heap)){
    //     free_binary_heap_pq(heap);
    // }

    ktfree(content);
    free_prefix_table(prefix_hash_table);
    free_table(hash_table);
    //free_heap_priority_queue(heap);
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

