#ifndef CLIENT_TX_DATA_H
#define CLIENT_TX_DATA_H
#include <iostream>
#include <ctime>
#include <cstring>
#include <json-c/json.h>
#include "tcp_client.h"
/*public_var*/
time_t curTime;
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
json_object *tx_dates;


/*send data*/
FILE* file = NULL;
size_t fsize, nsize = 0;
char buf[256];
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
void set_time()
{   
    curTime = time(nullptr);
    //pLocal = localtime(&curTime);
    localtime_r(&curTime, &pLocal); 
    printf("%04d-%02d-%02dT%02d:%02d:%02d",  
    pLocal.tm_year + 1900, pLocal.tm_mon + 1, pLocal.tm_mday,  
    pLocal.tm_hour, pLocal.tm_min, pLocal.tm_sec); 
}

void write_data()
{
    tx_dates = json_object_new_object();
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
}

void send_data()
{
    file = fopen("sample.json" , "rb");
    fseek(file, 0, SEEK_END);
	fsize=ftell(file);
	fseek(file, 0, SEEK_SET);
    int fpsize = fread(buf, 1, 256, file);
	send(serv_sock, buf, fpsize, 0);
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    //printf("%s\n",buf);
    //printf("%d\n",sizeof(buf));
    fclose(file);
    sleep(2);
}


#endif
