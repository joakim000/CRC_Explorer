#include "cmdargs.h"

int checkArg(int argc, char *argv[], char arg[]) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], arg))
            { 
                // printf("\n%s found at index %d\n", argv[i], i); 
                return i;
            }
    }
    return 0;
}

int processArgs(char* argv[], int argc, argdef_t defs[], int defcount) {
    if (argc >= 2 || PROCESS_EMPTY) {
        for (int i = 0; i < defcount; i++) {            
            if (defs[i].isFlag) {
                bool* p = (bool*)defs[i].var;
                *p = checkArg(argc, argv, defs[i].str) ? true : false;               
            } 
            if (defs[i].isInt) {
                int* p = (int*)defs[i].var;
                int index = checkArg(argc, argv, defs[i].str);
                if (argv[index + 1] == NULL) 
                    *p = defs[i].defaultInt;
                else
                    *p = index ? strtol(argv[index + 1], NULL, 0) : defs[i].defaultInt; //TODO errorchecking. Sets 0 on error now which is sort of ok.
            } 
            if (defs[i].isFloat) {
                double* p = (double*)defs[i].var;
                int index = checkArg(argc, argv, defs[i].str);
                if (argv[index + 1] == NULL) 
                    *p = defs[i].defaultFloat;
                else
                *p = index ? strtod(argv[index + 1], NULL) : defs[i].defaultFloat; //TODO errorchecking. Sets 0 on error now which is sort of ok.
            } 
            if (defs[i].isString) {
                char* p = (char*)defs[i].var;
                int index = checkArg(argc, argv, defs[i].str);
                if (argv[index + 1] == NULL) 
                    strcpy(p, defs[i].defaultString);    
                else
                    strcpy(p, index ? argv[index + 1] : defs[i].defaultString);
            }
        }
        return 0;
    }
    else
        return 1; // No arguments
}