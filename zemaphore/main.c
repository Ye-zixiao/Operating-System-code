#include<pthread.h>
#include"Pthread.h"
#include<stdio.h>
#include<stdlib.h>
#include"zemaphore.h"

typedef struct shared_data {
	int value;
	zem_t lock;
}shared_data;

typedef struct args_t {
	int threadID;
	int times;
	shared_data* psd;
}args_t;

void shared_data_init(shared_data* psd);
void shared_data_destroy(shared_data* psd);
void args_init(args_t* pa, int threadID, int times,shared_data*psd);
void increment_sd(shared_data* psd);
void* mythread(void* args);

int main(int argc, char** argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage:command [times]\n");
		exit(1);
	}

	shared_data sd;
	pthread_t p0, p1;
	args_t p0a, p1a;
	shared_data_init(&sd);
	args_init(&p0a, 0, atoi(argv[1]), &sd);
	args_init(&p1a, 1, atoi(argv[1]), &sd);

	Pthread_create(&p0, NULL, mythread, (void*)&p0a);
	Pthread_create(&p1, NULL, mythread, (void*)&p1a);
	Pthread_join(p0, NULL);
	Pthread_join(p1, NULL);
	printf("result value:%d\n", sd.value);

	shared_data_destroy(&sd);
	return 0;
}

void shared_data_init(shared_data* psd) {
	psd->value = 0;
	zem_init(&psd->lock, 1);
}

void shared_data_destroy(shared_data* psd) {
	zem_destroy(&psd->lock);
}

void args_init(args_t* pa, int threadID, int times,shared_data*psd) {
	pa->threadID = threadID;
	pa->times = times;
	pa->psd = psd;
}

void increment_sd(shared_data* psd) {
	zem_wait(&psd->lock);
	psd->value++;
	zem_post(&psd->lock);
}

void* mythread(void* args) {
	args_t* p = (args_t*)args;
	int i;
	for (i = 0; i < p->times; ++i) {
		increment_sd(p->psd);
	}
	return NULL;
}