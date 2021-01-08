#include "server.h"

int main(void) {
	char srvr_msg[256] = "You have reached the server!";
	int srvr_sock_fd = -1;
	int cl_sock_fd = -1;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t cl_addr_len = sizeof(client_addr);

// create server socket
	srvr_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (srvr_sock_fd < 0) {
		mx_server_err(errno, srvr_sock_fd, cl_sock_fd);
	}

// define addr structure
	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SRVR_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

// bind the cocket to our specified IP and port
	if ((bind(srvr_sock_fd, (struct sockaddr*)&server_addr,  sizeof(server_addr))) < 0) {
		mx_server_err(errno, srvr_sock_fd, cl_sock_fd);
	}

	if ((listen(srvr_sock_fd, SOMAXCONN)) < 0) {
		mx_server_err(errno, srvr_sock_fd, cl_sock_fd);
	}

	cl_sock_fd = accept(srvr_sock_fd, (struct sockaddr*)&client_addr, &cl_addr_len);
	if (cl_sock_fd < 0) {
		mx_server_err(errno, srvr_sock_fd, cl_sock_fd);
	}

	send(cl_sock_fd, srvr_msg, sizeof(srvr_msg), 0);

	close(srvr_sock_fd);
	close(cl_sock_fd);

	system("leaks -q server");
	return 0;
}
