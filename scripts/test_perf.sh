#!/bin/bash
echo Performance test	
for c in 0 23 49 60
do
    ../crc perf -c $c
done
