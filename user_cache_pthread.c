#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "./head/user_cache_pthread.h"
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
		struct MULTI_THREAD* left_mul_thread = (struct MULTI_THREAD*)malloc(sizeof(struct MULTI_THREAD));
		struct MULTI_THREAD* rigth_mul_thread = (struct MULTI_THREAD*)malloc(sizeof(struct MULTI_THREAD));
		left_mul_thread -> lock = 0;
		rigth_mul_thread -> lock = 0;
		left_mul_thread -> end = -1;
		rigth_mul_thread -> end = -1;
		pthread_mutex_init(&(left_mul_thread->mutex), 0);
		pthread_mutex_init(&(rigth_mul_thread->mutex), 0);
		user_cache_port -> left_mul_thread = left_mul_thread;
		user_cache_port -> rigth_mul_thread = rigth_mul_thread;
		user_cache_port -> del = 0;
	}

	//先拿到顶端元素
	if(tree->top == NULL)
	{
		tree->top = user_cache_port;
	}
	else
	{
		insert_port(tree->top, user_cache_port);	
	}
}

void* init_mul_thread_lock()
{
	struct MULTI_THREAD* p = (struct MULTI_THREAD*)malloc(sizeof(struct MULTI_THREAD));
	p->lock = 0;
	p->end = -1;
	return p;
}

void insert_port(struct USER_CACHE_PORT* old_user_cache_port,struct USER_CACHE_PORT* new_user_cache_port)
{

	struct USER_CACHE_PORT* user_cache_port_p;
	struct USER_CACHE_PORT** user_cache_port_p_p;	
	user_cache_port_p_p = &user_cache_port_p;
	int f;
	find_insert_port(old_user_cache_port, new_user_cache_port->user_cache.user_id, user_cache_port_p_p, &f);
	
	if(f == 0)
	{
		printf("此时需要操作的是上节点id为 %d 的右边节点 当前操作节点是 %d \n", user_cache_port_p -> user_cache.user_id, new_user_cache_port -> user_cache.user_id);
		 if(user_cache_port_p -> left_mul_thread != NULL && user_cache_port_p -> rigth_mul_thread -> lock == 0)
                 {
				user_cache_port_p -> rigth_mul_thread -> end = new_user_cache_port -> user_cache.user_id;
                                int res = pthread_mutex_trylock(&(user_cache_port_p -> rigth_mul_thread -> mutex));
                                if(res == 0)
                                {
					user_cache_port_p->rigthnext = new_user_cache_port;
					printf("%d 用户插入完成\n", new_user_cache_port -> user_cache.user_id);
					sleep(10);
					user_cache_port_p -> rigth_mul_thread -> lock = 1;      
					pthread_mutex_unlock(&(user_cache_port_p -> rigth_mul_thread -> mutex));                         
                                }
                                else
                                {
					printf("%d 对 %d 尝试加锁失败\n", new_user_cache_port -> user_cache.user_id, user_cache_port_p->user_cache.user_id);
					printf(">>>>>>>>>>>>>>>>>>>>>>>> 此时被锁住的id是 %d <<<<<<<<<<<<<<<<<<<<<\n", new_user_cache_port -> user_cache.user_id);
					pthread_mutex_lock(&(user_cache_port_p -> rigth_mul_thread -> mutex));
					pthread_mutex_unlock(&(user_cache_port_p -> rigth_mul_thread -> mutex)); 
					insert_port(user_cache_port_p->rigthnext,  new_user_cache_port);
                                }
				if(user_cache_port_p -> rigth_mul_thread != NULL &&  new_user_cache_port -> user_cache.user_id == user_cache_port_p -> rigth_mul_thread -> end)
				{
					pthread_mutex_destroy(&(user_cache_port_p -> rigth_mul_thread -> mutex));
					free(user_cache_port_p -> rigth_mul_thread);
					user_cache_port_p -> rigth_mul_thread = NULL;
				}
                 }
		else
		{
				insert_port(user_cache_port_p->rigthnext , new_user_cache_port);
		}
	}
	else if(f == 1)
	{
		printf("此时需要操作的是上节点id为 %d 的左边节点 当前操作节点是 %d \n", user_cache_port_p -> user_cache.user_id, new_user_cache_port -> user_cache.user_id);
		 if(user_cache_port_p -> left_mul_thread != NULL && user_cache_port_p -> left_mul_thread -> lock == 0)
                 {
                                user_cache_port_p -> left_mul_thread -> end = new_user_cache_port -> user_cache.user_id;
                                int res = pthread_mutex_trylock(&(user_cache_port_p -> left_mul_thread -> mutex));
                                if(res == 0)
                                {      
					printf("此时的 id = %d 的 leftnext 地址是 %p\n", user_cache_port_p->user_cache.user_id,new_user_cache_port);
                                        user_cache_port_p->leftnext = new_user_cache_port;
					printf("%d 用户插入完成\n", new_user_cache_port -> user_cache.user_id);
					sleep(10);
                                        user_cache_port_p -> left_mul_thread -> lock = 1;
                                        pthread_mutex_unlock(&(user_cache_port_p -> left_mul_thread -> mutex));
                                }
                                else
                                {
					printf("%d 对 %d 尝试加锁失败\n", new_user_cache_port -> user_cache.user_id, user_cache_port_p->user_cache.user_id);
					printf(">>>>>>>>>>>>>>>>>>>>>>>>>此时被锁住的id是 %d <<<<<<<<<<<<<<<<<<<<<<<<<<<\n", new_user_cache_port -> user_cache.user_id);
                                        pthread_mutex_lock(&(user_cache_port_p -> left_mul_thread -> mutex));
                                        pthread_mutex_unlock(&(user_cache_port_p -> left_mul_thread -> mutex)); 
                                        insert_port(user_cache_port_p->leftnext , new_user_cache_port);
                                }
                                if( user_cache_port_p -> left_mul_thread != NULL && new_user_cache_port -> user_cache.user_id == user_cache_port_p -> left_mul_thread -> end)
                                {
                                        pthread_mutex_destroy(&(user_cache_port_p -> left_mul_thread -> mutex));
                                        free(user_cache_port_p -> left_mul_thread);
					user_cache_port_p -> left_mul_thread = NULL;
                                }
                 }
                else
                {
                                insert_port(user_cache_port_p->leftnext , new_user_cache_port);
                }
	}
}

void find_insert_port(struct USER_CACHE_PORT* old_user_cache_port, int id, struct USER_CACHE_PORT** new_user_cache_port, int* f)
{
	//先获取old的ID
	if(old_user_cache_port->user_cache.user_id < id)
	{
		if(old_user_cache_port->rigthnext == NULL)
		{
			*new_user_cache_port = old_user_cache_port;
			*f = 0;
			return;
		}
		else
		{
			find_insert_port(old_user_cache_port->rigthnext, id, new_user_cache_port, f);
		}
	}
	else if(old_user_cache_port->user_cache.user_id > id)
	{
		if(old_user_cache_port->leftnext == NULL)
		{
			*new_user_cache_port = old_user_cache_port;
			*f = 1;
			return;
		}
		else
		{
			find_insert_port(old_user_cache_port->leftnext, id, new_user_cache_port, f);
		}
	}
	else
	{
		*f = -1;
		return;
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






void print_all_num(struct USER_CACHE_PORT* top_port, int* num)
{
	if(top_port != NULL)
	{
		printf("%d | ",top_port->user_cache.user_id);
		(*num)++;
		print_all_num(top_port->leftnext, num);
		print_all_num(top_port->rigthnext, num);
	}
}



