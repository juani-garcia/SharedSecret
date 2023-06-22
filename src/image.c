#include "image.h"
#include "encoding.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

void writeImageSecret(const uint8_t *data, unsigned j, unsigned k, BMPImage *output)
{
    output->header->reserved1 = j;
    size_t dataSize = 2 * output->header->imageSize / (2*k-2);
    if(k >= 3 && k <= 4)
        encodeLsb4(dataSize, data, output->data);
    else
        encodeLsb2(dataSize, data, output->data);
}

size_t readImageSecret(BMPImage *input, unsigned *j, unsigned k, uint8_t **out)
{
    size_t dataSize = 2 * input->header->imageSize / (2*k-2);
    *j = input->header->reserved1;
    uint8_t *data = malloc(dataSize);
    if(data == NULL)
        return 0;
    
    if(k >= 3 && k <= 4)
        decodeLsb4(dataSize, input->data, data);
    else
        decodeLsb2(dataSize, input->data, data);
    
    *out = data;
    return dataSize;
}

int loadImages(const char *imagePath, const char *imageDir, unsigned k, ImageFile **secretImage, ImageFile ***carrierImages, unsigned *carrierCount)
{
    if(imagePath != NULL)
    {
        BMPImage *sImage = readFromFile(imagePath);
        if(sImage == NULL)
        {
            fprintf(stderr, "Error reading BMP image: %s\n", imagePath);
            return -1;
        }
        if(sImage->header->bitDepth != 8 || sImage->header->compression != 0)
        {
            fprintf(stderr, "Error in %s. Image is compressed or BPP is different from 8\n", imagePath);
            destroyBMP(sImage);
            return -1;
        }
        if(sImage->header->imageSize % (2*k-2) != 0)
        {
            fprintf(stderr, "Error in %s. Data is not divisible by 2k - 2 (%d)\n", imagePath, 2*k-2);
            destroyBMP(sImage);
            return -1;
        }
        *secretImage = (ImageFile*)malloc(sizeof(ImageFile));
        **secretImage = (ImageFile)
        {
            .image = sImage,
            .path = strdup(imagePath),
        };
    }
    else
        secretImage = NULL;

    struct dirent *dir;
    DIR *d = opendir(imageDir);
    if(d == NULL)
    {
        fprintf(stderr, "Error opening directory %s\n", imageDir);
        if(errno)
            perror("");
        return -1;
    }

    unsigned i;
    unsigned maxCount = 4;
    ImageFile **carriers = malloc(sizeof(ImageFile*) * maxCount);
    char *cwd = getcwd(NULL, 0);
    chdir(imageDir);

    int width, height;
    bool sizeAssigned = false;
    if(secretImage != NULL)
    {
        width = (*secretImage)->image->header->width;
        height = (*secretImage)->image->header->height;
        sizeAssigned = true;
    }

    for(i = 0; (dir = readdir(d)) != NULL;) 
    {
        if(dir->d_type == DT_REG)
        {
            BMPImage *img = readFromFile(dir->d_name);
            if(i >= 255)
            {
                fprintf(stderr,
                        "Skipped carrier %s. Carrier limit (255) reached\n",
                        dir->d_name);
                continue;
            }
            if(img == NULL)
            {
                fprintf(stderr,
                        "Error reading carrier %s. Image is not a valid BMP file\n",
                        dir->d_name);
                continue;
            }
            if(img->header->bitDepth != 8 || img->header->compression != 0)
            {
                fprintf(stderr,
                        "Error reading carrier %s. Image is compressed or BPP is different from 8\n",
                        dir->d_name);
                destroyBMP(img);
                continue;
            }
            if(!sizeAssigned)
            {
                width = img->header->width;
                height = img->header->height;
                sizeAssigned = true;
            }
            if(img->header->width != width && img->header->height != height)
            {
                fprintf(stderr,
                        "Error reading carrier %s. Carrier dimensions differ\n", dir->d_name);
                destroyBMP(img);
                continue;
            }
            
            if(i >= maxCount)
            {
                maxCount *= 2;
                carriers = realloc(carriers, sizeof(ImageFile*) * maxCount);
            }
            carriers[i] = (ImageFile*)malloc(sizeof(ImageFile));
            *carriers[i] = (ImageFile)
            {
                .image = img,
                .path = strdup(dir->d_name),
            };
            i++;
        }
    }

    if(i < k) {
        fprintf(stderr, "Error: found %d carriers. Needed %d\n", i, k);
        if(secretImage != NULL)
            destroyImage(*secretImage);
        for(unsigned j = 0; j < i; j++)
            free(carriers[j]);
        free(carriers);
        return -1;
    }

    carriers = realloc(carriers, sizeof(ImageFile*) * i);
    *carrierCount = i;
    *carrierImages = carriers;
    chdir(cwd);
    free(cwd);
    closedir(d);

    return 0;
}

size_t saveCarriers(const char *imageDir, ImageFile **carrierImages, unsigned carrierCount)
{
    char *cwd = getcwd(NULL, 0);
    int saved = 0;
    chdir(imageDir);

    for(unsigned i = 0; i < carrierCount; i++)
    {
        if(writeToFile(carrierImages[i]->path, carrierImages[i]->image) == 0) {
            saved++;
        }
    }

    chdir(cwd);
    free(cwd);
    return saved;
}

void destroyImage(ImageFile *image)
{
    destroyBMP(image->image);
    free(image->path);
    free(image);
}
