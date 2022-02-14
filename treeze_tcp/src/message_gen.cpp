#include "ros/ros.h"
#include "car_status_msgs/car_status_msg.h"

int main(int argc, char ** argv)
{
    ros::init(argc,argv,"message_gen");
    ros::NodeHandle nh;
    ros::Publisher msg_pub = nh.advertise<car_status_msgs::car_status_msg>("car_status_msg",200);
    car_status_msgs::car_status_msg msg;
    ros::Rate loop_rate(10);
    while(ros::ok())
    {
        msg_pub.publish(msg);
        msg.c++;
        msg.d++;
        loop_rate.sleep();
    }
    return 0;
}