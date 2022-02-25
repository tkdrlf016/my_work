#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
#include "client_tx_data.h"
#include <fcntl.h>
//#include "../include/json-c/json.h"

int serv_sock, fd;
int str_len, len;
struct sockaddr_in serv_addr;
pthread_t r_thread;
/*
int org_flags;
int flags;
*/
/*ROS var*/
ros::Publisher control_pub;

void tcp_open(int argc, char* argv[])
{	
	
	printf("Usage : %s <IP> <PORT> \n", "192.168.0.7");
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
	if(serv_sock == -1)
		printf("socket() error");
        
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("192.168.0.7");
	serv_addr.sin_port=htons(5002);
	int res = connect(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));

	while(res != 0){
		sleep(4);

		res = connect(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));
		std::cout << "connect_error!" << std::endl;
	}
}
void set_publisher(ros::NodeHandle& nh)
{
	control_pub = nh.advertise<TCP_msgs::TCP_pub>("/tcp/pub",100);
}

void * rsv_callback(void * arg){  
	//int sock = (int)arg;
    int nbyte = 0;
	int32_t ui_e;
	double la;
	double lo;
	int32_t spd;
	int32_t mode; 
	int32_t waypoints_no;

	std::string waypoint_name;
    int bufsize = 3000;
    FILE *file = NULL;
	json_object *obj;
	json_object *waypoints_obj;
	json_object *waypoint_obj;
	json_object *in_waypoint_obj;
	json_object *in_in_waypoint_obj;
	json_object *val_obj;
	json_object *waypoints_num;
	while(1)
	{	
		char buf[3000];
		memset(buf, 0, 3000);
        nbyte = recv(serv_sock, buf, bufsize, 0);
		//printf("data len is %d\n",nbyte);
		//printf("%s\n",buf);
		obj = json_tokener_parse(buf);
		waypoints_obj = json_object_object_get(obj, "Waypoints");
		waypoints_num = json_object_object_get(waypoints_obj,"Waypoints_numbers");
		waypoints_no = json_object_get_int(waypoints_num);
		TCP_msgs::TCP_pub TCP_msg;
		TCP_msg.waypoint_no = waypoints_no;
		for(int i = 0 ; i <= waypoints_no; i ++)
		{
			char waypoint_name_c[20];
			std::string waypoint_name_s = "Waypoint[]";
			waypoint_name_s.insert(9,std::to_string(i));
			strcpy(waypoint_name_c,waypoint_name_s.c_str());

			waypoint_obj = json_object_object_get(waypoints_obj,waypoint_name_c);
			in_waypoint_obj = json_object_object_get(waypoint_obj, "Speed");
			spd = json_object_get_int(in_waypoint_obj);	
			TCP_msg.spd.push_back(spd);


			in_waypoint_obj = json_object_object_get(waypoint_obj, "Pos");
			in_in_waypoint_obj = json_object_object_get(in_waypoint_obj, "lat");
			la = json_object_get_double(in_in_waypoint_obj);
			//printf("la is %lf\n",la);
			TCP_msg.latitude.push_back(la);


			in_in_waypoint_obj = json_object_object_get(in_waypoint_obj, "lng");
			lo = json_object_get_double(in_in_waypoint_obj);
			//printf("lo is %lf\n",lo);
			TCP_msg.longitude.push_back(lo);
			
		}
		control_pub.publish(TCP_msg);
		
        if(nbyte == -1)
        {
            nbyte = 0;
        }else{
			
   
			nbyte = 0;

        }
		sleep(0.2);
	}
	pthread_exit(0);
	return NULL;	
}


#endif 
