#include "bmp.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

BMPImage *readFromFile(const char *path) {
    BMPImage *bmp = malloc(sizeof(BMPImage));
    if(bmp == NULL) {
        goto error_bmp;
    }

    FILE *stream = fopen(path, "r");
    if(stream == NULL) {
        goto error_bmp;
    }

    bmp->header = malloc(sizeof(BMPHeader));
    if(bmp->header == NULL) {
        goto error_header;
    }

    size_t bytes_read = fread(bmp->header, sizeof(uint8_t), sizeof(BMPHeader), stream);
    if(bytes_read != sizeof(BMPHeader)) {
        fprintf(stderr, "Error reading header: read %ld bytes\n", bytes_read);
        goto error_header;
    }

    if(bmp->header->signature[0] != 'B' || bmp->header->signature[1] != 'M') {
        fprintf(stderr, "Bad signature: not a bmp file\n");
        goto error_header;
    }
    
    size_t extra_size = bmp->header->dataOffset - sizeof(BMPHeader);
    bmp->extra = malloc(extra_size);
    if(bmp->extra == NULL) {
        goto error_extra;
    }

    bytes_read = fread(bmp->extra, sizeof(uint8_t), extra_size, stream);
    if(bytes_read != extra_size) {
        fprintf(stderr, "Error reading extra: read %ld bytes\n", bytes_read);
        goto error_extra;
    }

    bmp->data = malloc(bmp->header->imageSize);
    if(bmp->data == NULL) {
        goto error_data;
    }

    bytes_read = fread(bmp->data, sizeof(uint8_t), bmp->header->imageSize, stream);
    if(bytes_read < bmp->header->imageSize) {
        fprintf(stderr, "Error reading data. Read %ld bytes\n", bytes_read);
        goto error_data;
    }

    fclose(stream);
    return bmp;

error_data:
    free(bmp->data);
error_extra:
    free(bmp->extra);
error_header:
    free(bmp->header);
    fclose(stream);
error_bmp:
    free(bmp);
    fprintf(stderr, "Error creating BMP struct from file %s\n", path);
    if(errno) {
        perror("");
    }
    return NULL;
}


int writeToFile(const char *path, BMPImage *bmp) {
    if(bmp == NULL) {
        fprintf(stderr, "BMP is NULL\n");
        return -1;
    }

    FILE *stream = fopen(path, "w");

    if(stream == NULL) {
        goto error_fopen;
    }

    size_t bytes_written = fwrite(bmp->header, sizeof(uint8_t), sizeof(BMPHeader), stream);
    if(bytes_written != sizeof(BMPHeader)) {
        fprintf(stderr, "Error writing header. %ld bytes written\n", bytes_written);
        goto error_fwrite;
    }


    size_t extra_size = bmp->header->dataOffset - sizeof(BMPHeader);
    bytes_written = fwrite(bmp->extra, sizeof(uint8_t), extra_size, stream);
    if(bytes_written < extra_size) {
        fprintf(stderr, "Error writing extra. %ld bytes written\n", bytes_written);
        goto error_fwrite;
    }

    bytes_written = fwrite(bmp->data, sizeof(uint8_t), bmp->header->imageSize, stream);
    if(bytes_written != bmp->header->imageSize) {
        fprintf(stderr, "Error writing data. %ld bytes written\n", bytes_written);
        goto error_fwrite;
    }

    fclose(stream);
    return 0;

error_fwrite:
    fclose(stream);
error_fopen:
    fprintf(stderr, "Error writing image to %s\n", path);
    if(errno) {
        perror("");
    }
    return -1;

}

BMPImage *createBMP(const uint8_t *data, int width, int height) {
    BMPHeader *header = malloc(sizeof(BMPHeader));
    BMPImage *image = malloc(sizeof(BMPImage));

    size_t imageSize = width * height;
    *header = (BMPHeader)
    {
        .signature = {'B', 'M'},
        .fileSize = imageSize + sizeof(BMPHeader),
        .dataOffset = sizeof(BMPHeader),
        .headerSize = sizeof(BMPHeader) - offsetof(BMPHeader, headerSize),
        .width = width,
        .height = height,
        .planes = 1,
        .bitDepth = 8,
        .compression = 0,
        .imageSize = imageSize,
        .horizontalRes = 0,
        .verticalRes = 0,
        .colors = 256,
        .importantColors = 0,
    };
    uint8_t *imgData = malloc(imageSize);
    memcpy(imgData, data, imageSize);
    *image = (BMPImage)
    {
        .header = header,
        .data = imgData,
        .extra = malloc(0),
    };

    return image;
}


void destroyBMP(BMPImage *bmp) {
    if(bmp == NULL)
        return;
    
    free(bmp->header);
    free(bmp->extra);
    free(bmp->data);
    free(bmp);
}
