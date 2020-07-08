#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<assert.h>
#include<dirent.h>

int main(int argc, char* argv[])
{
	const char** p = argv;//这是一个很好说明怎么使用main参数的例子
	for (; *p != NULL; )
		printf("%s\n", *p++);

	return 0;
}
