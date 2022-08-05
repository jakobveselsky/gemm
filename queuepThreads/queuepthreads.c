#include "buffer.h"
#include "queuepthreads.h"

//global variables
float test_matrix[1000*1000]; //creates matrix of N by N size
float test_vector[1000]; //creates vector of length N 
float output_vector[1000]; //creates vector of length N

void vector_mult_basic(int index, int N, unsigned int row_num){
	for (int i = N*row_num; i < (N+1)*row_num; i++) {
		output_vector[row_num] += test_matrix[N*row_num + i] * test_vector[row_num];
	}	
}

void *supplier_thread_work(void *vargp){
	volatile b_tsd_t* b_tsd = (b_tsd_t *)vargp;
	buffer_t* b = b_tsd->b;
	int myId = b_tsd->index;	
	printf("hi from %d\n", myId);
	pthread_exit(NULL);
}

void *consumer_thread_work(void *vargp){
        volatile b_tsd_t* b_tsd = (b_tsd_t *)vargp;
        buffer_t* b = b_tsd->b;
	    int myId = b_tsd->index;	
	    printf("hi from %d\n", myId);
        //for(int i = b_tsd->index*b_tsd->N/THREADS; i < (b_tsd->index+1)*b_tsd->N/THREADS; i++){ vector_mult_basic(b_tsd->index, b_tsd->N, i); }
        pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	//declaration and  initalization
	pthread_attr_t attr;
	buffer_t b;
	int N = atoi(argv[1]);
	int num_suplliers = atoi(argv[2]);
	int num_consumers = atoi(argv[3]);	
	int threads_num = num_consumers + num_suplliers;
	pthread_t* threads = (pthread_t*) malloc(threads_num * sizeof(pthread_t));
    b_tsd_t* b_tsd = (b_tsd_t*) malloc(threads_num * sizeof(b_tsd_t));
	buffer_init(&b, N);
	pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for(int i=0; i<N; i++) {
		output_vector[i] = 0.;
		test_vector[i] = rand();
		for(int j=0; j<N; j++){ test_matrix[i*N+j] = rand(); }
	}

	//create threads
	for(int i = 0; i < num_suplliers; i++){
		b_tsd[i] = (b_tsd_t){ .b = &b, .index = i, .N = N };
    	pthread_create(&threads[i], &attr, supplier_thread_work, (void *)&b_tsd[i]);
	}
	for (int i = num_suplliers; i < num_consumers+num_suplliers; i++){ 
		b_tsd[i] = (b_tsd_t){ .b = &b, .index = i, .N = N };
		pthread_create(&threads[i], &attr, consumer_thread_work, (void *)&b_tsd[i]);
	}
	
	//closing
	for (int i = 0; i<threads_num; i++){ pthread_join(threads[i], NULL); }
    pthread_attr_destroy(&attr);
	buffer_cleanup(&b);
	free(threads);
	free(b_tsd);
	pthread_exit(NULL);
	return 0;
} 
