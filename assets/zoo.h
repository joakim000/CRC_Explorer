// Sources: https://crccalc.com/                                 
//          http://srecord.sourceforge.net/crc16-ccitt.html      
//          https://reveng.sourceforge.io/crc-catalogue/all.htm  

     // Name                  0 n   1 Gen          2 IL1 3 Init 4 Nondir. 5 RefIn 6 RefOut 7 XorOut   8 Residue              9 Check             10 AB
#ifndef CRC_ZOO
#define CRC_ZOO \
    { "CRC-8/SMBUS",       { 8,    0x07,               1, 0x00,               0, 0, 0, 0x00,               0,                  0xF4,               0x87,                }}, \
    { "CRC-3/GSM",         { 3,    0x3,                1, 0x0,                0, 0, 0, 0x7,                0x2,                0x4,                0,                   }}, \
    { "CRC-4/G-704",       { 4,    0x3,                1, 0x0,                0, 1, 1, 0x0,                0x0,                0x7,                0,                   }}, \
    { "CRC-5/USB",         { 5,    0x05,               1, 0x1f,               0, 1, 1, 0x1f,               0x06,               0x19,               0,                   }}, \
    { "CRC-6/G-704",       { 6,    0x03,               1, 0x00,               0, 1, 1, 0x00,               0x00,               0x06,               0,                   }}, \
    { "CRC-7/MMC",         { 7,    0x09,               1, 0x00,               0, 0, 0, 0x00,               0x00,               0x75,               0,                   }}, \
    { "CRC-8/AUTOSAR",     { 8,    0x2f,               1, 0xff,               0, 0, 0, 0xff,               0x42,               0xdf,               0,                   }}, \
    { "CRC-8/BLUETOOTH",   { 8,    0xa7,               1, 0x00,               0, 1, 1, 0x00,               0x00,               0x26,               0,                   }}, \
    { "CRC-8/CDMA2000",    { 8,    0x9B,               1, 0xFF,               0, 0, 0, 0x00,               0,                  0xDA,               0x18,                }}, \
    { "CRC-8/DARC",        { 8,    0x39,               1, 0x00,               0, 1, 1, 0x00,               0,                  0x15,               0x28,                }}, \
    { "CRC-8/DVB-S2",      { 8,    0xD5,               1, 0x00,               0, 0, 0, 0x00,               0,                  0xBC,               0x56,                }}, \
    { "CRC-8/EBU",         { 8,    0x1D,               1, 0xFF,               0, 1, 1, 0x00,               0,                  0x97,               0x9C,                }}, \
    { "CRC-8/I-CODE",      { 8,    0x1D,               1, 0xFD,               0, 0, 0, 0x00,               0,                  0x7E,               0x56,                }}, \
    { "CRC-8/ITU",         { 8,    0x07,               1, 0x00,               0, 0, 0, 0x55,               0,                  0xA1,               0xD2,                }}, \
    { "CRC-8/MAXIM",       { 8,    0x31,               1, 0x00,               0, 1, 1, 0x00,               0,                  0xA1,               0xA5,                }}, \
    { "CRC-8/ROHC",        { 8,    0x07,               1, 0xFF,               0, 1, 1, 0x00,               0,                  0xD0,               0x41,                }}, \
    { "CRC-8/WCDMA",       { 8,    0x9B,               1, 0x00,               0, 1, 1, 0x00,               0,                  0x25,               0x66,                }}, \
    { "CRC-10/ATM",        { 10,   0x233,              1, 0x000,              0, 0, 0, 0x000,              0x000,              0x199,              0,                   }}, \
    { "CRC-11/FLEXRAY",    { 11,   0x385,              1, 0x01a,              0, 0, 0, 0x000,              0x000,              0x5a3,              0,                   }}, \
    { "CRC-12/DECT",       { 12,   0x80f,              1, 0x000,              0, 0, 0, 0x000,              0x000,              0xf5b,              0,                   }}, \
    { "CRC-13/BBC",        { 13,   0x1cf5,             1, 0x0000,             0, 0, 0, 0x0000,             0x0000,             0x04fa,             0,                   }}, \
    { "CRC-14/DARC",       { 14,   0x0805,             1, 0x0000,             0, 1, 1, 0x0000,             0x0000,             0x082d,             0,                   }}, \
    { "CRC-15/CAN",        { 15,   0x4599,             1, 0x00,               0, 0, 0, 0x0000,             0x0000,             0x059e,             0,                   }}, \
    { "CRC-16/ARC",        { 16,   0x8005,             1, 0x0000,             0, 1, 1, 0x0000,             0,                  0xBB3D,             0x61B0,              }}, \
    { "CRC-16/CCITT",      { 16,   0x1021,             1, 0xFFFF,             1, 0, 0, 0x0000,             0,                  0xE5CC,             0XD2BB               }}, \
    { "CRC-16/AUG-CCITT",  { 16,   0x1021,             1, 0x1D0F,             0, 0, 0, 0x0000,             0,                  0xE5CC,             0xD2BB,              }}, \
    { "CRC-16/T10-DIF",    { 16,   0x8BB7,             1, 0x0000,             0, 0, 0, 0x0000,             0,                  0xD0DB,             0xEB7A,              }}, \
    { "CRC-16/TELEDISK",   { 16,   0xA097,             1, 0x0000,             0, 0, 0, 0x0000,             0,                  0x0FB3,             0x4D99,              }}, \
    { "CRC-16/BUYPASS",    { 16,   0x8005,             1, 0x0000,             0, 0, 0, 0x0000,             0,                  0xFEE8,             0x0789,              }}, \
    { "CRC-16/USB",        { 16,   0x8005,             1, 0xFFFF,             0, 1, 1, 0xFFFF,             0,                  0xB4C8,             0x2E4E,              }}, \
    { "CRC-16/CDMA2000",   { 16,   0xC867,             1, 0xFFFF,             0, 0, 0, 0x0000,             0,                  0x4C06,             0x144F,              }}, \
    { "CRC-16/DDS-110",    { 16,   0x8005,             1, 0x800D,             0, 0, 0, 0x0000,             0,                  0x9ECF,             0x07AD,              }}, \
    { "CRC-16/DECT-R",     { 16,   0x0589,             1, 0x0000,             0, 0, 0, 0x0001,             0,                  0x007E,             0xF121,              }}, \
    { "CRC-16/DNP",        { 16,   0x3D65,             1, 0x0000,             0, 1, 1, 0xFFFF,             0,                  0xEA82,             0xC078,              }}, \
    { "CRC-16/EN-13757",   { 16,   0x3D65,             1, 0x0000,             0, 0, 0, 0xFFFF,             0,                  0xC2B7,             0xEB03,              }}, \
    { "CRC-16/GENIBUS",    { 16,   0x1021,             1, 0xFFFF,             0, 0, 0, 0xFFFF,             0,                  0xD64E,             0xB48B,              }}, \
    { "CRC-16/MAXIM",      { 16,   0x8005,             1, 0x0000,             0, 1, 1, 0xFFFF,             0,                  0x44C2,             0x9E4F,              }}, \
    { "CRC-16/MCRF4XX",    { 16,   0x1021,             1, 0xFFFF,             0, 1, 1, 0x0000,             0,                  0x6F91,             0xCE10,              }}, \
    { "CRC-16/RIELLO",     { 16,   0x1021,             1, 0xB2AA,             0, 1, 1, 0x0000,             0,                  0x63D0,             0xCD9E,              }}, \
    { "CRC-16/TMS37157",   { 16,   0x1021,             1, 0x89EC,             0, 1, 1, 0x0000,             0,                  0x26B1,             0x7B11,              }}, \
    { "CRC-16/KERMIT",     { 16,   0x1021,             1, 0x0000,             0, 1, 1, 0x0000,             0,                  0x2189,             0x3EA8,              }}, \
    { "CRC-16/MODBUS",     { 16,   0x8005,             1, 0xFFFF,             0, 1, 1, 0x0000,             0,                  0x4B37,             0xD1B1,              }}, \
    { "CRC-16/X-25",       { 16,   0x1021,             1, 0xFFFF,             0, 1, 1, 0xFFFF,             0,                  0x906E,             0x31EF,              }}, \
    { "CRC-16/XMODEM",     { 16,   0x1021,             1, 0x0000,             0, 0, 0, 0x0000,             0,                  0x31C3,             0x567B,              }}, \
    { "CRC-17/CAN-FD",     { 17,   0x1685b,            1, 0x00000,            0, 0, 0, 0x00000,            0x00000,            0x04f03,            0,                   }}, \
    { "CRC-21/CAN-FD",     { 21,   0x102899,           1, 0x000000,           0, 0, 0, 0x000000,           0x000000,           0x0ed841,           0,                   }}, \
    { "CRC-24/BLE",        { 24,   0x00065b,           1, 0x555555,           0, 1, 1, 0x000000,           0x000000,           0xc25a56,           0,                   }}, \
    { "CRC-30/CDMA",       { 30,   0x2030b9c7,         1, 0x3fffffff,         0, 0, 0, 0x3fffffff,         0x34efa55a,         0x04c34abf,         0,                   }}, \
    { "CRC-31/PHILIPS",    { 31,   0x04c11db7,         1, 0x7fffffff,         0, 0, 0, 0x7fffffff,         0x4eaf26f1,         0x0ce9e46c,         0,                   }}, \
    { "CRC-32",            { 32,   0x04C11DB7,         1, 0xFFFFFFFF,         0, 1, 1, 0xFFFFFFFF,         0,                  0xCBF43926,         0x30694C07,          }}, \
    { "CRC-32/AUTOSAR",    { 32,   0xf4acfb13,         1, 0xffffffff,         0, 1, 1, 0xffffffff,         0x904cddbf,         0x1697d06a,         0,                   }}, \
    { "CRC-32/BZIP2",      { 32,   0x04C11DB7,         1, 0xFFFFFFFF,         0, 0, 0, 0xFFFFFFFF,         0,                  0xFC891918,         0x51137D0B,          }}, \
    { "CRC-32/POSIX",      { 32,   0x04C11DB7,         1, 0x00000000,         0, 0, 0, 0xFFFFFFFF,         0,                  0x765E7680,         0x51A41976,          }}, \
    { "CRC-32Q",           { 32,   0x814141AB,         1, 0x00000000,         0, 0, 0, 0x00000000,         0,                  0x3010BF7F,         0x53BFD78F,          }}, \
    { "CRC-32/XFER",       { 32,   0x000000AF,         1, 0x00000000,         0, 0, 0, 0x00000000,         0,                  0xBD0BE338,         0x002B459E,          }}, \
    { "CRC-32C",           { 32,   0x1EDC6F41,         1, 0xFFFFFFFF,         0, 1, 1, 0xFFFFFFFF,         0,                  0xE3069283,         0xBD9444EA,          }}, \
    { "CRC-32D",           { 32,   0xA833982B,         1, 0xFFFFFFFF,         0, 1, 1, 0xFFFFFFFF,         0,                  0x87315576,         0x9BF68F39,          }}, \
    { "CRC-32/MPEG-2",     { 32,   0x04C11DB7,         1, 0xFFFFFFFF,         0, 0, 0, 0x00000000,         0,                  0x0376E6E7,         0xAEEC82F4,          }}, \
    { "CRC-32/JAMCRC",     { 32,   0x04C11DB7,         1, 0xFFFFFFFF,         0, 1, 1, 0x00000000,         0,                  0x340BC6D9,         0xCF96B3F8,          }}, \
	{ "CRC-40/GSM",        { 40,   0x0004820009,       1, 0x0000000000,       0, 0, 0, 0xffffffffff,       0xc4ff8071ff,       0xd4164fc646,       0XDAD0FBB4A4,        }}, \
	{ "CRC-64/ECMA-182",   { 64,   0x42f0e1eba9ea3693, 1, 0x0000000000000000, 0, 0, 0, 0x0000000000000000, 0x0000000000000000, 0x6c40df5f0b497347, 0XA8A66E34CEF9D895,  }}, \
	{ "CRC-64/GO-ISO",     { 64,   0x000000000000001b, 1, 0xffffffffffffffff, 0, 1, 1, 0xffffffffffffffff, 0x5300000000000000, 0xb90956c775a41001, 0XF2200000000000,    }}, \
    { "CRC-64/XZ",         { 64,   0x42f0e1eba9ea3693, 1, 0xffffffffffffffff, 0, 1, 1, 0xffffffffffffffff, 0x49958c9abd7d353f, 0x995dc9bbdf1939fa, 0X7DAC6E8F2B4D348,   }}, \
    // { "CRC-82/DARC",       { 82,   0x0308c0111011401440411,  1,          0x0, 0, 1, 1,                0x0,                0x0, 0x09ea83f625023801fd612,  0,             }}, 
#endif
    // Name                  0 n   1 Gen           2 IL1   3 Init      4 Nondirect  5 RefIn  6 RefOut  7 XorOut         8 Residue           9 Check             10 AB              

#ifndef W_CRC_ZOO
#define W_CRC_ZOO \
 { "CRC-8/SMBUS",       8,    "0x07",               1, "0x00",               0, 0, 0, "0x00",               "0",                  "0xf4",               "0x87",                }, \
    { "CRC-3/GSM",         3,    "0x3",                1, "0x0",                0, 0, 0, "0x7",                "0x2",                "0x4",                "0",                   }, \
    { "CRC-4/G-704",       4,    "0x3",                1, "0x0",                0, 1, 1, "0x0",                "0x0",                "0x7",                "0",                   }, \
    { "CRC-5/USB",         5,    "0x05",               1, "0x1f",               0, 1, 1, "0x1f",               "0x06",               "0x19",               "0",                   }, \
    { "CRC-6/G-704",       6,    "0x03",               1, "0x00",               0, 1, 1, "0x00",               "0x00",               "0x06",               "0",                   }, \
    { "CRC-7/MMC",         7,    "0x09",               1, "0x00",               0, 0, 0, "0x00",               "0x00",               "0x75",               "0",                   }, \
    { "CRC-8/AUTOSAR",     8,    "0x2f",               1, "0xff",               0, 0, 0, "0xff",               "0x42",               "0xdf",               "0",                   }, \
    { "CRC-8/BLUETOOTH",   8,    "0xa7",               1, "0x00",               0, 1, 1, "0x00",               "0x00",               "0x26",               "0",                   }, \
    { "CRC-8/CDMA2000",    8,    "0x9b",               1, "0xff",               0, 0, 0, "0x00",               "0",                  "0xda",               "0x18",                }, \
    { "CRC-8/DARC",        8,    "0x39",               1, "0x00",               0, 1, 1, "0x00",               "0",                  "0x15",               "0x28",                }, \
    { "CRC-8/DVB-S2",      8,    "0xd5",               1, "0x00",               0, 0, 0, "0x00",               "0",                  "0xbc",               "0x56",                }, \
    { "CRC-8/EBU",         8,    "0x1d",               1, "0xff",               0, 1, 1, "0x00",               "0",                  "0x97",               "0x9c",                }, \
    { "CRC-8/I-CODE",      8,    "0x1d",               1, "0xfd",               0, 0, 0, "0x00",               "0",                  "0x7e",               "0x56",                }, \
    { "CRC-8/ITU",         8,    "0x07",               1, "0x00",               0, 0, 0, "0x55",               "0",                  "0xa1",               "0xd2",                }, \
    { "CRC-8/MAXIM",       8,    "0x31",               1, "0x00",               0, 1, 1, "0x00",               "0",                  "0xa1",               "0xa5",                }, \
    { "CRC-8/ROHC",        8,    "0x07",               1, "0xff",               0, 1, 1, "0x00",               "0",                  "0xd0",               "0x41",                }, \
    { "CRC-8/WCDMA",       8,    "0x9b",               1, "0x00",               0, 1, 1, "0x00",               "0",                  "0x25",               "0x66",                }, \
    { "CRC-10/ATM",        10,   "0x233",              1, "0x000",              0, 0, 0, "0x000",              "0x000",              "0x199",              "0",                   }, \
    { "CRC-11/FLEXRAY",    11,   "0x385",              1, "0x01a",              0, 0, 0, "0x000",              "0x000",              "0x5a3",              "0",                   }, \
    { "CRC-12/DECT",       12,   "0x80f",              1, "0x000",              0, 0, 0, "0x000",              "0x000",              "0xf5b",              "0",                   }, \
    { "CRC-13/BBC",        13,   "0x1cf5",             1, "0x0000",             0, 0, 0, "0x0000",             "0x0000",             "0x04fa",             "0",                   }, \
    { "CRC-14/DARC",       14,   "0x0805",             1, "0x0000",             0, 1, 1, "0x0000",             "0x0000",             "0x082d",             "0",                   }, \
    { "CRC-15/CAN",        15,   "0x4599",             1, "0x00",               0, 0, 0, "0x0000",             "0x0000",             "0x059e",             "0",                   }, \
    { "CRC-15/CAN*",      15,  "0xC599",             0, "0x00",               0, 1, 0, "0x0000",             "0",                  "0x5136",             "0x48B1",              }, \
    { "CRC-16/ARC",        16,   "0x8005",             1, "0x0000",             0, 1, 1, "0x0000",             "0",                  "0xbb3d",             "0x61b0",              }, \
    { "CRC-16/CCITT",      16,   "0x1021",             1, "0xffff",             1, 0, 0, "0x0000",             "0",                  "0xe5cc",             "0xd2bb"               }, \
    { "CRC-16/AUG-CCITT",  16,   "0x1021",             1, "0x1d0f",             0, 0, 0, "0x0000",             "0",                  "0xe5cc",             "0xd2bb",              }, \
    { "CRC-16/T10-DIF",    16,   "0x8bb7",             1, "0x0000",             0, 0, 0, "0x0000",             "0",                  "0xd0db",             "0xeb7a",              }, \
    { "CRC-16/TELEDISK",   16,   "0xa097",             1, "0x0000",             0, 0, 0, "0x0000",             "0",                  "0x0fb3",             "0x4d99",              }, \
    { "CRC-16/BUYPASS",    16,   "0x8005",             1, "0x0000",             0, 0, 0, "0x0000",             "0",                  "0xfee8",             "0x0789",              }, \
    { "CRC-16/USB",        16,   "0x8005",             1, "0xffff",             0, 1, 1, "0xffff",             "0",                  "0xb4c8",             "0x2e4e",              }, \
    { "CRC-16/CDMA2000",   16,   "0xc867",             1, "0xffff",             0, 0, 0, "0x0000",             "0",                  "0x4c06",             "0x144f",              }, \
    { "CRC-16/DDS-110",    16,   "0x8005",             1, "0x800d",             0, 0, 0, "0x0000",             "0",                  "0x9ecf",             "0x07ad",              }, \
    { "CRC-16/DECT-R",     16,   "0x0589",             1, "0x0000",             0, 0, 0, "0x0001",             "0",                  "0x007e",             "0xf121",              }, \
    { "CRC-16/DNP",        16,   "0x3d65",             1, "0x0000",             0, 1, 1, "0xffff",             "0",                  "0xea82",             "0xc078",              }, \
    { "CRC-16/EN-13757",   16,   "0x3d65",             1, "0x0000",             0, 0, 0, "0xffff",             "0",                  "0xc2b7",             "0xeb03",              }, \
    { "CRC-16/GENIBUS",    16,   "0x1021",             1, "0xffff",             0, 0, 0, "0xffff",             "0",                  "0xd64e",             "0xb48b",              }, \
    { "CRC-16/MAXIM",      16,   "0x8005",             1, "0x0000",             0, 1, 1, "0xffff",             "0",                  "0x44c2",             "0x9e4f",              }, \
    { "CRC-16/MCRF4XX",    16,   "0x1021",             1, "0xffff",             0, 1, 1, "0x0000",             "0",                  "0x6f91",             "0xce10",              }, \
    { "CRC-16/RIELLO",     16,   "0x1021",             1, "0xb2aa",             0, 1, 1, "0x0000",             "0",                  "0x63d0",             "0xcd9e",              }, \
    { "CRC-16/TMS37157",   16,   "0x1021",             1, "0x89ec",             0, 1, 1, "0x0000",             "0",                  "0x26b1",             "0x7b11",              }, \
    { "CRC-16/KERMIT",     16,   "0x1021",             1, "0x0000",             0, 1, 1, "0x0000",             "0",                  "0x2189",             "0x3ea8",              }, \
    { "CRC-16/MODBUS",     16,   "0x8005",             1, "0xffff",             0, 1, 1, "0x0000",             "0",                  "0x4b37",             "0xd1b1",              }, \
    { "CRC-16/X-25",       16,   "0x1021",             1, "0xffff",             0, 1, 1, "0xffff",             "0",                  "0x906e",             "0x31ef",              }, \
    { "CRC-16/XMODEM",     16,   "0x1021",             1, "0x0000",             0, 0, 0, "0x0000",             "0",                  "0x31c3",             "0x567b",              }, \
    { "CRC-17/CAN-FD",     17,   "0x1685b",            1, "0x00000",            0, 0, 0, "0x00000",            "0x00000",            "0x04f03",            "0",                   }, \
    { "CRC-21/CAN-FD",     21,   "0x102899",           1, "0x000000",           0, 0, 0, "0x000000",           "0x000000",           "0x0ed841",           "0",                   }, \
    { "CRC-24/BLE",        24,   "0x00065b",           1, "0x555555",           0, 1, 1, "0x000000",           "0x000000",           "0xc25a56",           "0",                   }, \
    { "CRC-30/CDMA",       30,   "0x2030b9c7",         1, "0x3fffffff",         0, 0, 0, "0x3fffffff",         "0x34efa55a",         "0x04c34abf",         "0",                   }, \
    { "CRC-31/PHILIPS",    31,   "0x04c11db7",         1, "0x7fffffff",         0, 0, 0, "0x7fffffff",         "0x4eaf26f1",         "0x0ce9e46c",         "0",                   }, \
    { "CRC-32",            32,   "0x04c11db7",         1, "0xffffffff",         0, 1, 1, "0xffffffff",         "0",                  "0xcbf43926",         "0x30694c07",          }, \
    { "CRC-32/AUTOSAR",    32,   "0xf4acfb13",         1, "0xffffffff",         0, 1, 1, "0xffffffff",         "0x904cddbf",         "0x1697d06a",         "0",                   }, \
    { "CRC-32/BZIP2",      32,   "0x04c11db7",         1, "0xffffffff",         0, 0, 0, "0xffffffff",         "0",                  "0xfc891918",         "0x51137d0b",          }, \
    { "CRC-32/POSIX",      32,   "0x04c11db7",         1, "0x00000000",         0, 0, 0, "0xffffffff",         "0",                  "0x765e7680",         "0x51a41976",          }, \
    { "CRC-32Q",           32,   "0x814141ab",         1, "0x00000000",         0, 0, 0, "0x00000000",         "0",                  "0x3010bf7f",         "0x53bfd78f",          }, \
    { "CRC-32/XFER",       32,   "0x000000af",         1, "0x00000000",         0, 0, 0, "0x00000000",         "0",                  "0xbd0be338",         "0x002b459e",          }, \
    { "CRC-32C",           32,   "0x1edc6f41",         1, "0xffffffff",         0, 1, 1, "0xffffffff",         "0",                  "0xe3069283",         "0xbd9444ea",          }, \
    { "CRC-32D",           32,   "0xa833982b",         1, "0xffffffff",         0, 1, 1, "0xffffffff",         "0",                  "0x87315576",         "0x9bf68f39",          }, \
    { "CRC-32/MPEG-2",     32,   "0x04c11db7",         1, "0xffffffff",         0, 0, 0, "0x00000000",         "0",                  "0x0376e6e7",         "0xaeec82f4",          }, \
    { "CRC-32/JAMCRC",     32,   "0x04c11db7",         1, "0xffffffff",         0, 1, 1, "0x00000000",         "0",                  "0x340bc6d9",         "0xcf96b3f8",          }, \
	{ "CRC-40/GSM",        40,   "0x0004820009",       1, "0x0000000000",       0, 0, 0, "0xffffffffff",       "0xc4ff8071ff",       "0xd4164fc646",       "0xdad0fbb4a4",        }, \
	{ "CRC-64/ECMA-182",   64,   "0x42f0e1eba9ea3693", 1, "0x0000000000000000", 0, 0, 0, "0x0000000000000000", "0x0000000000000000", "0x6c40df5f0b497347", "0xa8a66e34cef9d895",  }, \
	{ "CRC-64/GO-ISO",     64,   "0x000000000000001b", 1, "0xffffffffffffffff", 0, 1, 1, "0xffffffffffffffff", "0x5300000000000000", "0xb90956c775a41001", "0xf2200000000000",    }, \
    { "CRC-64/XZ",         64,   "0x42f0e1eba9ea3693", 1, "0xffffffffffffffff", 0, 1, 1, "0xffffffffffffffff", "0x49958c9abd7d353f", "0x995dc9bbdf1939fa", "0x7dac6e8f2b4d348",   }, \
    { "CRC-82/DARC",       82,   "0x0308c0111011401440411",  1,       "0x00",   0, 1, 1, "0x00",               "0x0",                "0x09ea83f625023801fd612",  "0x2398aed5b31b3ff140531",             }, 
#endif


// Template
   //    { "",    { ,    ,               , ,               , , , ,               ,               ,               ,                }}, \

 

// Removed specs
  // { "CRC-15/CAN*",       { 15,   0xC599,             0, 0x00,               0, 1, 0, 0x0000,             0,                  0x5136,             0x48B1,              }}, \
  // { "CRC-16/DECT-X",     { 16,   0x0589,             1, 0x0000,             0, 0, 0, 0x0000,             0,                  0x007F,             0xF120,              }}, \
  // { "CRC-A (n=16)",      { 16,   0x1021,             1, 0xC6C6,             0, 1, 1, 0x0000,             0,                  0xBF05,             0x2008,              }}, 



/* Various references and notes */
// http://can.marathon.ru/files/can2spec.pdf   CAN15
// https://web.archive.org/web/20151017122935/http://www.bosch-semiconductors.de/media/pdf_1/canliteratur/can_fd_spec.pdf CAN17 / CAN21

// CRC-16/CCITT   
// Width = 16 bits
// Truncated polynomial = 0x1021
// Initial value = 0xFFFF
// Input data is NOT reflected
// Output CRC is NOT reflected
// No XOR is performed on the output CRC
// “Zero” bits explicitly appended to message: yes
// check value for message, “123456789”, using “the” CRC16-CCITT is 0xE5CC

// width=5 poly=0x09 init=0x09 refin=false refout=false xorout=0x00 check=0x00 residue=0x00 name="CRC-5/EPC-C1G2"   RFID
// width=5 poly=0x15 init=0x00 refin=true refout=true xorout=0x00 check=0x07 residue=0x00 name="CRC-5/G-704"
// width=6 poly=0x03 init=0x00 refin=true refout=true xorout=0x00 check=0x06 residue=0x00 name="CRC-6/G-704"
// width=6 poly=0x2f init=0x00 refin=false refout=false xorout=0x3f check=0x13 residue=0x3a name="CRC-6/GSM"
// width=12 poly=0x80f init=0x000 refin=false refout=false xorout=0x000 check=0xf5b residue=0x000 name="CRC-12/DECT"

//    name="CRC-82/DARC"
//   width=82
//    poly=0x0308c0111011401440411
//   check=0x09ea83f625023801fd612
//    init=0x000000000000000000000
//  xorout=0x000000000000000000000
// residue=0x000000000000000000000
//   refin=true 
//  refout=true 


// width=82 poly=0x0308c0111011401440411 init=0x000000000000000000000 refin=true refout=true xorout=0x000000000000000000000 check=0x09ea83f625023801fd612 residue=0x000000000000000000000 name="CRC-82/DARC"
// width=7 poly=0x09 init=0x00 refin=false refout=false xorout=0x00 check=0x75 residue=0x00 name="CRC-7/MMC"
// width=6 poly=0x03 init=0x00 refin=true refout=true xorout=0x00 check=0x06 residue=0x00 name="CRC-6/G-704"
// width=5 poly=0x05 init=0x1f refin=true refout=true xorout=0x1f check=0x19 residue=0x06 name="CRC-5/USB"
// width=4 poly=0x3 init=0x0 refin=true refout=true xorout=0x0 check=0x7 residue=0x0 name="CRC-4/G-704"
// width=3 poly=0x3 init=0x0 refin=false refout=false xorout=0x7 check=0x4 residue=0x2 name="CRC-3/GSM"
// width=8 poly=0xa7 init=0x00 refin=true refout=true xorout=0x00 check=0x26 residue=0x00 name="CRC-8/BLUETOOTH"
// width=11 poly=0x385 init=0x01a refin=false refout=false xorout=0x000 check=0x5a3 residue=0x000 name="CRC-11/FLEXRAY"
// width=14 poly=0x0805 init=0x0000 refin=true refout=true xorout=0x0000 check=0x082d residue=0x0000 name="CRC-14/DARC"
// width=30 poly=0x2030b9c7 init=0x3fffffff refin=false refout=false xorout=0x3fffffff check=0x04c34abf residue=0x34efa55a name="CRC-30/CDMA"
// width=10 poly=0x233 init=0x000 refin=false refout=false xorout=0x000 check=0x199 residue=0x000 name="CRC-10/ATM"
// width=8 poly=0x2f init=0xff refin=false refout=false xorout=0xff check=0xdf residue=0x42 name="CRC-8/AUTOSAR"
// width=32 poly=0xf4acfb13 init=0xffffffff refin=true refout=true xorout=0xffffffff check=0x1697d06a residue=0x904cddbf name="CRC-32/AUTOSAR"
// width=31 poly=0x04c11db7 init=0x7fffffff refin=false refout=false xorout=0x7fffffff check=0x0ce9e46c residue=0x4eaf26f1 name="CRC-31/PHILIPS"
// width=8 poly=0x07 init=0x00 refin=false refout=false xorout=0x00 check=0xf4 residue=0x00 name="CRC-8/SMBUS"
// width=13 poly=0x1cf5 init=0x0000 refin=false refout=false xorout=0x0000 check=0x04fa residue=0x0000 name="CRC-13/BBC"
// width=21 poly=0x102899 init=0x000000 refin=false refout=false xorout=0x000000 check=0x0ed841 residue=0x000000 name="CRC-21/CAN-FD"
// width=16 poly=0x1021 init=0x1d0f refin=false refout=false xorout=0x0000 check=0xe5cc residue=0x0000 name="CRC-16/SPI-FUJITSU"    non-direct?
// width=17 poly=0x1685b init=0x00000 refin=false refout=false xorout=0x00000 check=0x04f03 residue=0x00000 name="CRC-17/CAN-FD"
// width=15 poly=0x4599 init=0x0000 refin=false refout=false xorout=0x0000 check=0x059e residue=0x0000 name="CRC-15/CAN"
// width=40 poly=0x0004820009 init=0x0000000000 refin=false refout=false xorout=0xffffffffff check=0xd4164fc646 residue=0xc4ff8071ff name="CRC-40/GSM"
// width=64 poly=0x42f0e1eba9ea3693 init=0x0000000000000000 refin=false refout=false xorout=0x0000000000000000 check=0x6c40df5f0b497347 residue=0x0000000000000000 name="CRC-64/ECMA-182"
// width=64 poly=0x42f0e1eba9ea3693 init=0xffffffffffffffff refin=true refout=true xorout=0xffffffffffffffff check=0x995dc9bbdf1939fa residue=0x49958c9abd7d353f name="CRC-64/XZ"
// width=64 poly=0x000000000000001b init=0xffffffffffffffff refin=true refout=true xorout=0xffffffffffffffff check=0xb90956c775a41001 residue=0x5300000000000000 name="CRC-64/GO-ISO"

  
  
  
