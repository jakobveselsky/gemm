#include "threads.h"

//#define N 32 //defines dimensions of array and vector
#define NROUNDS 10000 //defines how many rounds of matrix vector multiplaction is to be done
#define THREADS 3 //defines how many threads will be made

//global variables
clock_t start, end; //creates variables to log start and end time of matrix vector multiplaction
float test_matrix[1000*1000]; //creates matrix of N by N size
float test_vector[1000]; //creates vector of length N 

void vector_mult_basic(int index, int N, float *output_vector, unsigned int row_num){
	int i,k;
	output_vector[row_num] = 0.;
	k=0;
	for (i = N*row_num; i < (N+1)*row_num; i++) {
		output_vector[row_num] += test_matrix[index*N + i] * test_vector[k];
		k++;
	}
	
}

void *thread_work(void *vargp){
        volatile struct Thread_info *t_info = (struct Thread_info *)vargp;
	float thread_local_output_vector[1000]; //creates vector of length N
	while(1){	
		if(t_info -> do_mult == 1){
			for(int i = t_info->index*t_info->N/THREADS; i < (t_info->index+1)*t_info->N/THREADS; i++){ vector_mult_basic(t_info->index, t_info->N,thread_local_output_vector, i); }
			t_info -> do_mult = 0;
		}
	}
}

int main(int argc, char *argv[])
{
	//declaration
	pthread_t t[THREADS]; //creates an array of pthreads
	struct Thread_info t_info[THREADS]; //creates an array to log info about the threads
	float cpu_time_used; //creates variable to store time used in matrix vector multiplaction
	int count = 1; //creates variable to check if matrix vector multiplaction is done on all threads
	int N;
	
	N = atoi(argv[1]);

	//initalizations
	for(int i=0; i<N; i++) {
		test_vector[i] = rand();
		for(int j=0; j<N; j++){ test_matrix[i*N+j] = rand(); }
	}
	for (int i = 0; i<THREADS; i++){ 
		t_info[i].do_mult = 0;
		t_info[i].index = i;
		t_info[i].N = N;
		pthread_create(&t[i], NULL, thread_work, (void *)&t_info[i]); 
	}

	//timing
	start = clock(); //logs time before matrix vector multiplaction
	for(int i = 0; i < NROUNDS; i++){
		for (int i = 0; i<THREADS; i++){ 
			t_info[i].do_mult = 1; 
		}
		do{
			count = 0;
			for(int i = 0; i < THREADS; i++){
				count += t_info[i].do_mult;
        		}
		}while(count !=0);
	}		
	end = clock(); //logs time after matrix vector multiplaction
	cpu_time_used += ((float) (end - start))/CLOCKS_PER_SEC*1000; 
	cpu_time_used = cpu_time_used/NROUNDS; //divides total time used by rounds completed
	printf("%f\n", cpu_time_used); //prints time used on matrix vector multiplaction
	//pthread_exit(NULL); //kills all threads
	return 0;
}
