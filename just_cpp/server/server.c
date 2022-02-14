#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CLNT_MAX 10

#define BUFFSIZE 200
int g_clnt_socks[CLNT_MAX];
int g_clnt_count =0;

pthread_mutex_t g_mutex;
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
void * sd_callback(void * arg)
{
	struct m_msg_struct msg;
	int clnt_sock = (int)arg;
	int status;
	msg.a = 0;
	msg.b = 0;
	while(1)
	{
		status = write(clnt_sock,&msg,sizeof(msg));
		printf("%d",clnt_sock);
		if(status == -1)
		{
			printf("clnt[%d] close from write\n",clnt_sock);
			break;
		}
		msg.a++;
		msg.b++;
		printf("w:%d\n",msg.a);
		printf("W:%d\n",msg.b);
		sleep(1);
	}
	pthread_exit(0);
}

void * rsv_callback(void * arg){

	int clnt_sock = (int)arg;
	int size = 0;
	
	struct c_s_msg_struct msg;
	int i;
	msg.c = 0;
	msg.d = 0;
	while(1){
	size = read(clnt_sock,&msg,sizeof(msg));
	printf("%d",size);
		if(size == -1){
			printf("clnt[%d] close from read\n",clnt_sock);
			break;
		}
		printf("s:%d\n",msg.c);
		printf("s:%d\n",msg.d);
	}

	
	pthread_mutex_lock(&g_mutex);

        for(i=0; i<g_clnt_count; i++){
                if(clnt_sock == g_clnt_socks[i]){
                        for(;i<g_clnt_count-1;i++)
                                g_clnt_socks[i]=g_clnt_socks[i+1];
                        break;
                }//end if
        }//end for
	pthread_mutex_lock(&g_mutex);
	close(clnt_sock);
	pthread_exit(0);
	return NULL;
}



int main(int argc, char ** argv){

	int serv_sock;
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
	while(1){
		clnt_addr_size=sizeof(clnt_addr);
		clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
		pthread_mutex_lock(&g_mutex);
		g_clnt_socks[g_clnt_count++] = clnt_sock;
		pthread_mutex_unlock(&g_mutex);
		pthread_create(&r_thread,NULL,rsv_callback,(void *)clnt_sock);
		pthread_create(&s_thread,NULL,sd_callback,(void *)clnt_sock);
	}
}





