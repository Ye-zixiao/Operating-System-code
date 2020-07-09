#ifndef _DR_LINKLIST_H_
#define _DR_LINKLIST_H_

#include<stdlib.h>
#include<stdio.h>

typedef struct node_t {
	int line_sz;
	struct node_t* ppre;
	struct node_t* pnext;
}node_t;

typedef struct dr_linklist {
	int size;
	node_t* pbegin;
	node_t* pend;
}dr_linklist;

void init_dr_linklist(dr_linklist* p) {
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	new_node->pnext = new_node->ppre = NULL;
	new_node->line_sz = 0;
	p->size = 0;
	p->pbegin = p->pend = new_node;
}

int empty(const dr_linklist* p) {
	return p->pbegin == p->pend ? 0 : -1;
}

void insert(dr_linklist* p, node_t* new_node) {
	int bl = empty(p);
	node_t* pnext = p->pbegin->pnext;
	new_node->pnext = pnext;
	new_node->ppre = p->pbegin;
	p->pbegin->pnext = new_node;
	if(bl==0)
		p->pend = new_node;
	else {
		//printf("insert not first\n");
		pnext->ppre = new_node;
	}
	p->size++;
}

void destroy(dr_linklist* p) {
	node_t* pdel = p->pbegin;
	while (pdel != NULL) {
		node_t* ptem = pdel;
		pdel = pdel->pnext;
		free(ptem);
	}
	p->size = 0;
	p->pbegin = p->pend = NULL;
}

int size(const dr_linklist* p) {
	return p->size;
}

#endif