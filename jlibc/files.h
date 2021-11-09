#include "common.h"


char* ReadTextFromFile(char filename[], size_t max_readlength, bool verbose, uint8_t* error);
int WriteTextToFile(char filename[], char text[], bool verbose, uint8_t* error);
