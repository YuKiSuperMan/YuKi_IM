#ifndef _USER_CACHE_H
#define _USER_CACHE_H
#include <pthread.h>

/*挂载元素*/
struct USER_CACHE
{
	int user_id;
	char ip_str[15];
	int port;
};

/*多线程相关*/
struct MULTI_THREAD
{
	int lock;	/*节点加锁标志*/
	int end;	/*线程结束*/
	pthread_mutex_t mutex;
};

/*节点*/
struct USER_CACHE_PORT
{
	struct USER_CACHE user_cache;			/*挂载元素*/
	struct USER_CACHE_PORT* leftnext;		/*左节点*/
	struct USER_CACHE_PORT* rigthnext;		/*右节点*/
	struct MULTI_THREAD*	left_mul_thread;	/*左多线程相关*/
	struct MULTI_THREAD*	rigth_mul_thread;	/*右多线程相关*/
	int			del;			/*删除线程相关*/
	int 			del_flag;		/*删除结束相关*/
};

/*树*/
struct USER_CACHE_TREE
{
	struct USER_CACHE_PORT* top;
};


void* init_cache();
void find_insert_port(struct USER_CACHE_PORT* old_user_cache_port, int id, struct USER_CACHE_PORT** new_user_cache_port, int* f);
int find_up_port(struct USER_CACHE_PORT* old_user_cache_port, struct USER_CACHE_PORT** need_user_cache_port, struct USER_CACHE_PORT** need_up_user_cache_port,int* f, int user_id);
int remove_tree_port(struct USER_CACHE_TREE* tree,int user_id);
int find_port(struct USER_CACHE_PORT* old_user_cache_port,struct USER_CACHE* need_user_cache);
int find_tree_port(struct USER_CACHE_TREE* tree,struct USER_CACHE* need_user_cache);
void insert_tree_port(struct USER_CACHE_TREE* tree, int user_id, char* ip_str, int port,struct USER_CACHE_PORT* user_cache_port);
void insert_port(struct USER_CACHE_PORT* old_user_cache_port,struct USER_CACHE_PORT* new_user_cache_port);
void print_all(struct USER_CACHE_PORT* top_port);
void print_all_num(struct USER_CACHE_PORT* top_port, int* num);



#endif
