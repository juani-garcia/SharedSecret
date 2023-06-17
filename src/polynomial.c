#include <stdlib.h>
#include "polynomial.h"

Polynomial *init_polynomial(uint8_t degree) {
    Polynomial *p = malloc(sizeof(Polynomial));
    if(p == NULL)
        return NULL;

    p->coeffs = malloc(sizeof(uint8_t) * degree + 1);
    p->degree = degree;

    return p;
}

void destroy_polinomial(Polynomial *p) {
    if(p == NULL)
        return;
    free(p->coeffs);
    free(p);

}

uint8_t eval(Polynomial *p, uint8_t x, uint8_t mod) {
    uint8_t acc = p->coeffs[p->degree] % mod;

    for(int i = p->degree - 1; i >= 0; i--) {
        acc = (acc * x + p->coeffs[i]) % mod;
    }

    return acc;
}
