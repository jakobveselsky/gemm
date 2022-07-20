#include "queuepthreads.h"

#define THREADS 4 //defines how many threads will be made

//global variables
clock_t start, end; //creates variables to log start and end time of matrix vector multiplaction
float test_matrix[1000*1000]; //creates matrix of N by N size
float test_vector[1000]; //creates vector of length N 
float output_vector[1000]; //creates vector of length N
int N = 0;

void vector_mult_basic(int index, int N, unsigned int row_num){
	for (int i = N*row_num; i < (N+1)*row_num; i++) {
		output_vector[row_num] += test_matrix[N*row_num + i] * test_vector[row_num];
	}
	
}

void *thread_work(void *vargp){
	volatile struct Thread_info *t_info = (struct Thread_info *)vargp;
	for(int i = t_info->index*t_info->N/THREADS; i < (t_info->index+1)*t_info->N/THREADS; i++){ vector_mult_basic(t_info->index, t_info->N, i); }
}

int main(int argc, char *argv[]){
	//declaration
	pthread_t t[THREADS]; //creates an array of pthreads
	struct Thread_info t_info[THREADS]; //creates an array to log info about the threads
	float cpu_time_used; //creates variable to store time used in matrix vector multiplaction
	
	//initalizations
	N = atoi(argv[1]);
	for(int i=0; i<N; i++) {
		output_vector[i] = 0.;
		test_vector[i] = rand();
		for(int j=0; j<N; j++){ test_matrix[i*N+j] = rand(); }
	}
	for (int i = 0; i<THREADS; i++){ 
		t_info[i].index = i;
		t_info[i].N = N;
		if(0 == i){ start = clock(); }
		pthread_create(&t[i], NULL, thread_work, (void *)&t_info[i]);
	}

	for (int i = 0; i<THREADS; i++){
		pthread_join(t[i], NULL);
	}
	end = clock(); //logs time after matrix vector multiplaction
	cpu_time_used = ((float) (end - start))/CLOCKS_PER_SEC*1000; 
	printf("%f\n", cpu_time_used); //prints time used on matrix vector multiplaction
	return 0;
} 
