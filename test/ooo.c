#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "user.h"

int fd;
pthread_mutex_t mutex;

struct mm
{
	pthread_t thread;
	int num;
};

void* task(void* p)
{
	struct mm* bnm = (struct mm*)p;
	int num = gs_id(&mutex, fd);
	bnm->num = num;
	return &(bnm->num);
}

void init()
{
	int i = 1;
	int res = write(fd, &i, sizeof(int));
	if(res == -1)printf("错了\n");

}

int main()
{
	
	//先初始化fd
	int res = id_fd_init(&fd);
		if(res == -1)perror("id_fd_init"),exit(-1);
	//初始化线程锁
	pthread_init(&mutex);
	int i = 1;
	
	struct mm* bnm = (struct mm*)malloc(sizeof(struct mm) * 20);
	for(; i < 20; i++)
	{
		pthread_create(&(bnm[i].thread), 0, (void*)task, (void*)&(bnm[i]));
	}

	for(i = 0; i < 20; i++)
	{
		int* numptr;
	}

	sleep(30);

	for(i = 0; i < 20; i++)
	{
		printf("线程 %d 的值是 %d \n", i,bnm[i].num);
	}
	return 0;
}

