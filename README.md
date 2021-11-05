# CRC Explorer - **sail the seven seas of CRCs**


## Tour

Try these commands to get an overview.

* make
* crc zoo
* crc enc -m AB -s 17 -steps
* scripts/test_perf.sh
* crc help

## Raison d'être

This project was inspired by a school assignment where students were to implement a somewhat obscure CRC specification. It's obscurity was meant to hinder the reuse of example implementations on the Internet, but also meant the only means of checking one's work was some provided example pairs of input strings and their associated CRC values. Only problem - the example values were incorrect, being sourced from the educator's own broken implementation. 

A frustrating experience to be sure, but an excellent lesson in the importance of testing for correctness and validity of outputs.  

## Features

# By-hand validatable process

# Educational step-by-step output

# Zoo

# API for plug-in validation of other implemenations 

# Other features

## Step-by-step, oh baby

## External engine API

From engine.h

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


## Additional

