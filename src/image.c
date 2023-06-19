#include "image.h"
#include "encoding.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
