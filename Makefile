CC = gcc
UNAME_P := $(shell uname -p)
CFLAGS=  -O0 -lm -lpthread -g

all: simd threads

clean:
	rm -f simd_matrix_vecotor_mul-no-omp.o
	rm -f simd_matrix_vecotor_mul-openmp.o
	rm -f simd_matrix_vecotor_mul-openmp-simd.o
	rm -f threads.o

omp_threads: omp_threads.c
	$(CC) -o omp_threads.o omp_threads.c $(CFLAGS)

pthreads: pthreads.c
	$(CC) -o pthreads.o pthreads.c $(CFLAGS)

simd: simd.c
	$(CC) -o simd.o simd.c $(CFLAGS)

serial: serial.c
	$(CC) -o serial.o serial.c $(CFLAGS)

simd_matrix_vecotor_mul: simd_matrix_vecotor_mul.c
    ifeq ($(UNAME_P),x86_64)
		$(CC) -o simd_matrix_vecotor_mul-no-omp.o simd_matrix_vecotor_mul.c $(CFLAGS)
		$(CC) -fopenmp -o simd_matrix_vecotor_mul-openmp.o simd_matrix_vecotor_mul.c $(CFLAGS)
		$(CC) -fopenmp-simd -o simd_matrix_vecotor_mul-openmp-simd.o simd_matrix_vecotor_mul.c $(CFLAGS)
    endif
    ifeq ($(UNAME_P),ARM)
		gcc -o simd_matrix_vecotor.o simd_matrix_vecotor_mul.c mat_aarch64.o -march=apple-m1 -D__ARM_NEON -O1 -lm
    endif

threads: threads.c
     ifeq ($(UNAME_P),x86_64)
		$(CC) -o threads.o threads.c $(CFLAGS)
     endif
