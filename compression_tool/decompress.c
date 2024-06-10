#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "decompress.h"
#include "binary_utils.h"
#include "ktmem.h"


char* slice(const char* input, int start, int end, size_t real_len) {
    int input_length = real_len;
    if (start < 0 || end > input_length || start >= end) {
        fprintf(stderr, "Índices inválidos\n");
        return NULL;
    }
    int new_length = end - start;
    char* output = (char*)ktmalloc((new_length + 1) * sizeof(char));
    int conta_null = 0, stop_in = 0;
    for (size_t i = 0; i < new_length; i++){
        output[i]=input[i+start];
    }
    output[new_length] = '\0';

    return output;
}

void read_headers_from_compressed_file(char *compressed_file, char *decompressed_file, kthash_table_t **prefix_code_table){
    FILE *input_file = fopen(compressed_file, "rb");
    FILE *output_file = fopen(decompressed_file, "wb");

    if (!input_file || !output_file) {
        perror("Error: opening file");
        exit(1);
    }
    fseek(input_file, 0, SEEK_END);
    size_t file_size = ftell(input_file);
    rewind(input_file);

    char *content = ktmalloc(file_size+1);
    content[file_size] = '\0';
    printf("FILE SIZE: %ld, content = %ld\n", file_size, strlen(content));

    /*if(fread(&prefix_code_table_len, sizeof(uint8_t),1, input_file) != 1){
        perror("Can't read byte");
        fclose(input_file);
        exit(1);
    }*/
    size_t bytes_read = fread(content, 1, file_size, input_file);
    printf("BYTES READ %ld\n", bytes_read);
    if(bytes_read < file_size){
        perror("Can't read byte 2");
        fclose(input_file);
        exit(1);
    }
    uint8_t prefix_code_table_len = content[0]; //prefix code table length
    uint8_t padding = content[1]; //padding for the last byte readed

    *prefix_code_table = create_prefix_table(prefix_code_table_len);


    // get prefix code table
    int has_headers = 1, index = 2;
    int iters = 0;
    while(has_headers && index < file_size){
        //leer char
        uint8_t next_bytes_to_read = content[index];
        char *character = ktmalloc(next_bytes_to_read+1);
        for (size_t i = 0; i < next_bytes_to_read; i++){
            character[i] = (char)content[(i+1)+index];
        }
        character[next_bytes_to_read] = '\0';
        //printf("-----------------------------|||||--------\n");
        //printf("char %s\n", character);

        index+=(next_bytes_to_read+1);
        //printf("INDEX: %d\n", index);

        //leer bits
        uint8_t next_bits_to_read = content[index];
        //printf("BITS A LEER: %d\n", next_bits_to_read);
        uint8_t bits_remaining = next_bits_to_read % 8; // 6
        uint8_t bytes_to_read = next_bits_to_read / 8; // 0

        char *bits = ktmalloc(next_bits_to_read + 1);
        char *bits_string;
        //printf("en bits: bytes_to_read: %d, bits_remaining = %d\n", bytes_to_read, bits_remaining);
        int writter_index = 0;

        for (size_t i = 0; i < bytes_to_read; i++)
        {
            uint8_t byte = content[(i+1)+index];
            //pasar todo el byte a string o pasar solo lo necesitado
            bits_string = get_string_from_byte(byte, 8);
            for (size_t i = 0; i < 8; i++)
            {
                bits[writter_index] = bits_string[i];
                writter_index++;
            }
            
            //printf("bits_string 1: [%s]\n", bits_string);
            ktfree(bits_string);

        }

        //printf("writter index: %d\n", writter_index);

        if(bits_remaining > 0) {
            //printf("index aqui %d\n", index);
            uint8_t byte = content[(bytes_to_read+1)+index];
            bits_string = get_string_from_byte(byte, bits_remaining);
            for (size_t i = 0; i < bits_remaining; i++)
            {
                bits[writter_index] = bits_string[i];
                writter_index++;
            }
            //printf("writter index 2: %d\n", writter_index);
            //printf("bits_string 2: [%s]\n", bits_string);

            ktfree(bits_string);

        }
        bits[next_bits_to_read]='\0';

        //printf("FULL _ bits: [%s]\n", bits);

        insert_prefix_code(*prefix_code_table, bits, 0, character);


        if(next_bits_to_read % 8 != 0){
            index+=(bytes_to_read + 1);
        }else {
            index+=bytes_to_read; //ERROR:
        }

        //printf("INDEX 2: %d\n", index);

        //printf("readed %d\n", content[index]);
        if(content[index+1]==0x0A){
            has_headers = 0;
            index+=2;
        }else{
            index++;
        }
        //printf("INDEX 3: %d\n", index); //DEBERÍA SER LA CANTIDAD DE BYTES (next char)

        iters++;
        ktfree(character);
        ktfree(bits);

    }

    //INICIA PROCESAMIENTO DE DATA

    //print_prefix_table(*prefix_code_table);
    char *data = slice(content, index, file_size, file_size);
    size_t data_compressed_len = file_size-index;
    printf("padding: %d, data_c_l: %ld\n", padding, data_compressed_len);

    int remaining = 0;
    if(padding != 0){
        remaining = (8-padding % 8);
    }

    size_t bits_to_read = (data_compressed_len*8) - remaining;


    int bit_index;
    char current_bits[256] = {0};
    int current_length = 0;
    unsigned char buffer;
    int flag_to_stop = 0;
    int last_byte = 0;
    for (size_t i = 0; i < data_compressed_len; i++){
        buffer = data[i];
        int taken = 8;
        if((i)+1 == data_compressed_len){
            if (padding != 0){
                taken = padding;
            }
            last_byte = 1;
            printf("current prev: %s , taken: %d\n", current_bits, taken);
        }

        char *bits_as_string = get_string_from_byte(buffer, taken);
        int len = strlen(bits_as_string);
        //printf("bit_as_s %s - flag %d\n", bits_as_string, flag_to_stop);
        for (bit_index = 0; bit_index < len; bit_index++) {
            current_bits[current_length++] = bits_as_string[bit_index];
            current_bits[current_length] = '\0';
            
            
            if(last_byte){
                printf("bits_As_string %s\n", bits_as_string);
                printf("current_bits %s\n", current_bits);
                printf("flag = %d, bits_to_read=%ld\n", flag_to_stop, bits_to_read);
            }
            if (flag_to_stop <= bits_to_read){
                //printf("buscando: |%s|\n", current_bits);
                ktprefix_code_t *found = find_prefix_code(*prefix_code_table, current_bits);

                if (found != NULL && found->character) {
                    //printf("encontro algo con cb: %s\n", current_bits);
                    fprintf(output_file, "%s", found->bits);
                    current_length = 0;
                }
            } else{
                break;
            }
            flag_to_stop++;
            //
        }
        ktfree(bits_as_string);
        
    }

    printf("------ flag: %d\n",flag_to_stop);

    printf("prefix code table len: %d - iters [%d] - index [%d] - c[i-1] [%02X] - c[i] [%02X]\n", (int)prefix_code_table_len, iters, index, content[index-1], content[index]);
    ktfree(content);
    ktfree(data);
    fclose(input_file);
    fclose(output_file);
}