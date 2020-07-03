#include<pthread.h>
#include"Pthread.h"
#include<stdlib.h>
#include<stdio.h>

typedef struct node_t {
	int value;
	struct node_t* pnext;
}node_t;

typedef struct list_t {
	pthread_mutex_t lock;
	node_t* phead;
}list_t;

typedef struct args_t {
	int times;
	list_t* plist;
}args_t;


void list_init(list_t* l);
void list_insert(list_t* l, int insert_value);
void list_travel(list_t* l);
int list_size(list_t* l);
void list_destroy(list_t* l);
void args_init(args_t* a,int times,list_t*l);
void* mythread(void* args);

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage:command [counter]\n");
		return 0;
	}
	list_t shared_list;
	list_init(&shared_list);
	pthread_t p0, p1, p2, p3;
	args_t p0a, p1a, p2a, p3a;
	args_init(&p0a, atoi(argv[1]), &shared_list);
	args_init(&p1a, atoi(argv[1]), &shared_list);
	args_init(&p2a, atoi(argv[1]), &shared_list);
	args_init(&p3a, atoi(argv[1]), &shared_list);

	Pthread_create(&p0, NULL, mythread, (void*)&p0a);
	Pthread_create(&p1, NULL, mythread, (void*)&p1a);
	Pthread_create(&p2, NULL, mythread, (void*)&p2a);
	Pthread_create(&p3, NULL, mythread, (void*)&p3a);
	Pthread_join(p0, NULL);
	Pthread_join(p1, NULL);
	Pthread_join(p2, NULL);
	Pthread_join(p3, NULL);
	list_travel(&shared_list);
	printf("result:list size=%d\n", list_size(&shared_list));
	list_destroy(&shared_list);

	return 0;
}

void list_init(list_t* l) {
	l->phead = NULL;
	Pthread_mutex_init(&l->lock, NULL);
}

void list_insert(list_t* l, int insert_value) {
	node_t* pnew = (node_t*)malloc(sizeof(node_t));
	if (pnew == NULL) {
		perror("malloc failed!\n");
		return;
	}
	pnew->value = insert_value;

	Pthread_mutex_lock(&l->lock);
	pnew->pnext = l->phead;
	l->phead = pnew;
	Pthread_mutex_unlock(&l->lock);
}

void list_travel(list_t* l) {
	Pthread_mutex_lock(&l->lock);
	node_t* ptravel = l->phead;
	while (ptravel) {
		printf("detecting:%d\n", ptravel->value);
		ptravel = ptravel->pnext;
	}
	Pthread_mutex_unlock(&l->lock);
}

void list_destroy(list_t* l) {
	Pthread_mutex_lock(&l->lock);
	node_t* p = l->phead;
	while (p) {
		node_t* ptemp = p->pnext;
		//printf("deleting %d\n", p->value);
		free(p);
		p = ptemp;
	}
	Pthread_mutex_unlock(&l->lock);
}

int list_size(list_t* l) {
	Pthread_mutex_lock(&l->lock);
	node_t* p = l->phead;
	int sz = 0;
	while (p) {
		++sz;
		p = p->pnext;
	}
	Pthread_mutex_unlock(&l->lock);
	return sz;
}

void* mythread(void* args) {
	args_t* p = (args_t*)args;
	int i;
	for (i = 0; i < p->times; ++i) {
		list_insert(p->plist, i);
	}
	return NULL;
}

void args_init(args_t* a, int times, list_t* l) {
	a->plist = l;
	a->times = times;
}