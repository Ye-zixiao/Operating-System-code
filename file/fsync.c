#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<assert.h>

int main(int argc,char** argv)
{
	int fd=open("foo",O_CREAT|O_WRONLY|O_TRUNC);
	assert(fd>-1);
	int rc=write(fd,"hello china,hello world\n",24);
	assert(rc==24);
	rc=fsync(fd);
	assert(rc==0);
	close(fd);
	
	return 0;
}
