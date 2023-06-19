#ifndef IMAGE_H
#define IMAGE_H

#include "bmp.h"
#include <stdint.h>

void writeImageSecret(const uint8_t *data, unsigned j, unsigned k, BMPImage *output);
uint8_t *readImageSecret(BMPImage *input, unsigned *j, unsigned k);

#endif
