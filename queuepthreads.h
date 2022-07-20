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
  int done_multiplying;
  int index;
  int N;
};

#endif
