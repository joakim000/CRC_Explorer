/* Make errors to match assignment examples
    Input          Example result  Spec result
    "123456789"    0x67F5*         0x5136*       
    "AB"           0x54FB          0x48B1
    "ABC"          0xD59           0x6460
    "ABCD"         0x6531          0x5054
    "ABCDE"        0x3556          0x1AAB
    "Hello World!" 0xB35           0x6756
    "Faroch"       0x2535          0x7056
*/

// #define MATCH_EXAMPLES

#if defined(MATCH_EXAMPLES)
#define SPECIALWIDTH crc->n + 1             // Overpad by 1 bit
#define REMLOOPEND msgSize - crc->n    // Allow processing to continue after message end, 1 step / overpadding bit.
#else
#define SPECIALWIDTH crc->n                 // Normal padding
#define REMLOOPEND originalMsgSize       // Always stop processing at message end. This mitigates errors from overpadding.
#endif
