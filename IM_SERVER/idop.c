#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main(int args,char** argv)
{
	//创建一个消息队列
	key_t ket;
	int msgget(key, 0);	
	
	struct	

	while(1)
	{
		int msgrcv(int msqid, struct msgbuf *msgp, int msgsz, long msgtyp, int msgflg);
	}
	
}
