#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "decompress.h"
#include "binary_utils.h"
#include "ktmem.h"


void read_headers_from_compressed_file(char *compressed_file){
    FILE *input_file = fopen(compressed_file, "rb");

    if (!input_file) {
        perror("Error: opening file");
        exit(1);
    }
    fseek(input_file, 0, SEEK_END);
    size_t file_size = ftell(input_file);
    rewind(input_file);

    char *content = ktmalloc(file_size);
    printf("FILE SIZE: %ld\n", file_size);

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

    // get prefix code table
    int has_headers = 1, index = 2;
    while(has_headers && index < file_size)
    {
        //leer char
        uint8_t next_bytes_to_read = content[index];
        char *character = ktmalloc(next_bytes_to_read+1);
        for (size_t i = 0; i < next_bytes_to_read; i++){
            character[i] = (char)content[(i+1)+index];
        }
        character[next_bytes_to_read] = '\0';
        printf("-----------------------------|||||--------\n");
        printf("char %s\n", character);

        index+=(next_bytes_to_read+1);
        printf("INDEX: %d\n", index);

        //leer bits
        uint8_t next_bits_to_read = content[index];
        printf("BITS A LEER: %d\n", next_bits_to_read);
        uint8_t bits_remaining = next_bits_to_read % 8; // 6
        uint8_t bytes_to_read = next_bits_to_read / 8; // 0

        char *bits = ktmalloc(next_bits_to_read + 1);
        char *bits_string;
        printf("en bits: bytes_to_read: %d, bits_remaining = %d\n", bytes_to_read, bits_remaining);
        for (size_t i = 0; i < bytes_to_read; i++)
        {
            uint8_t byte = content[(i+1)+index];
            //pasar todo el byte a string o pasar solo lo necesitado
            bits_string = get_string_from_byte(byte, 8);
            printf("bits_string 1 %s\n", bits_string);
            ktfree(bits_string);

        }

        if(bits_remaining > 0) {
            //printf("index aqui %d\n", index);
            uint8_t byte = content[(bytes_to_read+1)+index];
            bits_string = get_string_from_byte(byte, bits_remaining);
            printf("bits_string %s\n", bits_string);
                    ktfree(bits_string);

        }

        if(next_bits_to_read % 8 != 0){
            index+=(bytes_to_read + 1);
        }else {
            index++;
        }

        printf("INDEX 2: %d\n", index);

        printf("readed %d\n", content[index]);
        if(content[index+1]==0x0A){
            has_headers = 0;
        }else{
            index++;
        }
        printf("INDEX 3: %d\n", index); //DEBERÍA SER LA CANTIDAD DE BYTES (next char)


        ktfree(character);
        ktfree(bits);

    }
    




    printf("prefix code table len: %d\n", (int)prefix_code_table_len);
    ktfree(content);
    fclose(input_file);
}