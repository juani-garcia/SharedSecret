#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <stddef.h>
#include <stdint.h>

uint8_t **generate_shadows(uint8_t* data, size_t data_size, uint8_t k, uint8_t n, uint8_t mod);

#endif // DISTRIBUTION_H
