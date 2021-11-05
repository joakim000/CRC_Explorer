/*  Data generation lib

  
*/
#include "datagenerator.h"
#include "common.h"


uint64_t* GetU64random(size_t set_size, size_t maxnum, uint8_t* error ) {
    srand(time(0));
    uint64_t* random = calloc(set_size, sizeof(uint64_t));
        assert( ("Memory allocation failed.", random != NULL) );
    for (size_t i = 0; i < set_size; i++) 
            random[i] = (uint64_t)GENRAND(maxnum);
    return random;
}
uint8_t* GetU8random(size_t set_size, size_t maxnum, uint8_t* error ) {
    srand(time(0));
    uint8_t* random = calloc(set_size+1, sizeof(uint8_t));
        assert( ("Memory allocation failed.", random != NULL) );
    for (size_t i = 0; i < set_size; i++) 
            random[i] = (uint8_t)GENRAND(maxnum);

    for I2(set_size) if (random[i] == 0) random[i]++;
    random[set_size] = '\0';
    return random;
}

void* GetU64linear(size_t set_size, uint8_t element_size, size_t start, uint8_t* error ) {
    uint64_t* linear = calloc(set_size, sizeof(uint64_t));
        assert( ("Memory allocation failed.", linear != NULL) );
    for (size_t i = 0; i < set_size; i++) 
            linear[i] = i;
    return linear;
}

void*  getDataMixed(size_t set_size, uint8_t element_size, size_t maxNum, size_t run_length, uint8_t* error ) {
    
}

void* getDataLorem(size_t set_size, uint8_t* error) {

    size_t count = set_size / 0x8000;
    char* lorem = calloc(count*0x8000, sizeof(char));
        assert( ("Memory allocation failed.", lorem != NULL) );
    // size_t count = set_size / 0x8000;
    // for (size_t i = 0; i < set_size; i++)
    //     sprintf(lorem, LOREM2237);   

        char filename[] = "./lorem/lorem32k.txt";
        FILE* fp; 
        char buf[0x400];
        size_t elementsRead;
        size_t totalRead = 0;
        fp = fopen(filename, "r");
        if (fp != NULL) {
            // printf("Reading file %s  ...  ", ca.inFile);
            while ((elementsRead = fread(buf, 1, sizeof buf, fp)) > 0 && totalRead < 0x8000)
                strcat(lorem, buf);
            if (ferror(fp)) {
                PRINTERR("File read error, exiting.");
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            fclose(fp);
            strcat(lorem, "\0");
            printf("%d characters read.\n", strlen(lorem));
            
        }
        else 
            PRINTERR("File not found."); 
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


