#include <stdint.h>
#include <stdio.h>
#include "engine.h"

#ifdef EXPLORER_ENGINE

#if EXPLORER_ENGINE_ID == 1  // Pontus 1
#define LSB 1
static uint64_t reflect(uint64_t input, int bits);

uint64_t Pontus_1(crc_t* crc, msg_t* msg, uint64_t check) {
    uint64_t MSB = (1ULL << crc->n - 1);
    uint64_t reg = crc->init;

    // Silly non direct things
    if (crc->nondirect == 0)
    {
        for (uint64_t i = 0; i < crc->n; i++)
        {
            uint64_t xor = reg & LSB;

            xor&&(reg ^= crc->g);
            reg >>= 1;
            xor&&(reg |= MSB);
        }
    }

    for (uint64_t pos = 0; pos < msg->len; pos++) // Not appended bits
    {
        uint8_t bits = msg->msgStr[pos];
        if (crc->inputLSF)
        {
            bits = reflect(bits, 8);
        }

        for (size_t i = 0; i < 8; i++)
        {
            uint64_t xor = reg & MSB;
            reg <<= 1;
            reg = reg | ((bits >> (7 - i)) & 0x01);
            xor&&(reg ^= crc->g);
        }
    }

    // appended bits
    for (uint64_t i = 0; i < crc->n; i++)
    {
        uint64_t xor = reg & MSB;
        reg <<= 1;
        xor&&(reg ^= crc->g);
    }

    // reflect out here if needed
    if (crc->resultLSF)
    {
        reg = reflect(reg, crc->n);
    }

    // XOR all the time XOR with all zeros is just same value
    reg ^= crc->xor;

    // Need to return only the bits of interest
    // And do a dirty 64 W thing
    if (crc->n != 64)
    {
        uint64_t mask = (1ULL << crc->n) - 1ULL;
        reg = reg & mask;
    }
    return reg;
}

static uint64_t reflect(uint64_t input, int bits)
{
    uint64_t i, j = 1, reflected = 0;

    for (uint64_t i = (uint64_t)1 << (bits - 1); i; i >>= 1)
    {
        if (input & i)
        {
            reflected |= j;
        }
        j <<= 1;
    }

    return reflected;
}
#endif // EXPLORER_ENGINE_ID == 1



#if EXPLORER_ENGINE_ID == 2
uint64_t Engine_2(crc_t* crc, msg_t* msg, uint64_t check) {

    return 0;
}
#endif // EXPLORER_ENGINE_ID == 2

#endif // #ifdef EXPLORER_ENGINE_ID
