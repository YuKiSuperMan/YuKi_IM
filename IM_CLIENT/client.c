#include <stdio.h>
#include "user.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int connect_resserver(int* res_sockfd)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1)return -2;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = inet_addr("192.168.1.8");
	//建立连接
	int res = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
	if(res == -1)return -1;
	*res_sockfd = fd;
	return 0;
}

void res_user()
{
	printf("请输入您的名字\n");
	struct USER user;
	scanf("%s",user.name);
	printf("请输入密码\n");
	scanf("%s",user.password);
	int sockfd;
	int res = connect_resserver(&sockfd);
		if(res != 0)
		{
			printf("连接出错!!\n");
		}
	send(sockfd, &user, sizeof(user), 0);
	recv(sockfd, &user, sizeof(user), 0);
	printf("注册成功，您的ID是 %d 请务必记住!!!  \n",user.user_id);

}

void login_user()
{


}



int main()
{
	printf("===================欢迎进入聊天室!!===================\n");
	printf("===================输入\"1\"进行注册====================\n");
	printf("===================输入\"2\"进行登陆====================\n");
	printf("=====================输入\"3\"退出======================\n");
	int op;
	scanf("%d",&op);
	switch(op)
	{
	case 1:
		res_user();
		break;
	case 2:
		break;
	case 3:
		printf("即将退出系统\n");
		sleep(1);
		return;
	}
	return 0;
}
