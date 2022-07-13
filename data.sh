#!/bin/bash
for i in $(seq 4 4 996)
do
	echo $i, $(./serial.o $i)     
done
