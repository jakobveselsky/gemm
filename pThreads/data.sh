#!/bin/bash
for i in $(seq 3 3 999)
do
	echo $i, $(./pthreads.o $i)     
done
