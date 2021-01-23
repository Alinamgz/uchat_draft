#include "server.h"

void mx_remove_cl_node(unsigned cur_uid, t_cl_data *client) {
	pthread_mutex_lock(client->mut);
	t_list *tmp = client->cl_list;

// TODO: check if can rm latest added node
// TODO: fix it: srvr started -> added some nodes -> deleted them all -> OK -> aded new node(s) -> 		one node hangs undeleted/unreacheable
	if (tmp && !tmp->next && tmp->uid == cur_uid) {
	printf("\tGonna del the last one: uid %u, fd %d\n", tmp->uid, tmp->sock_fd);
		free(tmp);
		tmp = NULL;
	client->cl_list = tmp;
	if(!client->cl_list) printf("Now list is empty\n");
	}
	
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
