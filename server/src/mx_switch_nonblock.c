#include "server.h"

bool mx_switch_nonblock(int sock_fd, bool turn_on) {
	bool is_ok = true;
	int flags = fcntl(sock_fd, F_GETFL, 0);

	if (flags < 0)
		// TODO: change to logger for daemon
		return false;		

	flags = turn_on ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
	is_ok = fcntl(sock_fd, F_SETFL, flags) ? false : true;

	return is_ok;

}
