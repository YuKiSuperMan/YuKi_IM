#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;

void *task(void* p)
{
	pthread_mutex_lock(&mutex);	
	printf("开始\n");
	sleep(2);
	printf("结束\n");
	pthread_mutex_unlock(&mutex);	
}

int main()
{
	pthread_t pthread;
	pthread_t pthread2;
	pthread_t pthread1;
	pthread_mutex_init(&mutex, 0);
	int res = pthread_mutex_destroy(&mutex);
	pthread_create(&pthread, 0, (void*)task, NULL);
	pthread_create(&pthread1, 0, (void*)task, NULL);
	pthread_create(&pthread2, 0, (void*)task, NULL);
	
	//int res = pthread_mutex_destroy(&mutex);
	printf("销毁线程锁的返回值 = %d \n", res);
	sleep(20);
	return 0;
}
