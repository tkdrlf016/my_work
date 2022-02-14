#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "ros/ros.h"
#include "monitor_msgs/monitor_msg.h"
#include "car_status_msgs/car_status_msg.h"
#include <iostream>
#include <boost/bind.hpp>
#include <json-c/json.h>
#define BUFFSIZE 100
#define NAMESIZE 20

int StoHEX(char fi, char sc);
void error_handling(char *msg);
void * send_message(void * arg);
void * recv_message(void * arg);
char message[BUFFSIZE];

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

struct monitor_struct
{
	ros::NodeHandle ndh;
	ros::Publisher pub;
	monitor_msgs::monitor_msg msg;
	int sock;
};

void * rcv(void* arg){
	printf("rcv thread created\n");
	struct monitor_struct* m_stuct = (struct monitor_struct*)arg;
	struct m_msg_struct  buff;
	int size;
	while(1){
		size = read(m_stuct->sock,&buff,sizeof(buff));
		printf("size : %d\n",size);
		if(size <= 0){
			printf("s close\n");
			printf("thread finish");
			pthread_exit(0);
			break;
		}
		printf("%d\n",buff.a);
		printf("%d\n",buff.b);
		m_stuct->msg.a = buff.a;
		m_stuct->msg.b = buff.b;
		m_stuct->pub.publish(m_stuct->msg);
	}
	//printf("thread finish");
	//pthread_exit(0);
	return 0;
}

void car_status_msg_callback(const car_status_msgs::car_status_msg::ConstPtr& c_msg, struct c_s_msg_struct& c_s_msg,int sock)
{	
	c_s_msg.c = c_msg->c;
	c_s_msg.d = c_msg->d;
	write(sock,&c_s_msg,sizeof(c_s_msg));
	printf("callback!\n");
}

int main(int argc,char **argv){
	// ------ 필요한 변수들을 만듭니다.
	ros::init(argc,argv,"treeze_tcp_client");
	struct monitor_struct monitor_arg;
	struct monitor_struct* monitor_arg_ptr = &monitor_arg;
	struct m_msg_struct m_msg;
	struct c_s_msg_struct c_s_msg;
	struct c_s_msg_struct& c_s_msg_ref = c_s_msg;
	monitor_arg.pub = monitor_arg.ndh.advertise<monitor_msgs::monitor_msg>("monitor_msg",200);
	car_status_msgs::car_status_msg c_msg;
    struct sockaddr_in serv_addr;
    pthread_t rcv_thread;
    void* thread_result;
	
    // ------ 아이디를 표시한다.
	char id[100];
	printf("argc : %d\n",argc);
	if(argc < 2){
		printf("you have to enter ID\n");
		return 0;
	}
	strcpy(id,argv[1]);
	printf("id : %s\n",id);

	// ------ 소켓을 연다.
    monitor_arg.sock=socket(PF_INET, SOCK_STREAM, 0);
    if(monitor_arg.sock==-1){
		printf("socket error");
	}else{

		printf("socket ok\n");
	}
	ros::Subscriber car_status = monitor_arg.ndh.subscribe<car_status_msgs::car_status_msg>("car_status_msg",100,boost::bind(&car_status_msg_callback,_1,c_s_msg_ref,monitor_arg.sock));
	// ------ 연결할 서버정보를 입력한다.
    memset(&serv_addr, 0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(7999);

	// ------ 서버와 연결한다.
    if(connect(monitor_arg.sock, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1){
		printf("connect error\n");
	}else{	
		printf("connection success\n");
	}

	// ------ 받기용 쓰레드를 만든다.
	pthread_create(&rcv_thread,NULL,rcv,(struct monitor_struct *)&monitor_arg);
 	// ------ 보내기용 while문을 만든다. 
	ros::spin();
	while(ros::ok()){
		sleep(1);
		printf("-----");
		//ros::spinOnce();
	}
	
	// ------ 소켓을 닫는다.
	printf("while end\n"); 
    close(monitor_arg.sock);
    return 0;
}//end main

