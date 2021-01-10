#include "server.h"

int main(void) {
	int srvr_socket = -1;
	int client_socket = -1;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t cl_addr_len = sizeof(client_addr);

	char buf[1024] = "";
	pid_t childpid;

// create server socket
	srvr_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (srvr_socket < 0) {
		mx_server_err(errno, srvr_socket, client_socket);
	}

// define addr structure
	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SRVR_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

// bind the cocket to our specified IP and port
	if ((bind(srvr_socket, (struct sockaddr*)&server_addr,  sizeof(server_addr))) < 0) {
		mx_server_err(errno, srvr_socket, client_socket);
	}

	if ((listen(srvr_socket, SOMAXCONN)) < 0) {
		mx_server_err(errno, srvr_socket, client_socket);
	}
	printf("Listening...\n");

	while (1) {
		client_socket = accept(srvr_socket, (struct sockaddr*)&client_addr, &cl_addr_len);
		if (client_socket < 0) {
			mx_server_err(errno, srvr_socket, client_socket);
		}

		printf("Connection acceptd from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		if ((childpid = fork()) == 0) {
			close(srvr_socket);

			while (1) {
				recv(client_socket, buf, 1024, 0);
				 if (!strcmp(buf, "exit")) {
					 printf("Disconnecting from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
					 break;				 
				 }
				 else {
					 printf("[+] msg from client:\t%s\n", buf);
					 send(client_socket, "Hoi frm srvr!", strlen("Hoi frm srvr!"), 0);
					 bzero(buf, sizeof(buf));
				 }
			}
		}

	}
	// send(client_socket, srvr_msg, sizeof(srvr_msg), 0);

	close(srvr_socket);
	close(client_socket);

	system("leaks -q server");
	return 0;
}
