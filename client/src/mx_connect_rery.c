#include "client.h"

int mx_connect_retry( const struct sockaddr *addr, socklen_t alen, int *err) {
	int numsec = 0;
	int fd = -1;

	for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) {
		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			*err = errno;
			return -1;
		}

		if (connect(fd, addr, alen) == 0) {
			return fd;
		}
		close(fd);

// printf(" failed. Trying to reconnect in %d seconds...\n", numsec);
perror("Client to srv");
write(2, "\tretrying...\n", 13);
		if (numsec <= MAXSLEEP) {
			sleep(numsec);
		}
	}
		*err = errno;
		return -1;
}
