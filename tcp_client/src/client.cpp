#include <iostream>
#include <json-c/json.h>
//#include "../include/json-c/json.h"
#include "tcp_client.h"
#include "client_tx_data.h"
#include "ros/ros.h"
int main(int argc, char* argv[]){
	ros::init(argc,argv, "tcp_client");
	set_time();
	set_up_var();


	tcp_open(argc,argv);
	/*
	str_len=read(serv_sock, message, sizeof(message)-1);

	if(str_len==-1)
		printf("read() error!");
	printf("Message from server: %s \n", message);
	*/

	pthread_create(&r_thread,NULL,rsv_callback,(void *)serv_sock);

	/*
	write_data();
	FILE* file = NULL;
	size_t fsize, nsize = 0;
    
	file = fopen("a.json" , "rb");

	fseek(file, 0, SEEK_END);

	fsize=ftell(file);

	fseek(file, 0, SEEK_SET);

	while (nsize!=fsize) {

		int fpsize = fread(buf, 1, 256, file);
        printf("%d\n",fpsize);
		nsize += fpsize;
		send(serv_sock, buf, fpsize, 0);
	}	
	*/
	while(1)
	{
		write_data();
		send_data(serv_sock);
	}
	fclose(file);
	close(serv_sock);
	return 0;
}

