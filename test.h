/** Testing ******************************************************************/
/**
 * @brief Validate encode phase of CRC implementation for a CRC definition
 * 
 * @param crc       CRC definition
 * @param output    Print text to stdout?
 *                  0: None
 *                  1: One-line
 *                  2: Verbose
 * @return true     Passed
 * @return false    Failed
 */
uint64_t ValueCheckTest(crc_t* crc, uint8_t type, uint8_t output);

/**
 * @brief Additional scenarios tested with check value
 *        1. Encode => check value
 *        2. Validate with check value => 0
 *        3. Validate with changed message => !0
 * 
 * @param crc 
 * @param msg 
 * @return implTest_t 
 */
implTest_t ImplValid(crc_t* crc);

/**
 * @brief Performance tests
 * 
 * @param crc 
 * @param set_size 
 * @return implTest_t 
 */
implTest_t ImplPerf(crc_t* crc, uint64_t set_size);

/**
 * @brief Custom value check 
 * @note  Checks message AB by default
 * 
 */
bool CustomValueCheck(crc_t* crc, msg_t* msg);
/** end Testing **************************************************************/