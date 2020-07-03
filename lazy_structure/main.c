#include<pthread.h>
#include"PPthread.h"
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#define CPUNUMS 4

typedef struct counter_t {
	int global_value;
	pthread_mutex_t global_lock;

	int local_value[CPUNUMS];
	pthread_mutex_t local_lock[CPUNUMS];
	int threshold;
}counter_t;

typedef struct myargs_t {
	int threadID;
	int times;
}myargs_t;

counter_t c;

void init_counter(counter_t* c,int threshold);
void init_myargs(myargs_t* m,int threadID,int times);
void destroy_counter(counter_t* c);
void increment(counter_t* c,int threadID);
int get(counter_t* c);
void* mythread(void* args);

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("Usage:command [counter] [threshold]\n");
		return 0;
	}
	init_counter(&c, atoi(argv[2]));
	pthread_t p0, p1, p2, p3;
	myargs_t p0a, p1a, p2a, p3a;
	init_myargs(&p0a, 0, atoi(argv[1]));
	init_myargs(&p1a, 1, atoi(argv[1]));
	init_myargs(&p2a, 2, atoi(argv[1]));
	init_myargs(&p3a, 3, atoi(argv[1]));

	Pthread_create(&p0, NULL, mythread, (void*)&p0a);
	Pthread_create(&p1, NULL, mythread, (void*)&p1a);
	Pthread_create(&p2, NULL, mythread, (void*)&p2a);
	Pthread_create(&p3, NULL, mythread, (void*)&p3a);
	Pthread_join(p0, NULL);
	Pthread_join(p1, NULL);
	Pthread_join(p2, NULL);
	Pthread_join(p3, NULL);
	printf("result:%d\n", get(&c));

	destroy_counter(&c);
	return 0;
}

void init_counter(counter_t* c, int threshold) {
	c->threshold = threshold;
	c->global_value = 0;
	Pthread_mutex_init(&c->global_lock, NULL);
	int i;
	for (i = 0; i < CPUNUMS; ++i) {
		c->local_value[i] = 0;
		Pthread_mutex_init(&c->local_lock[i], NULL);
	}
}

void init_myargs(myargs_t* m, int threadID, int times) {
	m->threadID = threadID;
	m->times = times;
}

void destroy_counter(counter_t* c) {
	Pthread_mutex_destroy(&c->global_lock);
	int i;
	for (i = 0; i < CPUNUMS; ++i)
		Pthread_mutex_destroy(&c->local_lock[i]);
}

void increment(counter_t* c, int threadID) {
	Pthread_mutex_lock(&c->local_lock[threadID]);
	c->local_value[threadID]++;
	if (c->local_value[threadID] >= c->threshold) {
		Pthread_mutex_lock(&c->global_lock);
		c->global_value += c->local_value[threadID];
		Pthread_mutex_unlock(&c->global_lock);
		c->local_value[threadID] = 0;
	}
	Pthread_mutex_unlock(&c->local_lock[threadID]);
}

int get(counter_t* c) {
	return c->global_value;
}

void* mythread(void* args) {
	myargs_t* p = (myargs_t*)args;
	int i;
	for (i = 0; i < p->times; ++i) {
		increment(&c, p->threadID);
	}
	return NULL;
}