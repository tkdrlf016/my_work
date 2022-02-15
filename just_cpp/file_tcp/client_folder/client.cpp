
#include <iostream>
#include "tcp_client.h"
#include "client_tx_data.h"
#include <json-c/json.h>
int main(int argc, char* argv[]){
	set_time();
	set_up_var();

	char message[30], buf[BUFSIZ];

	tcp_open(argc,argv);
	/*
	str_len=read(serv_sock, message, sizeof(message)-1);

	if(str_len==-1)
		printf("read() error!");
	printf("Message from server: %s \n", message);
	*/
	pthread_create(&r_thread,NULL,rsv_callback,(void *)serv_sock);
	write_data();
	
	FILE* file = NULL;
	size_t fsize, nsize = 0;
    

	file = fopen("a.json" , "rb");

	fseek(file, 0, SEEK_END);

	fsize=ftell(file);

	fseek(file, 0, SEEK_SET);

	while (nsize!=fsize) {

		int fpsize = fread(buf, 1, 256, file);
        printf("%d\n",fpsize);
		nsize += fpsize;
		send(serv_sock, buf, fpsize, 0);
	}	

	fclose(file);
	close(serv_sock);
	return 0;
}

