CC = gcc

userhead = ./head/user.h

res_server:res_server.o user.o
	$(CC) -o $@ $^ -pthread

res_server.o:res_server.c $(userhead)
	$(CC) -c $< 
user.o:user.c $(userhead)
	$(CC) -c $<

clean:
	rm -rf *.o




