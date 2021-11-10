/*  Data generation lib

  
*/
#include "datagenerator.h"
#include "common.h"
#include "files.h"

uint64_t* GetRandomU64(size_t set_size, size_t maxnum, uint8_t* error ) {
    srand(time(0));
    uint64_t* random = calloc(set_size, sizeof(uint64_t));
        assert( ("Memory allocation failed.", random != NULL) );
    for (size_t i = 0; i < set_size; i++) 
            random[i] = (uint64_t)GENRAND(maxnum);
            // random[i] = utils_rand(0, UINT64_MAX);
    return random;
}

int32_t* GetRandomI32(size_t set_size, size_t maxnum, uint8_t* error ) {
    srand(time(0));
    uint32_t* random = calloc(set_size, sizeof(uint64_t));
        assert( ("Memory allocation failed.", random != NULL) );
    for (size_t i = 0; i < set_size; i++) 
            // random[i] = (uint64_t)GENRAND(maxnum);
            random[i] = utils_rand(INT32_MIN, INT32_MAX);
    return random;
}


uint8_t* GetRandomU8(size_t set_size, size_t maxnum, uint8_t* error ) {
    srand(time(0));
    uint8_t* random = calloc(set_size+1, sizeof(uint8_t));
        assert( ("Memory allocation failed.", random != NULL) );
    for (size_t i = 0; i < set_size; i++) 
            random[i] = utils_rand(0, 255);
    random[set_size] = '\0';
    return random;
}
char* GetRandomPrintable(size_t set_size, uint8_t* error ) {
    srand(time(0));
    uint8_t* random = calloc(set_size+1, sizeof(uint8_t));
        assert( ("Memory allocation failed.", random != NULL) );
    for (size_t i = 0; i < set_size; i++) 
            random[i] = utils_rand(32, 126);
    random[set_size] = '\0';
    return random;
}

void* GetLinearU64(size_t set_size, uint8_t element_size, size_t start, uint8_t* error ) {
    uint64_t* linear = calloc(set_size, sizeof(uint64_t));
        assert( ("Memory allocation failed.", linear != NULL) );
    for (size_t i = 0; i < set_size; i++) 
            linear[i] = i;
    return linear;
}

void*  GetDataMixed(size_t set_size, uint8_t element_size, size_t maxNum, size_t run_length, uint8_t* error ) {
    
}

char* GetDataLorem(size_t set_size, uint8_t* error) {

    size_t count = set_size / 0x8000;
    char* lorem = calloc(count*0x8000, sizeof(char));
        assert( ("Memory allocation failed.", lorem != NULL) );
    // size_t count = set_size / 0x8000;
    // for (size_t i = 0; i < set_size; i++)
    //     sprintf(lorem, LOREM2237);   

    char filename[] = "./assets/lorem32k.asc";
    uint8_t file_error;
    ReadTextFromFile(filename, 0x10000, true, &file_error);
    if (file_error < 0) 
        PRINTERR("\nFile read error.\n");
}


void*    writeLorem(size_t set_size, char filename[FILENAME_MAX], uint8_t* error) {
    
}


    //         uint32_t size,  // set size 
    //             compo,      // set composition
    //             maxNum;     // set max number
            
    //         uint32_t tmax,  // max table size for printing
    //             maxThreads; // max threads for threaded sorts
  
    // // Alloc arrays for input, working and testing
    // uint32_t* random = calloc(size, sizeof(uint32_t));
    //     assert( ("Memory allocation failed.", random != NULL) );

    // Generate input
    // srand(time(0));
    // generate_array(random, size, compo, maxNum);
    // print_array(random, size);

    // Print some info
    // if (DATAGENERATOR_DEBUG)             
    //     printf("\nSet size: %d    Biggest: %d    Composition: %d", size, maxNum, compo);

    
            // copy_array(random, ba.size, numbers, ba.prtin, ba.tmax, ba.maxNum);
            // timer_start = clock();
            //     sorts[i].sort_ptr((void *)&args);
            // timer_end = clock();
            // // Process timing
            // sortTime = TIMING(timer_start, timer_end);
            // elem_over_ns = sortTime > 0 ? ba.size / sortTime / 1000000 : 0;
            // errorCount = compare_array(numbers, ba.size, compare);
            
            // Printing
                // print_array(numbers, ba.size, ba.tmax, ba.maxNum);
                // compare_print_array(numbers, ba.size, compare, ba.tmax, ba.maxNum);
                // printf("%d elements in %5.3f seconds, %5.3f ns/element. %5.3f %% of libsort performance. \n", ba.size, sortTime, elem_over_ns, timeComp(libTime, sortTime));
    
    

/* Helper functions */
size_t compare_array(uint32_t *num, uint32_t size, uint32_t *comp, uint8_t* error)
{
    uint32_t errors = 0;
    for (size_t i = 0; i < size; i++)
        if (num[i] != comp[i])
            errors++;
    return errors;
}

void compare_print_array(uint32_t *num, uint32_t size, uint32_t *comp, uint32_t tmax, uint32_t maxNum)
{
    // int orders = (int)log10(ba.maxNum) + 1;  // Build chain troubles with <math.h>
    int orders = 10;
    int space = 2;
    char fmt[10], errfmt[10];
    sprintf(fmt, "%%%dd", orders+space);
    sprintf(errfmt, "\e[1;31m%%%dd\e[m", orders+space); // red text
    int errors = 0;

    for (size_t i = 0; i < size && i < tmax; i++)
    {
        if (num[i] == comp[i])
            printf(fmt, num[i]);
        else {
            printf(errfmt, num[i]);
            errors++;
        }
        if (i != 0 && (i + 1) % 10 == 0 && i + 1 != size)
            printf("\n");
    }
    printf("\n\t%d errors printed.\n", errors);
}

void copy_array(uint32_t *num, uint32_t size, uint32_t *out, bool prtin, uint32_t tmax, uint32_t maxNum) {
    for (size_t i = 0; i < size; i++) 
        out[i] = num[i];
    if (prtin) printf("\nInput array:\n");
    if (prtin) print_array(out, size, tmax, maxNum); 
}

void generateU64array(uint64_t *num, size_t set_size, uint8_t element_size, size_t compo, uint64_t maxNum)
{
    if (compo <= 0)    
    // Linear array
        for (size_t i = 0; i < set_size; i++) 
            num[i] = i;

    else if (compo == 1)    
    // Random array
        for (size_t i = 0; i < set_size; i++) 
            num[i] = (uint64_t)GENRAND(maxNum);
    else {  
    // Mixed array
        int runSize = (compo % 2 == 0) ? compo + 1 : compo;  // Must be odd number

        for (size_t i = 0; i < set_size; ) 
            if (i % 2) {
                size_t runStart = num[i - 1];
                for (size_t runEnd = i + runSize; i < runEnd && i <  set_size; i++)
                    num[i] = runStart + i;
            }
            else 
                for (size_t runEnd = i + runSize; i < runEnd && i <  set_size; i++) 
                    num[i] = (uint64_t)GENRAND(maxNum);
    }
}

void generateU8array(uint8_t *num, size_t set_size, uint8_t element_size, size_t compo, uint64_t maxNum)
{
    if (compo <= 0)    
    // Linear array
        for (size_t i = 0; i <  set_size; i++) 
            num[i] = i;

    else if (compo == 1)    
    // Random array
        for (size_t i = 0; i < set_size; i++) 
            num[i] = (uint8_t)GENRAND(maxNum);
    else {  
    // Mixed array
        int runSize = (compo % 2 == 0) ? compo + 1 : compo;  // Must be odd number

        for (size_t i = 0; i < set_size; ) 
            if (i % 2) {
                size_t runStart = num[i - 1];
                for (size_t runEnd = i + runSize; i < runEnd && i <  set_size; i++)
                    num[i] = runStart + i;
            }
            else 
                for (size_t runEnd = i + runSize; i < runEnd && i <  set_size; i++) 
                    num[i] = (uint8_t)GENRAND(maxNum);
    }
}

// void generate_random_array(uint64_t *num, size_t size, uint64_t maxNum)
// {
//     for (size_t i = 0; i < size; i++) 
//         num[i] = (uint64_t)GENRAND(maxNum);
// }

void print_array(uint32_t *num, uint32_t size, uint32_t tmax, uint32_t maxNum)
{
    // int orders = (int)log10(maxNum) + 1;     // Build chain troubles with <math.h>
    int orders = 10;
    int space = 2;
    char fmt[10], errfmt[10];
    sprintf(fmt, "%%%dd", orders+space);  

    for (size_t i = 0; i < size && i < tmax; i++)
    {
        printf(fmt, num[i]);
        if (i != 0 && (i + 1) % 10 == 0 && i + 1 != size)
            printf("\n");
    }
    printf("\n");
}

double timeComp(double libTime, double sortTime) {
    if (!(libTime > 0))
        if (sortTime > 0)
            return 0;
        else
            return 100;
    else
        return libTime / sortTime * 100;
}


// #include <assert.h>
// #include <stdbool.h>
// #include <stdlib.h>
// https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c/67746081#67746081
/// \brief      Use linear interpolation to rescale, or "map" value `val` from range
///             `in_min` to `in_max`, inclusive, to range `out_min` to `out_max`, inclusive.
/// \details    Similar to Arduino's ingenious `map()` function:
///             https://www.arduino.cc/reference/en/language/functions/math/map/
///
/// TODO(gabriel): turn this into a gcc statement expression instead to prevent the potential for
/// the "double evaluation" bug. See `MIN()` and `MAX()` above.
#define UTILS_MAP(val, in_min, in_max, out_min, out_max) \
    (((val) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min))

/// \brief      Obtain a pseudo-random integer value between `min` and `max`, **inclusive**.
/// \details    1. If `(max - min + 1) > RAND_MAX`, then the range of values returned will be
///             **scaled** to the range `max - min + 1`, and centered over the center of the
///             range at `(min + max)/2`. Scaling the numbers means that in the case of scaling,
///             not all numbers can even be reached. However, you will still be assured to have
///             a random distribution of numbers across the full range.
///             2. Also, the first time per program run that you call this function, it will
///             automatically seed the pseudo-random number generator with your system's
///             current time in seconds.
/// \param[in]  min         The minimum pseudo-random number you'd like, inclusive. Can be positive
///                         OR negative.
/// \param[in]  max         The maximum pseudo-random number you'd like, inclusive. Can be positive
///                         OR negative.
/// \return     A pseudo-random integer value between `min` and `max`, **inclusive**.
/// @example    const int MIN = 1;
///             const int MAX = 1024;
///             int random_num = utils_rand(MIN, MAX);

int32_t utils_rand(int32_t min, int32_t max)
{
    static bool first_run = true;
    if (first_run)
    {
        // seed the pseudo-random number generator with the seconds time the very first run
        time_t time_now_sec = time(NULL);
        srand(time_now_sec);
        first_run = false;
    }

    int32_t range = max - min + 1;
    int32_t random_num = rand();  // random num from 0 to RAND_MAX, inclusive

    if (range > RAND_MAX)
    {
        static_assert(
            sizeof(int64_t) > sizeof(int32_t),
            "This must be true or else the below mapping/scaling may have undefined overflow "
            "and not work properly. In such a case, try casting to `long int64_t` instead of "
            "just `int64_t`, and update this static_assert accordingly.");

        random_num = UTILS_MAP((int64_t)random_num, (int64_t)0, (int64_t)RAND_MAX, (int64_t)min,
                               (int64_t)max);
        return random_num;
    }

    // This is presumably a faster approach than the map/scaling function above, so do this faster
    // approach below whenever you don't **have** to do the more-complicated approach above.
    random_num %= range;
    random_num += min;

    return random_num;
}
