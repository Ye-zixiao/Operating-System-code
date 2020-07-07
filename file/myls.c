#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<assert.h>

typedef struct dirent dirent;

int main(int argc,char**argv)
{
	if(argc!=2){
		printf("Usage:ls dir_name\n");
		return 0;
	}

	DIR* pdir;
	dirent *p_indir;
	pdir=opendir(argv[1]);
	assert(pdir!=NULL);
	while((p_indir=readdir(pdir))!=NULL)
		//printf("%d %s %s\n",(int)p_indir->d_ino,p_indir->d_type,p_indir->d_name);
		printf("%d %s\n",(int)p_indir->d_ino,p_indir->d_name);

	closedir(pdir);	
	return 0;
}
