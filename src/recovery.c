#include "recovery.h"
#include "polynomial.h"
#include "inverse.h"
#include <stdlib.h>

uint8_t *recover_secret(uint8_t *shadows[], uint8_t *X, size_t block_count, uint8_t k, uint8_t mod) {
    size_t block_size = 2*k -2;
    uint8_t *recovered = malloc(block_count * block_size * sizeof(uint8_t));
    uint8_t f_images[k], g_images[k];
    for(size_t i = 0; i < block_count; i++) {
        for(uint8_t j = 0; j < k; j++) {
            f_images[j] = shadows[j][2*i];
            g_images[j] = shadows[j][2*i + 1];
        }

        Polynomial *f = interpolate(X, f_images, k, mod);
        Polynomial *g = interpolate(X, g_images, k, mod);

        // Fill f and g using Lagrange
        uint8_t r[2];

        for(int it = 0; it < 2; it++) {
            r[it] = ((mod - 1) * get_inverse(f->coeffs[it], mod) * g->coeffs[it]) % mod;
        }

        if(r[0] != r[1]) {
            free(recovered);
            destroy_polynomial(f);
            destroy_polynomial(g);
            return NULL;
        }

        int it1, it2;
        for(it1 = 0; it1 < f->degree + 1; it1++) {
            recovered[i * block_size + it1] = f->coeffs[it1];
        }
        for(it2 = 2; it2 < g->degree + 1; it2++) {
            recovered[i * block_size + it1 + it2 - 2] = g->coeffs[it2];
        }

        destroy_polynomial(f);
        destroy_polynomial(g);
    }
    return recovered;
}
