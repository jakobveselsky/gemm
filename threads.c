#include "threads.h"

#define N 32 //defines dimensions of array and vector
#define NROUNDS 100 //defines how many rounds of matrix vector multiplaction is to be done
#define THREADS 32 //defines how many threads will be made

//global variables
clock_t start, end; //creates variables to log start and end time of matrix vector multiplaction
float test_matrix[N*N]; //creates matrix of N by N size
float test_vector[N]; //creates vector of length N 

void vector_mult_basic(int index){
	for (int i = 0; i < N; i++) {
		test_vector[i] += test_matrix[index*N + i] * test_vector[i];
	}
}

void *thread_work(void *vargp){
        volatile struct Thread_info *t_info = (struct Thread_info *)vargp;
	while(1){	
		if(t_info -> do_mult == 1){
			vector_mult_basic(t_info->index); 
			t_info -> do_mult = 0;
		}
	}
}

void main()
{
	//declaration
	pthread_t t[THREADS]; //creates an array of pthreads
	struct Thread_info t_info[THREADS]; //creates an array to log info about the threads
	float cpu_time_used; //creates variable to store time used in matrix vector multiplaction
	int count = 1; //creates variable to check if matrix vector multiplaction is done on all threads
	
	//initalizations
	for(int i=0; i<N; i++) {
		test_vector[i] = rand();
		for(int j=0; j<N; j++){ test_matrix[i*N+j] = rand(); }
	}
	for (int i = 0; i<THREADS; i++){ 
		t_info[i].do_mult = 0;
		t_info[i].index = i;
		pthread_create(&t[i], NULL, thread_work, (void *)&t_info[i]); 
	}

	//timing
	start = clock(); //logs time before matrix vector multiplaction
	for(int i = 0; i < NROUNDS; i++){
		for (int i = 0; i<THREADS; i++){ t_info[i].do_mult = 1; }
		while(count !=0){
			count = 0;
			for(int i = 0; i < THREADS; i++){
				t_info[i].do_mult;
        		}
		}
	}		
	end = clock(); //logs time after matrix vector multiplaction
	cpu_time_used += ((float) (end - start))/CLOCKS_PER_SEC*1000; 
	cpu_time_used = cpu_time_used/NROUNDS; //divides total time used by rounds completed
	printf("%f ms \n", cpu_time_used); //prints time used on matrix vector multiplaction
	pthread_exit(NULL); //kills all threads
}
