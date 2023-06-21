#include "bmp.h"
#include "distribution.h"
#include "image.h"
#include "recovery.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const int mod = 251;

static int encode(const char *imagePath, const char *dirPath, int k)
{
    ImageFile *secretImage;
    ImageFile **carrierImages;
    unsigned carrierCount;

    loadImages(imagePath, dirPath, k, &secretImage, &carrierImages, &carrierCount);
    uint8_t *imageData = secretImage->image->data;
    uint8_t **shadows = generate_shadows(imageData, secretImage->image->header->imageSize, k, carrierCount, mod);
    for (unsigned i = 0; i < carrierCount; i++)
    {
        writeImageSecret(shadows[i], i, k, carrierImages[i]->image);
    }
    saveCarriers(dirPath, carrierImages, carrierCount);

    for (unsigned i = 0; i < carrierCount; i++)
    {
        destroyImage(carrierImages[i]);
        free(shadows[i]);
    }
    free(shadows);
    destroyImage(secretImage);

    return EXIT_SUCCESS;
}

static int decode(const char *imagePath, const char *dirPath, int k)
{
    BMPImage *secretImage;
    ImageFile **carrierImages;
    unsigned carrierCount;

    loadImages(NULL, dirPath, k, NULL, &carrierImages, &carrierCount);
    uint8_t **shadows = malloc(sizeof(uint8_t*) * carrierCount);
    uint8_t *j = malloc(sizeof(uint8_t) * carrierCount);
    size_t secretSize;
    for (unsigned i = 0; i < carrierCount; i++)
    {
        unsigned tmp;
        secretSize = readImageSecret(carrierImages[i]->image, &tmp, k, &shadows[i]);
        j[i] = tmp;
    }
    uint8_t *secretData = recover_secret(shadows, j, secretSize / 2, k, mod);
    // int width = carrierImages[0]->image->header->width;
    // int height = carrierImages[0]->image->header->height;
    // secretImage = createBMP(secretData, width, height);
    secretImage = carrierImages[0]->image;
    secretImage->data = secretData;
    writeToFile(imagePath, secretImage);

    return EXIT_SUCCESS;
}

int main(int argc, const char **argv)
{
    if(argc <= 4)
    {
        ///TODO: Error
        return EXIT_FAILURE;
    }
    const char *secretPath = argv[2];
    const char *imagePath = argv[4];
    char *endptr;
    long k = strtol(argv[3], &endptr, 10);
    if(*endptr != 0)
    {
        ///TODO: Error
        return EXIT_FAILURE;
    }

    if(strcmp("d", argv[1]) == 0)
        return encode(secretPath, imagePath, k);
    else if(strcmp("r", argv[1]) == 0)
        return decode(secretPath, imagePath, k);
    else
    {
        ///TODO: Error
        return EXIT_FAILURE;
    }
}
