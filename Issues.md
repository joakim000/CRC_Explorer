# Issues

## Win

## Deb

* Read files (too many chars)                    Garbage chars in input              files.c

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

