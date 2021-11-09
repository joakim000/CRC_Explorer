/** Engine for CRC Explorer ******************************************
  @brief API for validating CRC implementations

  1. Uncomment the line
     #define EXPLORER_ENGINE_ID "Engine 1"
  2. Put your implementation inside the function GetRem (engine.c). 
  3. It will receive message deta and CRC definition as struct params.
  4. It should return the remainder of the calculation as an uint64_t.
  5. For validation the CRC value is received as an uint64_t.

  The fields of each input struct is described below.     

*********************************************************************/
// Uncomment this to enable external engine
#define EXPLORER_ENGINE_ID "Engine 1"

// Experimental 
#define WIDE_CRC

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct crc_s crc_t;
typedef struct msg_s msg_t;

/**
*  @brief Get remainder from CRC calculation
*  @return The remainder
*  @param  crc_t*   crc    A CRC definition
*  @param  msg_t*   msg    A message to process
*  @param  uint64_t check  0; return CRC value for msg 
*                          CRC value; return 0 for unchanged msg 
*
*  @note   Fields of crc and msg:
*
*          msg->
*            msgStr     Message as string 
*            len        Character-count excluding \0
*          
*          crc->
*            n          Bit width 
*            g          Generator polynomial
*            init       Initial CRC value, also known as seed.
*            nondirect  Init can be "direct" or "non-direct".
*                        Generally false, but provided because exceptions
*                        do exist (example: CRC-16/CCITT).
*            inputLSF   Input reflected; message bytes are encoded LSF.
*            resultLSF  Result reflected; result is encoded LSF.
*                        Note, reflection is over whole value, not per byte. 
*            xor        Final XOR; XOR result with this value after calculation
*                        is done, but before result reflection (if any). 
*
*                Some additional fields are available
*                if you have use for them. See definitions
*                of crc_t and msg_t.
*/
uint64_t GetRem(crc_t* crc, msg_t* msg, uint64_t check);


/**
 * @brief Definition of a CRC
*/
typedef struct crc_s {
    /** Definition of a CRC *************************************************************/
    uint64_t n;          // Bit width 
    uint64_t g;          // Generator polynomial
    uint64_t init;       // Initial CRC value, also known as seed.
    uint8_t  nondirect;  // Init can be "direct" or "non-direct".
                         //   Generally false, but provided because exceptions
                         //   do exist (example: CRC-16/CCITT).
    uint8_t inputLSF;    // Input reflected; message bytes are encoded LSF.
    uint8_t resultLSF;   // Result reflected; remainder is encoded LSF.
                         //   Note, reflection is over whole value, not per byte. 
    uint64_t xor;        // Final XOR; XOR result with this value after calculation
                         // is done, but before result reflection (if any). 

    uint8_t il1;         // Generator polynomial has implicit leading 1. 
                         //   Generally true, but provided because exceptions
                         //   do exist (example: CRC-15/CAN*).
    /************************************************************************************/
    //  * This spec I've later found to be incorrect, the real CRC-15/CAN also using 
    //  implicit leading 1. So this flag exists, should anyone need it, but can
    //  probably be safely ignored (assumed true).  

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

    // Wide CRC support
    #ifdef WIDE_CRC
    char w_g[35];          // Generator polynomial
    char w_init[35];       // Initial CRC value, also known as seed.
    char w_xor[35];        // Final XOR; XOR result with this value after calculation
    char w_residue[35];    // Given as spec on some sites, not sure what it's used for yet.
    char w_check[35];      // Expected result from "123456789"
    char w_checkAB[35];    // Expected result from "AB"
    char w_init_conv[35];  // Converted initial CRC value (seed)
    
    uint8_t hex_orders;

    // Work arrays
    uint8_t w_gBits[129];
    uint8_t w_initBits[128];
    uint8_t w_xorBits[128];
    uint8_t w_initBits_conv[128];  // Converted initial CRC value (seed)
    #endif
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

    #ifdef WIDE_CRC
    // Wide validation    
    char w_rem[35];
    uint8_t w_remBits[128];
    char* w_validation_rem;
    #endif

} msg_t;


