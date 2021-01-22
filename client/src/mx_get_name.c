#include "client.h"

static bool should_stop_asking(char *name) {
	bool rslt = 1;
	int len = strlen(name);

	if (len < 2 || len > 32) {
		if (!strcmp(name, "q")) {
			rslt = 1;
		}
		else {
			rslt = 0;
			write(STDERR_FILENO, NAME_ERR, sizeof(NAME_ERR));
		}
	}
	return rslt;
}

void mx_get_name(char **str) {
	char *name = *str;
	size_t name_sz = NAME_LEN;

	name = (char*)malloc(name_sz);

	memset(name, 0, name_sz);
	printf("Enter yur name\nMax len 31 chars\n NB: to exit enter 'q')\n >> ");

	while (1) {
		if (getline(&name, &name_sz, stdin) < 0)
			mx_client_err(errno, -1);

			mx_strtrim(&name);

		if (should_stop_asking(name)) {
			break;
		}
	}

	*str = name;
}
