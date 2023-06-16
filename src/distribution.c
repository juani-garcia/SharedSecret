#include "distribution.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>

static bool init = false;

static uint8_t uniform(uint8_t a, uint8_t b) {
    if(!init) {
        srand(time(NULL));
        init = true;
    }

    double prop = rand() / ((double) RAND_MAX + 1);
    return  (uint8_t) ((b - a + 1) * prop + a);
}

static void generate_polynomials(const uint8_t* block, Polynomial* f, Polynomial* g, uint8_t mod) {
    for (int i = 0; i < f->degree + 1; i++) {
        f->coeffs[i] = block[i];
    }
    uint8_t r = uniform(1, mod-1);

    printf("r-value: %d\n", r);

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

    Polynomial f = {
            .degree = k-1,
            .coeffs = malloc(k * sizeof(uint8_t))
    }, g = {
            .degree = k-1,
            .coeffs = malloc(k * sizeof(uint8_t))
    };

    for (uint16_t i = 0; i < block_count; i++) {
        uint8_t* curr_block = data + i * (2*k-2);
        printf("Block %d:\n", i);
        generate_polynomials(curr_block, &f, &g, mod);

        printf("Polynomial f: ");
        for(int iter = 0; iter < f.degree + 1; iter++){
            printf("%d*x^%d + ", f.coeffs[iter], iter);
        }
        puts("");

        printf("Polynomial g: ");
        for(int iter = 0; iter < g.degree + 1; iter++){
            printf("%d*x^%d + ", g.coeffs[iter], iter);
        }
        puts("");

        for(int j = 0; j < n; j++) {
            uint8_t *sj = shadows[j];
            sj[2*i] = eval(&f, j + 1, mod);
            sj[2*i + 1] = eval(&g, j + 1, mod);
            printf("v(%d, %d) = %d, %d\n", i+1, j+1, sj[2*i], sj[2*i + 1]);
        }
        puts("\n-----------------------------------------");
    }

    free(f.coeffs);
    free(g.coeffs);

    return shadows;

}
