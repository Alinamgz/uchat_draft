#include "client.h"

void mx_client_err(int err, int fd) {
	perror("Client err");

	if (fd >= 0)
		close(fd);

// // =================== LEAKS CHECK ==================
// 	write(1, "\n\t=== client err ===\n", strlen("\n\t=== server err ===\n"));
// 	system("leaks -q uchat");
// // =================== ============ ==================

	exit(err);
}
