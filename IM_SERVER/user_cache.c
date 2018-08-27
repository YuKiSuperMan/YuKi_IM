#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "./head/user_cache.h"
void* init_cache()
{
	return (void*)malloc(sizeof(struct USER_CACHE_TREE));
}

void insert_tree_port(struct USER_CACHE_TREE* tree, int user_id, char* ip_str, int port,struct USER_CACHE_PORT* user_cache_port)
{
	if(user_cache_port == NULL)
	{
		user_cache_port = (struct USER_CACHE_PORT*)malloc(sizeof(struct USER_CACHE_PORT));
		user_cache_port->user_cache.user_id = user_id;
		strcpy(user_cache_port->user_cache.ip_str,ip_str);
		user_cache_port->user_cache.port = port;
		user_cache_port -> rigthnext = NULL;
		user_cache_port -> leftnext = NULL;
	}

	//先拿到顶端元素
	if(tree->top == NULL)
	{
		tree->top = user_cache_port;
	}
	else
	{
		insert_port(tree->top,user_cache_port);
	}
}

void insert_port(struct USER_CACHE_PORT* old_user_cache_port,struct USER_CACHE_PORT* new_user_cache_port)
{
	//先获取old的ID
	if(old_user_cache_port->user_cache.user_id < new_user_cache_port->user_cache.user_id)
	{
		//如果右边为空则放到右边
		if(old_user_cache_port->rigthnext == NULL)
		{
			old_user_cache_port->rigthnext = new_user_cache_port;
			return;
		}
		else
		{
		//如果不是空则递归
			insert_port(old_user_cache_port->rigthnext,new_user_cache_port);
		}
	}
	else if(old_user_cache_port->user_cache.user_id > new_user_cache_port->user_cache.user_id)
	{
		//如果右边为空则放到右边
		if(old_user_cache_port->leftnext == NULL)
		{
			old_user_cache_port->leftnext = new_user_cache_port;
			return;
		}
		else
		{
		//如果不是空则递归
			insert_port(old_user_cache_port->leftnext,new_user_cache_port);
		}
	}

}

int find_tree_port(struct USER_CACHE_TREE* tree,struct USER_CACHE* need_user_cache)
{
	if(tree->top == NULL)
	{
		return -1;
	}
	return find_port(tree->top, need_user_cache);
	
}

int find_port(struct USER_CACHE_PORT* old_user_cache_port,struct USER_CACHE* need_user_cache)
{
	printf("经过节点id是 %d \n", old_user_cache_port->user_cache.user_id);
	if(old_user_cache_port->user_cache.user_id == need_user_cache->user_id)	
	{
		need_user_cache->user_id = old_user_cache_port->user_cache.user_id;
		strcpy(need_user_cache->ip_str,old_user_cache_port->user_cache.ip_str);
		need_user_cache->port = old_user_cache_port->user_cache.port;
		return 0;
	}
	if(old_user_cache_port->user_cache.user_id <= need_user_cache->user_id)
	{
		if(old_user_cache_port->rigthnext == NULL)
		{
			return -1;
		}
		return find_port(old_user_cache_port->rigthnext, need_user_cache);
	}
	else
	{
		if(old_user_cache_port->leftnext == NULL)
		{
			return -1;
		}
		return find_port(old_user_cache_port->leftnext, need_user_cache);
	}
	
}

int remove_tree_port(struct USER_CACHE_TREE* tree,int user_id)
{       
        if(tree->top == NULL)
        {       
                return 0;
        }
	if(tree->top->user_cache.user_id == user_id)
	{
		struct USER_CACHE_PORT* old_user_cache_port = tree->top;
		tree->top = NULL;
		if(old_user_cache_port->leftnext != NULL)
		insert_tree_port(tree,0,"",0,old_user_cache_port->leftnext);
		if(old_user_cache_port->rigthnext != NULL)
		insert_tree_port(tree,0,"",0,old_user_cache_port->rigthnext);
		//注意一定要free掉
		free(old_user_cache_port);
		return 0;
	}
	struct USER_CACHE_PORT* need_user_cache_ports;
	struct USER_CACHE_PORT* need_up_user_cache_ports;
	struct USER_CACHE_PORT** need_user_cache_port = &need_user_cache_ports;
	struct USER_CACHE_PORT** need_up_user_cache_port = &need_up_user_cache_ports;
	int f;
        if(find_up_port(tree->top, need_user_cache_port,need_up_user_cache_port, &f, user_id) == -1)
	{
			return 0;
	}

	printf("此时上节点ID是 %d \n", ((struct USER_CACHE_PORT*)(*need_up_user_cache_port))->user_cache.user_id);
	printf("此时子节点ID是 %d \n", ((struct USER_CACHE_PORT*)(*need_user_cache_port))->user_cache.user_id);

	if(f)
	{
		((struct USER_CACHE_PORT*)(*need_up_user_cache_port))->leftnext = NULL;
		if(((struct USER_CACHE_PORT*)(*need_user_cache_port)) -> leftnext != NULL)
		insert_port((struct USER_CACHE_PORT*)(*need_up_user_cache_port),((struct USER_CACHE_PORT*)(* need_user_cache_port)) -> leftnext);
		if(((struct USER_CACHE_PORT*)(*need_user_cache_port)) -> rigthnext != NULL)
		insert_port((struct USER_CACHE_PORT*)(*need_up_user_cache_port), ((struct USER_CACHE_PORT*)(*need_user_cache_port)) -> rigthnext);
		free(((struct USER_CACHE_PORT*)(* need_user_cache_port)));
	}
	else
	{
		((struct USER_CACHE_PORT*)(*need_up_user_cache_port)) -> rigthnext = NULL;
		if(((struct USER_CACHE_PORT*)(*need_user_cache_port)) -> leftnext != NULL)
		insert_port((struct USER_CACHE_PORT*)(*need_up_user_cache_port), ((struct USER_CACHE_PORT*)(*need_user_cache_port)) -> leftnext);
		if(((struct USER_CACHE_PORT*)(*need_user_cache_port)) -> rigthnext != NULL)
		insert_port((struct USER_CACHE_PORT*)(*need_up_user_cache_port), ((struct USER_CACHE_PORT*)(*need_user_cache_port)) -> rigthnext);
		free(((struct USER_CACHE_PORT*)(*need_user_cache_port)));
	}	

}

int find_up_port(struct USER_CACHE_PORT* old_user_cache_port, struct USER_CACHE_PORT** need_user_cache_port, struct USER_CACHE_PORT** need_up_user_cache_port,int* f, int user_id)
{
        if(old_user_cache_port->user_cache.user_id <= user_id)
        {
                if(old_user_cache_port->rigthnext == NULL)
                {
                        return -1;
                }
		if(old_user_cache_port->rigthnext->user_cache.user_id == user_id)
		{
			*need_up_user_cache_port = old_user_cache_port;
			printf("找到上节点ID = %d\n", old_user_cache_port->user_cache.user_id);
			*need_user_cache_port = old_user_cache_port->rigthnext;
			printf("找到右节点ID = %d\n", old_user_cache_port->rigthnext->user_cache.user_id);
			*f = 0;
			return 0;
		}
                return find_up_port(old_user_cache_port->rigthnext, need_user_cache_port, need_up_user_cache_port,f, user_id);
        }
        else
        {
                if(old_user_cache_port->leftnext == NULL)
                {
                        return -1;
                }
		if(old_user_cache_port->leftnext->user_cache.user_id == user_id)
		{
			*need_up_user_cache_port = old_user_cache_port;
			printf("找到上节点ID = %d\n", old_user_cache_port->user_cache.user_id);
                        *need_user_cache_port = old_user_cache_port->leftnext;
			printf("找到左节点ID = %d\n", old_user_cache_port->leftnext->user_cache.user_id);
			*f = 1;
			return 0;
		}
                return find_up_port(old_user_cache_port->leftnext, need_user_cache_port, need_up_user_cache_port,f,user_id);
        }
 
}



void print_all(struct USER_CACHE_PORT* top_port)
{
	if(top_port != NULL)
	{
		printf("%d | ",top_port->user_cache.user_id);
		print_all(top_port->leftnext);
		print_all(top_port->rigthnext);
	}
}









