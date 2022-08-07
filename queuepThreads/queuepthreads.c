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
	volatile b_tsd_t* thread_specfic_data = (b_tsd_t *)vargp;
	printf("hi from %d\n", thread_specfic_data->index);
	pthread_exit(NULL);
}

void *consumer_thread_work(void *vargp){
    volatile b_tsd_t* thread_specfic_data = (b_tsd_t *)vargp;
    printf("hi from %d\n", thread_specfic_data->index);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	//declaration and  initalization
	int N = atoi(argv[1]);
	int num_suplliers = atoi(argv[2]);
	int num_consumers = atoi(argv[3]);	
	int threads_num = num_consumers + num_suplliers;
	buffer_t input_vector_b, output_vector_b, scaler_b;
	pthread_attr_t attr;
	pthread_t* threads = (pthread_t*) malloc(threads_num * sizeof(pthread_t));
    b_tsd_t* thread_specifc_data = (b_tsd_t*) malloc(threads_num * sizeof(b_tsd_t));
	buffer_init(&input_vector_b, N);
	buffer_init(&output_vector_b, N);
	buffer_init(&scaler_b, N/threads_num);
	pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for(int i=0; i<N; i++) {
		output_vector[i] = 0.;
		test_vector[i] = rand();
		for(int j=0; j<N; j++){ test_matrix[i*N+j] = rand(); }
	}

	//create threads
	for(int i = 0; i < num_suplliers; i++){
		thread_specifc_data[i] = (b_tsd_t){ .input_vector = &input_vector_b, .output_vector = &output_vector_b, .scaler_list = &scaler_b, .index = i, .N = N };
    	pthread_create(&threads[i], &attr, supplier_thread_work, (void *)&thread_specifc_data[i]);
	}
	for (int i = num_suplliers; i < num_consumers+num_suplliers; i++){ 
		thread_specifc_data[i] = (b_tsd_t){ .input_vector = &input_vector_b, .output_vector = &output_vector_b, .scaler_list = &scaler_b, .index = i, .N = N };
		pthread_create(&threads[i], &attr, consumer_thread_work, (void *)&thread_specifc_data[i]);
	}
	
	//closing
	for (int i = 0; i<threads_num; i++){ pthread_join(threads[i], NULL); }
    pthread_attr_destroy(&attr);
	free(threads);
	free(thread_specifc_data);
	pthread_exit(NULL);
	return 0;
} 
