#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "./head/user_cache_pthread.h"
#define SIZE 100
void add();
char *ip = "192.168.1.1";
struct USER_CACHE_TREE* tree;
struct USER_CACHE user_cache_arr[SIZE];
int kk = 0;
void* task(void* p)
{
	for(;;)
	{
		if(kk == 1)
		{
			break;
		}
	}
	struct USER_CACHE* user = (struct USER_CACHE*)p;
	insert_tree_port(tree, user->user_id, ip,user->port, NULL);
}

void multi_pthread_add()
{
	printf("请输入从多少开始的数\n");
	int o = 0;
	scanf("%d",&o);
	int i = 0;
	for(; i < SIZE; i++)
	{
		user_cache_arr[i].user_id = i + o;
		user_cache_arr[i].port = i + o;
		pthread_t pthread;
		pthread_create(&pthread, 0, (void*)task, &(user_cache_arr[i]));
	}
	int l = 5;
	printf("倒计时开始\n");
	for(; l > 0; l--)
	{
		printf("%d | ", l);
		fflush(stdout);
		sleep(1);
	}
	printf("开始\n");
	kk = 1;

}
void add()
{
	printf("============================操作开始===========================\n");
	int id = 0;
	int port = 0;
	printf("请输入ID\n");
	scanf("%d",&id);
	printf("请输入port\n");
	scanf("%d",&port);
	insert_tree_port(tree, id, ip, port, NULL);	
	printf("============================操作结束===========================\n");
}

void remove_ss()
{
	printf("============================操作开始===========================\n");
	int id = 0;
	printf("请输入需要删除ID\n");
	scanf("%d",&id);
	remove_tree_port(tree,id);
	printf("============================操作结束===========================\n");
}

void find()
{
	printf("============================操作开始===========================\n");
		int id = 0;
                printf("请输入需要查询ID\n");
                scanf("%d",&id);
                struct USER_CACHE user;
                user.user_id = id;
                int a = find_tree_port(tree,&user);
                        if(a == -1){printf("没有\n");}
                else
                {
                        printf("id = %d,port = %d,ip = %s \n", user.user_id,user.port,user.ip_str);
                }
	printf("============================操作结束===========================\n");

}

void print()
{
	printf("============================操作开始===========================\n");
	printf("获取到的->方向[");
	int num = 0;
	print_all_num(tree->top, &num);
	printf("]\n");
	printf("一共有元素 %d 个", num);
	printf("============================操作结束===========================\n");
}

int main()
{
	printf("pid = %d\n",getpid());
	tree = (struct USER_CACHE_TREE*)init_cache();
	printf("==================请输入ID 系统自动创建名字ip和port=====================\n");
	int i = 0;
	int num;
	for(; ; )
	{
		int op = 0;
		printf("请输入操作码 0、多线程插入  1、添加 2、删除 3、查询 4、展示全部\n");
		scanf("%d",&op);
		if(op == 0)
		{
			multi_pthread_add();
		}
		if(op == 1)
		{
			add();
		}
		if(op == 2)
		{
			remove_ss();		
		}
		if(op == 3)
		{
			find();
		}
		if(op == 4)
		{
			print();
		}
	}
}
