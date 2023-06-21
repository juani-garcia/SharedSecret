#ifndef IMAGE_H
#define IMAGE_H

#include "bmp.h"
#include <stdint.h>

typedef struct ImageFile
{
    BMPImage *image;
    const char *path;
} ImageFile;

void writeImageSecret(const uint8_t *data, unsigned j, unsigned k, BMPImage *output);
uint8_t *readImageSecret(BMPImage *input, unsigned *j, unsigned k);
void loadImages(const char *imagePath, const char *imageDir, unsigned k, ImageFile **secretImage, ImageFile ***carrierImages, unsigned *carrierCount);

#endif
