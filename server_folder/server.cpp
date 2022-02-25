#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <json-c/json.h>
void printf(char *message);
FILE* file2 = NULL;
size_t fsize2, nsize2 = 0;
char buf2[3000];
int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	//char buf[256];
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[]="Hello World!";
	
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		printf("socket() error");
			
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
		printf("bind() error"); 
	
	if(listen(serv_sock, 5)==-1)
		printf("listen() error");
	
	clnt_addr_size=sizeof(clnt_addr);  
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock==-1)
		printf("accept() error");  
	
	write(clnt_sock, message, sizeof(message));
	int nbyte = 0;
    size_t filesize = 0, bufsize = 0;
    FILE *file = NULL;
    bufsize = 256;
    while(/*filesize != 0*/1) {
 		
		file = fopen("b.json"/* 새로 만들 파일 이름 */, "w");
		char buf[256] = {};
        nbyte = recv(clnt_sock, buf, bufsize, 0);
		if(nbyte == -1)
		{	
			printf("this is error");
			break;
		}

        fwrite(buf, sizeof(char), nbyte, file);		
        nbyte = 0;
		fclose(file);

		
		json_object * tx_dates = json_object_new_object();
    	json_object_object_add(tx_dates, "ctrl_mode", json_object_new_int(0));
    	json_object_object_add(tx_dates, "go_or_stop", json_object_new_int(0));
    	json_object_object_add(tx_dates, "emergency", json_object_new_int(0));
    	json_object_object_add(tx_dates, "chk_sum", json_object_new_int(0));
    	json_object_object_add(tx_dates, "scenario", json_object_new_string("a scenario"));
    	json_object_object_add(tx_dates, "pos_x", json_object_new_int(0));
    	json_object_object_add(tx_dates, "pos_y", json_object_new_int(0));
    	json_object_object_add(tx_dates, "ang", json_object_new_int(0));
    	json_object_object_add(tx_dates, "spd", json_object_new_int(0));
    	json_object_object_add(tx_dates, "acc", json_object_new_int(0));
    	json_object_object_add(tx_dates, "yaw", json_object_new_int(0));
    	json_object_object_add(tx_dates, "high_level_error", json_object_new_int(0));
    	json_object_object_add(tx_dates, "high_level_error", json_object_new_int(0));
    	json_object_to_file("sample.json",tx_dates);
		
		file2 = fopen("data0.json" , "rb");
    	fseek(file2, 0, SEEK_END);
		fsize2=ftell(file2);
		fseek(file2, 0, SEEK_SET);
    	int fpsize2 = fread(buf2, 1, 3000, file2);
		send(clnt_sock, buf2, fpsize2, 0);
		
   	 	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    	printf("%s\n",buf2);
    	
    	fclose(file2);
		
		///////////////////////////////////////////////
		sleep(2);
    }
 
/*
	while((nbyte = recv(clnt_sock, buf, bufsize, 0) != 0)){
 		fwrite(buf, sizeof(char), nbyte, file);
	}
*/	

	//fclose(file);
	close(clnt_sock);	
	close(serv_sock);
	return 0;
}

