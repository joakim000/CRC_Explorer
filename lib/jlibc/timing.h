#include <time.h>

// Utility
#ifndef COUNT_OF
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#endif

// Timing
#define TIMING(y, x) ((double)(x - y) / CLOCKS_PER_SEC)
clock_t timer_start; 
clock_t timer_end; 

                // printf("%d errors in %d elements.\n", errorCount, ba.size);
                // printf("%d elements in %5.3f seconds, %5.3f ns/element. %5.3f %% of libsort performance. \n", ba.size, sortTime, elem_over_ns, timeComp(libTime, sortTime));

