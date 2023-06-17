#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stdint.h>

typedef struct Polynomial {
    uint8_t *coeffs;
    uint8_t degree;
} Polynomial;

uint8_t eval(Polynomial *p, uint8_t x, uint8_t mod);

#endif