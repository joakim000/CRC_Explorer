#include "crc.h"
#include "jlibc/files.h"
#include "jlibc/datagenerator.h"
#include "jlibc/common.h"


void main(int argc, char* argv[] )
{
    // Data structures
    crc_t* crc;  
    msg_t* msg;

    //Timing
    clock_t timer_start; clock_t timer_end; 

    /* Setup */
    #include "crc_zoo.c" // Unconventional use of #include, just a convenient way to put the CRC definitions in a separate file.
    // Program settings and variables
    prog_t new_prog = {
        .verbose = VERBOSE,
        .printMsg = PRINTMSG,
        .printSteps = PRINTSTEPS,
        .selfTest = SELFTEST,
    };
    prog = &new_prog;    
    // Command line arguments
    struct benchargs {
        bool zoo, enc, validate, impl_test, perf_test,              // Command
             printSteps, verbose, timing, prt_nogen, prt_noskip, use_internal_engine,   // Flags
             refIn, refOut;                  // Custom spec
        int checksum, 
            crc_spec, n, g, init, xor; // Custom spec
        char* msg[MAX_MESSAGE_ARGLENGTH];
        char* inFile[FILENAME_MAX];
        char* outFile[FILENAME_MAX];
    } ca;
    if (argc < 2 || checkArg(argc, argv, "help")) {
        // Print help if no args or help command
        printf(HELPTEXT1, NULL);
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
        { .isInt = true,  .var = (int*)&ca.crc_spec, .str = "-c", .defaultString = 0 },          // CRC spec index
        { .isString = true, .var = (char*)&ca.msg, .str = "-m", .defaultString = "" },          // message
        { .isInt = true,  .var = (int*)&ca.checksum, .str = "-s", .defaultInt = 0 },           // checksum for validation
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
        { .isInt = true,  .var = (int*)&ca.n, .str = "-n", .defaultInt = 8 },    // bit width
        { .isInt = true,  .var = (int*)&ca.g, .str = "-g", .defaultInt = 0x07 }, // generator polynomial
        { .isInt = true,  .var = (int*)&ca.init, .str = "-i", .defaultInt = 0 }, // init seed
        { .isInt = true,  .var = (int*)&ca.xor, .str = "-x", .defaultInt = 0 },  // final xor
        { .isFlag = true, .var = (bool*)&ca.refIn, .str = "-ri" },               // inRef
        { .isFlag = true, .var = (bool*)&ca.refOut, .str = "-ro" },              // outRef

    };
    processArgs(argv, argc, defs, COUNT_OF(defs));
    // Set flags
    PROG.printSteps = ca.printSteps ? true : false;
    PROG.prt_nogen =  ca.prt_nogen  ? true : false;
    PROG.prt_noskip = ca.prt_noskip ? true : false;
    PROG.verbose =    ca.verbose    ? true : false;
    PROG.timing =     ca.timing     ? true : false;

    PROG.internal_engine = (!EXTERNAL_ENGINE_AVAILABLE || ca.use_internal_engine) ? true : false;
    GetRem_ptr = PROG.internal_engine ? GetRemInternal : GetRem;


    // Check for a known command
    if (!ca.zoo && !ca.enc && !ca.validate && !ca.impl_test && !ca.perf_test) {
        printf("Available commands:\n\tzoo\tWhere all the CRCs live\n\tenc\tEncode a message\n"
               "\tval\tValidate a message\n\timp\tImplemenation test\n\thelp\tMore help\n", NULL);
        exit(EXIT_SUCCESS);
    }
    // Command line arguments end

    // Commmand: Show CRC inventory
    if (ca.zoo) {
        zooTour(zoo, COUNT_OF(zoo));
        exit(EXIT_SUCCESS);
    }

    // Commands below this point require a CRC.
      // Load CRC definition 
        crc_t enc_crc;
        crc = &enc_crc;
        loadDefWrapper(zoo, ca.crc_spec, &enc_crc, false); 

    // The two tests may be run within one execution    
    // Commmand: Test implementation
    if (ca.impl_test) {
        TestImplemenation(crc);

        if (!ca.perf_test)
            exit(EXIT_SUCCESS);
    }
    // Commmand: Test implementation
    if (ca.perf_test) {
        PerfImplemenation(crc, 0x10000);
        PerfImplemenation(crc, 0x100000);
        PerfImplemenation(crc, 0x1000000);
        // PerfImplemenation(crc, 0x8000000);

        exit(EXIT_SUCCESS);
    }

    // Commands below this point require a message. Try to find one, else exit.
    char* message;
    // In command line?
    if (strlen((char*)ca.msg) > 0 ) 
        message = (char*)ca.msg;
    // Else in file?
    else if (strlen((char*)ca.inFile) > 0) {
        char* fcontent = ReadTextFromFile((char*)ca.inFile, MAX_MESSAGE_READLENGTH, true, NULL);
        if (fcontent != NULL) {
            message = fcontent;
            fcontent = NULL;
        }
    }
    // Still no message?
    if (strlen(message) < 1 ) {
        PRINTERR("No message, exiting.\n");
        exit(EXIT_FAILURE);
    }

    // Command: Encode
    if (ca.enc) {      
        // Prepare message 
        msg = PrepareMsg(crc, message);
       
        // Expected checksum value for testing checksum calculation. Essentially a custom check value.
        // If the message is "AB" an expected value is set matching the current CRC spec. 
        // Or set a custom value.  Check is skipped when set to 0. 
        msg->expected = strcmp(msg->msgStr, "AB") ? 0 : crc->checkAB; 

        // Calculate remainder with engine pointed to, also start and stop timer
        timer_start = clock();
            msg->rem = GetRem_ptr(crc, msg, 0);
        timer_end = clock();

        // Printing 
        if (PRINTMSG) {
            if (msg->len < PRINTLIMIT)
                printf("Message:\t%s\n", msg->msgStr);
            else
                printf("Message:\t[%llu characters]\n", msg->len);
        }
        printf("Checksum:\t%#llX\n", msg->rem);
        double elapsed = TIMING(timer_start, timer_end);
        if (ca.timing) printf("%d chars in %5.3f seconds, %5.3f MiB/s.\n", msg->len, elapsed, msg->len / elapsed / 0x100000);

        // Compare result with a expected value
        if ( msg->expected && (msg->rem != msg->expected || PROG.verbose) ) {
            printf("Expected:\t%#llX\n", msg->expected);
            if (PROG.verbose) {                                   // Print bits of result and expected for analysis
                uint8_t checksumBits[sizeof(msg->rem) * 8];
                int2bitsMSF(sizeof(msg->rem), &msg->rem, checksumBits, false);
                printBits("Checksum", checksumBits, COUNT_OF(checksumBits), crc->n);
                uint8_t expectedBits[sizeof(msg->expected) * 8];
                int2bitsMSF(sizeof(msg->expected), &msg->expected, expectedBits, false);
                printBits("Expected", expectedBits, COUNT_OF(expectedBits), crc->n);
            }
            msg->rem == msg->expected ?
                printf("\e[1;32m%s\e[m\n", "Checksum matches expected value.") :  // green
                printf("\e[1;31m%s\e[m\n", "Checksum does not match expected value."); // red
        }
        
        // Open file for writing result      
        char csStr[100];
        sprintf(csStr, "[%#llX]", msg->rem); 
        char outStr[strlen(msg->msgStr) + strlen(csStr)];
        sprintf(outStr, "%s%s", csStr, msg->msgStr); 
        // puts(outStr);
        if (ca.outFile != NULL) {
            FILE* fp; 
            fp = fopen((char*)ca.outFile, "w");
            if (fp != NULL) {
                fprintf(fp, outStr);
                fclose(fp);
            }
        }

        // Free allocations
        if (msg->msgStr != (char*)ca.msg)
            free(msg->msgStr);
        free(msg->msgBits);
        free(msg);
        exit(EXIT_SUCCESS);
    }
  
    // Command: Validate
    if (ca.validate) {     
        // Check for available checksum:
        uint64_t checksum;
        // In message?
        char checksumStr[0x20] = "";
        char* remaining;
        char** end;
        remaining = strchr(message, '[');
        if (remaining) {
            checksum = strtol(remaining + 1, end, 16); // 0 if no valid conversion
        }
        if (checksum > 0) {
            if (PROG.verbose) printf("Checksum in message: %#llX\n", checksum);
            // Remove from message-string
            remaining = strchr(message, ']');
            if (remaining) {
                message = remaining + 1;
            if (PROG.verbose) printf("Remaining message:%s\n", message);
            }
            else {
                // No ending ], where does message begin? Better to use end from strtol, so figure that out.
            }
        }        
        // In command line?
        else if (ca.checksum > 0) 
            checksum = ca.checksum;
        else {
            PRINTERR("No checksum for validation, exiting.\n");   
            exit(EXIT_FAILURE);
        }
        
        // Prepare message 
        msg = PrepareMsg(crc, message);
        msg->validation_rem = checksum;

       // if (PROG.verbose) 
        printf("Checksum:\t\t%#llX\n", msg->validation_rem);

         // Calculate remainder with engine pointed to, also start and stop timer
        timer_start = clock();
            msg->rem = GetRem_ptr(crc, msg, 0);
        timer_end = clock();

        
        // Validate the messsage
        msg->valid = validate(crc, msg);
        
        // Print result        
        validPrint(msg->msgStr, msg->len, msg->valid);

        // Free allocations
        // if (msg->msgStr != (char*)ca.msg && msg->msgStr != NULL) free(msg->msgStr);    
        if (msg->msgBits != NULL) free(msg->msgBits);
        if (msg != NULL) free(msg);    
        exit(EXIT_SUCCESS);
    }

}
