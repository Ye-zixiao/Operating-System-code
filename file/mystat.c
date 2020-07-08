#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<assert.h>

int main(int argc,char**argv)
{
	if(argc!=2){
		fprintf(stderr,"Usage:mystat dir/file_name\n");
		exit(-1);
	}		
	
	struct stat* pstat=(struct stat*)malloc(sizeof(struct stat));
	assert(pstat!=NULL);
	int rc=stat(argv[1],pstat);
	assert(rc==0);

	printf("file path: %s\n",argv[1]);
	printf("file size: %d\n",(int)pstat->st_size);
	printf("dick block numbers: %d\n",(int)pstat->st_blocks);
	printf("link size: %d\n",(int)pstat->st_nlink);	
	
	free(pstat);
	return 0;
}
