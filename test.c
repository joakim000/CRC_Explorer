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
    char* message = (char*)GetU8random(set_size, 100, NULL);
    msg_t* perf = PrepareMsg(crc, message);
    #ifdef WIDE_CRC
       perf->w_validation_rem = NULL;                
    #endif

    printf("Message: %llu ", message);
    // puts(message);
    timer_start = clock();
        perf->rem = GetRem_ptr(crc, perf, 0);
    timer_end = clock();

    double elapsed = TIMING(timer_start, timer_end);
    printf("  Encode: %10d chars in %6.3f seconds, %6.3f MiB/s.\n", perf->len, elapsed, perf->len / elapsed / 0x100000);

    // Validate
    #ifdef WIDE_CRC
       perf->w_validation_rem = perf->w_rem;                
    #endif

    printf("Message: %llu ", message);
    // puts(message);
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