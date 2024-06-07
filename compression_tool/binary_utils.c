#include <string.h>
#include <stdlib.h>

#include "binary_utils.h"


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