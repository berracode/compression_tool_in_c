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

//TODO: split this function into more small parts (S.O.L.I.D principles are violate)
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
    printf("Getting prefix code table...\n");
    int has_headers = 1, index = 2;
    while(has_headers && index < file_size){
        uint8_t next_bytes_to_read = content[index];
        char *character = ktmalloc(next_bytes_to_read+1);
        for (size_t i = 0; i < next_bytes_to_read; i++){
            character[i] = (char)content[(i+1)+index];
        }
        character[next_bytes_to_read] = '\0';

        index+=(next_bytes_to_read+1);

        //read bits
        uint8_t next_bits_to_read = content[index];
        uint8_t bits_remaining = next_bits_to_read % 8;
        uint8_t bytes_to_read = next_bits_to_read / 8;

        char *bits = ktmalloc(next_bits_to_read + 1);
        char *bits_string;
        int writter_index = 0;

        for (size_t i = 0; i < bytes_to_read; i++){
            uint8_t byte = content[(i+1)+index];
            bits_string = get_string_from_byte(byte, 8);
            for (size_t i = 0; i < 8; i++){
                bits[writter_index] = bits_string[i];
                writter_index++;
            }
            ktfree(bits_string);
        }
        if(bits_remaining > 0) {
            uint8_t byte = content[(bytes_to_read+1)+index];
            bits_string = get_string_from_byte(byte, bits_remaining);
            for (size_t i = 0; i < bits_remaining; i++){
                bits[writter_index] = bits_string[i];
                writter_index++;
            }
            ktfree(bits_string);

        }
        bits[next_bits_to_read]='\0';
        insert_prefix_code(*prefix_code_table, bits, 0, character);


        if(next_bits_to_read % 8 != 0){
            index+=(bytes_to_read + 1);
        }else {
            index+=bytes_to_read;
        }

        if(content[index+1]==0x0A){
            has_headers = 0;
            index+=2;
        }else{
            index++;
        }
        ktfree(character);
        ktfree(bits);
    }

    //read compressed data
    printf("Reading compressed data to decode...\n");
    //print_prefix_table(*prefix_code_table);
    char *data = slice(content, index, file_size, file_size);
    size_t data_compressed_len = file_size-index;

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
    for (size_t i = 0; i < data_compressed_len; i++){
        buffer = data[i];
        int taken = 8;
        if((i)+1 == data_compressed_len){
            if (padding != 0){
                taken = padding;
            }
        }

        char *bits_as_string = get_string_from_byte(buffer, taken);
        int len = strlen(bits_as_string);
        for (bit_index = 0; bit_index < len; bit_index++) {
            current_bits[current_length++] = bits_as_string[bit_index];
            current_bits[current_length] = '\0';
            flag_to_stop++;

            if(flag_to_stop > bits_to_read){
                break;
            }
            ktprefix_code_t *found = find_prefix_code(*prefix_code_table, current_bits);
            if (found != NULL && found->character) {
                fprintf(output_file, "%s", found->bits);
                current_length = 0;
            }
        }
        ktfree(bits_as_string);
    }
    ktfree(content);
    ktfree(data);
    fclose(input_file);
    fclose(output_file);
}