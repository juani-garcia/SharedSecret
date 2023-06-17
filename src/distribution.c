#include "distribution.h"
#include <stdlib.h>
#include <sys/random.h>

static uint8_t uniform(uint8_t a, uint8_t b) {
    uint32_t r;
    getrandom(&r, sizeof(r), GRND_RANDOM);

    double prop = r / ((double) UINT32_MAX + 1);
    return  (uint8_t) ((b - a + 1) * prop + a);
}

static void generate_polynomials(const uint8_t* block, Polynomial* f, Polynomial* g, uint8_t mod) {
    for (int i = 0; i < f->degree + 1; i++) {
        f->coeffs[i] = block[i];
    }
    uint8_t r = uniform(1, mod-1);

    for(int i = 0; i < 2; i++) {
        uint8_t a = f->coeffs[i] == 0 ? 1 : f->coeffs[i];
        g->coeffs[i] = ((mod - 1) * r * a) % mod;
    }

    for (int i = 2; i < g->degree + 1; i++) {
        g->coeffs[i] = block[f->degree + i - 2];
    }
}

uint8_t  **generate_shadows(uint8_t* data, size_t data_size, uint8_t k, uint8_t n, uint8_t mod) {
    size_t block_count = data_size / (2*k -2);
    uint8_t ** shadows = malloc(n * sizeof(uint8_t *));

    for(int i = 0; i < n; i++) {
        shadows[i] = malloc(2 * block_count * sizeof(uint8_t));
    }

    Polynomial f , g;
    init_polynomial(&f, k - 1, malloc(k * sizeof(uint8_t)));
    init_polynomial(&g, k - 1, malloc(k * sizeof(uint8_t)));

    for (uint16_t i = 0; i < block_count; i++) {
        uint8_t* curr_block = data + i * (2*k-2);
        generate_polynomials(curr_block, &f, &g, mod);

        for(int j = 0; j < n; j++) {
            uint8_t *sj = shadows[j];
            sj[2*i] = eval(&f, j + 1, mod);
            sj[2*i + 1] = eval(&g, j + 1, mod);
        }
    }

    free(f.coeffs);
    free(g.coeffs);

    return shadows;

}
