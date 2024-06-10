#include <string.h>
#include <stdlib.h>

#include "ktmem.h"
#include "binary_utils.h"


char* get_string_from_byte(uint8_t value, int bits){
    char *bits_string = ktmalloc(bits+1);
    for (int i = bits - 1; i >= 0; i--) {
        unsigned int mask = 1 << i;
        char put = (value & mask) ? '1' : '0';
        bits_string[bits-(i+1)] = put;
    }
    bits_string[bits]='\0';
    return bits_string;
}

void print_binary(unsigned int value, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        unsigned int mask = 1 << i;
        putchar((value & mask) ? '1' : '0');
    }
    putchar('\n');
}

void print_string_as_bits(const char *bits){
    int len = strlen(bits);
    for (int i = 0; i < len; i++){
        print_binary(bits[i], 8);
    }

}

uint8_t bits_to_byte(const char *bits) {
    uint8_t byte = 0;
    int iter = 0;
    while (*bits != '\0' && iter < 8) {
        byte <<= 1;

        uint8_t bit_value = (*bits == '1') ? 1 : 0;
        byte |= bit_value;

        bits++;
        iter++;
    }
    return byte;
}

void write_bits(FILE *file, const char *bits) {
    size_t len = strlen(bits);
    size_t required_bytes = len / 8;
    size_t remaining_bits = len % 8;

    for (size_t i = 0; i < required_bytes; i++) {
        uint8_t byte = bits_to_byte(bits + i * 8); //TODO: create byte_to_bits function
        fwrite(&byte, sizeof(uint8_t), 1, file);
    }

    if (remaining_bits > 0) {
        char last_byte_str[9] = {0};
        strncpy(last_byte_str, bits + required_bytes * 8, remaining_bits);
        uint8_t last_byte = bits_to_byte(last_byte_str);
        fwrite(&last_byte, sizeof(uint8_t), 1, file);
    }
}

char* get_char_as_string(const char *str, int *index) {
    char c = str[*index];
    int length = 1;
    //print_binary(c, 8);
    if ((c & 0x80) == 0x00) {
        length = 1; // 1 byte UTF-8 (ASCII)
    } else if ((c & 0xE0) == 0xC0) {
        length = 2; // 2 bytes
    } else if ((c & 0xF0) == 0xE0) {
        length = 3; // 3 bytes
    } else if ((c & 0xF8) == 0xF0) {
        length = 4; // 4 bytes
    }

    char* utf8_char = (char*)ktmalloc(length + 1);

    for (int i = 0; i < length; i++) {
        utf8_char[i] = str[*index + i];
    }
    utf8_char[length] = '\0';

    *index += length;
    return utf8_char;
}