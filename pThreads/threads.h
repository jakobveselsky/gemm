#ifndef _THREADS_H
#define _THREADS_H

//define included libaries
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/queue.h>

struct entry {
  float f;
  TAILQ_ENTRY(entry) entries;
};

struct Thread_info {
  float thread_local_matrix[1000*1000]; //creates matrix of N by N size
  float thread_local_input_vector[1000]; //creates vector of length N
  float thread_local_output_vector[1000]; //creates vector of length N
  int done_multiplying;
  int index;
  int N;
};

#endif
