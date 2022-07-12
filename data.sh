#!/bin/bash
for i in $(seq 4 4 996)
do
	#echo $i, $(./simd.o $i)
       echo $i, $(./simd.o $i)       
done
