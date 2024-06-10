#include <string.h>
#include <stdlib.h>

#include "ktmem.h"
#include "binary_utils.h"


char* get_string_from_byte(uint8_t value, int bits){
    char *bits_string = ktmalloc(bits+1);
    for (int i = bits - 1; i >= 0; i--) {
        unsigned int mask = 1 << i;
        char put = (value & mask) ? '1' : '0';
        //putchar(put);
        bits_string[bits-(i+1)] = put;
    }
    bits_string[bits]='\0';
    //putchar('\n');
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
    // iter 0, 1, 2, 3, 4 (\0)
    // 0111
    // 00000000,00000000,00000010,00000110, byte desplazado
    // 00000000,00000001,00000001,00000001 bit_value
    //B00000000,00000001,00000011,00000111 byte with bit value
    int iter = 0;
    while (*bits != '\0' && iter < 8) {
        //printf("byte antes %d\n", byte);
        // Desplazar el byte un lugar a la izquierda
        byte <<= 1;
        //printf("byte desplazado %d\n", byte);

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
    size_t required_bytes = len / 8;
    size_t remaining_bits = len % 8;

    //printf("len %zu, requiered %zu, r %zu\n", len, required_bytes, remaining_bits);

    // Escribir bytes completos
    for (size_t i = 0; i < required_bytes; i++) {
        //printf("QQQQQQQQQQQQQQQQQQQQQQ ocupa más de un byte\n");
        uint8_t byte = bits_to_byte(bits + i * 8);
        //printf("byte: %d\n", byte);
        fwrite(&byte, sizeof(uint8_t), 1, file);
    }

    // Escribir bits restantes
    if (remaining_bits > 0) {
        //printf("QQQQQQQQQQQQQQQQQQQQQQ NO ocupa más de un byte\n");

        char last_byte_str[9] = {0};  // Un byte más el terminador nulo
        strncpy(last_byte_str, bits + required_bytes * 8, remaining_bits);
        //printf("last byte str %s,  len: %ld\n", last_byte_str, strlen(last_byte_str));

        /*for (size_t i = remaining_bits; i < 8; i++)
        {
            last_byte_str[i]='0';
        }
        last_byte_str[9] = '\0';*/
        //printf("last byte str %s,  len: %ld\n", last_byte_str, strlen(last_byte_str));

        
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