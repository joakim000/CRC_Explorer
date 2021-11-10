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
        PRINTERR("File not found.\n");
        if (error != NULL) *error = 1;
        free(fcontent);
        return NULL;
    } 
    else {  
        if (verbose) printf("Reading file %s  ...  ", filename);
        while ( (totalRead + buf_size < max_readlength) && (elementsRead = fread(buf, 1, sizeof buf, fp) ) > 0 ) {
            totalRead += elementsRead;
            strncat(fcontent, buf, elementsRead);
        }
        if (ferror(fp)) {
            PRINTERR("File read error.\n")
            if (error != NULL) *error = 2;
            fclose(fp);
            if (fcontent != NULL) free(fcontent);
            return NULL;
        }
        fclose(fp);
        strcat(fcontent, "\0");
        if (verbose) printf("%d characters read.\n", strlen(fcontent));
        if (error != NULL) *error = 0;
        return fcontent;
    }
}

int WriteTextToFile(char filename[], char text[], bool verbose, uint8_t* error) {
    if (filename != NULL) {
        FILE* fp; 
        fp = fopen((char*)filename, "w");
        if (fp == NULL) {
            if (error != NULL) *error = 1;
            return 1;
        }
        else {
            fprintf(fp, text);
            fclose(fp);
            return 0;
        }
    }
}

int MetaAndMsgFromText(char* text, char* meta_out, char* msg_out, char delims[2], size_t max_search_len) {
        char delim1 = delims[0];
        char delim2 = delims[1];
        char* token_meta;
        char* token_msg;

        // Sanity check before strtok
        char* start_bracket = memchr(text, delim1, 1);                                                  // First char should be delim1
        size_t len_after_start = start_bracket == NULL ? 0 : strlen(start_bracket);                     // Msg len after bracket, 
        uint8_t search_len = len_after_start > max_search_len ? max_search_len : len_after_start;       // to avoid searching out of bounds
        char* end_bracket = search_len > 0 ? memchr(start_bracket, delim2, search_len) : NULL;          // Find ] after [, within max_search_len or msg len if shorter

        if (end_bracket == NULL) 
            return 2; // Unable to find end bracket
        else {
        // We now know there is a [ and a ] with max max_search_len between them, can start at start_bracket 
            token_meta = strtok(start_bracket, "[]");
            if (token_meta == NULL) 
                return 3; // Unable to find meta
            else {
                if ( CallocWtCheck(meta_out, strlen(token_meta) + 1, sizeof(char)) != 0 ) exit(EXIT_FAILURE);
                printf("token_meta (%d):%s\n", strlen(token_meta), token_meta);
                puts("MAMFT before thing");
                strcpy(meta_out, token_meta);
                puts("MAMFT after thing");
                
                // Found meta, look for msg               
                token_msg = strtok(NULL, "");
                if (token_msg == NULL) 
                    return 4; // Unable to find msg
                else {
                    if ( CallocWtCheck(msg_out, strlen(token_msg) + 1, sizeof(char)) != 0 ) exit(EXIT_FAILURE);
                    strcpy(msg_out, token_msg);
                }
            }
        }
        return 0;
}
