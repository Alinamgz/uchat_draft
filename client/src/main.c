#include "client.h"

int main(void) {
	int err = 0;
	int cl_sock_fd = -1;
	struct sockaddr_in server_addr;
	char srvr_res[256] = "";

	// specify an address for the socket
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SRVR_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// create client socket and connection with server
	// (see src/mx_connect_retry.c)
	cl_sock_fd = mx_connect_retry((struct sockaddr*)&server_addr, sizeof(server_addr), &err);

	// throw err msg if connection failed
	if (cl_sock_fd < 0) {
		perror("Chat client err");
		exit(err);
	}

	// recieve data from srvr
	recv(cl_sock_fd, &srvr_res, sizeof(srvr_res), 0);
	// print what we've received
	printf("Msg from srvr: %s\n", srvr_res);
	// close socket when we don't need it anymore
	close(cl_sock_fd);

	system("leaks -q client");
	return 0;
}
