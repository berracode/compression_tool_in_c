#ifndef BINARY_UTILS_H
#define BINARY_UTILS_H
#include <stdio.h>
#include <stdint.h>



void print_binary(unsigned int value, int bits);
void print_string_as_bits(const char *bits);
uint8_t bits_to_byte(const char *bits);

void write_bits(FILE *file, const char *bits);

#endif