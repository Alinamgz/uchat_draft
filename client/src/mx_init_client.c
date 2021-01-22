#include "client.h"

void mx_init_client(t_client *client, char *port) {
	struct sockaddr_in srvr_addr;
	
	pthread_mutex_init(&client->mut, NULL);
	client->th_ret = 1;
	
	mx_set_addr(&srvr_addr, port);
	
	client->sock_fd = mx_connect_retry((struct sockaddr *)&srvr_addr);
	
	printf("\n------------------- func -------------------\n");
	// system("leaks -q uchat");
}
