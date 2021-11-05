/** Engine for CRC Explorer ******************************************
  @brief API for validating CRC implementations
  1. Uncomment the line
     #define CRC_EXPLORER_EXTERNAL
  2. Put your implementation inside the function GetRem (engine.c). 
  3. It will receive message deta and CRC definition as struct params.
  4. It should return the remainder of the calculation as an uint64_t.
  5. For validation the CRC value is received as an uint64_t.

  The fields of each input struct is described below.     

*********************************************************************/
// Uncomment this to enable external engine
// #define CRC_EXPLORER_EXTERNAL

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct crc_s crc_t;
typedef struct msg_s msg_t;

/**
  @brief Get remainder from CRC calculation
  @return The remainder
  @param  crc_t*   crc    A CRC definition
  @param  msg_t*   msg    A message to process
  @param  uint64_t check  0; return CRC value for msg 
                          CRC value; return 0 for unchanged msg 

  @note   These are the fields of crc and msg
          you certainly will need:

          msg->
            msgStr     Message as string 
            len        Character-count excluding \0
          
          crc->
            n          Bit width 
            g          Generator polynomial
            il1        Generator polynomial has Implicit leading 1. 
                        Generally true, but provided because exceptions
                        do exist (example: CRC-15/CAN).
            init       Initial CRC value, also known as seed.
            nondirect  Init can be "direct" or "non-direct".
                        Generally false, but provided because exceptions
                        do exist (example: CRC-16/CCITT).
            inputLSF   Input reflected; message bytes are encoded LSF.
            resultLSF  Result reflected; result is encoded LSF.
                        Note, reflection is over whole value, not per byte. 
            xor        Final XOR; XOR result with this value after calculation
                        is done, but before result reflection (if any). 

                Some additional fields are available
                if you have use for them. See definitions
                of crc_t and msg_t.
*/
uint64_t GetRem(crc_t* crc, msg_t* msg, uint64_t check);


/**
 * @brief Definition of a CRC
*/
typedef struct crc_s {
    /** Definition of a CRC *************************************************************/
    uint64_t n;          // Bit width 
    uint64_t g;          // Generator polynomial
    uint8_t il1;         // Generator polynomial has Implicit leading 1. 
                         //   Generally true, but provided because exceptions
                         //   do exist (example: CRC-15/CAN).
    uint64_t init;       // Initial CRC value, also known as seed.
    uint8_t  nondirect;  // Init can be "direct" or "non-direct".
                         //   Generally false, but provided because exceptions
                         //   do exist (example: CRC-16/CCITT).
    uint8_t inputLSF;    // Input reflected; message bytes are encoded LSF.
    uint8_t resultLSF;   // Result reflected; remainder is encoded LSF.
                         //   Note, reflection is over whole value, not per byte. 
    uint64_t xor;        // Final XOR; XOR result with this value after calculation
                         // is done, but before result reflection (if any). 
    /************************************************************************************/
    
    // Extra info
    char description[64];
    uint64_t residue;    // Given as spec on some sites, not sure what it's used for yet.
    uint64_t check;      // Expected result from "123456789"
    uint64_t checkAB;    // Expected result from "AB"
    uint64_t init_conv;  // Converted initial CRC value (seed)

    // Work arrays
    uint8_t gBits[65];
    uint8_t initBits[64];
    uint8_t xorBits[64];
} crc_t;


/**
 * @brief Definition of a message
*/
typedef struct msg_s {
    /** Message data *********************************************/
    char* msgStr;        // Message as string 
    size_t len;          // Length excluding \0
    /*************************************************************/

    // Validation    
    uint64_t rem;
    uint8_t remBits[64];
    uint64_t validation_rem;
    bool valid;

    // Custom check value
    uint64_t expected;

    // Optional details
    uint8_t* msgBits;         
    size_t originalBitLen;     
    size_t paddedBitLen;  
    size_t initPad;   
} msg_t;





