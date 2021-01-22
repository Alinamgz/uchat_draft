#include "server.h"

void mx_send_msg(char *msg, t_cl_data *client) {
	pthread_mutex_lock(client->mut);
	
	t_list *tmp = client->cl_list;

	while (tmp) {
		if (tmp->uid != client->uid) {
			if (send(tmp->sock_fd, msg, strlen(msg), 0) < 0)
				perror("Sending err");
			break;
		}
		tmp = tmp->next;
	}
	
	// TODO: check if it's ok here
	memset(msg, 0, strlen(msg));
	pthread_mutex_unlock(client->mut);
}
