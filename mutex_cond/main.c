#include<pthread.h>
#include"Pthread.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct mutex_cond_t {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}mutex_cond_t;

int shared_value = 0;

void thread_exit(mutex_cond_t* mc);
void* child_thread(void* args);
void thread_join(mutex_cond_t* mc);
void mutex_cond_init(mutex_cond_t* mc);

int main(int argc, char** argv)
{
	printf("parent begin\n");

	mutex_cond_t mc;
	pthread_t p;
	mutex_cond_init(&mc);
	Pthread_create(&p, NULL, child_thread, (void*)&mc);
	thread_join(&mc);

	printf("parent end\n");
	return 0;
}

void mutex_cond_init(mutex_cond_t* mc) {
	Pthread_cond_init(&mc->cond,NULL);
	Pthread_mutex_init(&mc->mutex, NULL);
}

void thread_exit(mutex_cond_t* mc) {
	Pthread_mutex_lock(&mc->mutex);
	shared_value = 1;
	pthread_cond_signal(&mc->cond);
	Pthread_mutex_unlock(&mc->mutex);
}

void thread_join(mutex_cond_t* mc) {
	Pthread_mutex_lock(&mc->mutex);
	while (shared_value == 0)
		pthread_cond_wait(&mc->cond,&mc->mutex);
	Pthread_mutex_unlock(&mc->mutex);
}

void* child_thread(void* args) {
	mutex_cond_t* p = (mutex_cond_t*)args;
	printf("child\n");
	thread_exit(p);
	return NULL;
}