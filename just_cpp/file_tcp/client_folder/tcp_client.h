#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>


int serv_sock, fd;
int str_len, len;
struct sockaddr_in serv_addr;
pthread_t r_thread;

void tcp_open(int argc, char* argv[])
{
	if(argc!=3){
		printf("Usage : %s <IP> <PORT> \n", argv[0]);
		exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
	if(serv_sock == -1)
		printf("socket() error");
        
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
    
	if(connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
		printf("connect() error!");
}
void * rsv_callback(void * arg){
    char buf[256];
	int serv_sock = *(int*)arg;
    int nbyte = 256;
    size_t filesize = 0, bufsize = 0;
    FILE *file = NULL;
    file = fopen("b.json", "wb");
	while(1){
        nbyte = recv(serv_sock, buf, bufsize, 0);
        if(nbyte == -1)
        {
            nbyte = 0;
        }else{
            fwrite(buf, sizeof(char), nbyte, file);
        }
	}
    fclose(file);
	pthread_exit(0);
	return NULL;
}

#endif 