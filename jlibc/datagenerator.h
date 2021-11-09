#include "common.h"

#ifndef DATAGENERATOR_DEBUG
#define DATAGENERATOR_DEBUG false
#endif


// API functions //
// Get data
/**
  @brief 
  @return
*/
uint64_t* GetRandomU64(size_t set_size, size_t maxnum, uint8_t* error );
/**
  @brief 
  @return
*/
uint8_t* GetRandomU8(size_t set_size, size_t maxnum, uint8_t* error );

void* GetLinearU64(size_t set_size, uint8_t element_size, size_t start, uint8_t* error );

char* GetRandomPrintable(size_t set_size, uint8_t* error );

int32_t* GetRandomI32(size_t set_size, size_t maxnum, uint8_t* error );

/**
  @brief 
  @return
*/
void*  GetDataMixed(size_t set_size, uint8_t element_size, size_t maxNum, size_t run_length, uint8_t* error );
/**
  @brief 
  @return
*/
char*  GetDataLorem(size_t set_size, uint8_t* error );

// Save data
/**
  @brief 
  @return
*/
void*    writeLorem(size_t set_size, char filename[FILENAME_MAX], uint8_t* error );

// Validation
/**
  @brief 
  @return
*/
size_t compare_array(uint32_t *num, uint32_t size, uint32_t *comp, uint8_t* error);



// Printing
/**
  @brief 
  @return
*/
void print_array(uint32_t *num, uint32_t size, uint32_t tmax, uint32_t rnd_max);
/**
  @brief 
  @return
*/
void compare_print_array(uint32_t *num, uint32_t size, uint32_t *comp, uint32_t tmax, uint32_t rnd_max);



// Private functions //
/**
  @brief 
  @return
*/
static void copy_array(uint32_t *num, uint32_t size, uint32_t *out, bool prtin, uint32_t tmax, uint32_t maxNum);
/**
  @brief 
  @return
*/
static void generate_array(uint32_t *num, uint32_t size, uint32_t run_len, uint32_t rnd_max);
/**
  @brief 
  @return
*/
int32_t utils_rand(int32_t min, int32_t max);




// Timing
/**
  @brief 
  @return
*/
double timeComp(double libTime, double sortTime);



#define LOREM2237 "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec consequat neque et mi euismod, ac fringilla nisi vestibulum. Ut facilisis leo a lacus mattis ornare. Pellentesque nec nibh quis ipsum porta efficitur a et orci. Proin et odio a ante porttitor fringilla ac ac elit. Duis sagittis porttitor commodo. Integer sed felis at turpis lobortis laoreet. Suspendisse blandit velit rhoncus tempor dapibus. Vivamus tempus sollicitudin faucibus. Sed sem lacus, commodo et varius vitae, laoreet vitae ante. Nam molestie sapien ligula, ut aliquam mi dictum sed. Donec magna risus, ultrices eget ullamcorper eu, suscipit in diam. Aenean sollicitudin sem ac lobortis scelerisque. Quisque fermentum luctus hendrerit.\n\
\n\
Sed rutrum lectus et orci suscipit, vitae pharetra nibh cursus. Mauris ex justo, tempor vel elit quis, condimentum dictum nunc. Nullam sed leo ac felis semper fermentum non id lectus. Maecenas non ipsum semper, rutrum tortor eu, pulvinar lectus. Nullam vehicula ligula et posuere vehicula. Ut consequat molestie enim, et lobortis mauris finibus in. Aliquam odio nisl, mollis nec elit vitae, rhoncus mollis ligula. Sed rhoncus magna in lorem consectetur rutrum.\n\
\n\
Vivamus et porta nisl, quis vestibulum sem. Cras est felis, mollis id dolor sed, lobortis pellentesque eros. Morbi accumsan est eu egestas mattis. Cras sed rhoncus augue. Cras sed risus mauris. Suspendisse imperdiet sapien eu erat rutrum feugiat. Etiam id elementum lacus.\n\
\n\
Maecenas consectetur ex in risus hendrerit porta ac sed ex. Curabitur suscipit commodo felis, sit amet malesuada dui dapibus at. Nulla facilisi. Duis sit amet sapien volutpat, aliquam leo in, semper velit. Sed eget lobortis arcu. In et ornare purus, quis auctor leo. Vivamus ac mi velit. Nam convallis enim nisl, sit amet viverra tellus tincidunt sed. Ut ut tempus ante.\n\
\n\
Nullam placerat, velit vitae consequat convallis, lacus nibh ultrices ligula, a scelerisque magna erat vitae massa. Mauris fermentum dui a tellus varius, rutrum varius massa posuere. Interdum et malesuada fames ac ante ipsum primis in faucibus. Mauris hendrerit eget libero id lacinia. Curabitur in magna sollicitudin, mattis lacus et, euismod ante. Morbi fringilla nunc sit amet erat eleifend aliquet. Cras vel commodo dolor.\n\n"


