#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
#define CLNT_MAX 10

#define BUFFSIZE 200
int g_clnt_socks[CLNT_MAX];
int g_clnt_count =0;

pthread_mutex_t g_mutex;
char message[30], buf[BUFSIZ];
    	FILE* file = NULL;
struct m_msg_struct 
{
	int a;
	int b;
};

struct c_s_msg_struct
{
	int c;
	int d;
};




int main(int argc, char ** argv){

	int serv_sock; // 연결하는 과
	int clnt_sock;

	pthread_t r_thread;
	pthread_t s_thread;

	struct sockaddr_in clnt_addr;
        int clnt_addr_size;

	struct sockaddr_in serv_addr;

	int option = 1;	

	pthread_mutex_init(&g_mutex,NULL);

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
	size_t fsize, nsize = 0;
	size_t fsize2;
	file = fopen("a.json", "wb");
	fseek(file, 0, SEEK_END);
	// calculate file size
	fsize=ftell(file);
	// move file pointer to first
	fseek(file, 0, SEEK_SET);
	while(1){
		clnt_addr_size=sizeof(clnt_addr);
		clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
		g_clnt_socks[g_clnt_count++] = clnt_sock;
		int fpsize = fread(buf, 1, 256, file);
		nsize += fpsize;
		send(serv_sock, buf, fpsize, 0);
	}
	fclose(file);
	close(serv_sock);
}





