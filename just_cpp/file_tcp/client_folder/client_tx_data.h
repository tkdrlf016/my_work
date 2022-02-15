#ifndef CLIENT_TX_DATA_H
#define CLIENT_TX_DATA_H
#include <iostream>
#include <ctime>
#include <cstring>
#include <json-c/json.h>

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
//json_object *tx_dates;

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
    json_object *myObj = json_object_new_object();



    json_object_object_add(myObj, "나이", json_object_new_int(45));

    json_object_object_add(myObj, "기혼", json_object_new_boolean(1));
    json_object_to_file("sample.json",myObj);


}

#endif
