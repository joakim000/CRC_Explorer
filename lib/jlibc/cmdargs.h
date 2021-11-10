/**
 @brief 
 Process command line args into variables
 
 @example
 ** Define command line arguments **
    argdef_t argdefs[] = {
        {
            .isFlag = true,
            .var = &aFlag1,
            .str = "-flag1" 
            // Default: false
        },
        {
            .isInt = true,
            .var = &anInt1,
            .str = "-int1",
            .defaultInt = 101   // Optional, default default: 0
        },
        {
            .isString = true,
            .var = &aString1,
            .str = "-as1"
            .defaultString = "default string"  // Optional, default default: ""
        }
    }

 ** Set variables from args **
    processArgs(argv, argc, defs, COUNT_OF(defs));

 ** Example command line **
    out -flag1 -int1 100 -as1 "a string"

*/

// #include <stdint.h>
// #include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Process with no cmdline arguments, useful for setting defaults.
// If set to false; returns error code 1 on no arguments.
#define PROCESS_EMPTY true

// Max string lengths
#define MAXLEN_FLAG 32
#define MAXLEN_DEFAULTSTRING 1024

// Utility
#ifndef COUNT_OF
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#endif

// Definition of a command line argument
typedef struct argdef_struct {
    char str[MAXLEN_FLAG];
    bool isFlag, isInt, isFloat, isString;
    void* var;
    
    int defaultInt;
    double defaultFloat;
    char defaultString[MAXLEN_DEFAULTSTRING];
} argdef_t;

int processArgs(char* argv[], int argc, argdef_t defs[], int defcount);

int checkArg(int argc, char* argv[], char arg[]);  // Available for separate use


