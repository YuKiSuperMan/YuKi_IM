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
	if(user_cache_port != NULL)
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
	if(old_user_cache_port->user_cache.user_id <= new_user_cache_port->user_cache.user_id)
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
	else
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

int find_tree_port(struct USER_CACHE_TREE* tree,struct USER_CACHE_PORT* need_user_cache_port,int user_id)
{
	if(tree->top == NULL)
	{
		return -1;
	}
	return find_port(tree->top, need_user_cache_port, user_id);
	
}

int find_port(struct USER_CACHE_PORT* old_user_cache_port,struct USER_CACHE_PORT* need_user_cache_port,int user_id)
{
	if(old_user_cache_port->user_cache.user_id == user_id)	
	{
		need_user_cache_port = old_user_cache_port;
		return 0;
	}
	if(old_user_cache_port->user_cache.user_id <= need_user_cache_port->user_cache.user_id)
	{
		if(old_user_cache_port->rigthnext == NULL)
		{
			return -1;
		}
		return find_port(old_user_cache_port->rigthnext, need_user_cache_port, user_id);
	}
	else
	{
		if(old_user_cache_port->leftnext == NULL)
		{
			return -1;
		}
		return find_port(old_user_cache_port->leftnext, need_user_cache_port, user_id);
	}
	
}

int remove_tree_port(struct USER_CACHE_TREE* tree,int user_id)
{       
        if(tree->top == NULL)
        {       
                return 0;
        }
	if(tree->top->user_cache.user_id = user_id)
	{
		struct USER_CACHE_PORT* old_user_cache_port = tree->top;
		tree->top = NULL;
		if(old_user_cache_port->leftnext != NULL)
		insert_tree_port(tree,0,"",0,old_user_cache_port->leftnext);
		if(old_user_cache_port->rigthnext != NULL)
		insert_tree_port(tree,0,"",0,old_user_cache_port->rigthnext);
		return 0;
	}
	struct USER_CACHE_PORT* need_user_cache_port;
	struct USER_CACHE_PORT* need_up_user_cache_port;
	int f;
        if(find_up_port(tree->top, need_user_cache_port,need_up_user_cache_port, &f, user_id) == -1)
	{
			return 0;
	}
	if(f)
	{
		need_up_user_cache_port->leftnext = NULL;
		if(need_user_cache_port -> leftnext != NULL)
		insert_port(need_up_user_cache_port, need_user_cache_port -> leftnext);
		if(need_user_cache_port -> rigthnext != NULL)
		insert_port(need_up_user_cache_port, need_user_cache_port -> rigthnext);
	}
	else
	{
		need_up_user_cache_port-> rigthnext = NULL;
		if(need_user_cache_port -> leftnext != NULL)
		insert_port(need_up_user_cache_port, need_user_cache_port -> leftnext);
		if(need_user_cache_port -> rigthnext != NULL)
		insert_port(need_up_user_cache_port, need_user_cache_port -> rigthnext);
	}	

}

int find_up_port(struct USER_CACHE_PORT* old_user_cache_port, struct USER_CACHE_PORT* need_user_cache_port, struct USER_CACHE_PORT* need_up_user_cache_port,int* f, int user_id)
{
        if(old_user_cache_port->user_cache.user_id <= need_user_cache_port->user_cache.user_id)
        {
                if(old_user_cache_port->rigthnext == NULL)
                {
                        return -1;
                }
		if(old_user_cache_port->rigthnext->user_cache.user_id == user_id)
		{
			need_up_user_cache_port = old_user_cache_port;
			need_user_cache_port = old_user_cache_port->rigthnext;
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
			need_up_user_cache_port = old_user_cache_port;
                        need_user_cache_port = old_user_cache_port->leftnext;
			*f = 1;
			return 0;
		}
                return find_up_port(old_user_cache_port->leftnext, need_user_cache_port, need_up_user_cache_port,f,user_id);
        }
 
}














