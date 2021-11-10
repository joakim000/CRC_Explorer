#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Magic numbers
#ifndef BITSINBYTE
#define BITSINBYTE 8
#endif
#ifndef NIBBLE
#define NIBBLE 4
#endif

#ifndef HEXCHARS
#define HEXCHARS "xX0123456789abcdefABCDEF"
#endif

// Utility
#ifndef COUNT_OF
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#endif
#ifndef PRINTERR
#define PRINTERR(x) fprintf(stderr, "\n\e[1;31m%s\e[m", x);
#endif
#ifndef EACH
#define EACH (size_t i = 0; i < size; i++)
#endif
#ifndef I2
#define I2(x) (int i = 0; i < x; i++) 
#endif
#ifndef CROPSTR
#define CROPSTR(x, y) char y[strlen((char*)x)+1]; strcpy(y,(char*)x);
#endif

#define XSTR(x) STR(x)
#define STR(x) #x



// Timing
#ifndef TIMING
#define TIMING(y, x) ((double)(x - y) / CLOCKS_PER_SEC)
#endif
// clock_t timer_start; clock_t timer_end; 

// Flags
#ifndef VERBOSE
#define VERBOSE       false
#endif
#ifndef DEBUG
#define DEBUG         false
#endif

#ifndef GENRAND
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define GENRAND(x) (((rand() << 16) | rand()) % x)    // Workaround Windows RAND_MAX being 16 bit (max value 32767)
#elif __linux__
#define GENRAND(x) (rand() % x) 
#else
// #error "Unsupported platform"    // Safety first
#define GENRAND(x) (rand() % x)     // Hope that it works
#endif
#endif

short AllocCheck(void* p);

// #ifndef CALLOCOREXIT
// #define CALLOCOREXIT(x, y, z) if(CallocWtCheck(x, y, z) != 0) exit(EXIT_FAILURE);
// #endif
short CallocWtCheck(void* p, size_t elements, size_t element_size);

