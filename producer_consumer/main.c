#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include"Pthread.h"
#include"array_queue.h"

typedef struct shared_data {
	array_queue aq;
	pthread_mutex_t lock;
	pthread_cond_t producer_cond, consumer_cond;
}shared_data;

typedef struct args_t {
	int threadID;
	int times;
	shared_data* sd;
}args_t;

void shared_data_init(shared_data* sd) {
	queue_init(&sd->aq);
	Pthread_mutex_init(&sd->lock, NULL);
	Pthread_cond_init(&sd->consumer_cond, NULL);
	Pthread_cond_init(&sd->producer_cond, NULL);
}

void shared_data_destroy(shared_data* sd) {
	Pthread_mutex_destroy(&sd->lock);
	Pthread_cond_destroy(&sd->consumer_cond);
	Pthread_cond_destroy(&sd->producer_cond);
}

void args_init(args_t* a,int times,shared_data*sd,int threadID) {
	a->threadID = threadID;
	a->sd = sd;
	a->times = times;
}

void* producer_thread(void* args) {
	args_t* p = (args_t*)args;
	int i;
	for (i = 0; i < p->times; ++i) {
		Pthread_mutex_lock(&p->sd->lock);
		while (full(&p->sd->aq))
			pthread_cond_wait(&p->sd->producer_cond, &p->sd->lock);
		enqueue(&p->sd->aq, i);
		pthread_cond_signal(&p->sd->consumer_cond);
		Pthread_mutex_unlock(&p->sd->lock);
		printf("thread %d enqueue value:%d\n", p->threadID,i);
	}
	return NULL;
}

void* consumer_thread(void* args) {
	args_t* p = (args_t*)args;
	int i;
	for (i = 0; i < p->times; ++i) {
		Pthread_mutex_lock(&p->sd->lock);
		while (empty(&p->sd->aq))
			pthread_cond_wait(&p->sd->consumer_cond, &p->sd->lock);
		int get=-1;
		dequeue(&p->sd->aq, &get);
		pthread_cond_signal(&p->sd->producer_cond);
		Pthread_mutex_unlock(&p->sd->lock);
		printf("thread %d dequeue value:%d\n",p->threadID, get);
	}
	return NULL;
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage:command [times]\n");
		return 0;
	}
	int i;
	shared_data sd;
	pthread_t p[4];
	args_t parg[4];
	shared_data_init(&sd);
	for (i = 0; i < 4; ++i) 
		args_init(&parg[i],atoi(argv[1]), &sd,i);

	Pthread_create(&p[0], NULL, producer_thread, (void*)&parg[0]);
	Pthread_create(&p[1], NULL, consumer_thread, (void*)&parg[1]);
	Pthread_create(&p[2], NULL, producer_thread, (void*)&parg[2]);
	Pthread_create(&p[3], NULL, consumer_thread, (void*)&parg[3]);
	Pthread_join(p[0], NULL);
	Pthread_join(p[1], NULL);
	Pthread_join(p[2], NULL);
	Pthread_join(p[3], NULL);
	shared_data_destroy(&sd);

	//array_queue aq;
	//queue_init(&aq);
	//int i;
	//for (i = 0; i < 20; i++)
	//	enqueue(&aq, i);
	//printf("queue size now:%d\n", aq.size);
	////queue_travel(&aq);

	//for (i = 0; i < 20; ++i) {
	//	int value;
	//	dequeue(&aq, &value);
	//	printf("dequeue value:%d\n", value);
	//}
	//printf("queue size now:%d\n", aq.size);

	return 0;
}