/*************************************
 * Tests for binutils
 * 
 * @author Joakim Odermalm
 ************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "binutils.h"

// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#define EACH (size_t i = 0; i < size; i++)

#define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial

// Tests
void test_byte();
void test_int2bits2int(int2bits_t int2bits, bits2int_t bits2int);
void test_int2bits2int_multi(int2bits_t int2bits, bits2int_t bits2int, bits2ints_t bits2ints);
void test_ints2bits(ints2bits_t ints2bits, bits2ints_t bits2ints);

// Test values
// uint8_t message[] = {2};
// uint8_t message[] = {2, 4};
// uint8_t message[] = {0, 1, 2, 3, 4, 5, 6, 7, 157};
uint8_t message[14] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0, 1}; // 15 zeros have been appended to the message

// uint16_t message[] = {0, 1,  32767};
// uint16_t message[] = {0, 1,  32767};
// uint16_t message[] = {POLYNOMIAL, 32};

// uint32_t message[] = {1, 2, 65445, 16777216};
// uint32_t message[] = {0, 1, 2, 3, 4, 5, 6, 7, 157, 65445, 16777216};
// uint32_t message[] = {20000000};


// uint8_t testval = 2;
// uint16_t testval = POLYNOMIAL;
uint32_t testval = 20000000;

/**
  @brief Function pointer types and pointers for calling from MSF/LSF generalized functions 
  @note  Needs to be set before using
 */
typedef void (*int2bits_t)(size_t const size, void const * const ptr, uint8_t out[], bool extraBit); 
typedef uint32_t (*bits2int_t)(size_t const len, uint8_t* bits);
typedef void (*ints2bits_t)(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[], size_t padSize, uint8_t frontPad_size); 
typedef void (*bits2ints_t)(size_t const total_bits, size_t const type_size, uint8_t const bits[], void const * const out_ptr);
extern int2bits_t int2bits;
extern bits2int_t bits2int;
extern ints2bits_t ints2bits;
extern bits2ints_t bits2ints;


int main(void)
{
    // test_byte();
   
    int2bits = int2bitsLSF; //uint8 ok, uint16 ok, uint32 ok  
    bits2int = bits2intLSF; //uint8 ok, uint16 ok, uint32 ok  
    bits2ints = bits2intsLSF; //uint8 ok, uint16 ok, uint32 ok
    ints2bits = ints2bitsLSF; //uint8 ok, uint16 ok, uint32 ok  

    test_int2bits2int_multi(int2bits, bits2int, bits2ints);
    test_ints2bits(ints2bits, bits2ints); 

    int2bits = int2bitsMSF; //uint8 ok, uint16 ok, uint32 ok  
    bits2int = bits2intMSF; //uint8 ok, uint16 ok, uint32 ok
    bits2ints = bits2intsMSF; //uint8 ok, uint16 ok, uint32 ok
    ints2bits = ints2bitsMSF; //uint8 ok, uint16 ok, uint32 ok 
    
    test_int2bits2int_multi(int2bits, bits2int, bits2ints);
    test_ints2bits(ints2bits, bits2ints); 

    test_hexstr2bits();

}

void test_hexstr2bits() {


}


void test_int2bits2int(int2bits_t int2bits, bits2int_t bits2int) {
    size_t type_size = sizeof(testval);
    size_t type_bits = type_size * 8; 

    uint8_t bitsArray[type_bits];
    uint8_t messageBits[type_bits];

        // Print info on inputs
        printf("\ntype_size %d type_bits:%d",type_size, COUNT_OF(bitsArray));
        printf("\t%8d %8x %c\t", testval, testval, testval);

        // Call int2bits
        int2bits(type_size, &testval, bitsArray);
        // Print result
        for  (int i = 0; i < (type_bits); i++) 
            printf("%d", bitsArray[i]);
        
        // Call bits2int, print result
        printf("   reconst: %d", bits2int(type_bits, bitsArray));

        // Concat bitsArrays to messageBits 
        size_t el_start_index = 0 * type_bits;
        for (int j = 0; j < type_bits; j++)
            messageBits[j + el_start_index] = bitsArray[j];

    puts("");
    // Print messageBits
    for  (int i = 0; i < COUNT_OF(messageBits); i++) 
        if ((i+1) % (type_size * 8) != 0)
            printf("%d", messageBits[i]);
        else   
            printf("%d ", messageBits[i]);
}


void test_int2bits2int_multi(int2bits_t int2bits, bits2int_t bits2int, bits2ints_t bits2ints) {
    size_t type_size = sizeof(message[0]); 
    size_t type_bits = type_size * 8;
    size_t msg_size = (sizeof(message));

    uint8_t bitsArray[type_size * 8];
    uint8_t messageBits[sizeof(message) * 8];

    // Print info on inputs
    printf("\nmsg_size %d type_size %d type_bits:%d\n", msg_size, type_size, COUNT_OF(bitsArray));

    for (int i = 0; i < COUNT_OF(message); i++) {
        // Print int as dec, hex, char
        printf("%8d %8x %c\t", message[i], message[i], message[i]);
        
        // Call int2bits
        int2bits(type_size, &message[i], bitsArray);
        // Print result
        for  (int i = 0; i < (type_size * 8); i++) 
            printf("%d", bitsArray[i]);
        
        // Call bits2int, print result
        printf("   reconst: %d\n", bits2int(type_size * 8, bitsArray));

        // Concat bitsArrays to messageBits 
        size_t el_start_index = i * type_size * 8;
        for (int j = 0; j < type_size * 8; j++)
            messageBits[j + el_start_index] = bitsArray[j];

    }
    // puts("\nPrint messageBits");
    puts("");
    // Print messageBits
    for  (int i = 0; i < COUNT_OF(messageBits); i++) 
        if ((i+1) % (type_size * 8) != 0)
            printf("%d", messageBits[i]);
        else   
            printf("%d ", messageBits[i]);

    // Print reconst ints from messageBits
    size_t total_bits = COUNT_OF(messageBits);
    size_t total_nums = total_bits / type_bits;
    uint32_t nums[total_nums];
    bits2ints(total_bits, type_size, messageBits, nums);
    // puts("\nPrint reconst ints from messageBits");
    printf("\t");
    for (int i = 0; i < COUNT_OF(nums); i++) {
        printf("%d ", nums[i]);
    }
    // puts("\n");
}

void test_ints2bits(ints2bits_t ints2bits, bits2ints_t bits2ints) {
    size_t type_size = sizeof(message[0]); 
    size_t msg_size = (sizeof(message));

    uint8_t messageBits[sizeof(message) * 8];

    ints2bits(msg_size, type_size, &message, messageBits, 0, NULL);
    // puts("\nPrint messageBits from ints2bits:");
    puts("");
    for  (int i = 0; i < COUNT_OF(messageBits); i++) 
            if ((i+1) % (type_size * 8) != 0)
                printf("%d", messageBits[i]);
            else   
                printf("%d ", messageBits[i]);


    // Print reconst ints from messageBits
    size_t total_bits = COUNT_OF(messageBits);
    size_t total_nums = total_bits / (type_size * 8);
    uint32_t nums[total_nums];
    bits2ints(total_bits, type_size, messageBits, nums);
    // puts("\nPrint reconst ints from messageBits");
    printf("\t");
    for (int i = 0; i < COUNT_OF(nums); i++) {
        printf("%d ", nums[i]);
    }
    puts("");
}



// void test_byte() {
//     byte b = {
//         .bit.b0 = 1,
//         .bit.b1 = 0,
//         .bit.b2 = 1,
//         .bit.b3 = 0,
//         .bit.b4 = 1,
//         .bit.b5 = 0,
//         .bit.b6 = 1,
//         .bit.b7 = 0,        
//     };
//     for (int i = 0; i < 8; i++) {
//         printf("byte %d val %d\n", i, b.get[i]);
//     }
//     printf("\nsize of byte_union: %d\n", sizeof(b));
//     printf("\nsize of byte_struct: %d\n", sizeof(bit2));
// }
