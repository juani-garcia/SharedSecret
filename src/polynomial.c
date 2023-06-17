#include "polynomial.h"

void init_polynomial(Polynomial *p, uint8_t degree, uint8_t *coeffs) {
    p->coeffs = coeffs;
    p->degree = degree;
}

uint8_t eval(Polynomial *p, uint8_t x, uint8_t mod) {
    uint8_t acc = p->coeffs[p->degree] % mod;

    for(int i = p->degree - 1; i >= 0; i--) {
        acc = (acc * x + p->coeffs[i]) % mod;
    }

    return acc;
}
