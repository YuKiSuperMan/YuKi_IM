#ifndef __CHAT_H
#define _CHAT_H

struct chat
{

	int id;        /*发送着ID*/
	int toid;      /*接收者ID*/
	char value[90];/*需要发送的聊天内容*/

};


#endif
