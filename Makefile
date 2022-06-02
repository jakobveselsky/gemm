CC = gcc
UNAME_P := $(shell uname -p)
CFLAGS= -march=native -D__X86_SSE -O1 -lm

all: simd_matrix_vecotor_mul

clean:
	rm -f simd_matrix_vecotor_mul-no-omp.o
	rm -f simd_matrix_vecotor_mul-openmp.o
	rm -f simd_matrix_vecotor_mul-openmp-simd.o

simd_matrix_vecotor_mul: simd_matrix_vecotor_mul.c
    ifeq ($(UNAME_P),x86_64)
		$(CC) -o simd_matrix_vecotor_mul-no-omp.o simd_matrix_vecotor_mul.c $(CFLAGS)
		$(CC) -fopenmp -o simd_matrix_vecotor_mul-openmp.o simd_matrix_vecotor_mul.c $(CFLAGS)
		$(CC) -fopenmp-simd -o simd_matrix_vecotor_mul-openmp-simd.o simd_matrix_vecotor_mul.c $(CFLAGS)
    endif
    ifeq ($(UNAME_P),ARM)
		aarch64-linux-gnu-gcc -oeigtest eig_vec_decomp.c mat_aarch64.o -D__ARM_NEON -g -O1 -lm
    endif
