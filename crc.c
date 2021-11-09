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

/** CRC specification control ***************************************************/
void LoadDef(crcdef_t zoo[], size_t index, crc_t* crc) {
    #ifndef WIDE_CRC
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
    #else
    strcpy((crc)->description, zoo[index].name);
    crc->n =         zoo[index].n;
    crc->il1 =       zoo[index].il1;
    crc->nondirect = zoo[index].nondirect;
    crc->inputLSF =  zoo[index].inputLSF;
    crc->resultLSF = zoo[index].resultLSF;
    crc->hex_orders = crc->n % NIBBLE == 0 ? crc->n / NIBBLE : (crc->n / NIBBLE) + 1;

    strcpy((crc)->w_g, zoo[index].g);
    strcpy((crc)->w_init, zoo[index].init);
    strcpy((crc)->w_xor, zoo[index].xor);
    strcpy((crc)->w_residue, zoo[index].residue);
    strcpy((crc)->w_check, zoo[index].check);
    strcpy((crc)->w_checkAB, zoo[index].checkAB);

    if (crc->n <= 64) {                         
        crc->g =         strtoull(zoo[index].g, NULL, 16);
        crc->init =      strtoull(zoo[index].init, NULL, 16);
        crc->xor =       strtoull(zoo[index].xor, NULL, 16);
        crc->residue =   strtoull(zoo[index].residue, NULL, 16);
        crc->check =     strtoull(zoo[index].check, NULL, 16);
        crc->checkAB =   strtoull(zoo[index].checkAB, NULL, 16);
    }
    else {
        crc->g =         0;
        crc->init =      0;
        crc->xor =       0;
        crc->residue =   0;
        crc->check =     0;
        crc->checkAB =   0;
    }
    #endif

    if (crc->n <= 64) {
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
    #ifdef WIDE_CRC
    else {

        // Convert generator polynomial to array of bit values  
        hexstr2bitsMSF(16, crc->w_g, crc->w_gBits, true);
        if (crc->il1) crc->w_gBits[COUNT_OF(crc->w_gBits)-1-crc->n] = 1;
        // printBits("      Poly converted", crc->w_gBits, COUNT_OF(crc->w_gBits), 0);

        // If needed, convert direct init to non-direct
        if (!crc->nondirect && crc->w_init == 0) 
            strcpy(crc->w_init_conv, crc->w_init);
        else        
            strcpy(crc->w_init_conv, crc->w_init);
            // crc->w_init_conv = (crc->g, crc->init, crc->n);

        // Convert init and final xor to array of bit values  
        hexstr2bitsMSF(16, crc->w_xor, crc->w_xorBits, false);
        hexstr2bitsMSF(16, crc->w_init_conv, crc->w_init_conv, false);
    }
    #endif
}

void LoadDefWrapper(crcdef_t zoo[], size_t index, crc_t* crc, bool table) {
    LoadDef(zoo, index, crc);  
    
    if (table) {
    // Oneline print for zoo-list
        char    prt_g[35] = " ";  
        char prt_init[35] = " ";  
        char  prt_xor[35] = " ";  
        if (crc->n <= 64) {
            sprintf(prt_g, "%#18llx", crc->g);
            if (crc->init) sprintf(prt_init, "%#18llx", crc->init);
            if (crc->xor) sprintf(prt_xor, "%#18llx", crc->xor);
        }
        else {
            strcpy(prt_g, crc->w_g);
            strcpy(prt_init, crc->w_init);
            strcpy(prt_xor, crc->w_xor);
        }
        char* prt_nondirect = crc->nondirect ? "X" : " "; 
        char* prt_il1 = crc->il1 ? "X" : " "; 
        char* prt_refIn = crc->inputLSF ? "X" : " "; 
        char* prt_refOut = crc->resultLSF ? "X" : " "; 
        printf("\e[1;1m%-18s\e[m ", crc->description);
        printf("%#18s   %#18s   %-3s",  prt_g, prt_init, prt_nondirect);
        // printf("%#18llx   %-3s %#18s   %-3s ",  crc->g, prt_il1, prt_init, prt_nondirect); // With IL1
        printf("%#18s    %-2s    %-3s   ", prt_xor, prt_refIn, prt_refOut);
    }
    else {
    // Verbose print for normal execution
        char    prt_g[35] = " ";  
        char prt_init[35] = "No";  
        char  prt_xor[35] = "No";  
        if (crc->n <= 64) {
            sprintf(prt_g, "%#llx", crc->g);
            if (crc->init) sprintf(prt_init, "%#llx", crc->init);
            if (crc->xor) sprintf(prt_xor, "%#llx", crc->xor);
        }
        else {
            strcpy(prt_g, crc->w_g);
            strcpy(prt_init, crc->w_init);
            strcpy(prt_xor, crc->w_xor);
        }
        char* prt_nondirect = crc->nondirect ? "Yes" : "No"; 
        char* prt_il1 = crc->il1 ? "Yes" : "No"; 
        char* prt_refIn = crc->inputLSF ? "Yes" : "No"; 
        char* prt_refOut = crc->resultLSF ? "Yes" : "No"; 
        printf("\e[1;53m\e[1;7m%s\e[1;27m   ", crc->description);
        printf("Poly:%s   IL1:%s   Init:%s   NDI:%s   ",  prt_g, prt_il1, prt_init, prt_nondirect);
        printf("XorOut:%s   RefIn:%s   RefOut:%s   \e[m\n", prt_xor, prt_refIn, prt_refOut);
    }    

    // Check value-test for this spec
      // Disable printSteps when testing
    uint8_t tmp_printSteps = PROG.printSteps; 
    if (table) PROG.printSteps = false;
    PROG.printSteps = SELFTESTSTEPS;   //DEBUG

    ValueCheckTest(crc, 0, table ? 1 : 2);

    PROG.printSteps = tmp_printSteps; // Reset printSteps flag


    if (PROG.verbose && !table) { 
    // Diagnostic info
        // printf("     gBits: "); i2p(&crc->gBits, COUNT_OF(crc->gBits), crc->n+1, 0, 1);
        // if (VERBOSE || expected) printBits("Generator",  crc->gBits, COUNT_OF( crc->gBits ), crc->gBits_size);
        // printf("  initBits:  "); i2p(&crc->initBits, COUNT_OF(crc->initBits), crc->n, 0, 1);
        // printf("   xorBits:  "); i2p(&crc->xorBits, COUNT_OF(crc->xorBits), crc->n, 0, 1);
    }
}

void ZooTour(crcdef_t zoo[], size_t zoo_size) {
    printf("\e[1;3m\e[1;4m%5s %-18s %18s   %18s %4s %18s %5s %6s  %6s\e[m\n", "Index", "Spec", "Poly", "Init", "NDI", "XorOut", "RefIn", "RefOut", "Check value               ");
    // printf("\e[1;3m\e[1;4m%5s %-18s %18s %4s %19s %4s   %18s %5s %6s  %6s\e[m\n", "Index", "Spec", "Poly", "IL1", "Init", "NDI", "XorOut", "RefIn", "RefOut", "Check value              "); // Med IL1
    for (int i = 0; i < zoo_size; i++) {
        crc_t zooItem;
        printf("%5d ", i);
        LoadDefWrapper(zoo, i, &zooItem, true);
    }
}
/** end CRC specification control ***********************************************/

/** Testing *********************************************************************/
implTest_t ImplValid(crc_t* crc) {
    implTest_t test;
    uint64_t res;

    // res = ValueCheckTest(crc, 0, 2); 
    // test.passed_check = res == crc->check ? true : false;
    
    res = ValueCheckTest(crc, 1, 0); 
    test.passed_validate_check = res == 0 ? true : false;
    test.passed_validate_check ? printf("\e[1;32mPassed\e[m") : printf("\e[1;31mFailed\e[m");
    #ifndef WIDE_CRC
    printf(" check value validation; \"123456789\" with CRC value %#llx => %#llx\n", crc->check, res);
    #else
    printf(" check value validation; \"123456789\" with CRC value %s => %#llx\n", crc->w_check, res);
    #endif

    res = ValueCheckTest(crc, 2, 0); 
    test.passed_changed_check =  res != 0 ? true : false;
    test.passed_changed_check ? printf("\e[1;32mPassed\e[m") : printf("\e[1;31mFailed\e[m");
    #ifndef WIDE_CRC
    printf(" changed message; \"1x3456789\" with CRC value %#llx => %#llx\n", crc->check, res);
    #else
    printf(" changed message; \"1x3456789\" with CRC value %s => %#llx\n", crc->w_check, res);
    #endif
}

implTest_t ImplPerf(crc_t* crc, uint64_t set_size) {
    implTest_t test;
    uint64_t res;

    // Encode    
    // char* message = (char*)GetRandomPrintable(set_size, 100, NULL);
    char* message = (char*)GetDataLorem(0x1000, NULL);
    // char* message = "A";


    // printf("Message|%s|\n", message);

    msg_t* perf = PrepareMsg(crc, message);
    #ifdef WIDE_CRC
       perf->w_validation_rem = NULL;                
    #endif

    timer_start = clock();
        perf->rem = GetRem_ptr(crc, perf, 0);
    timer_end = clock();

    double elapsed = TIMING(timer_start, timer_end);
    printf("  Encode: %10d chars in %6.3f seconds, %6.3f MiB/s.\n", perf->len, elapsed, perf->len / elapsed / 0x100000);

    // Validate
    #ifdef WIDE_CRC
       perf->w_validation_rem = perf->w_rem;                
    #endif

    timer_start = clock();
        perf->rem = GetRem_ptr(crc, perf, perf->rem);
    timer_end = clock();

    test.passed_validate_msg = perf->rem == 0; 
    elapsed = TIMING(timer_start, timer_end);
    printf("Validate: %10d chars in %6.3f seconds, %6.3f MiB/s. ", perf->len, elapsed, perf->len / elapsed / 0x100000);
    test.passed_validate_msg ? printf("\e[1;32mPassed.\e[m\n") : printf("\e[1;31mFailed.\e[m\n");

    // Free
    if (perf->msgStr != NULL) free(perf->msgStr);
    if (perf->msgBits != NULL) free(perf->msgBits);
    if (perf != NULL) free(perf);
}

uint64_t ValueCheckTest(crc_t* crc, uint8_t type, uint8_t output) {
    // Prepare standard check message
    char message[] = "123456789";
    msg_t* test_msg = PrepareMsg(crc, message);
    if (type == 2)
        test_msg->msgStr[1] = 'x';

    // Call engine
    if (type == 0) {
        #ifdef WIDE_CRC
        test_msg->w_validation_rem = NULL;                
        #endif
        test_msg->rem = GetRem_ptr(crc, test_msg, 0);
    }
    else {
        #ifdef WIDE_CRC
        test_msg->w_validation_rem = crc->w_check;                
        #endif
        test_msg->rem = GetRem_ptr(crc, test_msg, crc->check);
    }

    bool valid = false;
    #ifndef WIDE_CRC
        valid = ( (type == 0 && test_msg->rem == crc->check) || (type != 0 && test_msg->rem == 0 ) ) ? true : false;
        // Print check value test result
        if (valid && output == 1) 
            // printf("\e[1;32mPassed\e[m\n");                                       // Short
            printf("\e[1;32mPassed\e[m %#0llx\n", crc->check);         // Show value
        if (valid && output == 2) 
                printf("\e[1;32mPassed check value-test for %s;\e[m matching %#llx\n", crc->description, crc->check);
        if (!valid && output == 1) 
            // printf("\e[1;31mFailed\e[m\n");                                            // Short
            printf("\e[1;31mFailed\e[m %#0llx != %#0llx\n", test_msg->rem, crc->check);      // Show values                                    
        if (!valid && output == 2) 
            printf("\e[1;31m\e[1;5mFailed\e[1;25m check value-test for %s;\e[m result %#0llx != check %#0llx\n", crc->description, test_msg->rem, crc->check); 
    #else
        if (PROG.verbose) printf("ValueCheckTest. w_rem:%s  w_check:%s\n", test_msg->w_rem, crc->w_check);
        valid = ( (type == 0 && !strcmp(test_msg->w_rem, crc->w_check)) || (type != 0 && test_msg->rem == 0 ) ) ? true : false;
        if (output == 2)
            printf("%s: ", PROG.engine_id);
        // Print check value test result
        if (valid && output == 1) 
            printf("\e[1;32mPassed\e[m %s\n", crc->w_check);         // Show value
        if (valid && output == 2) 
                printf("\e[1;32mPassed check value-test for %s;\e[m matching %s\n", crc->description, crc->w_check);
        if (!valid && output == 1) 
            printf("\e[1;31mFailed\e[m %s != %s\n", test_msg->w_rem, crc->w_check);      // Show values                                    
        if (!valid && output == 2) 
            printf("\e[1;31m\e[1;5mFailed\e[1;25m check value-test for %s;\e[m result %s != check %s\n", crc->description, test_msg->w_rem, crc->w_check); 
    #endif
    
    // Free allocation for msg struct
    if (test_msg != NULL) free(test_msg);
    // Return result
    return test_msg->rem;
}

bool CustomValueCheck(crc_t* crc, msg_t* msg) {
        if ( msg->expected && (msg->rem != msg->expected || PROG.verbose) ) {
            printf("Expected:\t%#llx\n", msg->expected);
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
        // int8_t augmentPad = crc->init && !crc->nondirect ? 0 : crc->n;   // Wrong way to apply init
        int8_t augmentPad = crc->n;

        r->msgStr = message;
        r->len = strlen(message);
        r->initPad = initPad;
        r->originalBitLen = strlen(message) * sizeof(uint8_t) * BITSINBYTE;
        // r->.paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + SPECIALWIDTH + initPad,     // Special
        r->paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + augmentPad + initPad;               // Normal
        if (PROG.verbose) printf("originalBitlen:%d  paddedBitlen:%d  ", r->originalBitLen, r->paddedBitLen);
        return r;
}

bool Validate(crc_t* crc, msg_t* msg) {
    if (crc->n <= 64) {
        if (PROG.verbose) printf("Remainder: %#llx\n", msg->rem);
        return msg->rem == 0 ? true : false;
    }
    #ifdef WIDE_CRC
    else {
        if (PROG.verbose) printf("Remainder: %s\n", msg->w_rem);
        size_t len_with_zeroes_only = strspn(msg->w_rem, "x0");
        return len_with_zeroes_only == strlen(msg->w_rem) ? true : false;
    }
    #endif
}

void ValidPrint(uint8_t msg[], size_t msgSize, bool valid) {
    if (PRINTMSG) {
        if (msgSize < PRINTLIMIT)
            printf("Message to validate:\t|%s|\n", msg);
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



/** end Framework *******************************************************************/

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
        if(PROG.verbose) { printf("\ninitpad: %d\n", msg->initPad);  printf("msgBits (initBits written to frontpad): "); i82p(msg->msgBits, msg->paddedBitLen, 0, 0, 1); }
    }

    if (PROG.verbose) {printf("ArrangeMsg validation rem: %#llx\n", msg->validation_rem);}

    #ifndef WIDE_CRC
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

        // printf("         rem: %#llx\n", msg->validation_rem);
        // printf("msg->remBits: "); i82p(msg->remBits, 64, 0, 0, 1); 
        // printf("     remBits: "); i82p(remBits, crc->n, 0, 0, 1); 

        // Write 
        for (int i = msg->paddedBitLen - crc->n, j = 0; j < crc->n; i++, j++) 
            msg->msgBits[i] = remBits[j];

        if(PROG.verbose)
         { printf("\nrem: %#llx  ", msg->validation_rem);  printf("msgBits (remBits written to backpad): "); i82p(msg->msgBits, msg->paddedBitLen, 0, 0, 1); }
    }
    #else
    if ( (crc->n <= 64 && msg->validation_rem != 0) || crc->n > 64 && msg->w_validation_rem != NULL ) {
    // Write check bits to padding
        // Convert checksum to array of bit values
        // Truncate to CRC width

        uint8_t remBits[crc->n]; 
        if (crc->n <= 64) {
            if (crc->resultLSF) {
                int2bitsLSF(sizeof crc->n, &msg->validation_rem, msg->remBits, false);
                bitSlice(0, crc->n, &(msg->remBits), 0, remBits);
            }
            else {
                int2bitsMSF(sizeof crc->n, &msg->validation_rem, msg->remBits, false);
                bitSlice(sizeof(msg->validation_rem) * BITSINBYTE - crc->n, crc->n, &(msg->remBits), 0, remBits);
            }
        }
        else {
            if (crc->resultLSF) {
                hexstr2bitsLSF(crc->n, msg->w_validation_rem, msg->w_remBits, false);
                // printBits("hexstr2bitsLSF produced msg->w_rembits", msg->w_remBits, COUNT_OF(msg->w_remBits), 0);
                bitSlice(0, crc->n, &(msg->w_remBits), 0, remBits);
            }
            else {
                hexstr2bitsMSF(crc->n, msg->w_validation_rem, msg->w_remBits, false);
                bitSlice(sizeof(msg->w_validation_rem) * BITSINBYTE - crc->n, crc->n, &(msg->w_remBits), 0, remBits);
            }
        }
        // Write to msg padding
        for (int i = msg->paddedBitLen - crc->n, j = 0; j < crc->n; i++, j++) 
            msg->msgBits[i] = remBits[j];

        if(PROG.verbose) { 
            if (crc->n <= 64) printf("\nrem: %#llx  ", msg->validation_rem);
            else              printf("\nrem: %s  ", msg->w_validation_rem);  
            printf("msgBits (remBits written to backpad):\n"); i82p(msg->msgBits, msg->paddedBitLen, 0, 0, 1); }
    }
    #endif
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
    if (crc->n <= 64)
        for (int i = gBits_size - 1, j = COUNT_OF(crc->gBits) - 1; i >= 0; i--, j--)
            gBits[i] = crc->gBits[j];
    #ifdef WIDE_CRC
    else
        for (int i = gBits_size - 1, j = COUNT_OF(crc->w_gBits) - 1; i >= 0; i--, j--)
            gBits[i] = crc->w_gBits[j];
    #endif
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
        // Table header
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
        // for (; i < REMLOOPEND; i++)                                   // Special accomodation, cf. error.h
        for (int i = 0; i < msg->originalBitLen + msg->initPad; i++)    // Standard loop ending condition
            if (msg->msgBits[i]) {
                if (!PROG.prt_nogen)
                    i2pc(gBits, gBits_size, separator, newLines, 33, 0, gBits_size, space1-i, (crc->init && i > msg->initPad) ? space2-i+1 : space2-i, i+9); // 9 is len of line-header of other lines
                for (int j = 0, k = i; j < gBits_size; j++, k++) 
                    msg->msgBits[k] ^= gBits[j];
                printf("   @%3d: ", i); i2pc(msg->msgBits, msg->paddedBitLen, separator, newLines, 36, i, gBits_size, space1, space2, 0);
            }
            else if (PROG.prt_noskip) {  
                if (!PROG.prt_nogen)
                    i2pc(gSkip, gBits_size, separator, newLines, 90, 0, gBits_size, space1-i, (crc->init && i > msg->initPad) ? space2-i+1 : space2-i, i+9); // 9 is len of line-header of other lines
                printf("Skip%3d: ", i); i2pc(msg->msgBits, msg->paddedBitLen, separator, newLines, 36, i, gBits_size, space1, space2, 0);
            }  
        printf("  After: "); i2pc(msg->msgBits, msg->paddedBitLen, separator, newLines, 37, msg->originalBitLen+msg->initPad, crc->n, space1, space2, 0); 
    } 
    if (PROG.verbose) { puts("Message post calculation:"); i2p(msg->msgBits, msg->paddedBitLen, 0, 0, 1);  }

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
            i2pc(xorBits, crc->n, 0, 1, 34, 0, crc->n, -1, -1, msg->paddedBitLen - crc->n + (crc->init ? 11 : 10) -8); // 8 is len of "    XOR:"
            i2pc(remBits, crc->n, 0, 1, 37, 0, crc->n, -1, -1, msg->paddedBitLen - crc->n + (crc->init ? 11 : 10) ); 
        }
    }

    // Convert remBits to int with choice of bit ordering
    uint64_t rem;
    if (crc->n <= 64) {
        if ( crc->resultLSF )
            rem = (uint64_t)bits2intLSF(COUNT_OF(remBits), remBits);
        else
            rem = (uint64_t)bits2intMSF(COUNT_OF(remBits), remBits);
        char fmt[10];
        sprintf(fmt, "%%#0%dllx", crc->hex_orders + 2);
        sprintf(msg->w_rem, fmt, rem);
        // if (PROG.verbose) printf("Remainder: %#llx\n", rem);
    }
    #ifdef WIDE_CRC
    else {
        if ( crc->resultLSF ) {
            bits2hexstrLSF(crc->n, remBits, msg->w_rem);
        }
        else
            bits2hexstrMSF(crc->n, remBits, msg->w_rem);

        rem = strtoull(msg->w_rem, NULL, 16);

        // size_t len_with_zeroes_only = strspn(msg->w_rem, "x0");
        // if (len_with_zeroes_only == strlen(msg->w_rem)) 
        //     rem = 0;
    }
    if (PROG.verbose) printf("Remainder (w_rem):\t%s\n", msg->w_rem);
    #endif

    if (PROG.verbose) printf("Remainder   (rem):\t%#llx\n", rem);
    return rem;
}
/** end Internal engine *********************************************************/
