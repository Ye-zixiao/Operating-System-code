#ifndef PTHREAD_H_
#define PTHREAD_H_

#include<pthread.h>
#include<assert.h>

void Pthread_mutex_init(pthread_mutex_t*mutex,
	const pthread_mutex_t*attr) {
	int rc = pthread_mutex_init(mutex, NULL);
	assert(rc == 0);
}

void Pthread_mutex_destroy(pthread_mutex_t* mutex) {
	int rc = pthread_mutex_destroy(mutex);
	assert(rc == 0);
}

void Pthread_mutex_lock(pthread_mutex_t* mutex) {
	int rc = pthread_mutex_lock(mutex);
	assert(rc == 0);
}

void Pthread_mutex_unlock(pthread_mutex_t* mutex) {
	int rc = pthread_mutex_unlock(mutex);
	assert(rc == 0);
}

void Pthread_create(pthread_t* thread, const pthread_attr_t* attr,
	void* (*pf)(void*), void* args) {
	int rc = pthread_create(thread, attr, pf, args);
	assert(rc == 0);
}

void Pthread_join(pthread_t thread, void** retval) {
	int rc = pthread_join(thread, retval);
	assert(rc == 0);
}

void Pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr) {
	int rc = pthread_cond_init(cond, attr);
	assert(rc == 0);
}

void Pthread_cond_destroy(pthread_cond_t* cond) {
	int rc = pthread_cond_destroy(cond);
	assert(rc == 0);
}

#endif