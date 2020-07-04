#include<pthread.h>
#include"Pthread.h"
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include"queue.h"

typedef struct shared_data {
	queue q;
	sem_t mutex;
	sem_t producer_cond;
	sem_t consumer_cond;
}shared_data;

typedef struct args_t {
	shared_data* psd;
	int threadID;
	int times;
}args_t;

void shared_data_init(shared_data* psd);
void shared_data_destroy(shared_data* psd);
void args_init(args_t* pa,shared_data*psd,int threadID,int times);
void* producer_thread(void* args);
void* consumser_thread(void* args);

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage:command [times]\n");
		return 0;
	}

	shared_data sd;
	pthread_t p0, p1;
	args_t p0a, p1a;
	shared_data_init(&sd);
	args_init(&p0a, &sd, 0, atoi(argv[1]));
	args_init(&p1a, &sd, 1, atoi(argv[1]));

	Pthread_create(&p0, NULL, producer_thread, (void*)&p0a);
	Pthread_create(&p1, NULL, consumser_thread, (void*)&p1a);
	Pthread_join(p0, NULL);
	Pthread_join(p1, NULL);
	printf("result size of the shared_data's queue:%d\n", sd.q.size);

	shared_data_destroy(&sd);
	return 0;
}

void shared_data_init(shared_data* psd) {
	queue_init(&psd->q);
	sem_init(&psd->mutex, 0, 1);
	sem_init(&psd->consumer_cond, 0, 0);
	sem_init(&psd->producer_cond, 0, MAX);
}

void shared_data_destroy(shared_data* psd) {
	sem_destroy(&psd->mutex);
	sem_destroy(&psd->consumer_cond);
	sem_destroy(&psd->producer_cond);
}

void args_init(args_t* pa, shared_data* psd, int threadID, int times) {
	pa->psd = psd;
	pa->threadID = threadID;
	pa->times = times;
}

void* producer_thread(void* args) {
	args_t* p = (args_t*)args;
	int i;
	for (i = 0; i < p->times; ++i) {
		sem_wait(&p->psd->producer_cond);
		sem_wait(&p->psd->mutex);
		enqueue(&p->psd->q, i);
		sem_post(&p->psd->mutex);
		sem_post(&p->psd->consumer_cond);
		printf("enqueue value:%d\n", i);
	}
	return NULL;
}

void* consumser_thread(void* args) {
	args_t* p = (args_t*)args;
	int i;
	for (i = 0; i < p->times; ++i) {
		sem_wait(&p->psd->consumer_cond);
		sem_wait(&p->psd->mutex);
		int result;
		dequeue(&p->psd->q, &result);
		sem_post(&p->psd->mutex);
		sem_post(&p->psd->producer_cond);
		printf("dequeue value:%d\n", result);
	}
	return NULL;
}