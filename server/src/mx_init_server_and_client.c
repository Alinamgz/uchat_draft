#include "server.h"

// void mx_init_server(int argc, char **argv, t_server_data *server, &t_cl_data *client) {
// 	pthread_mutex_init(&server->mut, NULL);

// 	server->uid = 1;
// 	client

// 	server->sock_fd = mx_create_listener(argc, argv[0]);
// 	mx_set_addr(&server->addr, &client->addr, argv[1]);

// 	if (bind(server->sock_fd, (struct sockaddr*)&server->addr, sizeof(server->addr)) < 0)
// 		mx_server_err(errno, server->sock_fd, -1);
	
// 	if (listen(server->sock_fd, SOMAXCONN) < 0)
// 		mx_server_err(errno, server->sock_fd, -1);
// }

void mx_init_server_and_client(int argc, char **argv, t_srvr_data *server, t_cl_data *client) {
	server->uid = 1;
	server->sock_fd = mx_create_listener(argc, argv[0]);

	client->sock_fd = -1;
	client->addr_len = sizeof(client->addr);
	client->cl_list = NULL;
	client->uid = server->uid;
	client->mut = &(server->mut);

	mx_set_addr(&server->addr, &client->addr, argv[1]);

	if (bind(server->sock_fd, (struct sockaddr*)&server->addr, sizeof(server->addr)) < 0)
		mx_server_err(errno, server->sock_fd, client->sock_fd);
	
	if (listen(server->sock_fd, SOMAXCONN) < 0)
		mx_server_err(errno, server->sock_fd, client->sock_fd);
	
	pthread_mutex_init(&server->mut, NULL);

	// TODO: Delete it
	printf("------ ----  srvr ---- ------\n");
	printf("\taddr: %s\n\tport: %d\n", inet_ntoa(server->addr.sin_addr), ntohs(server->addr.sin_port));
	printf("---- ---- ---- ---- ---- ----\n\n");

}

