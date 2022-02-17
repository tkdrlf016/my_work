#ifndef CLIENT_TX_DATA_H
#define CLIENT_TX_DATA_H
#include <iostream>
#include <ctime>
//#include <cstring>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include "../include/json-c/json.h"
#include "tcp_client.h"

struct tm pLocal;

/*var*/
uint8_t ctrl_mode;
bool go_or_stop;
bool emergency;
uint8_t chk_sum;

std::string scen;
double pos_xy[2];
float ang;
float spd;
float acc;
float yaw;

uint32_t high_level_error;
uint32_t low_level_error;

/*json var*/
json_object *ctrl_status;
json_object *robot_status;
json_object *Movement_status ;
/*send data*/
FILE* file = NULL;
size_t fsize, nsize = 0;
char buf[256];

//int serv_sock;

/*func*/


void set_up_var()
{
    ctrl_mode = 0;
    go_or_stop = false;
    emergency = false;
    chk_sum = 0;

    std::string scen("no scenario");
    high_level_error = 0;
    low_level_error = 0;
}



void write_data()
{
    ctrl_status = json_object_new_object();
    robot_status = json_object_new_object();
    Movement_status = json_object_new_object();
    json_object_object_add(ctrl_status, "ctrl_mode", json_object_new_int(0));
    json_object_object_add(ctrl_status, "go_or_stop", json_object_new_int(0));
    json_object_object_add(ctrl_status, "emergency", json_object_new_int(0));
    json_object_object_add(ctrl_status, "chk_sum", json_object_new_int(0));

    json_object_object_add(robot_status, "scenario", json_object_new_string("a scenario"));
    json_object_object_add(robot_status, "pos_x", json_object_new_int(0));
    json_object_object_add(robot_status, "pos_y", json_object_new_int(0));
    json_object_object_add(robot_status, "ang", json_object_new_int(0));
    json_object_object_add(robot_status, "spd", json_object_new_int(0));
    json_object_object_add(robot_status, "acc", json_object_new_int(0));
    json_object_object_add(robot_status, "yaw", json_object_new_int(0));

    json_object_object_add(Movement_status, "high_level_error", json_object_new_int(0));
    json_object_object_add(Movement_status, "high_level_error", json_object_new_int(0));
    json_object_object_add(Movement_status, "can_status", json_object_new_int(0));
    json_object_to_file("ctrl_status.json",ctrl_status);
    json_object_to_file("robot_status.json",robot_status);
    json_object_to_file("Movement_status.json",Movement_status);
}
void init_signal(int sock)
{
    char robot_no[10];
    printf("no.1\n");
   
    write(sock,robot_no,strlen(robot_no)+1);
}
void ckecked_signal(int sock)
{
    char buff[100];
    int len;
    while(1){
	len = read(sock,buff,sizeof(buff));
		
	if(len == -1){
		printf("wait...\n");
		break;
	}
    }
	printf("server check thie robot!!");
}
void send_data(int sock)
{
    file = fopen("ctrl_status.json" , "rb");
    fseek(file, 0, SEEK_END);
	fsize=ftell(file);
	fseek(file, 0, SEEK_SET);
    int fpsize = fread(buf, 1, 256, file);
	send(sock, buf, fpsize, 0);
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    //printf("%s\n",buf);
    //printf("%d\n",sizeof(buf));
    fclose(file);
    sleep(2);
}


#endif
