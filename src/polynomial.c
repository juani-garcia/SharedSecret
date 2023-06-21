#include <stdlib.h>
#include <string.h>
#include "polynomial.h"
#include "inverse.h"

#define MAX(a, b) ((a) > (b)? (a) : (b))

Polynomial *init_polynomial(uint8_t degree) {
    Polynomial *p = malloc(sizeof(Polynomial));
    if(p == NULL)
        return NULL;

    p->coeffs = malloc(sizeof(uint8_t) * degree + 1);
    p->degree = degree;

    return p;
}

void destroy_polynomial(Polynomial *p) {
    if(p == NULL)
        return;
    free(p->coeffs);
    free(p);
}

// Performs p * (x - a) in place (i.e. p = p * (x - a))
static void mult_mod_by_monomial(Polynomial *p, uint8_t a, uint8_t mod) {
    memmove(p->coeffs + 1, p->coeffs, p->degree + 1);
    a %= mod;
    p->coeffs[0] = 0;
    for(int i = 0; i < p->degree + 1; i++) {
        p->coeffs[i] = (p->coeffs[i] + p->coeffs[i + 1] * (mod - a)) % mod;
    }
    p->degree++;
}

// Performs k * p in place (i.e. p = k * p)
static void scalar_mult_mod(Polynomial *p, uint8_t k, uint8_t mod) {
    for(int i = 0; i < p->degree + 1; i++) {
        p->coeffs[i] = (p->coeffs[i] * k) % mod;
    }
}

// Performs p + q in place (i.e. p = p + q)
static void sum_mod(Polynomial *p, Polynomial *q, uint8_t mod) {
    int degree = MAX(p->degree, q->degree);
    for(int i = 0; i < q->degree + 1; i++) {
        if(i < p->degree + 1) {
            p->coeffs[i] = (p->coeffs[i] + q->coeffs[i]) % mod;
        } else {
            p->coeffs[i] = q->coeffs[i] % mod;
        }
    }
    p->degree = degree;
}

Polynomial *interpolate(const uint8_t *X, const uint8_t *Y, size_t points, uint8_t mod) {
    Polynomial *ans = init_polynomial(points - 1);
    Polynomial *l = init_polynomial(points - 1);
    memset(ans->coeffs, 0x0, ans->degree + 1);
    ans->degree = 0;
    for(size_t i = 0; i < points; i++) {
        int dividend = 1;
        memset(l->coeffs, 0x0, ans->degree + 1);
        l->coeffs[0] = 1;
        l->degree = 0;
        for(size_t j = 0; j < points; j++) {
            if(i != j) {
                dividend = (dividend * (X[i] - X[j])) % mod;
                mult_mod_by_monomial(l, X[j], mod);
            }
        }
        dividend = dividend > 0? dividend : mod + dividend;
        uint8_t inv = get_inverse(dividend, mod);
        scalar_mult_mod(l, (inv * Y[i]) % mod, mod);
        sum_mod(ans, l, mod);
    }
    destroy_polynomial(l);
    return ans;
}

uint8_t eval(Polynomial *p, uint8_t x, uint8_t mod) {
    uint8_t acc = p->coeffs[p->degree] % mod;

    for(int i = p->degree - 1; i >= 0; i--) {
        acc = (acc * x + p->coeffs[i]) % mod;
    }

    return acc;
}
