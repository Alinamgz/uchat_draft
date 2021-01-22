#include "server.h"

void *mx_handle_client(void *arg) {
	int leave_fl = 0;
	int recv_rslt = -1;
	t_cl_data *client = (t_cl_data*)arg;
	// t_list *cur_client = client->cl_list;
	// int cur_uid = client->uid;
	char buf[BUF_SZ] = "";

	// Name

	if (recv(client->sock_fd, buf, BUF_SZ, 0) <= 0
		|| strlen(buf) < 2
		|| strlen(buf) >= NAME_LEN - 1) {
			write(STDERR_FILENO, NAME_ERR, sizeof(NAME_ERR) - 1);
			leave_fl = 1;
	}
	else {
		strcpy(client->cl_list->name, buf);
		strcat(buf, " has joined!");
		printf("%s\n", buf);
		mx_send_msg(buf, client);

	}

	while (!leave_fl) {
		recv_rslt = recv(client->sock_fd, buf, BUF_SZ, 0);

		if (recv_rslt > 0) {
			if (strlen(buf) > 0) {
				mx_send_msg(buf, client);
				printf("%s -> %s\n", buf, client->cl_list->name);
			}
		}
		else if (!recv_rslt || !strcmp(buf, "exit")) {
			sprintf(buf, "%s has let\n", client->cl_list->name);
			mx_send_msg(buf, client);
			leave_fl = 1;
		}
		else {
			write(STDERR_FILENO, "  !!  Recv err !!\n", strlen("  !!  Recv err !!\n"));
			leave_fl = 1;
		}
		// memset(buf, 0, BUF_SZ);
	}

	close(client->sock_fd);
	mx_remove_cl_node(client);

	printf("\n----------------------- THREAD EXIT -------------\n");
	system("leaks -q uchat_server");

//TODO: check if this func (pthread_detach()) is OK
	pthread_detach(pthread_self());
	return NULL;
}
