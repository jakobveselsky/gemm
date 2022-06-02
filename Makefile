CC = gcc
UNAME_P := $(shell uname -p)

clean:
	rm -f simd_matrix_vecotor_mul-no-omp.s
	rm -f simd_matrix_vecotor_mul-openmp.s
	rm -f simd_matrix_vecotor_mul-openmp-simd.s

all: simd_matrix_vecotor_mul

simd_matrix_vecotor_mul: simd_matrix_vecotor_mul.c
    ifeq ($(UNAME_P),x86_64)
		$(CC) -S -march=native -o simd_matrix_vecotor_mul-no-omp.s simd_matrix_vecotor_mul.c -D__X86_SSE -O1 -lm 
		$(CC) -S -march=native -fopenmp -o simd_matrix_vecotor_mul-openmp.s simd_matrix_vecotor_mul.c -D__X86_SSE -O1 -lm
		$(CC) -S -march=native -fopenmp-simd -o simd_matrix_vecotor_mul-openmp-simd.s simd_matrix_vecotor_mul.c -D__X86_SSE -O1 -lm
    endif
    ifeq ($(UNAME_P),ARM)
		aarch64-linux-gnu-gcc -oeigtest eig_vec_decomp.c mat_aarch64.o -D__ARM_NEON -g -O1 -lm
    endif
