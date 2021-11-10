/*************************************
 * Utils for working with bit values
 * 
 * @author Joakim Odermalm
 ************************************/
#include "common.h"


//* Hexstring conversions *//

/**
 @brief Convert a hex-string to array of bit values 
 @note Variants for Most / Least Significant First 
 @example
*/
void hexstr2bitsMSF(size_t const bits, char hexstr[], uint8_t out[], bool extraBit);
void hexstr2bitsLSF(size_t const bits, char hexstr[], uint8_t out[], bool extraBit);

/**
 @brief Convert an array of bit values to a hex-string
 @note Variants for Most / Least Significant First 
 @example
*/
void bits2hexstrMSF(size_t const size, uint8_t* bits,  char out[]);
void bits2hexstrLSF(size_t const size, uint8_t* bits,  char out[]);

//* end Hexstring conversions *//


/**
 @brief Convert a single int to array of bit values 
 @note Variants for Most / Least Significant First 
 @example
    size_t type_size = sizeof(anInt);
    uint8_t bitArray[type_size * 8];
    int2bits(type_size, &testval, bitArray);
*/
void int2bitsMSF(size_t const size, void const * const ptr, uint8_t out[], bool extraBit);
void int2bitsLSF(size_t const size, void const * const ptr, uint8_t out[], bool extraBit);

/**
 @brief Convert an array of bit values to a single int 
 @note Variants for Most / Least Significant First 
 @return Converted int as uint32_t
 @param bits array of bitvalues
 @param size count of bits
*/
uint64_t bits2intMSF(size_t const size, uint8_t* bits);
uint64_t bits2intLSF(size_t const size, uint8_t* bits);


/**
 @brief Convert an array of ints to a concatenated array of bit values 
 @note Variants for Most / Least Significant First 
 @example
    uint32_t message[] = {1, 2, 3};
    size_t type_size = sizeof(message[0]); 
    size_t msg_size = (sizeof(message));
    uint8_t messageBits[sizeof(message) * 8];

    ints2bits(msg_size, type_size, &message, messageBits, 8);
*/
void ints2bitsMSF(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[], size_t padSize, uint8_t frontPad_size);
void ints2bitsLSF(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[], size_t padSize, uint8_t frontPad_size);


/**
 @brief Convert an array bit values to array of ints 
 @note Variants for Most / Least Significant First 
 @example
    size_t total_bits = COUNT_OF(bitValues);        // Length of bit value array
    size_t type_size = sizeof(uint32_t);            // Expected type size
    uint32_t nums[total_bits / type_size * 8];      // Count of nums based on expected type size
    bits2ints(total_bits, type_size, bitValues, nums);
*/
void bits2intsMSF(size_t const total_bits, size_t const type_size, uint8_t const bits[], void const * const out_ptr);
void bits2intsLSF(size_t const total_bits, size_t const type_size, uint8_t const bits[], void const * const out_ptr);


/**
  @brief Function pointer types and pointers for calling from MSF/LSF generalized functions 
  @note  Needs to be set before using
 */
// typedef void (*int2bits_t)(size_t const size, void const * const ptr, uint8_t out[], bool extraBit); 
// typedef uint32_t (*bits2int_t)(size_t const len, uint8_t* bits);
// typedef void (*ints2bits_t)(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[], size_t padSize, uint8_t frontPad_size); 
// typedef void (*bits2ints_t)(size_t const total_bits, size_t const type_size, uint8_t const bits[], void const * const out_ptr);
// extern int2bits_t int2bits;
// extern bits2int_t bits2int;
// extern ints2bits_t ints2bits;
// extern bits2ints_t bits2ints;


/**
  @brief Convert array of chars to null-terminated string  
*/
void charArrayToString(char ca[], size_t size, char* out);

/**
  @brief Get slice of uint8_t[]
  @param start Set -1 to start from end
  @param ptr input array  
  @note No bounds checking
*/
void bitSlice(int start, int count, void const * const ptr, size_t size, uint8_t out[]);

/**
  @brief Print array of ints
  @param ptr     pointer to array
@param size      size of array
@param separator a sepator char, set 0 for none
@param newline   number of newlines at end
*/
void i2p(void const * const ptr, size_t size, size_t cropTo, char separator, int newline); 
void i82p(uint8_t nums[], size_t size, size_t cropTo, char separator, int newline);


/**
  @brief Print array of ints; with colors and a space somewhere  
  @note ANSI codes:        "\e[1;COLm TEXT \e[m"
           Look     on/off   Color  Fg nrm/bright   Bg nrm/bright
          Reset     0       Black       30   90         40   100
          Bold      1  23   Red         31   91         41   101
          Italic    3  23   Green       32   92         42   102
          Underline 4  24   Yellow      33   93         43   103
		      Blink     5  25	  Blue        34   94         44   104
          Invert    7  27   Magenta     35   95         45   105
          Strike    9  29   Cyan        36   96         46   106
          Framed   51  54   White       37   97         47   107
          Circled  52  54     
		      Overline 53  55		    

  @param ptr       pointer to array of ints
  @param size      size of array
  @param separator a sepator char, set 0 for none
  @param newline   number of newlines at end
  @param col       color code
  @param colStart  index to start coloring
  @param colLen    length to color
  @param space     insert a space before this index, set -1 for no space
  @param space2    second space

*/
void i2pc(void const * const ptr, size_t size, char separator, int newline, uint8_t col, uint32_t colStart, uint32_t colLen, int32_t space, int32_t space2, size_t lead);


/**
  @brief Print formatted bits  
  @note 
*/
void printBits(char label[], uint8_t bits[], size_t size, size_t cropTo) ;

/**
  @brief Various (failed) experiments for reducing memory usage 
*/
// typedef union byte_union {
//     struct byte_struct
//     {
//         int b0 : 1;
//         int b1 : 1;
//         int b2 : 1;
//         int b3 : 1;
//         int b4 : 1;
//         int b5 : 1;
//         int b6 : 1;
//         int b7 : 1;
//     } bit;
//     int get[8];
// } byte;

//  struct byte_struct2
//  {
//      int b0 : 1;
//      int b1 : 1;
//      int b2 : 1;
//      int b3 : 1;
//      int b4 : 1;
//      int b5 : 1;
//      int b6 : 1;
//      int b7 : 1;
//      int* next;
//  } bit2;
