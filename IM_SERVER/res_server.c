//注册服务器 用tcp 模拟http
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "./head/user.h"
#include <stdlib.h>
#define ERROR(P) perror(P),exit(-1)


pthread_mutex_t mutex;
int ffd;


void* accept_client(void* p)
{
	int* sockfd = (int*)p;
	struct USER user;
	while(1)
	{
		int res = recv(*sockfd, &user, sizeof(user), 0);
		if(res <= 0)
		{
			printf("断开连接\n");
			close(*sockfd);
			return;
		}
		printf("接收到字符串 : %s  和  %s\n", user.name, user.password);
		int id = gs_id(&mutex, ffd);
		user.user_id = id;
		int rres = reg_user(&user);
			if(rres != 0)
			{
				printf("注册失败\n");
			}
		send(*sockfd, &user, sizeof(user), 0);
	}

}



int main(int args,char** argv)
{
	//获取启动参数
	char ip_str[15] = {};
	char port_str[4] = {};
	int backlog = 10;
	if(argv[1] == NULL)
	{
		printf("启动失败 : 缺少参数 ip\n");
		return 0;
	}
	if(argv[2] == NULL)
	{
		printf("启动失败 : 缺少参数 port\n");
		return 0;
	}
	if(argv[3] != NULL)
	{
		backlog = atoi(argv[3]);
	}
	strcpy(ip_str,argv[1]);
	strcpy(port_str,argv[2]);

	int rres = id_fd_init(&ffd);
	rres = pthread_init(&mutex);

	//创建套接字 
	int fd = socket(AF_INET, SOCK_STREAM, 0);
		if(fd == -1)perror("创建套接字失败"),exit(-1);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port_str));
	addr.sin_addr.s_addr = inet_addr(ip_str);

	//绑定端口
	int res = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
		if(res == -1)perror("绑定端口失败"),exit(-1);
	//建立监听
	res = listen(fd, backlog);
		if(res == -1)ERROR("建立监听失败");

	struct sockaddr_in from;
	while(1)
	{
		socklen_t len = sizeof(from);//是传入传出参数
		int* sockfd = (int*)malloc(sizeof(int));
		*sockfd = accept(fd, (struct sockaddr*)&from, &len);
		printf("接收到来自客户端 %s 的请求\n", inet_ntoa(from.sin_addr));
		//创建线程 处理该客户端
		pthread_t thread;
		pthread_create(&thread, 0, (void*)accept_client, (void*)sockfd);
		//pthread_join(thread, 0);
		
	}

	id_fd_close(ffd);
	pthread_destroy(&mutex);
	
	return 0;
}
