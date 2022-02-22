#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

geometry_msgs::TransformStamped odom_trans;

void odom_callback(const nav_msgs::Odometry::ConstPtr& msg)
{
        tf::TransformBroadcaster broadcaster;
        odom_trans.header.frame_id = "odom";
        odom_trans.child_frame_id = "base_footprint";
        odom_trans.header.stamp = msg->header.stamp;
        odom_trans.transform.rotation = msg->pose.pose.orientation;
        odom_trans.transform.translation.x = msg->pose.pose.position.x;
        odom_trans.transform.translation.y = msg->pose.pose.position.y;
        odom_trans.transform.translation.z = msg->pose.pose.position.z;
        broadcaster.sendTransform(odom_trans);
        ROS_INFO("+++++++++++++++++++++++");
}

int main(int argc, char** argv) 
{
    ros::init(argc, argv, "odom_tf");
    ros::NodeHandle nh;
    ros::Subscriber odom_sub = nh.subscribe("/odom",100,odom_callback);

    while (ros::ok()) {
        ros::spinOnce();
    }
    return 0;
}
