#include "client.h"

void mx_set_addr(struct sockaddr_in *srvr_addr, char *addr_str, char *port_str) {
	int port = atoi(port_str);

	memset(srvr_addr, 0, sizeof(*srvr_addr));

	// TODO: add addr_str validation
	srvr_addr->sin_family = AF_INET;
	// TODO: do we need any criteria for port number?
	srvr_addr->sin_port = htons(port);
	// TODO: would it be INADDR_ANY or some specified srvr_addr?
	srvr_addr->sin_addr.s_addr = htonl(inet_addr(addr_str));
}
