#include "server.h"

void mx_add_cl_node(t_cl_data *client) {
	pthread_mutex_lock(client->mut);

	t_list *tmp = client->cl_list;
	t_list *new_node = (t_list*)malloc(sizeof(t_list));

	new_node->addr = client->addr;
	new_node->sock_fd = client->sock_fd;
	new_node->uid = client->uid;
	new_node->next = tmp ? tmp : NULL;

	client->cl_list = new_node;

	// TODO: Delete it
	printf("Connection acceptd from %s:%d\n",
			inet_ntoa(client->cl_list->addr.sin_addr),
			ntohs(client->cl_list->addr.sin_port));

	pthread_mutex_unlock(client->mut);
}

