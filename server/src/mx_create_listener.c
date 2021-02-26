#include "server.h"

int mx_create_listener(int argc, char *app_name) {
	int rslt_sock_fd = -1;
	int opt = 1;

	if (argc != 2)
		mx_usg_err(app_name);
	
	if ((rslt_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		mx_server_err(errno, rslt_sock_fd, -1);

// TODO: do we need nonblock I/O ?
	// if (mx_switch_nonblock(rslt_sock_fd, 0) == 0)
	// 	mx_server_err(errno, rslt_sock_fd, -1);

	if (setsockopt(rslt_sock_fd, SOL_SOCKET, (SO_REUSEADDR), &opt, sizeof(opt)) < 0) {
		mx_server_err(errno, rslt_sock_fd, -1);
	}
	return rslt_sock_fd;
}
