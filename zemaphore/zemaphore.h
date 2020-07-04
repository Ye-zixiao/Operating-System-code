#ifndef ZEMAPHORE_H_
#define ZEMAPHORE_H_

#include<pthread.h>
#include"Pthread.h"

typedef struct zem_t {
	int value;
	pthread_mutex_t lock;
	pthread_cond_t cond;
}zem_t;

void zem_init(zem_t* z,int init_value) {
	/*
		1、当初始值为1时，可以用这个自定义zemaphore信号量实现二值信号量（即锁）；
		2、当初始值为0时，可以用这个自定义zemaphore信号量实现（消费者这样的）条件变量；
		3、当初始值为buffer_size时，可以将这个自定义zemaphore信号量实现（生产者这样的）条件变量。
	*/
	z->value = init_value;
	Pthread_mutex_init(&z->lock, NULL);
	Pthread_cond_init(&z->cond, NULL);
}

void zem_destroy(zem_t* z) {
	Pthread_mutex_destroy(&z->lock);
	Pthread_cond_destroy(&z->cond);
}

void zem_wait(zem_t* z) {
	Pthread_mutex_lock(&z->lock);
	while (z->value <= 0)
		pthread_cond_wait(&z->cond, &z->lock);
	z->value--;
	Pthread_mutex_unlock(&z->lock);
}

void zem_post(zem_t* z) {
	Pthread_mutex_lock(&z->lock);
	z->value++;
	pthread_cond_signal(&z->cond);
	Pthread_mutex_unlock(&z->lock);
}


#endif