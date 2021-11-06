# CRC Explorer - **sail the seven seas of CRCs**

## Tour

Try these commands to get an overview.

* make
* crc zoo
* crc enc -m AB -c 17 -steps -noskip
* scripts/test_perf.sh
* crc help

## Raison d'être

This project was inspired by a school assignment where students were to implement a somewhat obscure CRC specification. It's obscurity was meant to hinder the reuse of example implementations on the Internet, but also meant the only direct means of checking one's work was some provided example pairs of input strings and their associated CRC values. Only problem - the example values were faulty, being sourced from the educator's own incorrect implementation. 

A frustrating experience to be sure, but an excellent lesson in the importance of testing for correctness and validity of outputs.  

## Features

### Validatable (it's a word) process

### Educational step-by-step output

### Zoo

### API for plug-in validation of other implemenations

### Other features

## Step-by-step

## External engine API

engine.h declaration of GetRem:
~~~c
/**
*  @brief  Get remainder from CRC calculation
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
*            il1        Generator polynomial has Implicit leading 1. 
*                        Generally true, but provided because exceptions
*                        do exist (example: CRC-15/CAN).
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
*/
uint64_t GetRem(crc_t* crc, msg_t* msg, uint64_t check);
~~~

## Additional

