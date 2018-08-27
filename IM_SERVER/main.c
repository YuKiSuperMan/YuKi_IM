#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "./head/user_cache.h"
char *ip = "192.168.1.1";
struct USER_CACHE_TREE* tree;
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
	print_all(tree->top);
	printf("]\n");
	printf("============================操作结束===========================\n");
}

int main()
{

	tree = (struct USER_CACHE_TREE*)init_cache();
	printf("==================请输入ID 系统自动创建名字ip和port=====================\n");
	int i = 0;
	int num;
	for(; ; )
	{
		int op = 0;
		printf("请输入操作码 1、添加 2、删除 3、查询 4、展示全部\n");
		scanf("%d",&op);
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
