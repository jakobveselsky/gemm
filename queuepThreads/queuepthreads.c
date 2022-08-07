//includes
#include "buffer.h" //include header for buffer structure
#include "queuepthreads.h" //include header for this file

//global variables
float test_matrix[1000*1000]; //creates matrix of N by N size
float test_vector[1000]; //creates vector of length N 
float output_vector[1000]; //creates vector of length N

//functions
void scaler_vector_mult_basic(int index, int N, unsigned int row_num){} //does the multiplaction

void *supplier_thread_work(void *vargp){
	volatile b_tsd_t* thread_specfic_data = (b_tsd_t *)vargp; //takes in thread specfic data and saves it locally
	printf("hi from %d\n", thread_specfic_data->index);
	pthread_exit(NULL);} ///exit thread/

void *consumer_thread_work(void *vargp){
    volatile b_tsd_t* thread_specfic_data = (b_tsd_t *)vargp; //takes in thread specfic data and saves it locally
    printf("hi from %d\n", thread_specfic_data->index);
    pthread_exit(NULL);} //exit thread

int main(int argc, char *argv[]){
	//declaration and  initalization
	int N = atoi(argv[1]); //takes in N from the command line
	int num_suppliers = atoi(argv[2]); //takes in number of suppliers from command line
	int num_consumers = atoi(argv[3]); //rakes in number of consumers from command line
	int threads_num = num_consumers + num_suppliers; //saves total number of threads from number of consumers and suppliers
	buffer_t input_vector_b, output_vector_b, scalers_b; //declares templets buffer for input, output, and scalers
	pthread_attr_t attr; //creates pthread thing
	pthread_t* threads = (pthread_t*) malloc(threads_num * sizeof(pthread_t)); //creates threads and allocates memory for them
    b_tsd_t* thread_specifc_data = (b_tsd_t*) malloc(threads_num * sizeof(b_tsd_t)); //creates threads specfic data and allocates memory for them
	buffer_init(&input_vector_b, N); //initalizes input buffer template
	buffer_init(&output_vector_b, N); //initalizes output buffer template
	buffer_init(&scalers_b, N/threads_num); //initalizes scalers buffer template
	pthread_attr_init(&attr); //initalizes pthread thing
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); //idk
	for(int i=0; i<N; i++) {
		output_vector[i] = 0.; //initalize output vector to zero 
		test_vector[i] = rand(); //sets test vector to random number
		for(int j=0; j<N; j++){ test_matrix[i*N+j] = rand(); }} //sets test matrix to random number

	//create threads
	for(int i = 0; i < num_suppliers; i++){
		thread_specifc_data[i] = (b_tsd_t){ .input_vector = &input_vector_b, .output_vector = &output_vector_b, 
											.scalers_list = &scalers_b, .index = i, .N = N };
    	pthread_create(&threads[i], &attr, supplier_thread_work, (void *)&thread_specifc_data[i]);} //creates suppliers threads
	for (int i = num_suppliers; i < num_consumers+num_suppliers; i++){ 
		thread_specifc_data[i] = (b_tsd_t){ .input_vector = &input_vector_b, .output_vector = &output_vector_b, 
											.scalers_list = &scalers_b, .index = i, .N = N };
		pthread_create(&threads[i], &attr, consumer_thread_work, (void *)&thread_specifc_data[i]);} //creates consumers threads
	
	//closing
	for (int i = 0; i<threads_num; i++){ pthread_join(threads[i], NULL); } //joins threads when they finish process
	buffer_cleanup(&input_vector_b); //clears template buffer for input
	buffer_cleanup(&output_vector_b); //clears template buffer for output
	buffer_cleanup(&scalers_b); //clears template buffer for scalers 
    pthread_attr_destroy(&attr); //destroy's pthread thing
	free(threads); //free memory for threads
	free(thread_specifc_data); //free memory for threads spefic data
	pthread_exit(NULL);//exit thread
	return 0;} 