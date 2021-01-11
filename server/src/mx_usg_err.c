#include "server.h"

void mx_usg_err(char *name) {
	write(2, USG_ERR_START, strlen(USG_ERR_START));
	write(2, name, strlen(name));
	write(2, USG_ERR_END, strlen(USG_ERR_END));
	exit(EXIT_FAILURE);
}
