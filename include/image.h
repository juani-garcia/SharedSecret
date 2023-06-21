#ifndef IMAGE_H
#define IMAGE_H

#include "bmp.h"
#include <stddef.h>
#include <stdint.h>

typedef struct ImageFile
{
    BMPImage *image;
    char *path;
} ImageFile;

void saveCarriers(const char *imageDir, ImageFile **carrierImages, unsigned carrierCount);
void writeImageSecret(const uint8_t *data, unsigned j, unsigned k, BMPImage *output);
size_t readImageSecret(BMPImage *input, unsigned *j, unsigned k, uint8_t **out);
void loadImages(const char *imagePath, const char *imageDir, unsigned k, ImageFile **secretImage, ImageFile ***carrierImages, unsigned *carrierCount);
void destroyImage(ImageFile *image);

#endif
