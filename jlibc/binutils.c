/*************************************
 * Utils for working with bit values
 * 
 * @author Joakim Odermalm
 ************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #include "binutils.h"

// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#define EACH (size_t i = 0; i < size; i++)


// Printing functions
void i2p(void const * const ptr, size_t size, size_t cropTo, char separator, int newline){
    // uint32_t *nums = (uint32_t*) ptr;
    if (cropTo == 0)
        cropTo = size;
    uint8_t *nums = (uint8_t*) ptr;
    for (int i = size - cropTo; i < size; i++)
        if (i < size - 1)
            printf("%d%c", nums[i], separator);
        else 
            printf("%d", nums[i]);
    for (int i = 0; i < newline; i++)
        printf("\n");
}

void i82p(uint8_t nums[], size_t size, size_t cropTo, char separator, int newline){
    // uint32_t *nums = (uint32_t*) ptr;
    if (cropTo == 0)
        cropTo = size;
    // uint8_t *nums = (uint8_t*) ptr;
    for (int i = size - cropTo; i < size; i++)
        if (i < size - 1)
            printf("%d%c", nums[i], separator);
        else 
            printf("%d", nums[i]);
    for (int i = 0; i < newline; i++)
        printf("\n");
}

void i2pc(void const * const ptr, size_t size, char separator, int newline, uint8_t col, uint32_t colStart, uint32_t colLen, int32_t space1, int32_t space2, size_t lead) {
    uint8_t *nums = (uint8_t*) ptr;

    char* fmt_ptr;
    char* fmt_final_ptr;
    char fmt[] = "%d%c"; 
    char fmt_final[] = "%d";

    char colfmt[16]; 
    char colfmt_final[16]; 
    sprintf(colfmt, "\e[1;%dm%%d%%c\e[m", col);
    sprintf(colfmt_final, "\e[1;%dm%%d\e[m", col);
    
    // Leading spaces
    char leadfmt[20];
    sprintf(leadfmt, "%%%ds", lead);
    printf(leadfmt, "");

    for EACH {
        if (i == space1 || i == space2) printf(" ");
        // if (i == space1) printf("a");  if (i == space2)  printf("b");  // DEBUG

        fmt_ptr       = (colStart <= i && i < (colStart + colLen)) ? colfmt : fmt;
        fmt_final_ptr = (colStart <= i && i < (colStart + colLen)) ? colfmt_final : fmt_final;
        if (i < size - 1)
            printf(fmt_ptr, nums[i], separator);
        else 
            printf(fmt_final_ptr, nums[i]);
    }
    for (int i = 0; i < newline; i++)
        printf("\n");
}

void printBits(char label[], uint8_t bits[], size_t size, size_t cropTo) {
        if (cropTo == 0)
            cropTo = size;
        if (size > 40) {
            printf("%10s bits (%d):\n", label, cropTo);
            i2p(bits, size, cropTo, 0, 2); 
        }
        else {
            printf("%10s bits (%d): ", label, cropTo);
            i2p(bits, size, cropTo, 0, 1);
        } 
}

void int2bitsLSF(size_t const size, void const * const ptr, uint8_t out[], bool extraBit) {
    int  byte_index, 
         bit_index;
 
    uint8_t *byte = (uint8_t*) ptr;
    uint8_t bit;

    // This is MEANT to allow n+1 output (9 bits from 8, etc.)
    // used for CRC generators with implicit leading 1.
    //    As I have yet to come across a CRC spec with LSF 
    // generator it has not been tested. For now, recommend only
    // using this function with extraBit set to false.
    size_t modifiedSize = size;
    if (extraBit)
        modifiedSize--;

    for (byte_index = 0; byte_index < modifiedSize; byte_index++)         // LSF
        for (bit_index = 0; bit_index < 8; bit_index++) {                  // LSF
            bit = (byte[byte_index] >> bit_index) & 1;
            out[byte_index * 8 + bit_index] =
            bit ? 1 : 0;
        }
}

void int2bitsMSF(size_t const bytes, void const * const ptr, uint8_t out[], bool extraBit) {
    int  byte_index, 
         bit_index,
         byte_write_index = 0,
         bit_write_index;
 
    uint8_t *byte = (uint8_t*) ptr;
    uint8_t bit;

    // This is to allow n+1 output (9 bits from 8, etc.)
    // used for CRC generators with implicit leading 1.
    uint8_t displace = extraBit ? 1 : 0;

    for (byte_index = bytes-1; byte_index >= 0; byte_index--) {   //MSF
        bit_write_index = 0;
        for (bit_index = 7; bit_index >= 0; bit_index--) {         // MSF
            bit = (byte[byte_index] >> bit_index) & 1;
            out[byte_write_index * 8 + bit_write_index++ + displace] = 
            bit ? 1 : 0;
            // printf("Wrote %u to bit[%d] byte[%d] to out[%d]\n", bit, bit_index, byte_index, byte_write_index);
        }
        byte_write_index++;
    }
}


uint64_t bits2intLSF(size_t const size, uint8_t* bits) {
    uint64_t r = 0;
    for (uint8_t bit_index = 0; bit_index < size; bit_index++)    {   
        if (bits[bit_index]) 
            // OR 1 with bit at index means 1
            r |= 1ULL << (bit_index);
        else
            // AND with NOT of bit at index means 0
            r &= ~(1ULL << (bit_index) );

        // printf("i:%2d b:%u r:%#llX\n", bit_index, bits[bit_index], r, r);
    }
    // printf("bits2intLSF returns %#0llX from %d bits: ", r, size);
    // for EACH printf("%d%s", bits[i], (i+1)%4==0 ? " " : "" );     puts("");
    return r; 
}


uint64_t bits2intMSF(size_t const size, uint8_t* bits) {
    uint64_t r = 0;
    int bit_write_index = size;

    for (int bit_index = 0; bit_index < size; bit_index++) {       
        if (bits[bit_index]) 
            r |= 1ULL << (--bit_write_index);
        else    
            r &= ~(1ULL << (--bit_write_index) );
        // printf("i:%2d bwi: %d  b:%u   r:%#llX\n", bit_index, bit_write_index, bits[bit_index], r, r);
    }
    return r; 
}





















void ints2bitsLSF(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[], size_t padSize, uint8_t frontPad_size) {
    uint8_t bitsArray[type_size * 8];

    uint8_t*  ints8  = (uint8_t*) ptr;
    uint16_t* ints16 = (uint16_t*) ptr;
    uint32_t* ints32 = (uint32_t*) ptr;
    uint64_t* ints64 = (uint64_t*) ptr;

    for (int i = 0; i < (size / type_size); i++) {
        switch (type_size) {
        case 1:
            int2bitsLSF(type_size, &ints8[i], bitsArray, false);
            break;
        case 2:
            int2bitsLSF(type_size, &ints16[i], bitsArray, false);
            break;
        case 4:
            int2bitsLSF(type_size, &ints32[i], bitsArray, false);
            break;
        case 8:
            int2bitsLSF(type_size, &ints64[i], bitsArray, false);
            break;
        default:
            fprintf(stderr, "Unsupported type size, exiting.");
            exit(EXIT_FAILURE);
        }

        size_t el_start_index = i * type_size * 8 + frontPad_size;
        for (int j = 0; j < type_size * 8; j++)
            out[j + el_start_index] = bitsArray[j];
    }
}   

void ints2bitsMSF(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[], size_t padSize, uint8_t frontPad_size) {
    uint8_t bitsArray[type_size * 8];

    uint8_t*  ints8  = (uint8_t*) ptr;
    uint16_t* ints16 = (uint16_t*) ptr;
    uint32_t* ints32 = (uint32_t*) ptr;
    uint64_t* ints64 = (uint64_t*) ptr;
    
    for (int i = 0; i < (size / type_size); i++) {
        switch (type_size) {
        case 1:
            int2bitsMSF(type_size, &ints8[i], bitsArray, false);
            break;
        case 2:
            int2bitsMSF(type_size, &ints16[i], bitsArray, false);
            break;
        case 4:
            int2bitsMSF(type_size, &ints32[i], bitsArray, false);
            break;
        case 8:
            int2bitsMSF(type_size, &ints64[i], bitsArray, false);
            break;
        default:
            fprintf(stderr, "Unsupported type size, exiting.");
            exit(EXIT_FAILURE);
        }
        size_t el_start_index = i * type_size * 8 + frontPad_size;
        for (int j = 0; j < type_size * 8; j++)
            out[j + el_start_index] = bitsArray[j];
    }
}

void bits2intsMSF(size_t const total_bits, size_t const type_size, uint8_t const bits[], void const * const out_ptr) {
    uint8_t bitsArray[type_size * 8];
    size_t elem_total = total_bits / (type_size * 8);
    
    size_t bits_iter = 0;
    size_t bitsArray_iter = 0;
    size_t out_iter = 0;
    
    uint8_t*  out8  = (uint8_t*) out_ptr;
    uint16_t* out16 = (uint16_t*) out_ptr;
    uint32_t* out32 = (uint32_t*) out_ptr;
    uint64_t* out64 = (uint64_t*) out_ptr;

    while (bits_iter <= total_bits) {      
        if (bitsArray_iter < (type_size * 8 )) {
            bitsArray[bitsArray_iter] = bits[bits_iter];
            bitsArray_iter++; 
            bits_iter++;
        }
        else {
            switch (type_size) {
                case 1:
                    out8[out_iter++] = bits2intMSF(type_size * 8, bitsArray);
                    break;
                case 2:
                    out16[out_iter++] = bits2intMSF(type_size * 8, bitsArray);
                    break;
                case 4:
                    out32[out_iter++] = bits2intMSF(type_size * 8, bitsArray);
                    break;
                case 8:
                    out64[out_iter++] = bits2intMSF(type_size * 8, bitsArray);
                    break;  
                default:
                    fprintf(stderr, "Unsupported type size, exiting.");
                    exit(EXIT_FAILURE);
            }
            bitsArray_iter = 0;
        }
    } 
}   

void bits2intsLSF(size_t const total_bits, size_t const type_size, uint8_t const bits[], void const * const out_ptr) {
    uint8_t bitsArray[type_size * 8];
    size_t elem_total = total_bits / (type_size * 8);
    
    size_t bits_iter = 0;
    size_t bitsArray_iter = 0;
    size_t out_iter = 0;
    
    uint8_t*  out8  = (uint8_t*) out_ptr;
    uint16_t* out16 = (uint16_t*) out_ptr;
    uint32_t* out32 = (uint32_t*) out_ptr;
    uint64_t* out64 = (uint64_t*) out_ptr;

    while (bits_iter <= total_bits) {      
        if (bitsArray_iter < (type_size * 8 )) {
            bitsArray[bitsArray_iter] = bits[bits_iter];
            bitsArray_iter++; bits_iter++;
        }
        else {
            switch (type_size) {
                case 1:
                    out8[out_iter++] = bits2intLSF(type_size * 8, bitsArray);
                    break;
                case 2:
                    out16[out_iter++] = bits2intLSF(type_size * 8, bitsArray);
                    break;
                case 4:
                    out32[out_iter++] = bits2intLSF(type_size * 8, bitsArray);
                    break;
                case 8:
                    out64[out_iter++] = bits2intLSF(type_size * 8, bitsArray);
                    break;    
                default:
                    fprintf(stderr, "Unsupported type size, exiting.");
                    exit(EXIT_FAILURE);
            }
            bitsArray_iter = 0;
        }
    } 
}   

void charArrayToString(char ca[], size_t size, char* out) {
    // char buf[size + 1];
    int  count = 0;
    for EACH 
        if ( isprint(ca[i]) ) {
            out[i] = ca[i];
            count++;
        }
    out[count] = '\0';
}    

void bitSlice(int start, int count, void const * const ptr, size_t size, uint8_t out[]) {
    uint8_t* in = (uint8_t*) ptr;
    // Start from end, finish at size?
    if (start < 0) 
        for (int i = size + (start), j = count - 1; i > size - count + (start); i--, j--)
            out[j] = in[i];
    // Start from... start.    
    else
        for (int i = start, j = 0; j < count; i++, j++)
            out[j] = in[i];
}

