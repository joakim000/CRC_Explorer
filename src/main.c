#include "crc.h"
#include "../lib/jlibc/files.h"
#include "../lib/jlibc/datagenerator.h"
#include "../lib/jlibc/common.h"
#include "../assets/zoo.h"

void main(int argc, char* argv[] )
{
    // Data structures
    crc_t* crc;  
    msg_t* msg;

    // Timing
    clock_t timer_start; clock_t timer_end; 

    // CRC data
    #ifndef WIDE_CRC
    crcdef_t zoo[] = { CRC_ZOO };
    #else
    crcdef_t zoo[] = { W_CRC_ZOO };
    #endif

    // Program settings and variables
    prog_t new_prog = {
        .verbose = VERBOSE,
        .printMsg = PRINTMSG,
        .printSteps = PRINTSTEPS,
        .selfTest = SELFTEST,
    };
    prog = &new_prog;    

    //* Command line arguments *//
    struct benchargs {
        bool zoo, enc, validate, impl_test, perf_test,              // Command
             printSteps, verbose, timing, prt_nogen, prt_noskip, use_internal_engine,   // Flags
             refIn, refOut;                  // Custom spec
        int  crc_spec, n, g, init, xor; // Custom spec
        char checksum[35];
        char msg[MAX_MESSAGE_ARGLENGTH];
        char inFile[FILENAME_MAX];
        char outFile[FILENAME_MAX];
    } ca;
    if (argc < 2 || checkArg(argc, argv, "help")) {
        // Print help if no args or help command
        printf(HELPTEXT1, EXPLORER_VERSION);
        exit(EXIT_SUCCESS);
    }
    argdef_t defs[] = {
        // Commands
        { .isFlag = true, .var = (bool*)&ca.zoo, .str = "zoo" },                     // zoo
        { .isFlag = true, .var = (bool*)&ca.enc, .str = "enc" },                     // encode
        { .isFlag = true, .var = (bool*)&ca.validate, .str = "val" },                // validate
        { .isFlag = true, .var = (bool*)&ca.impl_test, .str = "imp" },               // 3-step validation of implementation
        { .isFlag = true, .var = (bool*)&ca.perf_test, .str = "perf" },              // Performance of implementation

        // Input
        { .isInt = true,  .var = (int*)&ca.crc_spec, .str = "-c", .defaultInt = 0 },          // CRC spec index
        { .isString = true, .var = (char*)&ca.msg, .str = "-m", .defaultString = "" },          // message
        { .isString = true,  .var = (char*)&ca.checksum, .str = "-s", .defaultString = "" },    // checksum for validation
        { .isString = true, .var = (char*)&ca.inFile, .str = "-in", .defaultString = "" },    // input file
        { .isString = true, .var = (char*)&ca.outFile, .str = "-out", .defaultString = "" }, // output file
        // Flags
        { .isFlag = true, .var = (bool*)&ca.printSteps, .str = "-steps" },                 // print steps
        { .isFlag = true, .var = (bool*)&ca.prt_nogen, .str = "-nogen" },                 // Don't print generator
        { .isFlag = true, .var = (bool*)&ca.prt_noskip, .str = "-noskip" },              // Do print skipped steps
        { .isFlag = true, .var = (bool*)&ca.verbose, .str = "-v" },                     // verbose
        { .isFlag = true, .var = (bool*)&ca.timing, .str = "-t" },                     // timing
        { .isFlag = true, .var = (bool*)&ca.use_internal_engine, .str = "-i" },       // Use internal engine even with external present

        // Custom spec
        { .isInt = true,  .var = (int*)&ca.n, .str = "-n", .defaultInt = 8 },       // bit width
        { .isInt = true,  .var = (int*)&ca.g, .str = "-g", .defaultInt = 0x07 },    // generator polynomial
        { .isInt = true,  .var = (int*)&ca.init, .str = "-init", .defaultInt = 0 }, // init seed
        { .isInt = true,  .var = (int*)&ca.xor, .str = "-x", .defaultInt = 0 },     // final xor
        { .isFlag = true, .var = (bool*)&ca.refIn, .str = "-ri" },                  // inRef
        { .isFlag = true, .var = (bool*)&ca.refOut, .str = "-ro" },                 // outRef

    };
    processArgs(argv, argc, defs, COUNT_OF(defs));
    // Set flags
    PROG.printSteps = ca.printSteps ? true : false;
    PROG.prt_nogen =  ca.prt_nogen  ? true : false;
    PROG.prt_noskip = ca.prt_noskip ? true : false;
    PROG.verbose =    ca.verbose    ? true : false;
    PROG.timing =     ca.timing     ? true : false;
    
    PROG.internal_engine = (!EXTERNAL_ENGINE_AVAILABLE || ca.use_internal_engine) ? true : false;
    GetRem_ptr = PROG.internal_engine ? GetRemInternal : EXPLORER_ENGINE;
    if (PROG.internal_engine) PROG.engine_id = "Internal"; 
    else PROG.engine_id = XSTR(EXPLORER_ENGINE);

    // Check for a known command
    if (!ca.zoo && !ca.enc && !ca.validate && !ca.impl_test && !ca.perf_test) {
        printf("Available commands:\n\tzoo\tWhere all the CRCs live\n\tenc\tEncode a message\n"
               "\tval\tValidate a message\n\timp\tImplementation test\n\tperf\tPerformance test\n\thelp\tMore help\n", NULL);
        exit(EXIT_SUCCESS);
    }
    //* end Command line arguments  *//

    // Commmand: Show CRC inventory
    if (ca.zoo) {
        ZooTour(zoo, COUNT_OF(zoo));
        exit(EXIT_SUCCESS);
    }

    // Commands below this point require a CRC.
      // Load CRC definition 
        crc_t enc_crc;
        crc = &enc_crc;
        LoadDefWrapper(zoo, ca.crc_spec, &enc_crc, false); 
    
    // Commmand: Test implementation
    if (ca.impl_test) {
        ImplValid(crc);

        if (!ca.perf_test)  // The two tests may be run within one execution    
            exit(EXIT_SUCCESS);
    }
    // Commmand: Test performance
    if (ca.perf_test) {
        // ImplPerf(crc, 0x40);
        // ImplPerf(crc, 0x1000);
        // ImplPerf(crc, 0x10000);
        ImplPerf(crc, 0x100000);
        ImplPerf(crc, 0x1000000);
        // ImplPerf(crc, 0x4000000);

        exit(EXIT_SUCCESS);
    }

    // Commands below this point require a message. Try to find one, else exit.
    char* message = NULL;
    // In command line?
    if (strlen((char*)ca.msg) > 0 ) 
        message = ca.msg;
    // Else in file?
    else if (strlen(ca.inFile) > 0) {
        char* fcontent = ReadTextFromFile(ca.inFile, MAX_MESSAGE_READLENGTH, true, NULL);
        if (fcontent != NULL) {
            message = fcontent;
            fcontent = NULL;
        }
    }
    // Still no message?
    if (message == NULL) {
        PRINTERR("No message, exiting.\n");
        exit(EXIT_FAILURE);
    }


    // Command: Encode
    if (ca.enc) {      
        // Prepare message 
        msg = PrepareMsg(crc, message);
        #ifdef WIDE_CRC
        msg->w_validation_rem = NULL;    // For experimental wide crc support
        #endif
       
        // Expected checksum value for testing checksum calculation. Essentially a custom check value.
        // If the message is "AB" an expected value is set matching the current CRC spec. 
        // Or set a custom value.  Check is skipped when set to 0. 
        msg->expected = strcmp(msg->msgStr, "AB") ? 0 : crc->checkAB; 
        // msg->expected = 

        // Calculate remainder with engine pointed to, also start and stop timer
        timer_start = clock();
            msg->rem = GetRem_ptr(crc, msg, 0);
        timer_end = clock();

        // Printing 
        if (PRINTMSG) {
            if (msg->len < PRINTLIMIT)
                printf("Message:\t|%s|\n", msg->msgStr);
            else
                printf("Message:\t[%llu characters]\n", msg->len);
        }
        if (crc->n <= 64)
            printf("Checksum:\t%#llx\n", msg->rem);
        #ifdef WIDE_CRC
        else
            printf("Checksum:\t%s\n", msg->w_rem);
        #endif
        double elapsed = TIMING(timer_start, timer_end);
        if (ca.timing) printf("%d chars in %5.3f seconds, %5.3f MiB/s.\n", msg->len, elapsed, msg->len / elapsed / 0x100000);

        // Compare result with a expected value
        CustomValueCheck(crc, msg);
        
        // Open file for writing result      
        char csStr[100];
        sprintf(csStr, "[%#llx %s]", msg->rem, crc->description); 
        char outStr[strlen(msg->msgStr) + strlen(csStr)];
        sprintf(outStr, "%s%s", csStr, msg->msgStr); 
        // puts(outStr);
        if (strlen(ca.outFile) > 0 ) {
            if ( WriteTextToFile(ca.outFile, outStr, true, NULL) == 1 ) 
                PRINTERR("File write error.");
        }

        // Free allocations
        if (msg->msgStr != (char*)ca.msg)
            if (msg->msgStr != NULL) free(msg->msgStr);
        if (msg->msgBits != NULL) free(msg->msgBits);
        if (msg != NULL) free(msg);
        exit(EXIT_SUCCESS);
    }


    // Command: Validate
    if (ca.validate) {     
        // Check for available checksum:
        char* checksumWork = "";   
        // char* checksumGet;   
        // char* checksumWork;   

        //* In message? *//
        // int8_t parse_error = MetaAndMsgFromText(message, checksumWork, message, "[]", MAX_HEXSTRING_LEN + 0x40);
        // printf("Parse result:%d\n", parse_error);
        
        char* token;
        // Sanity check before strtok
        size_t max_search_len = MAX_HEXSTRING_LEN + 0x40;                                               // Allow some space for a note  
        char* start_bracket = memchr(message, '[', 1);                                                  // First char should be [
        size_t len_after_start = start_bracket == NULL ? 0 : strlen(start_bracket);                     // Msg len after bracket, 
        uint8_t search_len = len_after_start > max_search_len ? max_search_len : len_after_start;       // to avoid searching out of bounds
        char* end_bracket = search_len > 0 ? memchr(start_bracket, ']', search_len) : NULL;             // Find ] after [, within max_search_len or msg len if shorter

        if (end_bracket != NULL) {
        // We now know there is a [ and a ] with max max_search_len between them, can start at start_bracket 
            token = strtok(start_bracket, "[]");
            if (token != NULL) {
                checksumWork = token;
                token = strtok(NULL, "");
                if (token != NULL) {
                    message = token;
                }
            }
        }

        if (PROG.verbose) 
        ("checksumWork (%d):%s\n", strlen(checksumWork), checksumWork);

        // If not in message then in command line?
        if (strlen(checksumWork) < 1) {
            if (strlen(ca.checksum) > 0) {
                checksumWork = ca.checksum;
            }
            else {
                PRINTERR("No checksum for validation, exiting.\n");   
                exit(EXIT_FAILURE);
            }
        }

        // Handle checksum string 
        char* checksumNote;
        uint64_t checksum = strtoull(checksumWork, &checksumNote, 16);
        char checksumStr[MAX_HEXSTRING_LEN];
        size_t checksum_len = strlen(checksumWork) - strlen(checksumNote);
        strncpy(checksumStr, checksumWork, checksum_len > MAX_HEXSTRING_LEN ? MAX_HEXSTRING_LEN : checksum_len);

        // Prepare message 
        msg = PrepareMsg(crc, message);
        #ifdef WIDE_CRC
        msg->w_validation_rem = checksumStr;    // For experimental wide crc support
        #endif

         // Calculate remainder with engine pointed to, also start and stop timer
        timer_start = clock();
            // #ifndef WIDE_CRC
            msg->rem = GetRem_ptr(crc, msg, checksum);
            // #else
            // msg->rem = GetRem_ptr(crc, msg, checksumStr);
            // #endif
        timer_end = clock();

        // Validate the messsage
        msg->valid = Validate(crc, msg);
       
        // Printing 
        printf("Checksum:\t\t%s\t%s\n", checksumStr, checksumNote);
        double elapsed = TIMING(timer_start, timer_end);
        if (ca.timing) printf("%d chars in %5.3f seconds, %5.3f MiB/s.\n", msg->len, elapsed, msg->len / elapsed / 0x100000);
        ValidPrint(msg->msgStr, msg->len, msg->valid);

        // Free allocations
        if (msg->msgBits != NULL) free(msg->msgBits);
        if (msg != NULL) free(msg);    
        exit(EXIT_SUCCESS);
    }

}
