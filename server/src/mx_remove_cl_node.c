#include "server.h"

void mx_remove_cl_node(unsigned cur_uid, t_cl_data *client) {
	pthread_mutex_lock(client->mut);
	t_list *tmp = client->cl_list;

	while(tmp && tmp->next) {
		if (tmp->next->uid == cur_uid) {
			free(tmp->next);
			tmp->next = tmp->next->next ? tmp->next->next : NULL;

			break;
		}
		tmp = tmp->next;
	}

	pthread_mutex_unlock(client->mut);
}
