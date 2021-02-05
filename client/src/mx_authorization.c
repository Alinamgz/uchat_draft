#include "client.h"

void mx_authorization(t_client *client) {
	// cJSON *req = cJSON_CreateObject();

	mx_get_name(&client->name);

	if (send(client->sock_fd, client->name, NAME_LEN, 0) < 0) {
		mx_client_err(errno, client->sock_fd);
	}
}
