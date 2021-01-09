#include "client.h"

int main(void) {
	int err = 0;
	int client_socket = -1;
	struct sockaddr_in server_addr;
	// char srvr_res[256] = "";
	char *buf = NULL;
	size_t buf_sz = 32;
	ssize_t read_chars = -1;

	// specify an address for the socket
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SRVR_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// create client socket and connection with server
	// (see src/mx_connect_retry.c)
	client_socket = mx_connect_retry((struct sockaddr*)&server_addr, sizeof(server_addr), &err);

	// throw err msg if connection failed
	if (client_socket < 0) {
		perror("Chat client err");
		exit(err);
	}

// --------------- OLD tutorial: -----------------
	// // recieve data from srvr
	// recv(client_socket, &srvr_res, sizeof(srvr_res), 0);
	// // print what we've received
	// printf("Msg from srvr: %s\n", srvr_res);
	// // close socket when we don't need it anymore
	// close(client_socket);
// --------------- ----------------- -----------------
// ============================================================================
// --------------- NEW tutorial: ---------------------
	while (1) {
		printf("Client:\t");

		buf = (char*)malloc(buf_sz);
		memset(buf, 0, buf_sz);
		
		read_chars = getline(&buf, &buf_sz, stdin);
		if (read_chars < 0) {
			perror("Read from client");
			close(client_socket);
			// if (buf) free(buf);
			write(1, "\t --- ch frm clnt read ---\n", strlen( "\t --- ch frm clnt read ---\n"));
			system("leaks -q uchat");
		}
		else {
			buf[read_chars - 1] = '\0';
			printf("--- you entered: %s\t---\n", buf);
		}

		if (!strcmp(buf, "exit")) {
			close(client_socket);
			// if (buf) free(buf);
			write(1, "\t --- ch frm clnt exit ---\n", strlen( "\t --- ch frm clnt read ---\n"));
			system("leaks -q uchat");
			exit(0);
		}
		free(buf);
		buf = NULL;

// printf("ama gonna send...\n");
// 		if (send(client_socket, buf, strlen(buf), 0) < 0) {
// 			err = errno;
// 			perror("Sending from client");
// 			// if (buf) free(buf);
// 			close(client_socket);
// 			exit(err);
// 		}
	}

	system("leaks -q uchat");
	return 0;
}
