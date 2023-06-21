#ifndef BMP_H
#define BMP_H

#include <stdint.h>

#pragma pack(2)

typedef struct BMPHeader {
    // BMP header fields
    uint8_t signature[2]; // Should always be BM
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitDepth;
    uint32_t compression;
    uint32_t imageSize;
    int32_t horizontalRes;
    int32_t verticalRes;
    uint32_t colors;
    uint32_t importantColors;
} BMPHeader;

#pragma pack()

typedef struct BMPImage {
    BMPHeader *header;
    uint8_t *extra;
    uint8_t *data;
} BMPImage;

BMPImage *readFromFile(const char *path);
int writeToFile(const char *path, BMPImage *bmp);
void destroyBMP(BMPImage *bmp);

#endif
