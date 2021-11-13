#include <stdint.h>
#include <stdio.h>
#include "engine.h"


#ifdef EXPLORER_ENGINE


#if EXPLORER_ENGINE_ID == 101
uint64_t Another_CRC_Function(crc_t* crc, msg_t* msg, uint64_t check) {

    return 0;
}
#endif // EXPLORER_ENGINE_ID == 101


#if EXPLORER_ENGINE_ID == 'P'  // Pontus_1
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
#endif // EXPLORER_ENGINE_ID == P


#if EXPLORER_ENGINE_ID == 'S'  // Pontus_1 med steps
#include "../lib/jlibc/binutils.h"
uint8_t displayBits8[sizeof(uint8_t) * 8];
uint8_t displayBits64[sizeof(uint64_t) * 8];
#define DISPLAYSIZE ( (sizeof (uint64_t) * 8) - crc->n )
#define PRINT_HEAD(x) printf("%16s\t", x);
#define CONVERT8(x) int2bitsMSF(sizeof(uint8_t), &x, displayBits8, false);
#define CONVERT64(x) int2bitsMSF(sizeof(uint64_t), &x, displayBits64, false);
#define PRINT_BITS8 for (int i = 0; i < DISPLAYSIZE; i++) printf(" "); i82p(displayBits8, COUNT_OF(displayBits8), 0, 0, 1);
#define PRINT_BITS64 i82p(displayBits64, COUNT_OF(displayBits64), 0, 0, 1);
#define DISPLAY8(x, y) if (disp) {PRINT_HEAD(x) CONVERT8(y) PRINT_BITS8}
#define DISPLAY(x, y) if (disp) {PRINT_HEAD(x) CONVERT64(y) PRINT_BITS64}


#define LSB 1
static uint64_t reflect(uint64_t input, int bits);

uint64_t Pontus_1S(crc_t* crc, msg_t* msg, uint64_t check) {
    uint64_t MSB = (1ULL << crc->n - 1);
    uint64_t reg = crc->init;

    bool disp = !strcmp(msg->msgStr, "A");
    DISPLAY("MSB", MSB);
    DISPLAY("Reg init", reg);


    // Silly non direct things
    if (crc->nondirect == 0)
    {
        for (uint64_t i = 0; i < crc->n; i++)
        {
            uint64_t xor = reg & LSB;

            xor && (reg ^= crc->g);
            reg >>= 1;
            xor && (reg |= MSB);
        }
    }

    for (uint64_t pos = 0; pos < msg->len; pos++) // Not appended bits
    {
        uint8_t bits = msg->msgStr[pos];
        if (disp) printf("Msg char: %c\n", msg->msgStr[pos]);
        DISPLAY8("Char bits", bits); if (disp) puts("");
        if (crc->inputLSF)
        {
            bits = reflect(bits, 8);
            DISPLAY8("Char bits (ref)", bits);
        }

        for (size_t i = 0; i < 8; i++)
        {
            if (disp) printf("Char %c step %3d\n", msg->msgStr[pos], i);
            DISPLAY("reg", reg);

            uint64_t xor = reg & MSB;
            DISPLAY("reg & MSB", xor);
            reg <<= 1;
            DISPLAY("reg shift", reg);
            reg = reg | ((bits >> (7 - i)) & 1);
            DISPLAY("reg | bits", reg);
            // xor && (reg ^= crc->g);
            if (xor) {
                reg ^= crc->g;
                DISPLAY("Gen", crc->g);
                DISPLAY("reg^gen", reg);
            }
            else {
                if (disp) printf("\t\t\t\tSkip reg^gen\n");
            }
        }
    }

    // appended bits
    for (uint64_t i = 0; i < crc->n; i++)
    {
        if (disp) printf("Pad step %3d\n", i);
        DISPLAY("reg", reg);
        uint64_t xor = reg & MSB;
        DISPLAY("reg & MSB", xor);
        reg <<= 1;
        DISPLAY("reg shift", reg);

        if (xor) {
            reg ^= crc->g;
            DISPLAY("Gen", crc->g);
            DISPLAY("reg^gen", reg);
        }
        else {
            if (disp) printf("\t\t\t\tSkip reg^gen\n");
        }
    }

    // reflect out here if needed
    if (crc->resultLSF)
    {
        reg = reflect(reg, crc->n);
        DISPLAY("Reg (ref)", reg);
    }

    // XOR all the time XOR with all zeros is just same value
    reg ^= crc->xor;
    if (crc->xor) DISPLAY("Final xor", reg);
    if (disp) puts("");

    // Need to return only the bits of interest
    // And do a dirty 64 W thing
    if (crc->n != 64)
    {
        uint64_t mask = (1ULL << crc->n) - 1ULL;
        DISPLAY("Mask", mask);
        reg = reg & mask;
    }
    DISPLAY("After", reg);
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
#endif // EXPLORER_ENGINE_ID == 'S'




#endif // #ifdef EXPLORER_ENGINE
