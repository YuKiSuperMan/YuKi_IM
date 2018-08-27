#ifndef _USER_H
#define _USER_H
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define id_path  "/user.id"
#define user_path  "/user"
#define STRINGF 20
struct USER
{
	int user_id;
	char name[20];
	char password[20];
};

//用户注册
int reg_user(struct USER* user);
//初始化id文件描述符
int id_fd_init(int* fd);
//关闭id文件描述符
int id_fd_close(int fd);
//初始化线程锁
int pthread_init(pthread_mutex_t* mutex);
//销毁线程锁
int pthread_destroy(pthread_mutex_t* mutex);
//获取ID并修改ID 此处需要进行线程同步
int gs_id(pthread_mutex_t* mutex, int fd);




#endif
