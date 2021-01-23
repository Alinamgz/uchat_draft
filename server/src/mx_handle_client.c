#include "server.h"

void *mx_handle_client(void *arg) {
	int leave_fl = 0;
	int recv_rslt = -1;
	char buf[BUF_SZ] = "";
	t_cl_data *client = (t_cl_data*)arg;
	t_list *cur_client = client->cl_list;

	// Name

	if (recv(cur_client->sock_fd, buf, BUF_SZ, 0) <= 0
		|| strlen(buf) < 2
		|| strlen(buf) >= NAME_LEN - 1) {
			write(STDERR_FILENO, NAME_ERR, sizeof(NAME_ERR) - 1);
			leave_fl = 1;
	}
	else {
		strcpy(cur_client->name, buf);
		strcat(buf, " has joined!");
		printf("%s\n", buf);
		mx_send_msg(buf, cur_client, client);

	}

	while (!leave_fl) {
		recv_rslt = recv(cur_client->sock_fd, buf, BUF_SZ, 0);

		if (recv_rslt > 0) {
			if (strlen(buf) > 0) {
				mx_send_msg(buf, cur_client, client);
				printf("%s -> %s\n", buf, cur_client->name);
			}
		}
		else if (!recv_rslt || !strcmp(buf, "exit")) {
			sprintf(buf, "%s has let\n", cur_client->name);
			mx_send_msg(buf, cur_client, client);
			leave_fl = 1;
		}
		else {
			write(STDERR_FILENO, "  !!  Recv err !!\n", strlen("  !!  Recv err !!\n"));
			leave_fl = 1;
		}
		memset(buf, 0, BUF_SZ);
	}

	close(cur_client->sock_fd);
	mx_remove_cl_node(cur_client->uid, client);

	printf("\n----------------------- THREAD EXIT -------------\n");
	system("leaks -q uchat_server");

//TODO: check if this func (pthread_detach()) is OK
	pthread_detach(pthread_self());
	return NULL;
}
