#include "crc.h"
#include "jlibc/binutils.h"
#include "jlibc/common.h"

// Data structures 
prog_t* prog;
// crc_t* crc;  
// msg_t* msg;

// Timing
clock_t timer_start; clock_t timer_end; 

// Pointer to engine
GetRem_ptr_t GetRem_ptr;


/** Internal engine ***************************************************/
void ArrangeMsg(crc_t* crc, msg_t* msg) {
    // Alloc for msgBits
    msg->msgBits = calloc(msg->paddedBitLen, sizeof(uint8_t));         
        assert( ("Memory allocation failed.", msg->msgBits != NULL) );
     
     // Arrange message bits and pad
    if ( crc->inputLSF )
        ints2bitsLSF(strlen(msg->msgStr), sizeof(uint8_t), (msg->msgStr), msg->msgBits, SPECIALWIDTH, msg->initPad);  // Special accomodation, cf. error.h
        // ints2bitsLSF(strlen(msg->msgStr), sizeof(uint8_t), (msg->msgStr), msg->msgBits, crc->n, crc->initBits);           // Normal
    else
        ints2bitsMSF(strlen(msg->msgStr), sizeof(uint8_t), (msg->msgStr), msg->msgBits, crc->n, msg->initPad); 

    if (crc->init > 0) {
        // Local initBits to crc width
        TOWIDTH(initBits);

        // Write initbits into front padding
        for (int i = 0; i < crc->n; i++) 
            msg->msgBits[i] = initBits[i];
        if(PROG.verbose) { printf("\ninitpad: %d  ", msg->initPad);  printf("msgBits (initBits written to frontpad): "); i82p(msg->msgBits, msg->paddedBitLen, 0, 0, 1); }
    }

    if (PROG.verbose) {printf("ArrangeMsg validation rem: %#llX\n", msg->validation_rem);}

    if (msg->validation_rem != 0) {
    // Write check bits to padding
        // Convert checksum to array of bit values
        if (crc->resultLSF)
            int2bitsLSF(sizeof crc->n, &msg->validation_rem, msg->remBits, false);
        else
            int2bitsMSF(sizeof crc->n, &msg->validation_rem, msg->remBits, false);

        // Handled as uint64, truncate to CRC width
        uint8_t remBits[crc->n]; 
        if (crc->resultLSF)
            bitSlice(0, crc->n, &(msg->remBits), 0, remBits);
        else
            bitSlice(sizeof(msg->validation_rem) * BITSINBYTE - crc->n, crc->n, &(msg->remBits), 0, remBits);

        // printf("         rem: %#llX\n", msg->validation_rem);
        // printf("msg->remBits: "); i82p(msg->remBits, 64, 0, 0, 1); 
        // printf("     remBits: "); i82p(remBits, crc->n, 0, 0, 1); 

        // Write 
        for (int i = msg->paddedBitLen - crc->n, j = 0; j < crc->n; i++, j++) 
            msg->msgBits[i] = remBits[j];

        if(PROG.verbose)
         { printf("\nrem: %#llX  ", msg->validation_rem);  printf("msgBits (remBits written to backpad): "); i82p(msg->msgBits, msg->paddedBitLen, 0, 0, 1); }
    }
}

uint64_t ConvertInit(uint64_t poly, uint64_t init, uint8_t width) {
    uint64_t next = 2;
    for (int i = 1; i < width - 1; i++) next *= 2;  // pow() funkar inte under linux, saknar math.h

   // Direct to non-direct
   for (int i = 0; i < width; i++) {
       if (init & 1) {
           init ^= poly;
		   init >>= 1;
		   init |= next;
        }
        else
            init >>= 1;
   }
   return init;
}

uint64_t GetRemInternal(crc_t* crc, msg_t* msg, uint64_t check) {
    msg->validation_rem = check;
    ArrangeMsg(crc, msg);
    return PolyDivision(crc, msg);
}

uint64_t PolyDivision(crc_t* crc, msg_t* msg) {
    // gBits to actual bit width
    size_t gBits_size = crc->n + 1; // Generator is 1 bit wider than CRC  
    uint8_t gBits[gBits_size];
    for (int i = gBits_size - 1, j = COUNT_OF(crc->gBits) - 1; i >= 0; i--, j--)
        gBits[i] = crc->gBits[j];
    
    if (!PROG.printSteps) {
    // Poly division 
        for (int i = 0; i < msg->originalBitLen + msg->initPad; i++)  // Standard loop ending condition
        // for (int i = 0; i < REMLOOPEND; i++)                       // Special accomodation, cf. error.h
            if (msg->msgBits[i]) 
                for (int j = 0, k = i; j < gBits_size; j++, k++) 
                    msg->msgBits[k] ^= gBits[j];
    }
    else {
    // Poly division with printing of steps
        // Header
        char fmt[0x40];
        uint8_t gSkip[gBits_size]; for I2(gBits_size) gSkip[i] = 0;
        if (crc->init) {
            sprintf(fmt, "         \e[1;1m\e[1;3m%%%ds %%%ds %%%ds\e[m", msg->initPad, msg->originalBitLen, crc->n);
            printf(fmt, "Init", "Message", "Padding");
        }
        else {
            sprintf(fmt, "         \e[1;1m\e[1;3m%%%ds %%%ds\e[m", msg->originalBitLen, crc->n);
            printf(fmt, "Message", "Padding");
        }
        int32_t space1 = crc->init ? msg->initPad : -1; 
        int32_t space2 = crc->init ? msg->originalBitLen + msg->initPad  : msg->originalBitLen; 
        int separator = 0; int newLines = 1;
        // Content
        printf("\n Before: "); i2pc(msg->msgBits, msg->paddedBitLen, separator, newLines, 0, msg->originalBitLen+msg->initPad, crc->n, space1, space2, 0); 
        // for (; i < REMLOOPEND; i++)                       // Special accomodation, cf. error.h
        for (int i = 0; i < msg->originalBitLen + msg->initPad; i++)  // Standard loop ending condition
            if (msg->msgBits[i]) {
                if (!PROG.prt_nogen)
                    i2pc(gBits, gBits_size, separator, newLines, 33, 0, gBits_size, space1-i, (crc->init && i > msg->initPad) ? space2-i+1 : space2-i, i+9); 
                for (int j = 0, k = i; j < gBits_size; j++, k++) 
                    msg->msgBits[k] ^= gBits[j];
                printf("   @%3d: ", i); i2pc(msg->msgBits, msg->paddedBitLen, separator, newLines, 36, i, gBits_size, space1, space2, 0);
            }
            else if (PROG.prt_noskip) {  
                if (!PROG.prt_nogen)
                    i2pc(gSkip, gBits_size, separator, newLines, 90, 0, gBits_size, space1-i, (crc->init && i > msg->initPad) ? space2-i+1 : space2-i, i+9); 
                printf("Skip%3d: ", i); i2pc(msg->msgBits, msg->paddedBitLen, separator, newLines, 36, i, gBits_size, space1, space2, 0);
            }  
        printf("  After: "); i2pc(msg->msgBits, msg->paddedBitLen, separator, newLines, 37, msg->originalBitLen+msg->initPad, crc->n, space1, space2, 0); 
    } 
        

    if (PROG.verbose) { puts("Message post calculation"); i2p(msg->msgBits, msg->paddedBitLen, 0, 0, 1);  }

    // Slice off the remBits from msgBits
    uint8_t remBits[crc->n];
    bitSlice(-1, crc->n, msg->msgBits, msg->paddedBitLen, remBits);
    if (PROG.verbose) printBits("Remainder", remBits, COUNT_OF(remBits), crc->n);

    // Final xoring of validation_rem as required by some CRC specs
    if (crc->xor > 0) {
        TOWIDTH(xorBits);
        for (int i = 0; i < COUNT_OF(remBits); i++) 
            remBits[i] ^= xorBits[i];
        if (PROG.printSteps) {
            printf("    XOR:"); 
            i2pc(xorBits, crc->n, 0, 1, 34, 0, crc->n, -1, -1, msg->paddedBitLen - crc->n + (crc->init ? 11 : 10) -8); 
            i2pc(remBits, crc->n, 0, 1, 37, 0, crc->n, -1, -1, msg->paddedBitLen - crc->n + (crc->init ? 11 : 10) ); 
        }
    }
    

    // Convert remBits to int with choice of bit ordering
    uint64_t rem;
    if ( crc->resultLSF )
        rem = (uint64_t)bits2intLSF(COUNT_OF(remBits), remBits);
    else
        rem = (uint64_t)bits2intMSF(COUNT_OF(remBits), remBits);

    if (PROG.verbose) printf("Remainder: %#llX\n", rem);
    return rem;
}
/** end Internal engine *********************************************************/

/** CRC specification control ***************************************************/
void LoadDef(crcdef_t zoo[], size_t index, crc_t* crc) {
        // 0 n   1 Gen    2 IL1  3 Init  4 Nondir. 5 RefIn 6 RefOut 7 XorOut   8 Residue 9 Check      10 "AB"
        strcpy((crc)->description, zoo[index].name);
        crc->n =         zoo[index].specs[0];
        crc->g =         zoo[index].specs[1];
        crc->il1 =       zoo[index].specs[2];
        crc->init =      zoo[index].specs[3];
        crc->nondirect = zoo[index].specs[4];
        crc->inputLSF =  zoo[index].specs[5];
        crc->resultLSF = zoo[index].specs[6];
        crc->xor =       zoo[index].specs[7];
        crc->residue =   zoo[index].specs[8];
        crc->check =     zoo[index].specs[9];
        crc->checkAB =   zoo[index].specs[10];

    // Convert generator polynomial to array of bit values  
    int2bitsMSF(sizeof(crc->g), &crc->g, crc->gBits, true );          
    crc->gBits[COUNT_OF(crc->gBits) - crc->n - 1] = crc->il1;         

    // If needed, convert direct init to non-direct
    crc->init_conv = (crc->init && !crc->nondirect) ?
        ConvertInit(crc->g, crc->init, crc->n) : crc->init;

    // Convert init and final xor to array of bit values  
    int2bitsMSF(sizeof(crc->init_conv), &crc->init_conv, crc->initBits, 0 );    
    int2bitsMSF(sizeof(crc->xor), &crc->xor, crc->xorBits, 0 );       

}

void LoadDefWrapper(crcdef_t zoo[], size_t index, crc_t* crc, bool table) {
    LoadDef(zoo, index, crc);  
    
    if (table) {
    // Oneline print for zoo-list
        char prt_init[18] = " ";  if (crc->init) sprintf(prt_init, "%#18llX", crc->init);
        char prt_xor[18] = " ";  if (crc->xor) sprintf(prt_xor, "%#18llX", crc->xor);
        char* prt_nondirect = crc->nondirect ? "X" : " "; 
        char* prt_refIn = crc->inputLSF ? "X" : " "; 
        char* prt_refOut = crc->resultLSF ? "X" : " "; 
        printf("\e[1;1m%16s\e[m ", crc->description);
        printf("%#18llX %#18s  %-3s ",  crc->g, prt_init, prt_nondirect);
        printf("%#18s    %-2s    %-3s ", prt_xor, prt_refIn, prt_refOut);
    }
    else {
    // Verbose print for normal execution
        char prt_init[18] = "No";  if (crc->init) sprintf(prt_init, "%#llX", crc->init);
        char prt_xor[18] = "No";  if (crc->xor) sprintf(prt_xor, "%#llX", crc->xor);
        char* prt_nondirect = crc->nondirect ? "Yes" : "No"; 
        char* prt_refIn = crc->inputLSF ? "Yes" : "No"; 
        char* prt_refOut = crc->resultLSF ? "Yes" : "No"; 
        printf("\e[1;53m\e[1;7m%s\e[1;27m   ", crc->description);
        printf("Poly:%#llX   Init:%s   NDI:%s   ",  crc->g, prt_init, prt_nondirect);
        printf("XorOut:%s   RefIn:%s   RefOut:%s   \e[1;55m\n", prt_xor, prt_refIn, prt_refOut);
    }    

    // Check value-test for this spec
      // Disable printSteps when testing
    uint8_t tmp_printSteps = PROG.printSteps; 
    if (table) PROG.printSteps = false;
    PROG.printSteps = SELFTESTSTEPS;   //DEBUG

    ValueCheckTest(crc, 0, table ? 1 : 2);

    PROG.printSteps = tmp_printSteps; // Reset printSteps flag


    if (VERBOSE && !table) { 
    // Diagnostic info
        printf("     gBits: "); i2p(&crc->gBits, COUNT_OF(crc->gBits), crc->n+1, 0, 1);
        // if (VERBOSE || expected) printBits("Generator",  crc->gBits, COUNT_OF( crc->gBits ), crc->gBits_size);
        printf("  initBits:  "); i2p(&crc->initBits, COUNT_OF(crc->initBits), crc->n, 0, 1);
        printf("   xorBits:  "); i2p(&crc->xorBits, COUNT_OF(crc->xorBits), crc->n, 0, 1);
    }
}

void ZooTour(crcdef_t zoo[], size_t zoo_size) {
    printf("\e[1;3m\e[1;4m%5s %18s %18s %5s %18s %4s %18s %5s %6s %6s\e[m\n", "Index", "Spec", "Poly", "NoIL1", "Init", "NDI", "XorOut", "RefIn", "RefOut", "Check value");
    for (int i = 0; i < zoo_size; i++) {
        crc_t zooItem;
        printf("%5d ", i);
        LoadDefWrapper(zoo, i, &zooItem, true);
    }
}
/** end CRC specification control ***********************************************/

/** Testing *********************************************************************/
implTest_t TestImplemenation(crc_t* crc) {
    implTest_t test;
    uint64_t res;

    // res = ValueCheckTest(crc, 0, 2); 
    // test.passed_check = res == crc->check ? true : false;
    
    res = ValueCheckTest(crc, 1, 0); 
    test.passed_validate_check = res == 0 ? true : false;
    test.passed_validate_check ? printf("\e[1;32mPassed\e[m") : printf("\e[1;31mFailed\e[m");
    printf(" check value validation; \"123456789\" with CRC value %#llX => %#llX\n", crc->check, res);

    res = ValueCheckTest(crc, 2, 0); 
    test.passed_changed_check =  res != 0 ? true : false;
    test.passed_changed_check ? printf("\e[1;32mPassed\e[m") : printf("\e[1;31mFailed\e[m");
    printf(" changed message; \"1b3456789\" with CRC value %#llX => %#llX\n", crc->check, res);

}

implTest_t PerfImplemenation(crc_t* crc, uint64_t set_size) {
    implTest_t test;
    uint64_t res;

    // Encode    
    char* message = (char*)GetU8random(set_size, 255, NULL);
    msg_t* perf = PrepareMsg(crc, message);

    timer_start = clock();
        perf->rem = GetRem_ptr(crc, perf, 0);
    timer_end = clock();

    double elapsed = TIMING(timer_start, timer_end);
    printf("  Encode: %10d chars in %6.3f seconds, %6.3f MiB/s.\n", perf->len, elapsed, perf->len / elapsed / 0x100000);

    // Validate
    timer_start = clock();
               perf->rem = GetRem_ptr(crc, perf, perf->rem);
    timer_end = clock();

    test.passed_validate_msg = perf->rem == 0; 
    elapsed = TIMING(timer_start, timer_end);
    printf("Validate: %10d chars in %6.3f seconds, %6.3f MiB/s. ", perf->len, elapsed, perf->len / elapsed / 0x100000);
    test.passed_validate_msg ? printf("\e[1;32mPassed.\e[m\n") : printf("\e[1;31mFailed.\e[m\n");

    // Free
    free(perf->msgStr);
    free(perf->msgBits);
    free(perf);
}

uint64_t ValueCheckTest(crc_t* crc, uint8_t type, uint8_t output) {
    // Prepare standard check message
    char message[] = "123456789";
    msg_t* test_msg = PrepareMsg(crc, message);
    if (type == 2)
        test_msg->msgStr[1] = 'x';

    // Call engine
    if (type == 0) 
        test_msg->rem = GetRem_ptr(crc, test_msg, 0);
    else 
        test_msg->rem = GetRem_ptr(crc, test_msg, crc->check);

    bool valid = ( (type == 0 && test_msg->rem == crc->check) || (type != 0 && test_msg->rem == 0 ) ) ? true : false;

    // Print check value test result
    if (valid && output == 1) 
        // printf("\e[1;32mPassed\e[m\n");                                       // Short
        printf("\e[1;32mPassed\e[m %#0llX\n", test_msg->rem, crc->check);         // Show value
    if (valid && output == 2) 
            printf("\e[1;32mPassed check value-test for %s;\e[m matching %#llX\n", crc->description, crc->check);
    if (!valid && output == 1) 
        // printf("\e[1;31mFailed\e[m\n");                                            // Short
        printf("\e[1;31mFailed\e[m %#0llX != %#0llX\n", test_msg->rem, crc->check);      // Show values                                    
    if (!valid && output == 2) 
        printf("\e[1;31m\e[1;5mFailed\e[1;25m check value-test for %s;\e[m result %#0llX != check %#0llX\n", crc->description, test_msg->rem, crc->check); 
    // Free allocation for msg struct
    if (test_msg != NULL) free(test_msg);
    // Return result
    return test_msg->rem;
}

bool CustomValueCheck(crc_t* crc, msg_t* msg) {
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
        bool pass = msg->rem == msg->expected ? true : false;
        return pass;
}
/** end Testing *****************************************************************/


/** Framework *******************************************************************/
msg_t* PrepareMsg(crc_t* crc, char* message) {
// Prepare message struct
        msg_t* r = calloc(1, sizeof(msg_t));

        int8_t initPad = crc->init > 0 ? crc->n : 0;
        // int8_t augmentPad = crc->init && !crc->nondirect ? 0 : crc->n;
        int8_t augmentPad = crc->n;

        r->msgStr = message;
        r->len = strlen(message);
        r->initPad = initPad;
        r->originalBitLen = strlen(message) * sizeof(uint8_t) * BITSINBYTE;
        // r->.paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + SPECIALWIDTH + initPad,     // Special
        r->paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + augmentPad + initPad;               // Normal
        return r;
}

bool Validate(crc_t* crc, msg_t* msg) {

    if (PROG.verbose) printf("Remainder: %#llX\n", msg->rem);
    return msg->rem == 0 ? true : false;
}

void ValidPrint(uint8_t msg[], size_t msgSize, bool valid) {
    if (PRINTMSG) {
        // char msgStr[msgSize + 1];
        // charArrayToString(msg, msgSize, msgStr);
        if (msgSize < PRINTLIMIT)
            printf("Message to validate:\t%s\n", msg);
        else
            printf("Message to validate:\t[%d characters]\n", msgSize);
    }
    if (valid) 
        printf("\e[1;32m%s\e[m\n", "The data is OK"); // green
    else
        printf("\e[1;31m%s\e[m\n", "The data is not OK"); // red
}

//  uint64_t encode(char* msg, int crcIndex) {
//  }

//  bool Validate(char* msg, int crcIndex, uint64_t check) {
//  }

static short allocCheck(void* p) {
    if(p == NULL ) {
        if (true) fprintf(stderr, "Unable to allocate memory.\n");
        return 1;
    }
    else 
        return 0;
}
/** end Framework *******************************************************************/
