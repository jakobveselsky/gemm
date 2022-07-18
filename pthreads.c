#include "threads.h"

#define NROUNDS 10000 //defines how many rounds of matrix vector multiplaction is to be done
#define THREADS 4 //defines how many threads will be made

int multiply;
pthread_t t[THREADS]; //creates an array of pthreads
struct Thread_info threads_log[THREADS]; //creates an array to log info about the threads
float cpu_time_used; //creates variable to store time used in matrix vector multiplaction
clock_t start_clock, end_clock; //creates variables to log start and end time of matrix vector multiplaction
float test_matrix[1000*1000]; //creates matrix of N by N size
float test_vector[1000]; //creates vector of length N 
//TAILQ_HEAD(tailhead, entry) head; //maybe making the head of the queue
struct timespec start_nanoepoch, end_nanoepoch;
//void add_to_queue(float fl) {
//  struct entry *elem;
//  elem = malloc(sizeof(struct entry));
//  if (elem) { elem->f = fl; } //what is this if statement
//  TAILQ_INSERT_HEAD(&head, elem, entries); //what is this doing
//}

void vector_mult_basic(volatile struct Thread_info *thread_log_local){
	for (int i = 0; i < (thread_log_local->N/THREADS); i++) {
		for(int j = 0; j < thread_log_local->N; j++){
			thread_log_local->thread_local_output_vector[i] += thread_log_local->thread_local_matrix[i+j] * thread_log_local->thread_local_input_vector[i];
		}
	}
	
}

void *thread_work(void *vargp){
        volatile struct Thread_info *thread_log_local = (struct Thread_info *)vargp;
	int count;
	while(1){	
		if(multiply == 1){
			for(int i = 0; i < NROUNDS; i++){
				vector_mult_basic(thread_log_local);
			}	

			threads_log[thread_log_local -> index].done_multiplying = 1; //Label thread as done/does this right this globaly

			if(thread_log_local -> index != 0) { pthread_exit(NULL); } //close thread if its not 0
			else{
				while(1){
					count = 0; 
					for(int j = 0; j < THREADS; j++){
						if(threads_log[j].done_multiplying == 1){ count += 1; }
					}
					if(count == THREADS){
						clock_gettime( CLOCK_REALTIME, &end_nanoepoch );  // CLOCK_REALTIME returns 1970 Epoch Time
						int64_t elapsed = (int64_t)(end_nanoepoch.tv_sec - start_nanoepoch.tv_sec) * (int64_t)1000000000UL + (int64_t)(end_nanoepoch.tv_nsec - start_nanoepoch.tv_nsec);
						printf("elapsed = %f ns\n", (float)elapsed / 1000000000);
						end_clock = clock(); //logs time after matrix vector multiplaction
						cpu_time_used += ((float) (end_clock - start_clock))/CLOCKS_PER_SEC*1000; 
						cpu_time_used = cpu_time_used/NROUNDS;
						printf("%f\n", cpu_time_used); //prints time used on matrix vector multiplaction
						printf("Started @ %d ended @ %d\n", start_clock, end_clock);
						exit(0);
					}
				}
			}
		}
	}
}


void init_vector_matrix(int N){
	for(int i=0; i<N; i++) {
		test_vector[i] = rand();
		for(int j=0; j<N; j++){
			test_matrix[i*N+j] = rand();
		}
	}
}

int main(int argc, char *argv[])
{
	//declaration
	int N; //creates variable to store dimensions of vector and matrix
	
	//initalizations
	N = atoi(argv[1]); //taking in N from command line
	multiply = 0; //has threads spind up but not multiply yet
	init_vector_matrix(N); //creates a randome matraix and vector of size N in the global
	for (int i = 0; i<THREADS; i++){ 
		for(int j=0; j<(N/THREADS); j++) {
			threads_log[i].thread_local_output_vector[j] = 0.0;
			threads_log[i].thread_local_input_vector[j] = test_vector[j+(i*(N/THREADS))];
			for(int k=0; k<N; k++){
				threads_log[i].thread_local_matrix[j+k] = test_matrix[j+k+(i*(N/THREADS))];
			}	
		}
		threads_log[i].done_multiplying = 0; //sets each thread log at not done multiplying
		threads_log[i].index = i; //logs index of thread in thread log
		threads_log[i].N = N; //logs dimensions for the matrix and vector to thread log
		pthread_create(&t[i], NULL, thread_work, (void *)&threads_log[i]); //creates threads and passes local log to it
	}

	//multiplication
	clock_gettime( CLOCK_REALTIME, &start_nanoepoch );  // CLOCK_REALTIME returns 1970 Epoch Time

	start_clock = clock(); //logs time before matrix vector multiplaction
	multiply = 1; //tells threads to start multipling
	pthread_exit(NULL);
	//sleep(1000); //sleep main thread for 1000 seconds
	return 0;
}
