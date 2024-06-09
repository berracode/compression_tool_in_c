#ifndef BINARY_UTILS_H
#define BINARY_UTILS_H
#include <stdio.h>
#include <stdint.h>



void print_binary(unsigned int value, int bits);
void print_string_as_bits(const char *bits);
uint8_t bits_to_byte(const char *bits);

void write_bits(FILE *file, const char *bits);

char* get_char_as_string(const char *str, int *index);

char* get_string_from_byte(uint8_t value, int bits);

#endif