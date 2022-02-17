#include <iostream>
#include <json-c/json.h>
//#include "../include/json-c/json.h"
#include "tcp_client.h"
#include "client_tx_data.h"
#include "ros/ros.h"
int main(int argc, char* argv[]){
	ros::init(argc,argv, "tcp_client");
	set_up_var();
	tcp_open(argc,argv);
	printf("---\n");
	init_signal(serv_sock);
	printf("@@@@@@@@@\n");
	/*
	while(1)
	{
		write_data();
		send_data(serv_sock);
	}
	*/
	//fclose(file);
	close(serv_sock);
	return 0;
}

