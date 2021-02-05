#include "client.h"

void mx_init_client(t_client *client, char *addr_input, char *port_input) {
	struct sockaddr_in srvr_addr;

	pthread_mutex_init(&client->mut, NULL);
	client->th_ret = 1;

	mx_set_addr(&srvr_addr, addr_input,  port_input);

	client->sock_fd = mx_connect_retry((struct sockaddr *)&srvr_addr);

	printf("\n------------------- func -------------------\n");
	// system("leaks -q uchat");
}
