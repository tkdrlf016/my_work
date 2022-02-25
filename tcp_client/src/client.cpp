#include "tcp_client.h"
#include "client_tx_data.h"

int main(int argc, char* argv[]){
	ros::init(argc,argv, "tcp_client");
	ros::NodeHandle nh;
	
	set_up_var();
	set_publisher(nh);
	tcp_open(argc,argv);
	pthread_create(&r_thread,NULL,rsv_callback,(void *)argc);
	while(1)
	{
		//publish_TCP_msg();
		write_data();
		send_data(serv_sock);
		ros::spinOnce();
	} 
	fclose(file);
	close(serv_sock);
	return 0;
}

