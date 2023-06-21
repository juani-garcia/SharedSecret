#include "encoding.h"

void encodeLsb4(size_t n, const uint8_t input[restrict static n], uint8_t output[restrict static 2*n])
{
    uint16_t *out = (uint16_t*)output;
    for(unsigned long i = 0; i < n; i++)
    {
        uint8_t byte = input[i];
        out[i] &= 0xF0F0;
        out[i] |= (byte & 0xF0) >> 4;
        out[i] |= (byte & 0x0F) << 8;
    }
}

void decodeLsb4(size_t n, const uint8_t input[restrict static 2*n], uint8_t output[restrict static n])
{
    uint16_t * in = (uint16_t*)input;
    for(unsigned long i = 0; i < n; i++)
    {
        uint8_t byte = 0;
        byte |= (in[i] & 0x000F) << 4;
        byte |= (in[i] & 0x0F00) >> 8;
        output[i] = byte;
    }
}

void encodeLsb2(size_t n, const uint8_t input[restrict static n], uint8_t output[restrict static 4*n])
{
    uint32_t *out = (uint32_t*)output;
    for(unsigned long i = 0; i < n; i++)
    {
        uint8_t byte = input[i];
        out[i] &= 0xFCFCFCFC;
        out[i] |= (byte & 0xC0) >> 6;
        out[i] |= (byte & 0x30) << 4;
        out[i] |= (byte & 0x0C) << 14;
        out[i] |= (byte & 0x03) << 24;
    }
}

void decodeLsb2(size_t n, const uint8_t input[restrict static 4*n], uint8_t output[restrict static n])
{
    uint32_t * in = (uint32_t*)input;
    for(unsigned long i = 0; i < n; i++)
    {
        uint8_t byte = 0;
        byte |= (in[i] & 0x00000003) << 6;
        byte |= (in[i] & 0x00000300) >> 4;
        byte |= (in[i] & 0x00030000) >> 14;
        byte |= (in[i] & 0x03000000) >> 24;
        output[i] = byte;
    }
}
