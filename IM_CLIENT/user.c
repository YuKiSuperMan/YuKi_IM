#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "user.h"

int reg_user(struct USER* user)
{
	
}

//初始化id文件描述符
int id_fd_init(int* fd)
{
	*fd = open(id_path,  O_CREAT | O_RDWR, 0666);
	return *fd;
}

//关闭id文件描述符
int id_fd_close(int fd)
{
	close(fd);
}

//初始化线程锁
int pthread_init(pthread_mutex_t* mutex)
{
	pthread_mutex_init(mutex,0);
}

//销毁线程锁
int pthread_destroy(pthread_mutex_t* mutex)
{
	pthread_mutex_destroy(mutex);
}


//获取ID并修改ID 此处需要进行线程同步
int gs_id(pthread_mutex_t* mutex, int fd)
{
	pthread_mutex_lock(mutex);
	char buf[20];
	lseek(fd, 0, SEEK_SET);
	int res = read(fd, buf, STRINGF);
		if(res == -1){pthread_mutex_unlock(mutex);return -1;};
	//读取到这二十位数据之后将其转成int型
	int n = atoi(buf);
	printf("此时读到的的值是 %d\n",n);
	n++;
	//然后又将数字转换成字符串
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d",n);
	sleep(1);
	lseek(fd, 0, SEEK_SET);
	res = write(fd, buf, STRINGF);
		if(res == -1){printf("没写成功\n");pthread_mutex_unlock(mutex);return -2;}	
	printf("此时写入的值是 %d\n",n);
	pthread_mutex_unlock(mutex);
	return n;
	
}
