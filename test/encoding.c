#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "encoding.h"

int test_lsb2(void)
{
    uint8_t inputBuffer[128], outputBuffer[sizeof(inputBuffer)*4], testBuffer[sizeof(inputBuffer)];
    size_t inputLength;
    memset(outputBuffer, 0xAF, sizeof(outputBuffer));

    FILE *in = fopen("test_in", "r");
    inputLength = fread(inputBuffer, 1, sizeof(inputBuffer), in);
    fclose(in);

    encodeLsb2(inputLength, inputBuffer, outputBuffer);
    decodeLsb2(inputLength, outputBuffer, testBuffer);

    FILE *out = fopen("test_lsb2", "w");
    fwrite(outputBuffer, 1, inputLength*4, out);
    fclose(out);

    int result = memcmp(inputBuffer, testBuffer, inputLength);
    printf("%d\n", result);
    return result;
}

int test_lsb4(void)
{
    uint8_t inputBuffer[128], outputBuffer[sizeof(inputBuffer)*2], testBuffer[sizeof(inputBuffer)];
    size_t inputLength;
    memset(outputBuffer, 0xAF, sizeof(outputBuffer));

    FILE *in = fopen("test_in", "r");
    inputLength = fread(inputBuffer, 1, sizeof(inputBuffer), in);
    fclose(in);

    encodeLsb4(inputLength, inputBuffer, outputBuffer);
    decodeLsb4(inputLength, outputBuffer, testBuffer);

    FILE *out = fopen("test_lsb4", "w");
    fwrite(outputBuffer, 1, inputLength*2, out);
    fclose(out);

    int result = memcmp(inputBuffer, testBuffer, inputLength);
    printf("%d\n", result);
    return result;
}
