#include <stdint.h>
#include <stdlib.h>


short AllocCheck(void* p) {
    if(p == NULL ) {
        #ifdef DEBUG
        if (DEBUG) fprintf(stderr, "Unable to allocate memory.\n");
        #endif
        return 1;
    }
    else 
        return 0;
}

short CallocWtCheck(void* p, size_t elements, size_t element_size) {
    p = calloc(elements, element_size);
    if(p == NULL ) {
        #ifdef DEBUG
        if (DEBUG) fprintf(stderr, "Unable to allocate memory.\n");
        #endif
        return 1;
    }
    else 
        return 0;
}

