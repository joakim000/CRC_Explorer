#!/bin/bash
LAST_SPEC=62
echo Implementation test	
for c in {0..62}
do
    ../crc imp -c $c
done
