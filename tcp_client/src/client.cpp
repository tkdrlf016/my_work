#include "tcp_client.h"
#include "client_tx_data.h"

int main(int argc, char* argv[]){
	ros::init(argc,argv, "tcp_client");
	ros::NodeHandle nh;
	car_status_sub = nh.subscribe("/ins/pub_can",100,rev_car_status);
	car_commend_pub = nh.advertise<ufo_msgs::monitor_commend>("/ufo/commend",100);
	set_up_var();
	tcp_open(argc,argv);
	pthread_create(&r_thread,NULL,rsv_callback,(void *)serv_sock);
	while(1)
	{
		write_data();
		send_data(serv_sock);
		ros::spinOnce();
	} 
	fclose(file);
	close(serv_sock);
	return 0;
}

