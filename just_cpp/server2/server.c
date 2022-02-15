#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <json-c/json.h>
#define CLNT_MAX 10

#define BUFFSIZE 200
int g_clnt_socks[CLNT_MAX];
int g_clnt_count =0;




FILE *fp;
int main(int argc, char ** argv){

	int serv_sock;
	int clnt_sock;


	struct sockaddr_in clnt_addr;
        int clnt_addr_size;

	struct sockaddr_in serv_addr;

	int option = 1;	



	serv_sock = socket(PF_INET,SOCK_STREAM,0);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port=htons(7999);
	setsockopt(serv_sock,SOL_SOCKET, SO_REUSEADDR,&option,sizeof(option));
	if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1){
		printf("bind error\n");
	}


	if(listen(serv_sock,5) == -1){
		printf("listen error");
	}
	
	

	char buff[200];
	int recv_len =0;
	while(1){
		clnt_addr_size=sizeof(clnt_addr);
		clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
		g_clnt_socks[g_clnt_count++] = clnt_sock;
		printf("enter client %d",g_clnt_count);
	}
}




