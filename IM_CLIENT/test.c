#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#pragma pack(8)

struct U
{
	int i;
	char n[20];
	char p[20];
};

int main()
{
	int fd;
	fd = open("./test.txt", O_CREAT | O_RDWR, 0666);
	
	struct U u;
	u.i = 1;
	strcpy(u.n,"Name");
	strcpy(u.p,"123");
	write(fd, &u, sizeof(u));

	struct U u1;

	read(fd, &u1, sizeof(u));

	printf("%d , %s , %s",u1.i,u1.n,u1.p);
	return 0;


}
