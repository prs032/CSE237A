#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <string.h>
#include <pthread.h>

#include <sys/timeb.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "governor_part1.h"

// Workload that traverses allocated memory area in a reverse order (to minimize prefetching effect)
static int DEF_ITERATION = 10000;
static int DEF_STRIDE = 128;
static int DEF_SIZE = 1024*1024;//4MB

static int* v;
unsigned int ret;
unsigned int reg[7];
unsigned int counter_value;
void workload_init(int STRIDE, int SIZE) {
	// Set up array
	v = (int*)malloc(sizeof(int)*SIZE);
	int i;
	for (i = STRIDE; i < SIZE; i += STRIDE) {
		v[i] = (int)&v[i-STRIDE];
	}
	v[0] = (int)&v[i-STRIDE];

	// Warmup the memory
	int* p = v;
	do {
		p = (int*)*p;
	} while (p != (int*)v);
}

void workload_finish() {
	free(v);
}

void workload_body(int iteration, int STRIDE, int SIZE) {
	if (SIZE <= STRIDE) return;

	// Get result
	long long sum_t = 0;
	int n = 0;
	int* p = v;
	int i;

	for (i = 0; i < iteration; i++) {
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 

	}

}

static long long get_current_time_us() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long long curTime = (long long)tv.tv_sec * 1000 * 1000 + (long long)tv.tv_usec;
	return curTime;
}

static inline void reset_counters()
{
	// Reset all cycle counter and event counters
	asm volatile ("mcr p15, 0, %0, c9, c12, 0\n\t" :: "r"(0x00000017));  
}

int main(int argc, char *argv[]) {
	char szOldGovernor[32];
	unsigned long long t0, t1;

	// Initialize
	set_governor("userspace", szOldGovernor);
	set_by_min_freq(); // You can also test the minimum frequency using "set_by_min_freq()"
	workload_init(DEF_STRIDE, DEF_SIZE);
	printf("Measurement start.\n");
	
	// 1. Reset measurement metrics
	t0 = get_current_time_us();
	reset_counters();

	// 2. Run workload
	workload_body(DEF_ITERATION, DEF_STRIDE, DEF_SIZE);

	// 3. Here, you may need to get performance counters.
	t1 = get_current_time_us();
	// Read the configurable counters(PMXEVCNTR #r)
        // This is done by first writing the counter number to PMSELR and then reading PMXEVCNTR

        for (int r=0; r<6; ++r) {
          asm volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(r));     // Select event counter in PMSELR
          asm volatile ("MRC p15, 0, %0, c9, c13, 2\t\n" : "=r"(ret));// Read from PMXEVCNTR 
          reg[r] = ret;
          switch(r) {
             case 0 :
         	printf("L1D_CACHE: %d \n", ret);
         	break;
      	     case 1 :
		printf("L1D_CACHE_REFILL: %d \n", ret);
		break;
             case 2 :
         	printf("L2D_CACHE: %d \n", ret);
         	break;
      	     case 3 :
         	printf("L2D_CACHE_REFILL: %d \n", ret);
         	break;
      	     case 4 :
         	printf("COND_BR_EXEC: %d \n", ret);
         	break;
      	     case 5 :
         	printf("COND_BR_MISPRED: %d \n", ret);
         	break;
	   }
         
        }

        // Read CPU cycle count from the CCNT Register
        asm volatile ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(reg[6]));
        printf("Reading %u cycle counters.\n", reg[6]);

	// Print results
	int freq = get_cur_freq();
	printf("Exe_time: %lld us at %d\n", t1-t0, freq);

	// Finish the program
	workload_finish();
	set_governor(szOldGovernor, NULL);

	return 0;
}
