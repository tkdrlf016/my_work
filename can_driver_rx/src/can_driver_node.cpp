#include <can_driver/can_driver_node.h>

using namespace std;



int main(int argc, char **argv)
{
    cout<< "start" << endl;
    ros::init(argc,argv,"can_driver");
    ros::NodeHandle nh;
    ins_can_pub = nh.advertise<ufo_msgs::car_status>("/ins/pub_can",100);
    ros::Rate loop(10);
    pthread_create(&r_thread[0],NULL,rxcan,(void *)argv);
    while(ros::ok())
    {
        //rxcan();
        pthread_mutex_lock(&mutex);
        ins_can_pub.publish(msg);   
        pthread_mutex_unlock(&mutex);  
        txcan();    
        loop.sleep();
    }
}