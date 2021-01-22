#include "client.h"

int mx_connect_retry( const struct sockaddr *addr) {
	int numsec = 0;
	int fd = -1;
	socklen_t addr_len  = sizeof(*addr);

	for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) {
		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			mx_client_err(errno, fd);

		if (connect(fd, addr, addr_len) == 0)
			return fd;

		close(fd);

		perror("Connecting to server");
		write(2, "\tretrying...\n", 13);
		if (numsec <= MAXSLEEP)
			sleep(numsec);
	}
		mx_client_err(errno, fd);
		return -1;
}
