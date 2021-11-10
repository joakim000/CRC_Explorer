#include "common.h"


char* ReadTextFromFile(char filename[], size_t max_readlength, bool verbose, uint8_t* error);
int WriteTextToFile(char filename[], char text[], bool verbose, uint8_t* error);
int MetaAndMsgFromText(char* text, char* meta_out, char* msg_out, char delims[2], size_t max_search_len);
