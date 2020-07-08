#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<assert.h>
#include<dirent.h>
#define Max 256

int haveOption(const char**argv,const char*opt){
	const char** p = argv;
	for (; *p != NULL; ++p)
		if (strcmp(opt, *p) == 0)
			return 0;
	return -1;
}

const char* get_path_pointer(const char** argv) {
	const char** p = argv + 1;
	for (; *p != NULL; ++p)
		if ((*p)[0] != '-')
			return *p;
		
	return NULL;
}

void get_defaultdir(char* file_dir_name, size_t sz) {
	char* rc = getcwd(file_dir_name, sz);
	assert(rc != NULL);
}

int main(int argc, char** argv)
{
	char dir_file[Max];
	DIR* pdir;
	struct dirent* p_indir;
	struct stat mystat;
	const char* p_to_path;

	int lopt = haveOption(argv, "-l");					//返回是否有选项的标记值
	if ((p_to_path = get_path_pointer(argv)) != NULL)
		strcpy(dir_file, p_to_path);
	else
		get_defaultdir(dir_file,Max);

	if ((pdir = opendir(dir_file)) == NULL) {
		fprintf(stderr, "Can't open this dir %s\n", dir_file);
		return -1;
	}
	while ((p_indir = readdir(pdir)) != NULL) {
		if (lopt == 0) {
			char temp_path[Max] = "";
			strcat(temp_path, p_indir->d_name);
			stat(temp_path, &mystat);
			//printf("type_n_mode:%d uid:%d gid:%d size:%dk filename:%s\n",(int)mystat.st_mode, (int)mystat.st_uid, (int)mystat.st_gid,
			//	mystat.st_size/1024, p_indir->d_name);		//权限，用户名，用户组名，大小，文件名
			printf("%d %d %d %ld %s\n", (int)mystat.st_mode, (int)mystat.st_uid, (int)mystat.st_gid,
				mystat.st_size, p_indir->d_name);
		}
		else
			printf("%s\n", p_indir->d_name);
	}
	closedir(pdir);

	return 0;
}
