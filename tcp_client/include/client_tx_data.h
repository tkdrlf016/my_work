#ifndef CLIENT_TX_DATA_H
#define CLIENT_TX_DATA_H
#include <cstdio>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <json-c/json.h>
#include <iostream>
#include <ctime>
#include <cstring>
#include <string>
#include "TCP_msgs/TCP_pub.h"
#include "ros/ros.h"
//#include "../include/json-c/json.h"
#include "tcp_client.h"

/*public_var*/
time_t curTime;
struct tm pLocal;

/*var*/
uint8_t ctrl_mode;
bool go_or_stop;
bool emergency;
uint8_t chk_sum;
double pos_xy[2];
float ang;
int spd;
float acc;
float yaw;
uint8_t hle;
uint8_t lle;
uint8_t can_status;

uint32_t high_level_error;
uint32_t low_level_error;

/*json var*/
json_object *ctrl_status;

/*send data*/
FILE* file = NULL;
size_t fsize, nsize = 0;

/*ROS var*/



/*func_d*/
void set_up_var();
void write_data();
void send_data(int sock);

/*fun*/
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
    json_object_object_add(ctrl_status, "ctrl_mode", json_object_new_int(0));
    json_object_object_add(ctrl_status, "go_or_stop", json_object_new_int(0));
    json_object_object_add(ctrl_status, "emergency", json_object_new_int(0));
    json_object_object_add(ctrl_status, "chk_sum", json_object_new_int(0));
    json_object_object_add(ctrl_status, "scenario", json_object_new_string("a scenario"));
    json_object_object_add(ctrl_status, "pos_x", json_object_new_int(0));
    json_object_object_add(ctrl_status, "pos_y", json_object_new_int(0));
    json_object_object_add(ctrl_status, "ang", json_object_new_int(0));
    json_object_object_add(ctrl_status, "spd", json_object_new_int(spd));
    json_object_object_add(ctrl_status, "acc", json_object_new_int(0));
    json_object_object_add(ctrl_status, "yaw", json_object_new_int(0));
    json_object_object_add(ctrl_status, "high_level_error", json_object_new_int(0));
    json_object_object_add(ctrl_status, "low_level_error", json_object_new_int(0));
    json_object_object_add(ctrl_status, "can_status", json_object_new_int(0));
    json_object_to_file("ctrl_status.json",ctrl_status);
}

void send_data(int sock)
{
    char buf[256];
    file = fopen("ctrl_status.json" , "rb");
    fseek(file, 0, SEEK_END);
	fsize=ftell(file);
	fseek(file, 0, SEEK_SET);
    int fpsize = fread(buf, 1, 256, file);
	send(sock, buf, 256, 0);
    fclose(file);
    sleep(2);
}




#endif
