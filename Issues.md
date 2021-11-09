# Issues

## Win

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

