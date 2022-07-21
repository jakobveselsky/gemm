#include "queuepthreads.h"

#define THREADS 4 //defines how many threads will be made

//global variables
clock_t start, end; //creates variables to log start and end time of matrix vector multiplaction
float test_matrix[1000*1000]; //creates matrix of N by N size
float test_vector[1000]; //creates vector of length N 
float output_vector[1000]; //creates vector of length N
int N = 0;

void buffer_init(buffer_t* b, int N)
{
    b->entries = (entry_t**) malloc(N * sizeof(entry_t*));
    for (int i=0; i < N; i++){ b->entries[i] = NULL; }
    b->size = N;
    b->head = b->tail = 0;
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init (&b->has_space, NULL);
    pthread_cond_init (&b->has_items, NULL);
}

void buffer_put(buffer_t* b, entry_t* item)
{
    pthread_mutex_lock(&b->lock);

    while (b->tail - b->head >= b->size)
    {
        pthread_cond_wait(&b->has_space, &b->lock);
    }

    b->entries[b->tail++ % b->size] = item;
    pthread_cond_signal(&b->has_items);

    pthread_mutex_unlock(&b->lock);
}

entry_t* buffer_get(buffer_t* b)
{
    pthread_mutex_lock(&b->lock);

    while (b->tail == b->head)
    {
        pthread_cond_wait(&b->has_items, &b->lock);
    }

    entry_t* entry = b->entries[b->head % b->size];
    b->entries[b->head++ % b->size] = NULL;
    pthread_cond_signal(&b->has_space);

    pthread_mutex_unlock(&b->lock);
    return entry;
}

int buffer_count(buffer_t* b)
{
    return b->tail - b->head;
}

void buffer_cleanup(buffer_t* b)
{
    int unfreed_count = 0;
    for (int i=0; i < b->size; i++)
        if (b->entries[i] != NULL)
            unfreed_count++;
    free(b->entries);
    pthread_mutex_destroy(&b->lock);
    pthread_cond_destroy(&b->has_space);
    pthread_cond_destroy(&b->has_items);
}

void vector_mult_basic(int index, int N, unsigned int row_num){
	for (int i = N*row_num; i < (N+1)*row_num; i++) {
		output_vector[row_num] += test_matrix[N*row_num + i] * test_vector[row_num];
	}	
}

void *supplier_thread_work(void *vargp){
	volatile b_tsd_t* b_tsd = (b_tsd_t *)vargp;
	for(int i = b_tsd->index*b_tsd->N/THREADS; i < (b_tsd->index+1)*b_tsd->N/THREADS; i++){ vector_mult_basic(b_tsd->index, b_tsd->N, i); }
	pthread_exit(NULL);
}

void *consumer_thread_work(void *vargp){
        volatile b_tsd_t* b_tsd = (b_tsd_t *)vargp;
        for(int i = b_tsd->index*b_tsd->N/THREADS; i < (b_tsd->index+1)*b_tsd->N/THREADS; i++){ vector_mult_basic(b_tsd->index, b_tsd->N, i); }
        pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	//declaration
	pthread_t* threads; //creates an array of pthreads
	pthread_attr_t attr;
	buffer_t b;
	b_tsd_t* b_tsd;
	float cpu_time_used; //creates variable to store time used in matrix vector multiplaction
	
	//initalizations
	N = atoi(argv[1]);
	threads = (pthread_t*) malloc(THREADS * sizeof(pthread_t));
    	b_tsd = (b_tsd_t*) (THREADS * sizeof(b_tsd_t));
	buffer_init(&b, N);
	pthread_attr_init(&attr);
    	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for(int i=0; i<N; i++) {
		output_vector[i] = 0.;
		test_vector[i] = rand();
		for(int j=0; j<N; j++){ test_matrix[i*N+j] = rand(); }
	}

	//create threads
	b_tsd[0] = (b_tsd_t)
	{
		.b = &b, .index = 0, .N = N
	};
        start = clock();
        pthread_create(&threads[0], &attr, supplier_thread_work, (void *)&b_tsd[0]);
	
	for (int i = 1; i<THREADS; i++){ 
		b_tsd[i] = (b_tsd_t)
        	{
                	.b = &b, .index = i, .N = N
        	};
		pthread_create(&threads[i], &attr, consumer_thread_work, (void *)&b_tsd[i]);
	}
	
	//closing
	for (int i = 0; i<THREADS; i++){
		pthread_join(threads[i], NULL);
	}
	end = clock(); //logs time after matrix vector multiplaction
	cpu_time_used = ((float) (end - start))/CLOCKS_PER_SEC*1000; 
	printf("%f\n", cpu_time_used); //prints time used on matrix vector multiplaction
	pthread_attr_destroy(&attr);
	buffer_cleanup(&b);
	free(threads);
	free(b_tsd);
	pthread_exit(NULL);
	return 0;
} 
