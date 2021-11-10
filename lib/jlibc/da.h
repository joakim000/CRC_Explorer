/*  
    Dynamic array implementation
    with optional sparse arrays

    210916 Joakim O

    da_compo_test.c     Ocular component tests
    da_perf_test.c      Performance tests

    Errors
    0 OK
    1 Memory allocation
    2 Index out of range
*/

// Doxygen template
/**
  @brief
  @return exit code 
*/


#include <stdint.h>

#define DA_DEBUG false

#ifndef DA_TYPE
#define DA_TYPE uint8_t
#endif

typedef struct DynArr {
   DA_TYPE *p;
   short error;
   long elements;
   long slots;
   float growthFactor;
   unsigned short compactFreq;
   
   // Sparse
   bool *vacant;  
   long vacantTotal;
   long vacantFirst;
   long vacantLast;

   unsigned short* lookup;
   bool lookupCurrent;

} da;

/**
  @brief Init array
  @return struct
  @param initAllocation Initial allocation
  @note Check da.error
*/
da daInit(int initAllocation);  

/**
  @brief Set growth factor
  @return exit code
  @param initAllocation Initial allocation
  @param growthFactor   2 means double, default is 1.5
*/
int daGrowth(float growthFactor);

/**
  @brief Clear allocations
  @return exit code 
*/
int daFree(da* a);

/**
  @brief Copy array (destructive)
  @return exit code 
    // Args: da* a, datatype values[], int len
*/
int daCopy(da* a, DA_TYPE values[], int len);

// Insert or add (index -1)
// Args: da* a, int index, DA_TYPE value
/**
  @brief
  @return exit code 
*/  
int daAdd(da* a, DA_TYPE value);  

// Insert or add (index -1)
// Args: da* a, int index, DA_TYPE value
/**
  @brief
  @return exit code 
*/int daIns(da* a, int index, DA_TYPE value);  

// Remove element or range
// Args: da* a, int startIndex, int endIndex
/**
  @brief
  @return exit code 
*/
int daRem(da* a, int startIndex, int endIndex);

/**
  @brief
  @return exit code 
*/
int daSparseRem(da* a, int startIndex, int endIndex);

// Get value
// Args: da* a, int index
// Return: value
// Args: da* a, int index, datatype &target
/**
  @brief
  @return exit code 
*/
DA_TYPE daGet(da* a, int index);

/**
  @brief
  @return exit code 
*/
DA_TYPE daSparseGet(da* a, int index);

// Set value
// Args: da* a, int index, datatype value 
/**
  @brief
  @return exit code 
*/

int daSet(da* a, int index, DA_TYPE value);

/**
  @brief
  @return exit code 
*/
int daSparseSet(da* a, int index, DA_TYPE value);

// Compact sparse allocation
// Args: da* a
/**
  @brief
  @return exit code 
*/
int daCompact(da* a);

/**
  @brief Return element count
  @return element count 
*/
int daCount(da* a);

/**
  @brief Return allocated slots
  @return allocated slots 
*/
int daAlloc(da* a);

/* Helpers - for internal use */
/**
  @brief
  @return exit code 
*/
static int daVacs(da* a, int index);                   // Vacancy counter

/**
  @brief
  @return exit code 
*/
static int daRealloc(da* a, int extraSlots);           // Realloc routine

/**
  @brief
  @return exit code 
*/
static short allocCheck(void *p);





