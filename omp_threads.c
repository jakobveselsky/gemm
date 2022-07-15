#include "headers.h"

#define NROUNDS 1000
#define THREADS 3
#define CHUNK 4

float test_matrix[1000*1000];
float test_vector[1000];
float new_vec[1000];

void matrix_mult_omp(int n, float *x, float *A, float *y){
	#pragma omp parallel for schedule(dynamic, CHUNK) num_threads(THREADS)
        for (int i = 0; i < n; i++) {  //columns
                for (int j = 0; j < n; j++) {  //rows
                        y[j] += A[n*i + j] * x[i]; //y = Ax
                }
        }
}

void random_vector_matrix(int N){
	for(int i=0; i<N; i++) {
		test_vector[i] = rand();
		for(int j=0; j<N; j++){
			test_matrix[i+j] = rand();
		}
	}
}

void print_vector_matrix(int N){
	printf("Test Vector:\n");
	for (int q = 0; q < N; q++) { printf("%d, %f\n", q, test_vector[q]); }
	printf("Test Matrix:\n");
	for (int r = 0; r < N; r++) {
		for (int s = 0; s < N; s++) {
			printf("%f ", test_matrix[r*N + s]);
		}
		printf("\n");
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	//declaration
	time_t t;
	//srand((unsigned) time(&t));
 	clock_t start, end;
	double cpu_time_used;
	int N;

	//initalizations
	N = atoi(argv[1]);
	random_vector_matrix(N);

	//printing of matrix and vector
    	//print_vector_matrix();

	//benchmarking
	cpu_time_used = 0.;
	start = clock();
	for(int z = 0; z < NROUNDS; z++){ matrix_mult_omp(N, test_vector, test_matrix, new_vec); }
	end = clock();
 	cpu_time_used +=((double) (end - start))/CLOCKS_PER_SEC*1000;
        printf("%f\n", cpu_time_used/NROUNDS);
	return 0;
}
