#!/bin/bash

for ((i=0; i <= 1000; i+=4))
do
	./simd_matrix_vecotor_mul $i
done
