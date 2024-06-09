#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "compress.h"
#include "ktmem.h"
#include "heap_priority_queue.h"
#include "binary_utils.h"


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
    //*count_characters = (int)hash_table->size;
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
    visit_tree_node_post_order(node->left, edges, increase(count_left), count_right, 1, code, prefix_code_table);
    visit_tree_node_post_order(node->right, edges, count_left, increase(count_right), 0, code, prefix_code_table);
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
            insert_prefix_code(prefix_code_table, node->data->element, node->data->weight, code);
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

void build_prefix_code_table(binary_heap_pq_t *heap, kthash_table_t *prefix_code_table){
    /*
    recorrer hasta las hojas
    si es un nodo a la izquierda ponerle 0
    */

    heap_pq_node_t *head = heap->head;
    int i = 0, count_left = 0, count_right = 0;
    char *code = NULL;
    printf("|\t\tCHAR\t\t|\t\tFREQ\t\t|\t\tCODE\t\t|\t\tBITS\t\t|\n");
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
    // Escribir el tamaño de la tabla de prefijos
    printf("tabla size: %ld, sizeof: %zu\n", prefix_code_table->size, sizeof(prefix_code_table->size));
    //printf("count_characters size of: %zu\n", sizeof(count_characters));
    fwrite(&prefix_code_table->size, sizeof(uint8_t), 1, output); //save numbers of "characters"
    fwrite(&padding, sizeof(uint8_t), 1, output); //padding right

    // Escribir cada prefijo en el encabezado
    for (size_t i = 0; i < prefix_code_table->size; i++) {
        ktprefix_code_t *current = prefix_code_table->prefix_codes[i];
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
            //fwrite(prefix_code_table->prefix_codes[i].bits, sizeof(char), bits_len, output);
            write_bits(output, current->bits);

            current = current->next;
        }

    }

    // Escribir un delimitador (un salto de línea) para separar el encabezado del contenido
    char delimiter = '\n';
    fwrite(&delimiter, sizeof(char), 1, output);

    fclose(output);

}

void write_data_encoded_in_compressed_file(const char *content, const char *output_file_path, kthash_table_t *prefix_code_table){

    printf("---------- INICIA ESCRITURA DE DATA COMPRIMIDA---------\n");
    //recorrer el archivo plano y sacar los caracteres y buscarlos en la hashtable
    // TODO: (función hash para optimizar busqueda)
    // TODO: volver esto un metodo ya que se reutiliza

    size_t file_size = strlen(content);
    size_t max_bits_len = prefix_code_table->max_bits_len;
    printf("#######################\n");
    printf("file size: %ld, mbl = [%ld]\n", file_size, max_bits_len);

    char *char_as_string;
    char *compress_data = ktmalloc(max_bits_len+1);
    int writter_index = 0;

    int index = 0;
    printf("bytes_read_pf %ld\n", file_size);
    while (index < file_size){
        char_as_string = get_char_as_string(content, &index);
        //printf("char_string %s | ht size: %zu| index=%d\n", char_as_string, prefix_code_table->size, index);

        ktprefix_code_t *found = find_prefix_code(prefix_code_table, char_as_string);
        int bits_len = strlen(found->bits);
        for (size_t i = 0; i < bits_len; i++){
            compress_data[writter_index] = found->bits[i];
            writter_index++;
        }
        //concatenate_bits2(&compress_data, found->bits);
        //strcat(compress_data, found->bits);
        //printf("compress_data %s | wi = %d\n", compress_data, writter_index);
        ktfree(char_as_string);
    }

    compress_data[max_bits_len] = '\0';
    printf("compressed data: %s\n", compress_data);

    //write headers
    int padding = strlen(compress_data) % 8;
    write_headers_in_compressed_file(prefix_code_table, output_file_path, padding);

    FILE *output_file = fopen(output_file_path, "ab");
    if (!output_file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    //TODO: optimizar el write_bits, pero para optimizarlo necesito la cadena de caracteres
    write_bits(output_file, compress_data);

    printf("---------- BITS CD:|%ld|\n", strlen(compress_data));
    printf("---------- FINALIZA ESCRITURA DE DATA COMPRIMIDA---------\n");
    ktfree(compress_data);
    fclose(output_file);

}
