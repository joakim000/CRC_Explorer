#!/bin/bash
echo Performance test	
for c in 0 23 49 60
do
    ../crc.exe perf -c $c
done
