#!/bin/bash
echo Performance test	
for c in 0 11 34 44
do
    ../crc perf -c $c
done
