#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stdint.h>

typedef struct Polynomial {
    uint8_t *coeffs;
    uint8_t degree;
} Polynomial;


Polynomial *init_polynomial(uint8_t degree);
Polynomial *interpolate(const uint8_t *X, const uint8_t *Y, size_t points, uint8_t mod);
uint8_t eval(Polynomial *p, uint8_t x, uint8_t mod);
void destroy_polynomial(Polynomial *p);

#endif
