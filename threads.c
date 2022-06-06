#include "threads.h"

#define N 32
#define NROUNDS 1
#define THREADS 32

clock_t start, end;
float test_matrix[N*N];
float test_vector[N];
float new_vec[N];

void vector_mult_basic(int id){
	for (int i = 0; i < N; i++) {
		new_vec[i] += test_matrix[(id)*N + i] * test_vector[i];
	}
}

void *thread_work(void *vargp){
        struct Thread_simd *t = (struct Thread_simd *)vargp;
	for(int i =0; i<NROUNDS; i++){ vector_mult_basic(t->index); }
	t -> do_mult = 0;
}

void main()
{
	//declaration
	pthread_t tid[THREADS];
	struct Thread_simd t[THREADS];
	float cpu_time_used;
	int count = 1;
	
	//initalizations
	for(int i=0; i<N; i++) {
		test_vector[i] = rand();
		for(int j=0; j<N; j++){ test_matrix[i*N+j] = rand(); }
	}
	for (int i = 0; i < THREADS; i++){ 
		t[i].do_mult = 1;
		t[i].index = i;
		pthread_create(&tid[i], NULL, thread_work, (void *)&t[i]); 
	}

	//timing
	start = clock();
	while(count !=0){
		count = 0;
		for(int i = 0; i < THREADS; i++){
			count += t[i].do_mult;
        	}
	}
	end = clock();
	cpu_time_used += ((float) (end - start))/CLOCKS_PER_SEC*1000;
	cpu_time_used = cpu_time_used/NROUNDS;
	printf("%fms serial\n", cpu_time_used);
	pthread_exit(NULL);
}
