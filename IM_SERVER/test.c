/**demo**/
#include <stdio.h>
#include <pthread.h>
//需要操作的资源
int a = 1;
pthread_mutex_t mutex;
//抢占资源是否加锁标志位
int o = 0;
//用于判断是否是最后一个线程
int pthread_flag = 0;

struct pthread_m
{
	pthread_t pthread;
	int id;
};


struct pthread_m pp[20];


void* task(void *p)
{
	int num = *((int*)p);
	if(o == 0)
	{
		pthread_flag = num;
		int res = pthread_mutex_trylock(&mutex);
		if(res == 0)
		{
			//尝试加锁成功
			a++;
			sleep(1);
			printf("线程 %d 获取到的a = %d\n", num, a);
			o++;
			pthread_mutex_unlock(&mutex);

		}
		else
		{
			pthread_mutex_lock(&mutex);
			printf("当时排队的线程num = %d \n", num);
			pthread_mutex_unlock(&mutex);
			a++;
			sleep(1);
			printf("线程 %d 获取到的a = %d\n", num, a);	
		}

		if(pthread_flag == num)
		{
			printf("最后一个线程的ID = %d \n", pthread_flag);
			pthread_mutex_destroy(&mutex);
		}
	}
	else
	{
		a++;
		sleep(1);
		printf("不用排队的线程 %d 获取到的a = %d\n", num, a);
	}
}

int main()
{
	pthread_mutex_init(&mutex, 0);
	int i = 0;
	for(; i < 20; i++)
	{
		pp[i].id = i;
		pthread_create(&(pp[i].pthread), 0, (void*)task, &(pp[i].id));
	}
	sleep(30);
}
