#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/QuaternionStamped.h>
#include <sensor_msgs/NavSatFix.h>
#include <math.h>
#include <stdio.h>
#include <sstream>
#define std_x 37.214643
#define std_y 127.078531
#define rad(x) x*3.14159/180.0

double my_x = 37.209298;
double my_y = 127.080638;
geometry_msgs::TransformStamped odom_trans;

struct tagPT
{
	double x;
	double y;
	double z;
};

void GetDistance()
{
	int radius = 6371;
        tagPT pt1;
	tagPT pt2;
        double th;
	pt1.y = 37.214643;
	pt1.x = 127.078531;
	pt2.y = 37.209298;
	pt2.x = 127.080638;
        th = atan2(pt2.y - pt1.y,pt2.x - pt1.x);
	double dLat = rad( (pt2.y-pt1.y) );
	double dLon = rad( (pt2.x-pt1.x) );

	pt1.y = rad( pt1.y );
	pt2.y = rad( pt2.y );

	double a = sin(dLat/2) * sin(dLat/2) + sin(dLon/2) * sin(dLon/2) * cos(pt1.y) * cos(pt2.y);
	double c = 2 * atan2f(sqrtf(a), sqrtf(1-a));
	double dDistance = radius * c;
	dDistance*=1000;
        odom_trans.transform.translation.x = dDistance*cos(th);
        odom_trans.transform.translation.y = dDistance*sin(th);
}

void quantcallback(const sensor_msgs::NavSatFix::ConstPtr msg)
{
        double linedis;
        double radius = 6371000; //Earth radius(km)
        double toradian = 3.14159265359 / 180; // pi/180
         // = sqrt( abs( ( pow(car_.gnssFixState.latitude - map_entrance[i][0], 2) + (pow(car_.gnssFixState.longitude - map_entrance[i][1], 2) )  ) ) );
        double deltaLatitude = (msg->longitude - std_x)*toradian;
        double deltaLongtitude = (msg->latitude - std_y)*toradian;
        double sindeltaLat = sin(deltaLatitude / 2);
        double sindeltaLon = sin(deltaLongtitude / 2);
        double squareroot = sqrt(sindeltaLat * sindeltaLat + 
                                cos(msg->latitude*toradian)*
                                cos(std_x*toradian)*
                                sindeltaLon * sindeltaLon);
        linedis = 2 * radius * asin(squareroot);
        odom_trans.transform.translation.y = linedis*cos(atan((msg->latitude - std_x)/(msg->longitude - std_y)));
        odom_trans.transform.translation.x = linedis*sin(atan((msg->latitude - std_x)/(msg->longitude - std_y)));
}
void poscallback(const geometry_msgs::QuaternionStamped::ConstPtr& msg)
{
        odom_trans.transform.rotation.x = msg->quaternion.x;
        odom_trans.transform.rotation.y = msg->quaternion.y;
        odom_trans.transform.rotation.z = msg->quaternion.z;
        odom_trans.transform.rotation.w = msg->quaternion.w;
}

void first_xy(double x, double y)
{
        double linedis;
        double radius = 6371000; //Earth radius(km)
        double toradian = 3.14159265359 / 180; // pi/180
         // = sqrt( abs( ( pow(car_.gnssFixState.latitude - map_entrance[i][0], 2) + (pow(car_.gnssFixState.longitude - map_entrance[i][1], 2) )  ) ) );
        double deltaLatitude = (x - std_x)*toradian;
        double deltaLongtitude = (y - std_y)*toradian;
        double sindeltaLat = sin(deltaLatitude / 2);
        double sindeltaLon = sin(deltaLongtitude / 2);
        double squareroot = sqrt(sindeltaLat * sindeltaLat + 
                                cos(y*toradian)*
                                cos(std_x*toradian)*
                                sindeltaLon * sindeltaLon);
        linedis = 2 * radius * asin(squareroot);
        odom_trans.transform.translation.x = linedis*cos(atan((y - std_x)/(x - std_y)));
        odom_trans.transform.translation.y = linedis*sin(atan((y - std_x)/(x - std_y)));
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "state_publisher");
    ros::NodeHandle n;
    ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
    ros::Subscriber position_sub = n.subscribe("/filter/quanternion",100,poscallback);
    ros::Subscriber quant_sub = n.subscribe("/gnss",100,quantcallback);
    tf::TransformBroadcaster broadcaster;
    ros::Rate loop_rate(30);

    const double degree = M_PI/180;

    // robot state
    double inc= 0.005, base_arm_inc= 0.005, arm1_armbase_inc= 0.005, arm2_arm1_inc= 0.005, gripper_inc= 0.005, tip_inc= 0.005;
    double angle= 0 ,base_arm = 0, arm1_armbase = 0, arm2_arm1 = 0, gripper = 0, tip = 0;
    // message declarations
    sensor_msgs::JointState joint_state;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";
    odom_trans.header.stamp = ros::Time::now();
    odom_trans.transform.translation.x = 10;
    odom_trans.transform.translation.y = 10;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(0);
    while (ros::ok()) {
        //update joint_state
        joint_state.header.stamp = ros::Time::now();
        joint_state.name.resize(11);
        joint_state.position.resize(11);
        joint_state.name[0] ="base_to_arm_base";
        joint_state.position[0] = base_arm;
        joint_state.name[1] ="arm_1_to_arm_base";
        joint_state.position[1] = arm1_armbase;
        joint_state.name[2] ="arm_2_to_arm_1";
        joint_state.position[2] = arm2_arm1;
	joint_state.name[3] ="left_gripper_joint";
        joint_state.position[3] = gripper;
	joint_state.name[4] ="left_tip_joint";
        joint_state.position[4] = tip;
	joint_state.name[5] ="right_gripper_joint";
        joint_state.position[5] = gripper;
	joint_state.name[6] ="right_tip_joint";
        joint_state.position[6] = tip;
	joint_state.name[7] ="base_to_wheel1";
        joint_state.position[7] = 0;
	joint_state.name[8] ="base_to_wheel2";
        joint_state.position[8] = 0;
	joint_state.name[9] ="base_to_wheel3";
        joint_state.position[9] = 0;
	joint_state.name[10] ="base_to_wheel4";
        joint_state.position[10] = 0;






        // update transform
        // (moving in a circle with radius)
        
        //odom_trans.header.stamp = ros::Time::now();
        //odom_trans.transform.translation.x = cos(angle);
        //odom_trans.transform.translation.y = sin(angle);
        //odom_trans.transform.translation.z = 0.0;
        //odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(angle);
        
        //send the joint state and transform
        odom_trans.header.stamp = ros::Time::now();
	//first_xy(my_x, my_y);
        GetDistance();
        joint_pub.publish(joint_state);
        broadcaster.sendTransform(odom_trans);


	// Create new robot state
        arm2_arm1 += arm2_arm1_inc;
        if (arm2_arm1<-1.5 || arm2_arm1>1.5) arm2_arm1_inc *= -1;
		  arm1_armbase += arm1_armbase_inc;
        if (arm1_armbase>1.2 || arm1_armbase<-1.0) arm1_armbase_inc *= -1;
        base_arm += base_arm_inc;
        if (base_arm>1. || base_arm<-1.0) base_arm_inc *= -1;
        gripper += gripper_inc;
        if (gripper<0 || gripper>1) gripper_inc *= -1;
		
		  angle += degree/4;

        // This will adjust as needed per iteration
        ros::spinOnce();
        loop_rate.sleep();
    }


    return 0;
}
