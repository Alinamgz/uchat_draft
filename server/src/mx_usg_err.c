#include "server.h"

void mx_usg_err(char *name) {
	write(STDERR_FILENO, USG_ERR_START, sizeof(USG_ERR_START) - 1);
	write(STDERR_FILENO, name, strlen(name));
	write(STDERR_FILENO, USG_ERR_END, sizeof(USG_ERR_END) - 1);
	exit(EXIT_FAILURE);
}
