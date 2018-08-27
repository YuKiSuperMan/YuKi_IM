#ifndef _USER_CACHE_H
#define _USER_CACHE_H

struct USER_CACHE
{
	int user_id;
	char ip_str[15];
	int port;
};

struct USER_CACHE_PORT
{
	struct USER_CACHE user_cache;
	struct USER_CACHE_PORT* leftnext;
	struct USER_CACHE_PORT* rigthnext;
};

struct USER_CACHE_TREE
{
	struct USER_CACHE_PORT* top;
};


void* init_cache();
int find_up_port(struct USER_CACHE_PORT* old_user_cache_port, struct USER_CACHE_PORT** need_user_cache_port, struct USER_CACHE_PORT** need_up_user_cache_port,int* f, int user_id);
int remove_tree_port(struct USER_CACHE_TREE* tree,int user_id);
int find_port(struct USER_CACHE_PORT* old_user_cache_port,struct USER_CACHE* need_user_cache);
int find_tree_port(struct USER_CACHE_TREE* tree,struct USER_CACHE* need_user_cache);
void insert_tree_port(struct USER_CACHE_TREE* tree, int user_id, char* ip_str, int port,struct USER_CACHE_PORT* user_cache_port);
void insert_port(struct USER_CACHE_PORT* old_user_cache_port,struct USER_CACHE_PORT* new_user_cache_port);
void print_all(struct USER_CACHE_PORT* top_port);




#endif
