CC = gcc

TARGET = simd_matrix_vecotor_mul.so

CFLAGS = -g -fPIC -shared -Wall -lm -Xlinker --verbose

CFILES = simd_matrix_vecotor_mul.c

all: $(CFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(CFILES) $(LIBS)
	mv $(TARGET) ..

clean:
	rm ../$(TARGET)

UNAME_P := $(shell uname -p)

simd_matrix_vecotor_mul: simd_matrix_vecotor_mul.c mat.o
    ifeq ($(UNAME_P),x86_64)
		gcc -march=native -o simd_matrix_vecotor_mul simd_matrix_vecotor_mul.c -D__X86_SSE -O1 -lm -g
    endif
    ifeq ($(UNAME_P),ARM)
		aarch64-linux-gnu-gcc -oeigtest eig_vec_decomp.c mat_aarch64.o -D__ARM_NEON -g -O1 -lm
    endif
