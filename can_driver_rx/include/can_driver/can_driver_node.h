#include "ros/ros.h"
#include "can_msgs/Frame.h"
#include "ufo_msgs/ufo_can.h"
#include "ufo_msgs/car_status.h"
#include "ufo_msgs/monitor_commend.h"
#include "std_msgs/String.h"
#include "geometry_msgs/QuaternionStamped.h"
#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/NavSatFix.h"

#include <cstdio>
#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <pthread.h>

using namespace std;
//ufo_msgs::ufo_can msg;
ufo_msgs::car_status msg;
char candata[8];
uint32_t can_id_arr[8] = {0x111, 0x120 , 0x130 , 0x131 , 0x171 , 0x172 , 0x174 , 0x175};
int s; 
int i; 
int nbytes;
struct sockaddr_can addr;
struct ifreq ifr;
struct can_frame frame;
pthread_mutex_t mutex;
pthread_t r_thread[2];
ros::Publisher ins_can_pub;
ros::Subscriber monitor_comm_sub;
int test_flag;
__u8 testdata[8] = {0,0,0,0,0,0,0,0};


int check_id(uint32_t a);
void utc_encoding();
void imu_encoding();
void quat_time_encoding();
void quat_encoding();
void gps_vel();
void gps_acc();
void pos_info();
void gps_pos();
void ctrl_info();
void * rxcan(void * arg);
void monitor_comm_callback(const ufo_msgs::monitor_commend::ConstPtr& msg);
int txcan();

int check_id(uint32_t a)
{	
	pthread_mutex_lock(&mutex);
	int re;
	switch(a)
	{
		case 0x111 :
			utc_encoding();
			re = 1;
			break;
		case 0x120 :
			imu_encoding();
			re = 1;
			break;
		case 0x130 :
			quat_time_encoding();
			re = 1;
			break;					
		case 0x131 :
			quat_encoding();
			re = 1;
			break;	
		case 0x171 :
			gps_vel();
			re = 1;
			break;
		case 0x172 :
			gps_acc();
			re = 1;
			break;
		case 0x174 :
			pos_info();
			re = 1;
			break;					
		case 0x175 :
			gps_pos();
			re = 1;
			break;
		case 0x80 :
			ctrl_info();
			re = 1;
			break;
		default :
			re = 0;
	}
	pthread_mutex_unlock(&mutex);
	/*
	for(int i = 0 ; i < sizeof(can_id_arr) ; i++)
	{
		if(a == can_id_arr[i])
		{
			re = 1;
			break;
		}
		re = 0;
	}
	*/
	return re;
}
void utc_encoding()
{
    msg.year = frame.data[0] + 2000;
	msg.mouth = frame.data[1];
	msg.day = frame.data[2];
	msg.hour = frame.data[3];
	msg.min = frame.data[3];
	msg.sec = frame.data[4];
}

void imu_encoding()
{
	msg.imu_time_stamp = frame.data[0] + (frame.data[1]<<8);
	msg.imu_status = frame.data[2] + (frame.data[3]<<8);
	msg.imu_tmp = frame.data[4] + (frame.data[5]<<8);
}

void quat_time_encoding()
{
	msg.quat_time_stamp = frame.data[0] + (frame.data[1]<<8) + (frame.data[2]<<16) + (frame.data[3]<<24);
}

void quat_encoding()
{
	msg.quat1 =	(frame.data[0] + (frame.data[1]<<8))*0.0000305;
	msg.quat2 =	(frame.data[2] + (frame.data[3]<<8))*0.0000305;
	msg.quat3 = (frame.data[4] + (frame.data[5]<<8))*0.0000305;
	msg.quat4 = (frame.data[6] + (frame.data[7]<<8))*0.0000305;
}

void gps_vel()
{
	msg.vel_d = (frame.data[0] + (frame.data[1]<<8))*0.01;
	msg.vel_e = (frame.data[2] + (frame.data[3]<<8))*0.01;
	msg.vel_n = (frame.data[4] + (frame.data[5]<<8))*0.01;
}

void gps_acc()
{
	msg.acc_d = (frame.data[0] + (frame.data[1]<<8))*0.01;
	msg.acc_e = (frame.data[2] + (frame.data[3]<<8))*0.01;
	msg.acc_n = (frame.data[4] + (frame.data[5]<<8))*0.01;
}

void pos_info()
{
	msg.gps_time_stamp = frame.data[0] + (frame.data[1]<<8) + (frame.data[2]<<16) + (frame.data[3]<<24);
	msg.gps_status = frame.data[4] + (frame.data[5]<<8) + (frame.data[6]<<16) + (frame.data[7]<<24);
}

void gps_pos()
{
	msg.longitude = (frame.data[0] + (frame.data[1]<<8) + (frame.data[2]<<16) + (frame.data[3]<<24))*0.0000001;
	msg.latitude = (frame.data[4] + (frame.data[5]<<8) + (frame.data[6]<<16) + (frame.data[7]<<24))*0.0000001;
}
void ctrl_info()
{
	msg.car_auto_mode = frame.data[0]&15;
	msg.car_acc = frame.data[3];
	msg.car_dec = frame.data[4];
}
void * rxcan(void * arg)
{
	while(1){
		printf("CAN Sockets Receive\r\n");
		printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");	
		if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0){
			perror("Socket");
			
		}
		printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");	
		strcpy(ifr.ifr_name, "can0" );
		ioctl(s, SIOCGIFINDEX, &ifr);

		memset(&addr, 0, sizeof(addr));
		addr.can_family = AF_CAN;
		addr.can_ifindex = ifr.ifr_ifindex;
		printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("Bind");
			
		}
		printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		nbytes = read(s, &frame, sizeof(struct can_frame));  // read here!!!
		printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");	
		if (nbytes < 0) {
			perror("Read");
			
		}
		printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");	
		check_id(frame.can_id);
		printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");	
		if (close(s) < 0) {
			perror("Close");
	
		}
	}
	pthread_exit(0);
	return NULL;
}
void monitor_comm_callback(const ufo_msgs::monitor_commend::ConstPtr& msg)
{
	if(msg->go_stop == 1)
	{
		test_flag = 1;
	}else{
		test_flag = 0;
	}
}
int txcan()
{
	int s; 
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;
	
	printf("CAN Sockets Transmit\r\n");

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, "can0" );
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

	if(test_flag == 1)
	{
		frame.can_id = 0x80;
		frame.can_dlc = 8;
    	frame.data[0] = 1;
    	frame.data[1] = 0;
    	frame.data[2] = 0;
    	frame.data[3] = 50;
    	frame.data[4] = 0;
    	frame.data[5] = 0;
		frame.data[6] = 0;
    	frame.data[7] = 0;
    	frame.data[8] = 0;
	}else{
		frame.can_id = 0x80;
		frame.can_dlc = 8;
    	frame.data[0] = 1;
    	frame.data[1] = 0;
    	frame.data[2] = 0;
    	frame.data[3] = 0;
    	frame.data[4] = 0;
    	frame.data[5] = 0;
		frame.data[6] = 0;
    	frame.data[7] = 0;
    	frame.data[8] = 0;
	}


/*
	frame.can_id = 0x555;
	frame.can_dlc = 8;
    frame.data[0] = testdata[0];
    frame.data[1] = testdata[1];
    frame.data[2] = testdata[2];
    frame.data[3] = testdata[3];
    frame.data[4] = testdata[4];
    frame.data[5] = testdata[5];
	frame.data[6] = testdata[6];
    frame.data[7] = testdata[7];
    frame.data[8] = testdata[8];
*/
	if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
		perror("Write~!");
		return 1;
	}

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}