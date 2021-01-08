#include "server.h"

void mx_server_err(int err, int serv_fd, int client_fd) {
	perror("Server error");
	
	if (serv_fd >= 0)
		close(serv_fd);
	
	if (client_fd >= 0)
		close(client_fd);

// =================== LEAKS CHECK ==================
	write(1, "\n\t=== server err ===\n", strlen("\n\t=== server err ===\n"));
	system("leaks -q server");
// =================== ============ ==================

	exit(err);
}
