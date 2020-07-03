#include<pthread.h>
#include"PPthread.h"
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>

typedef struct shared_data {
	sem_t m;
	int value;
}shared_data;

typedef struct args_t {
	shared_data* psd;
	int threadID;
	int times;
}args_t;

void shared_data_init(shared_data* sd) {
	sem_init(&sd->m,0,1);//当信号量作为锁的时候其值必须初始化成为1
	sd->value = 0;
}

void shared_data_destroy(shared_data* sd) {
	sem_destroy(&sd->m);
}

void args_init(args_t* a, shared_data* psd, int threadID, int times) {
	a->psd = psd;
	a->threadID = threadID;
	a->times = times;
}

void increment(shared_data* psd) {
	sem_wait(&psd->m);
	psd->value++;
	sem_post(&psd->m);
}

void* mythread(void* args) {
	args_t*p = (args_t*)args;
	int i;
	for (i = 0; i < p->times; ++i) {
		increment(p->psd);
	}
	return NULL;
}

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
	Pthread_create(&p0, NULL, mythread, (void*)&p0a);
	Pthread_create(&p1, NULL, mythread, (void*)&p1a);
	Pthread_join(p0, NULL);
	Pthread_join(p1, NULL);
	printf("result:shared_data's value=%d\n", sd.value);

	shared_data_destroy(&sd);
	return 0;
}
