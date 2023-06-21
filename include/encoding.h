#ifndef ENCODING_H
#define ENCODING_H

#include <stdint.h>
#include <stddef.h>

void encodeLsb4(size_t n, const uint8_t input[restrict static n], uint8_t output[restrict static 2*n]);
void decodeLsb4(size_t n, const uint8_t input[restrict static 2*n], uint8_t output[restrict static n]);
void encodeLsb2(size_t n, const uint8_t input[restrict static n], uint8_t output[restrict static 4*n]);
void decodeLsb2(size_t n, const uint8_t input[restrict static 4*n], uint8_t output[restrict static n]);

#endif
