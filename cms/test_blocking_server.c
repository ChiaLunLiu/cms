#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<string.h>
#include "minimsg.h"
#define TIME 10
int main(int argc,char** argv)
{
	int fd;
	int sock;
	int r,i;
	const char* content;
	struct sockaddr_in sin;
	frame_t * f;
	msg_t * m;
	char buf[204];
	int type = 2;
	const char* const str = "This is from client";
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1)
	{
		perror("could not create socket");
		return 1;
	}
	puts("Socket created");
	sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(12345);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    int one = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return;
    }

    if (listen(sock, 16)<0) {
        perror("listen");
        return;
    }

	puts("Connected");
	struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    while(1){
	fd = accept(sock, (struct sockaddr*)&ss, &slen);

		/* message testing */
		for(i = 0 ;i < 1000 ; i++){
			printf("frame %d\n",i);
			if(msg_recv(fd,&m) == MINIMSG_OK){
			//	msg_print(m);
				msg_free(m);
			}
			else printf("recv fail\n");
			
			m = msg_alloc();
			msg_append_string(m,"frame 1");
			msg_append_string(m,"frame 2");
			msg_append_string(m,"frame 3");
		//	msg_print(m);
			if(msg_send(fd,m) == MINIMSG_OK){
				printf("send OK\n");
			}
			else printf("send FAIL\n");
			printf("after send frame\n");
		}
	close(fd);
	}
	close(sock);
	
	return 0;
}
