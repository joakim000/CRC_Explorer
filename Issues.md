# Issues

## Win

* Wide CRC not working, error in generator conversion
PS R:\code\CRC_Explorer> .\crc enc -m A -c 63
      Poly converted bits (129):
000000000000000000000000000000000000000000000000000110000100011000000000000000000000000000000000001111111111111111111111111111111

joakim@Porcelina:/mnt/r/code/CRC_Explorer$ ./crc enc -m A -c 63
      Poly converted bits (129):
000000000000000000000000000000000000000000000000000110000100011000000000100010001000000010001010000000001010001000000010000010001


PS R:\code\CRC_Explorer> .\crc perf -c 63 -steps
CRC-82/DARC   Poly:0x0308c0111011401440411   IL1:Yes   Init:0x00   NDI:No   XorOut:0x00   RefIn:Yes   RefOut:Yes
Engine 0: Failed check value-test for CRC-82/DARC; result 0x20106ffaafc6e9bbcdd43 != check 0x09ea83f625023801fd612
          Message                                                                            Padding
 Before: 10000010 0000000000000000000000000000000000000000000000000000000000000000000000000000000000
         10000110 000100011000000000000000000000000000000000001111111111111111111111111111111           <----
   @  0: 00000100 0001000110000000000000000000000000000000000011111111111111111111111111111110000000
              100 00110000100011000000000000000000000000000000000001111111111111111111111111111111
   @  5: 00000000 0010000100001100000000000000000000000000000011111000000000000000000000000001111100
  After: 00000000 0010000100001100000000000000000000000000000011111000000000000000000000000001111100
  Encode:          1 chars in  0.015 seconds,  0.000 MiB/s.
          Message                                                                            Padding
 Before: 10000010 1111111111111111111111111111111000000000000000000000000000000000000000000001111100
         10000110 000100011000000000000000000000000000000000001111111111111111111111111111111           <----
   @  0: 00000100 1110111001111111111111111111111000000000000011111111111111111111111111111111111100
              100 00110000100011000000000000000000000000000000000001111111111111111111111111111111
   @  5: 00000000 1101111011110011111111111111111000000000000011111000000000000000000000000000000000
  After: 00000000 1101111011110011111111111111111000000000000011111000000000000000000000000000000000
Validate:          1 chars in  0.016 seconds,  0.000 MiB/s. Failed.
Compare to Deb:
joakim@Porcelina:/mnt/r/code/CRC_Explorer$ ./crc perf -c 63 -steps
CRC-82/DARC   Poly:0x0308c0111011401440411   IL1:Yes   Init:0x00   NDI:No   XorOut:0x00   RefIn:Yes   RefOut:Yes
Engine 0: Passed check value-test for CRC-82/DARC; matching 0x09ea83f625023801fd612
          Message                                                                            Padding
 Before: 10000010 0000000000000000000000000000000000000000000000000000000000000000000000000000000000
         10000110 000100011000000000100010001000000010001010000000001010001000000010000010001
   @  0: 00000100 0001000110000000001000100010000000100010100000000010100010000000100000100010000000
              100 00110000100011000000000100010001000000010001010000000001010001000000010000010001
   @  5: 00000000 0010000100001100001000110011000100100011100101000010100111000100100001100011000100
  After: 00000000 0010000100001100001000110011000100100011100101000010100111000100100001100011000100
  Encode:          1 chars in  0.000 seconds,    inf MiB/s.
          Message                                                                            Padding
 Before: 10000010 0010000100001100001000110011000100100011100101000010100111000100100001100011000100
         10000110 000100011000000000100010001000000010001010000000001010001000000010000010001
   @  0: 00000100 0011000010001100000000010001000100000001000101000000000101000100000001000001000100
              100 00110000100011000000000100010001000000010001010000000001010001000000010000010001
   @  5: 00000000 0000000000000000000000000000000000000000000000000000000000000000000000000000000000
  After: 00000000 0000000000000000000000000000000000000000000000000000000000000000000000000000000000
Validate:          1 chars in  0.000 seconds,    inf MiB/s. Passed.

## Deb

* Read files (too many chars)                    Garbage chars in input              files.c
PS R:\code\CRC_Explorer> ./crc val -in troll.txt
CRC-8/SMBUS   Poly:0x7   IL1:Yes   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8/SMBUS; matching 0xf4
Reading file troll.txt  ...  21 characters read.
Checksum:               0x49     CRC-8/SMBUS
Message to validate:    ABF
The data is OK

joakim@Porcelina:/mnt/r/code/CRC_Explorer$ ./crc val -in troll.txt
CRC-8/SMBUS   Poly:0x7   IL1:Yes   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8/SMBUS; matching 0xf4
Reading file troll.txt  ...  22 characters read.
Checksum:               0x49     CRC-8/SMBUS
Message to validate:    ABF
The data is not OK

* Intermittent perf validation errors
CRC-82/DARC   Poly:0x0308c0111011401440411   IL1:Yes   Init:0x00   NDI:No   XorOut:0x00   RefIn:Yes   RefOut:Yes
Passed check value-test for CRC-82/DARC; matching 0x09ea83f625023801fd612
Reading file ./assets/lorem32k.asc  ...  33990 characters read.
  Encode:      33990 chars in  0.016 seconds,  2.075 MiB/s.
Validate:      33990 chars in  0.000 seconds,    inf MiB/s. Failed.
joakim@Porcelina:/mnt/r/code/CRC_Explorer$ ./crc perf -c 63
CRC-82/DARC   Poly:0x0308c0111011401440411   IL1:Yes   Init:0x00   NDI:No   XorOut:0x00   RefIn:Yes   RefOut:Yes
Passed check value-test for CRC-82/DARC; matching 0x09ea83f625023801fd612
Reading file ./assets/lorem32k.asc  ...  33990 characters read.
  Encode:      33990 chars in  0.000 seconds,    inf MiB/s.
Validate:      33990 chars in  0.016 seconds,  2.075 MiB/s. Passed.

## Both

* Read files, constant read size  (set to 16M)                                       files.c

* To many chars read
PS R:\code\CRC_Explorer> .\crc enc -t -in .\assets\lorem32k.txt -out test.txt
CRC-8/SMBUS   Poly:0X7   IL1:Yes   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8/SMBUS; matching 0XF4
Reading file .\assets\lorem32k.txt  ...  33792 characters read.
Message:        [33792 characters]
Checksum:       0XC8
33792 chars in 0.002 seconds, 16.113 MiB/s.
PS R:\code\CRC_Explorer> .\crc -t val -in .\test.txt
CRC-8/SMBUS   Poly:0X7   IL1:Yes   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8/SMBUS; matching 0XF4
Reading file .\test.txt  ...  34816 characters read.
Checksum:               0XC8
Message to validate:    [34810 characters]
===
joakim@Porcelina:/mnt/r/code/CRC_Explorer$ ./crc enc -t -in ./assets/lorem32k.txt -out testdeb.txt
CRC-8/SMBUS   Poly:0X7   IL1:Yes   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8/SMBUS; matching 0XF4
Reading file ./assets/lorem32k.txt  ...  33792 characters read.
Message:        [33792 characters]
Checksum:       0X29
33792 chars in 0.000 seconds,   inf MiB/s.
joakim@Porcelina:/mnt/r/code/CRC_Explorer$ ./crc val -t -in testdeb.txt
CRC-8/SMBUS   Poly:0X7   IL1:Yes   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8/SMBUS; matching 0XF4
Reading file testdeb.txt  ...  34816 characters read.
Checksum:               0
34810 chars in 0.000 seconds,   inf MiB/s.
Message to validate:    [34810 characters]
The data is not OK

* segfault on file not found:
joakim@Porcelina:/mnt/r/code/CRC_Explorer$ ./crc enc -t -in .\assets\lorem32k.txt -out testdeb.txt
CRC-8/SMBUS   Poly:0X7   IL1:Yes   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8/SMBUS; matching 0XF4
File not found.Segmentation fault (core dumped)

PS R:\code\CRC_Explorer> .\crc enc -t -m Troll -out test.txt
CRC-8/SMBUS   Poly:0X7   IL1:Yes   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8/SMBUS; matching 0XF4
Message:        Troll
Checksum:       0XE3
5 chars in 0.000 seconds, 1.#IO MiB/s.
PS R:\code\CRC_Explorer> .\crc val -t -in test.txt
CRC-8/SMBUS   Poly:0X7   IL1:Yes   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8/SMBUS; matching 0XF4
Reading file test.txt  ...  12 characters read.
Checksum:               0
6 chars in 0.000 seconds, 1.#IO MiB/s.
Message to validate:    Troll                         <-trailing space
The data is not OK

joakim@Porcelina:/mnt/r/code/CRC_Explorer$ ./crc val -t -in test.txt
CRC-8/SMBUS   Poly:0X7   IL1:Yes   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8/SMBUS; matching 0XF4
Reading file test.txt  ...  11 characters read.
Checksum:               0
5 chars in 0.000 seconds,   inf MiB/s.
Message to validate:    Troll
The data is not OK

