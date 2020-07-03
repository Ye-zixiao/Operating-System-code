#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<pthread.h>
#include"PPthread.h"

typedef struct node_t {
	int value;
	struct node_t* pnext;
}node_t;

typedef struct queue_t {
	node_t* phead;
	node_t* ptail;
	pthread_mutex_t head_lock;
	pthread_mutex_t tail_lock;
}queue_t;

typedef struct args_t {
	int times;
	queue_t* queue;
}args_t;

void queue_init(queue_t* q);
void enqueue(queue_t* q, int value);
bool dequeue(queue_t* q, int* ret_value);
int queue_size(queue_t* q);
void queue_destroy(queue_t* q);
void queue_clean(queue_t* q);
void* mythread_add(void* args);
void* mythread_pop(void* args);
void args_init(args_t* a,queue_t*q,int times);

int main(int args, char** argv)
{
	if (args != 2) {
		printf("Usage:command [counter]\n");
		return 0;
	}
	int i;
	queue_t q;
	queue_init(&q);
	pthread_t p[4];
	args_t a[4];
	for (i = 0; i < 4; ++i)
		args_init(&a[i], &q, atoi(argv[1]));

	Pthread_create(&p[0], NULL, mythread_add, (void*)&a[0]);
	Pthread_create(&p[1], NULL, mythread_pop, (void*)&a[1]);
	Pthread_create(&p[2], NULL, mythread_add, (void*)&a[2]);
	Pthread_create(&p[3], NULL, mythread_pop, (void*)&a[3]);
	for (i = 0; i < 4; ++i)
		Pthread_join(p[i], NULL);
	printf("quque size:%d\n", queue_size(&q));
	queue_destroy(&q);

	return 0;
}

void queue_init(queue_t* q) {
	node_t* ptemp = (node_t*)malloc(sizeof(node_t));
	assert(ptemp != NULL);
	ptemp->pnext = NULL;
	q->phead = q->ptail = ptemp;
	Pthread_mutex_init(&q->head_lock,NULL);
	Pthread_mutex_init(&q->tail_lock, NULL);
	//printf("queue init success\n");
}

void enqueue(queue_t* q, int value) {
	node_t* pnew = (node_t*)malloc(sizeof(node_t));
	assert(pnew != NULL);
	pnew->pnext = NULL;
	pnew->value = value;
	//printf("add %d\n", pnew->value);

	Pthread_mutex_lock(&q->tail_lock);
	q->ptail->pnext = pnew;
	q->ptail = pnew;
	Pthread_mutex_unlock(&q->tail_lock);
}

bool dequeue(queue_t* q,int *ret_value) {
	Pthread_mutex_lock(&q->head_lock);
	node_t* ptemp = q->phead;
	node_t* new_head = ptemp->pnext;
	if (new_head == NULL) {
		Pthread_mutex_unlock(&q->head_lock);
		return false;
	}
	*ret_value = ptemp->value;
	q->phead = new_head;
	Pthread_mutex_unlock(&q->head_lock);
	free(ptemp);
	return true;
}

void queue_clean(queue_t* q) {
	int temp;
	while (dequeue(q, &temp));
}

void queue_destroy(queue_t* q) {
	queue_clean(q);
	free(q->phead);
	q->phead = NULL;
	q->ptail = NULL;
	Pthread_mutex_destroy(&q->head_lock);
	Pthread_mutex_destroy(&q->tail_lock);
}

void* mythread_add(void* args) {
	args_t* p = (args_t*)args;
	int i;
	//printf("times:%d\n", p->times);
	for (i = 0; i < p->times; ++i) {
		enqueue(p->queue, i);
		printf("enqueue value:%d\n", i);
	}
	return NULL;
}

void* mythread_pop(void* args) {
	args_t* p = (args_t*)args;
	int i, temp;
	for (i = 0; i < p->times; ++i) {
		if (dequeue(p->queue, &temp))
			printf("dequeue value:%d\n", temp);
		else
			printf("dequeue value:NULL\n");
	}
	return NULL;
}

void args_init(args_t* a,queue_t*q,int times) {
	a->queue = q;
	a->times = times;
	//printf("args init success %d\n",a->times);
}

int queue_size(queue_t* q) {
	node_t* p = q->phead;
	int ret = 0;
	while (p != q->ptail) {
		++ret;
		p = p->pnext;
	}
	return ret;
}