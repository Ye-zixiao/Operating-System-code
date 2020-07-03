#ifndef ARRAY_QUEUE_H_
#define ARRAY_QUEUE_H_
#define ARRAY_QUEUE_SIZE 20

#include<stdbool.h>
#include<assert.h>
#include<stdio.h>

typedef struct array_queue {
	int arr[ARRAY_QUEUE_SIZE];
	int head_pos;
	int tail_pos;
	int size;
}array_queue;

void queue_init(array_queue* aq) {
	aq->head_pos = aq->tail_pos = 0;
	aq->size = 0;
}

bool empty(const array_queue* aq) {
	return aq->size == 0 ? true : false;
}

bool full(const array_queue* aq) {
	return aq->size == ARRAY_QUEUE_SIZE ? true : false;
}

void enqueue(array_queue* aq, int value) {
	assert(!full(aq));
	aq->arr[aq->tail_pos] = value;
	aq->tail_pos = (aq->tail_pos + 1) % ARRAY_QUEUE_SIZE;
	aq->size++;
}

void dequeue(array_queue* aq, int* ret_value) {
	assert(!empty(aq));
	*ret_value = aq->arr[aq->head_pos];
	aq->head_pos = (aq->head_pos + 1) % ARRAY_QUEUE_SIZE;
	aq->size--;
}

void queue_travel(const array_queue* aq) {
	//printf("head_pos:%d,tail_pos:%d\n", aq->head_pos, aq->tail_pos);
	int i, pos = aq->head_pos, size = aq->size;
	for (i = 0; i < size; ++i, pos = (pos + 1) % ARRAY_QUEUE_SIZE)
		printf("value:%d\n", aq->arr[pos]);
}

#endif
