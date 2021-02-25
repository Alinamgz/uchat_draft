#include "server.h"

// TODO: del it
static inline void leaks_ch(int sig) {

	printf("\n--------------- Signal ------------\n");
	system("leaks -q uchat_server");

	exit(sig);
}

int main(int argc, char **argv) {
	pthread_t tid;

	t_srvr_data server;
	t_cl_data client;

	// TODO: del it
	signal(SIGINT, &leaks_ch);
	signal(SIGPIPE, SIG_IGN);

	mx_init_server_and_client(argc, argv, &server, &client);

	mx_db_init(&client);

	while (1) {
		client.sock_fd = accept(server.sock_fd,
								(struct sockaddr*)&client.addr,
								&client.addr_len);
		if(client.sock_fd < 0)
			mx_server_err(errno, server.sock_fd, client.sock_fd);
		// TODO: Do we need nonblocking I/O ?
                        
		client.uid = ++server.uid;
		mx_add_cl_node(&client);

		pthread_create(&tid, NULL, mx_handle_client, (void*)&client);

	// --------------------------------------
		// Reduce CPU usage/
		// TODO: find better way
		sleep(1);
	}

	close(server.sock_fd);
	close(client.sock_fd);
	pthread_mutex_destroy(&server.mut);

	printf("\n---------------------------\n");
	system("leaks -q uchat_server");

	return 0;
}
