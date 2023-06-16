#include "polynomial.h"

uint8_t eval(Polynomial *p, uint8_t x, uint8_t mod) {
    uint8_t acc = 0, pow = 1;
    x %= mod;

    for(int i = 0; i < p->degree + 1; i++) {
        uint8_t c = p->coeffs[i] % mod;
        acc = (acc + pow * c) % mod;
        pow = (x * pow) % mod;
    }

    return acc;
}
