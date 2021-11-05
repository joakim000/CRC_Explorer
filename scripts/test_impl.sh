#!/bin/bash
LAST_SPEC=46
echo Implementation test	
for c in {0..46}
do
    ../crc imp -c $c
done
