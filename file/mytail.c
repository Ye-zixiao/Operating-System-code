#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include"dr_linklist.h"
#include<assert.h>

int getOpt(const char**);
const char* getfilename(const char** argv);
void fill_link(dr_linklist* p, const char* fname);

int main(int argc, char* argv[])
{
	if (argc < 2) {
		fprintf(stderr, "usage:mytail [option] filename\n");
		exit(-1);
	}
	const char* pfilename = getfilename(argv);
	if (pfilename == NULL) {
		fprintf(stderr, "error:please enter a filename\n");
		exit(-1);
	}

	dr_linklist dl;
	init_dr_linklist(&dl);
	fill_link(&dl, pfilename);
	int want_line_num = getOpt(argv);
	int file_line_num = size(&dl);
	if (want_line_num > file_line_num)want_line_num = file_line_num;

	char buf[1024];
	int fd = open(pfilename, O_RDONLY);
	int offset = 0;
	int line_delta = file_line_num - want_line_num;
	node_t* p = dl.pend;
	for (; line_delta > 0; --line_delta) {
		offset += p->line_sz;
		p = p->ppre;
	}
	lseek(fd, offset, SEEK_SET);
	while ((offset = read(fd, buf, 1024)) > 0)
		write(STDOUT_FILENO, buf, offset);
	close(fd);
	destroy(&dl);
	exit(0);
}

/*
-------------
*/
int getOpt(const char** argv) {
	const char** p = argv + 1;
	while (*p != NULL) {
		if ((*p)[0] == '-' && (*p)[1] == 'n')
			return atoi(&(*p)[2]);
		++p;
	}
	return 10;
}

const char* getfilename(const char** argv) {
	const char** p = argv + 1;
	while (*p != NULL) {
		if ((*p)[0] != '-')
			return *p;
		++p;
	}
	return NULL;
}

void fill_link(dr_linklist* p, const char* fname) {
	FILE* infile = fopen(fname, "r");
	assert(infile != NULL);
	char tempchar[1024];

	while (fgets(tempchar, 1024, infile)) {
		node_t* pnew = (node_t*)malloc(sizeof(node_t));
		//printf("linesz=%d\n", strlen(tempchar));
		pnew->line_sz = strlen(tempchar);
		pnew->pnext = pnew->ppre = NULL;
		insert(p, pnew);
	}
	//putchar('\n');
	fclose(infile);
}