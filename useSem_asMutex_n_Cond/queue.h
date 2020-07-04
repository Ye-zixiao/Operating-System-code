#pragma once

#include<stdbool.h>
#include<assert.h>
#define MAX 20//buffer size

typedef struct queue {
	int arr[MAX];
	int head_pos;
	int tail_pos;
	int size;
}queue;

bool full(const queue* q) {
	return q->size == MAX;
}

bool empty(const queue* q) {
	return q->size == 0;
}

void queue_init(queue* q) {
	q->head_pos = q->tail_pos = 0;
	q->size = 0;
}

void enqueue(queue* q, int value) {
	assert(!full(q));
	q->arr[q->tail_pos] = value;
	q->tail_pos = (q->tail_pos + 1) % MAX;
	q->size++;
}

void dequeue(queue* q, int* pvalue) {
	assert(!empty(q));
	*pvalue = q->arr[q->head_pos];
	q->head_pos = (q->head_pos + 1) % MAX;
	q->size--;
}

int queue_size(const queue* q) {
	return q->size;
}
