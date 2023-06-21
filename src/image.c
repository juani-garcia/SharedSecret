#include "image.h"
#include "bmp.h"
#include "encoding.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <ftw.h>
#include <unistd.h>

void writeImageSecret(const uint8_t *data, unsigned j, unsigned k, BMPImage *output)
{
    output->header->reserved1 = j;
    size_t dataSize = 2 * output->header->imageSize / (2*k-2);
    if(k >= 3 && k <= 4)
        encodeLsb4(dataSize, data, output->data);
    else
        encodeLsb2(dataSize, data, output->data);
}

uint8_t *readImageSecret(BMPImage *input, unsigned *j, unsigned k)
{
    size_t dataSize = 2 * input->header->imageSize / (2*k-2);
    *j = input->header->reserved1;
    uint8_t *data = malloc(dataSize);
    if(data == NULL)
        return NULL;
    
    if(k >= 3 && k <= 4)
        decodeLsb4(dataSize, input->data, data);
    else
        decodeLsb2(dataSize, input->data, data);
    return data;
}

void loadImages(const char *imagePath, const char *imageDir, unsigned k, ImageFile **secretImage, ImageFile ***carrierImages, unsigned *carrierCount)
{
    if(imagePath != NULL)
    {
        BMPImage *sImage = readFromFile(imagePath);
        if(sImage == NULL)
        {
            ///TODO: Inform error
            return;
        }
        if(sImage->header->bitDepth != 8 || sImage->header->compression != 0)
        {
            ///TODO: Inform error
            destroyBMP(sImage);
            return;
        }
        if(sImage->header->imageSize % (2*k-2) != 0)
        {
            ///TODO: Inform error
            destroyBMP(sImage);
            return;
        }
        *secretImage = (ImageFile*)malloc(sizeof(ImageFile));
        **secretImage = (ImageFile)
        {
            .image = sImage,
            .path = imagePath,
        };
    }
    else
        *secretImage = NULL;

    struct dirent *dir;
    DIR *d = opendir(imageDir);
    if(d == NULL)
    {
        ///TODO: Error
        return;
    }

    int i;
    int maxCount = 4;
    ImageFile **carriers = malloc(sizeof(ImageFile*) * maxCount);
    char *cwd = getcwd(NULL, 0);
    chdir(imageDir);

    for(i = 0; (dir = readdir(d)) != NULL;) 
    {
        if(dir->d_type == DT_REG)
        {
            BMPImage *img = readFromFile(dir->d_name);
            if(img == NULL)
                continue;
            if(img->header->bitDepth != 8 || img->header->compression != 0)
            {
                ///TODO: Inform error
                destroyBMP(img);
                continue;
            }
            if(*secretImage != NULL && img->header->imageSize != (*secretImage)->image->header->imageSize)
            {
                ///TODO: Inform error
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
                .path = dir->d_name,
            };
            i++;
        }
    }
    carriers = realloc(carriers, sizeof(ImageFile*) * i);
    *carrierCount = i;
    chdir(cwd);
    free(cwd);
    closedir(d);

    return;
}

void saveCarriers(const char *imageDir, ImageFile **carrierImages, unsigned carrierCount)
{
    char *cwd = getcwd(NULL, 0);
    chdir(imageDir);

    for(unsigned i = 0; i < carrierCount; i++)
    {
        writeToFile(carrierImages[i]->path, carrierImages[i]->image);
    }

    chdir(cwd);
    free(cwd);
}
