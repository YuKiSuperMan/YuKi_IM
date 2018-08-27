#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "./head/user.h"


int get_user(struct USER* user,char* buf,int len)
{
	int i = 0,z = 0,y = 0,n = 0,p = 0;
	for(; i < len; i++)
	{
		printf("%c",buf[i]);
		if(buf[i] == '[')
		{
			z++;continue;
		}
		if(buf[i] == ']')
		{
			y++;continue;
		}
		if(z == 1 && y == 0)
		{
			user->name[n] = buf[i];
			n++;
		}
		if(z == 2 && y == 1)
		{
			user->password[p] = buf[i];
			p++;
		}
		if(z > 2 || y >= 2)
		{
			break;
		}
	}
	user->name[n] = '\0';
	user->password[p] = '\0';
	printf("\n");
}

int reg_user(struct USER* user)
{
	//先拼接字符串得到文件路径及其名称
	char fname[100] = {};
	strcpy(fname,user_path);
	char buf[20] = {};
	sprintf(buf,"%d.user",user->user_id);	
	strcat(fname,buf);
	printf("%s\n",fname);
	int fd = open(fname, O_CREAT | O_RDWR, 0666);
		if(fd == -1)return -1;
	char ubuf[100] = {};
	ubuf[0] = '[';
	strcat(ubuf, user->name);
	strcat(ubuf,"]");
	strcat(ubuf,"[");
	strcat(ubuf,user->password);
	strcat(ubuf,"]");
	int res = write(fd, ubuf, 100);
		if(res == -1)return -2;
	printf("长度是 %d\n",strlen(ubuf));
	lseek(fd, 0, SEEK_SET);
	memset(ubuf,0,sizeof(ubuf));
	res = read(fd, ubuf, 100);
	printf("长度是 %d\n",strlen(ubuf));
	struct USER u;
	printf("Name = %s", u.name);
	printf("Password = %s", u.password);
	get_user(&u, ubuf, 40);
	printf("Name = %s", u.name);
	printf("Password = %s", u.password);
	return 0;
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
