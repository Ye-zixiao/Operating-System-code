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
		1������ʼֵΪ1ʱ������������Զ���zemaphore�ź���ʵ�ֶ�ֵ�ź�������������
		2������ʼֵΪ0ʱ������������Զ���zemaphore�ź���ʵ�֣������������ģ�����������
		3������ʼֵΪbuffer_sizeʱ�����Խ�����Զ���zemaphore�ź���ʵ�֣������������ģ�����������
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