#include "server.h"

void mx_send_msg(char *msg, t_list *cur_client, t_cl_data *client) {
	pthread_mutex_lock(client->mut);
	
	t_list *tmp = client->cl_list;
printf("\n------\n");
	while (tmp) {
		if (tmp->uid && tmp->uid != cur_client->uid) {
			if (send(tmp->sock_fd, msg, strlen(msg), 0) < 0) {
				perror("Sending err");
				printf("FAILED Send to: name %s, uid %d , fd %d||\n", tmp->name, tmp->uid, tmp->sock_fd);
				break;
			}
			printf("Send to: name %s, uid %d , fd %d\n ", tmp->name, tmp->uid, tmp->sock_fd);
		}
		tmp = tmp->next;
	}	
	// TODO: check if it's ok here
	memset(msg, 0, strlen(msg));
	pthread_mutex_unlock(client->mut);
}
