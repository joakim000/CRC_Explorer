#include "common.h"


char* ReadTextFromFile(char filename[], size_t max_readlength, bool verbose, uint8_t* error){
    char* fcontent = calloc(max_readlength, sizeof(char));
        assert( ("Memory allocation failed.", fcontent != NULL) );
    FILE* fp;
    size_t buf_size = 0x400; 
    char buf[buf_size];
    size_t elementsRead;    
    size_t totalRead = 0;

    fp = fopen((char*)filename, "r");
    if (fp == NULL) {
        PRINTERR("File not found.");
        if (error != NULL) *error = 1;
        free(fcontent);
        return NULL;
    } 
    else {  
        if (verbose) printf("Reading file %s  ...  ", filename);
        while ( (totalRead + buf_size < max_readlength) && (elementsRead = fread(buf, 1, sizeof buf, fp) ) > 0 )
            totalRead += elementsRead;
            strcat(fcontent, buf);
        if (ferror(fp)) {
            PRINTERR("File read error.")
            if (error != NULL) *error = 2;
            fclose(fp);
            free(fcontent);
            return NULL;
        }
        fclose(fp);
        strcat(fcontent, "\0");
        if (verbose) printf("%d characters read.\n", strlen(fcontent));
        if (error != NULL) *error = 0;
        return fcontent;
    }
}