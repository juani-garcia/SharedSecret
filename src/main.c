#include "bmp.h"
#include "distribution.h"
#include "image.h"
#include "recovery.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

const int mod = 251;

static int distribute(const char *imagePath, const char *dirPath, int k)
{
    ImageFile *secretImage;
    ImageFile **carrierImages;
    unsigned carrierCount;

    if(loadImages(imagePath, dirPath, k, &secretImage, &carrierImages, &carrierCount) < 0) {
        return EXIT_FAILURE;
    }

    uint8_t *imageData = secretImage->image->data;
    uint8_t **shadows = generate_shadows(imageData, secretImage->image->header->imageSize, k, carrierCount, mod);
    for (unsigned i = 0; i < carrierCount; i++)
    {
        writeImageSecret(shadows[i], i+1, k, carrierImages[i]->image);
    }

    int saved = saveCarriers(dirPath, carrierImages, carrierCount);
    if(saved < k) {
        fprintf(stderr, "Error: could only save %d carriers. Needed at least %d\n", saved, k);
    }

    for (unsigned i = 0; i < carrierCount; i++)
    {
        destroyImage(carrierImages[i]);
        free(shadows[i]);
    }
    free(shadows);
    free(carrierImages);
    destroyImage(secretImage);

    return EXIT_SUCCESS;
}

static int recover(const char *imagePath, const char *dirPath, int k)
{
    bool error = false;
    BMPImage *secretImage;
    ImageFile **carrierImages;
    unsigned carrierCount;

    if(loadImages(NULL, dirPath, k, NULL, &carrierImages, &carrierCount) < 0) {
        return EXIT_FAILURE;
    }
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

    if(secretData == NULL) {
        fprintf(stderr, "Error recovering secret. r missmatch\n");
        error = true;
        goto cleanup;
    }

    secretImage = carrierImages[0]->image;

    free(secretImage->data);

    secretImage->data = secretData;
    if(writeToFile(imagePath, secretImage) < 0) {
        error = true;
    }

cleanup:
    for (unsigned i = 0; i < carrierCount; i++)
    {
        destroyImage(carrierImages[i]);
        free(shadows[i]);
    }

    free(j);
    free(shadows);
    free(carrierImages);

    return error? EXIT_FAILURE : EXIT_SUCCESS;
}

int main(int argc, const char **argv)
{
    if(argc != 5)
    {
        fprintf(stderr, "Error: wrong usage\n");
        fprintf(stderr, "To distribute, use %s d <destination> k <dir>\n", argv[0]);
        fprintf(stderr, "To recover, use %s d <source> k <dir>\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char *secretPath = argv[2];
    const char *imagePath = argv[4];
    char *endptr;
    long k = strtol(argv[3], &endptr, 10);
    if(*endptr != 0)
    {
        fprintf(stderr, "Error: k value must be an integer, received %s\n", argv[3]);
        return EXIT_FAILURE;
    }
    if(k < 3 || k > 8)
    {
        fprintf(stderr, "Error: k value must be between 3 and 8, received %ld\n", k);
        return EXIT_FAILURE;
    }

    if(strcmp("d", argv[1]) == 0)
        return distribute(secretPath, imagePath, k);
    else if(strcmp("r", argv[1]) == 0)
        return recover(secretPath, imagePath, k);
    else
    {
        fprintf(stderr, "Error: option must be d or r, received %s\n", argv[1]);
        return EXIT_FAILURE;
    }
}
