#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

BMPImage *readFromFile(const char *path) {
    bool error = false;
    BMPImage *bmp = malloc(sizeof(BMPImage));
    if(bmp == NULL) {
        error = true;
        // Handle
    }

    bmp->header = malloc(sizeof(BMPHeader));
    if(bmp->header == NULL) {
        error = true;
        // Handle
    }

    FILE *stream = fopen(path, "r");
    if(stream == NULL) {
        perror("");
        error = true;
        // Handle
    }

    size_t bytes_read = fread(bmp->header, sizeof(uint8_t), sizeof(BMPHeader), stream);
    if(bytes_read != sizeof(BMPHeader)) {
        fprintf(stderr, "Error reading header: read %ld bytes\n", bytes_read);
        error = true;
        // Handle
    }

    if(bmp->header->signature[0] != 'B' || bmp->header->signature[1] != 'M') {
        fprintf(stderr, "Bad signature: not a bmp file\n");
        error = true;
        // Handle
    }
    
    size_t extra_size = bmp->header->dataOffset - sizeof(BMPHeader);
    bmp->extra = malloc(extra_size);
    if(bmp->extra == NULL) {
        error = true;
        // Handle
    }

    bytes_read = fread(bmp->extra, sizeof(uint8_t), extra_size, stream);
    if(bytes_read != extra_size) {
        fprintf(stderr, "Error reading extra: read %ld bytes\n", bytes_read);
        error = true;
        // Handle
    }

    bmp->data = malloc(bmp->header->imageSize);
    if(bmp->data == NULL) {
        error = true;
        // Handle
    }

    bytes_read = fread(bmp->data, sizeof(uint8_t), bmp->header->imageSize, stream);
    printf("Read %ld bytes from data\n", bytes_read);
    if(bytes_read < bmp->header->imageSize) {
        //error = true;
        fprintf(stderr, "Error reading data. Read %ld bytes\n", bytes_read);
        // Handle
    }

    return error? NULL : bmp;
}


int writeToFile(const char *path, BMPImage *bmp) {
    int ret = 0;
    FILE *stream = fopen(path, "w");

    if(stream == NULL) {
        puts("Error: bad offset");
    }

    size_t bytes_written = fwrite(bmp->header, sizeof(uint8_t), sizeof(BMPHeader), stream);
    if(bytes_written != sizeof(BMPHeader)) {
        puts("Error: bad header");
    }


    size_t extra_size = bmp->header->dataOffset - sizeof(BMPHeader);
    bytes_written = fwrite(bmp->extra, sizeof(uint8_t), extra_size, stream);
    if(bytes_written < extra_size) {
        puts("Error: bad extra");
    }

    bytes_written = fwrite(bmp->data, sizeof(uint8_t), bmp->header->imageSize, stream);
    if(bytes_written != bmp->header->imageSize) {
        puts("Error: bad data");
    }

    fclose(stream);

    return ret;

}


void destroyBMP(BMPImage *bmp) {
    if(bmp == NULL)
        return;
    
    free(bmp->header);
    free(bmp->data);
    free(bmp);
}
