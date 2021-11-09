# Issues

## Win

* SOLVED Wide CRC not working, error in generator conversion. 
Fix: In Hexstring2bits; strtol => strtoull

## Deb

* SOLVED Read files (too many chars)                    Garbage chars in input              files.c
Fix: strcat(fcontent, buf);  =>    strncat(fcontent, buf, elementsRead);

* SOLVED Intermittent perf validation errors.
Fix: In Hexstring2bits; strtol => strtoull

## Both

* Read files, constant read size  (set to 16M)                                       files.c

* SOLVED segfault on file not found:

