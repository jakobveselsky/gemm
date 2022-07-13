#!/bin/bash
for i in $(seq 4 4 996)
do
	echo $i, $(./omp_threads.o $i)     
done
