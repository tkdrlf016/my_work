#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
#include "client_tx_data.h"
//#include "../include/json-c/json.h"

int serv_sock, fd;
int str_len, len;
struct sockaddr_in serv_addr;
pthread_t r_thread;


void tcp_open(int argc, char* argv[])
{	
	printf("sssssss");
	if(argc!=3){
		printf("Usage : %s <IP> <PORT> \n", "192.168.0.15");
		//exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
	if(serv_sock == -1)
		printf("socket() error");
        
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	inet_pton(AF_INET, (const char*)"192.168.0.15", &(serv_addr.sin_addr));
	serv_addr.sin_family=AF_INET;
	//serv_addr.sin_addr.s_addr=inet_addr("192.168.0.15");
	serv_addr.sin_port=htons(atoi("5001"));
    
	if(connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
		printf("connect() error!");
}

void * rsv_callback(void * arg){  
	//int sock = (int)arg;
    int nbyte = 0;
	int i;
    size_t filesize = 0, bufsize = 256;
    FILE *file = NULL;
	json_object *myobj;
	json_object *to_val;
    //file = fopen("b.json", "wb");
	while(1)
	{	
		//file = fopen("b.json", "w");
		char buf[256];
		memset(buf, 0, 256);
		printf("11111111111111111111");
        nbyte = recv(serv_sock, buf, bufsize, 0);
		printf("EEEEEE%sEEEEEE\n",buf);
		printf("LLLLLLL%dLLLLLLLLL\n",strlen(buf));
		myobj = json_tokener_parse(buf);
		to_val = json_object_object_get(myobj,"Mode");
		i = json_object_get_int(to_val);
		printf("$$$$$$$$$$$$ %d\n", i );
		if(i == 1)
		{
			monitor_msg.go_stop == 1;
			/*must remove code*/ 
			spd = 10;
		}else{	
			monitor_msg.go_stop == 0;
			/*must remove code*/ 
			spd = 0;
		}
		//if(to_val == 1)
		//{
			
		//}
        if(nbyte == -1)
        {
            nbyte = 0;
        }else{
			
            //fwrite(buf, sizeof(char), nbyte, file);
			nbyte = 0;

        }
		//car_commend_pub.publish(monitor_msg);
		//fclose(file);
		sleep(0.2);
	}
	pthread_exit(0);
	return NULL;	
}


#endif 