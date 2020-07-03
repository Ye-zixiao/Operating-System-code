#include<pthread.h>
#include"Pthread.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>

typedef struct counter_t {
	pthread_mutex_t lock;
	int value;
}counter_t;

typedef struct myargs_t {
	int pthreadID;
	int times;
}myargs_t;

counter_t c;

void init_counter(counter_t* c);
void destroy_counter(counter_t* c);
void init_args(myargs_t* m,int threadID,int times);
void increment(counter_t* c);
int get(counter_t* c);
void* mythread(void* args);

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage:command [counter]\n");
		return 0;
	}
	init_counter(&c);
	pthread_t p1, p2, p3, p4;
	myargs_t p1_args, p2_args, p3_args, p4_args;
	init_args(&p1_args, 0, atoi(argv[1]));
	init_args(&p2_args, 1, atoi(argv[1]));
	init_args(&p3_args, 2, atoi(argv[1]));
	init_args(&p4_args, 3, atoi(argv[1]));

	Pthread_create(&p1, NULL, mythread, (void*)&p1_args);
	Pthread_create(&p2, NULL, mythread, (void*)&p2_args);
	Pthread_create(&p3, NULL, mythread, (void*)&p3_args);
	Pthread_create(&p4, NULL, mythread, (void*)&p4_args);
	Pthread_join(p1, NULL);
	Pthread_join(p2, NULL);
	Pthread_join(p3, NULL);
	Pthread_join(p4, NULL);
	printf("result:%d\n", get(&c));

	return 0;
}

void init_counter(counter_t* c) {
	Pthread_mutex_init(&c->lock, NULL);
	c->value = 0;
}

void destroy_counter(counter_t* c) {
	Pthread_mutex_destroy(&c->lock);
}

void init_args(myargs_t* m, int threadID, int times) {
	m->pthreadID = threadID;
	m->times = times;
}

void increment(counter_t* c) {
	Pthread_mutex_lock(&c->lock);
	c->value++;
	Pthread_mutex_unlock(&c->lock);
}

int get(counter_t* c) {
	return c->value;
}

void* mythread(void* args) {
	myargs_t* p = (myargs_t*)args;
	int i;
	for (i = 0; i < p->times; ++i) {
		increment(&c);
	}
	return NULL;
}
