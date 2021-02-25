#include "server.h"

void *mx_handle_client(void *arg) {
	int leave_fl = 0;
	int recv_rslt = -1;
	char buf[BUF_SZ] = "";
	t_cl_data *client = (t_cl_data*)arg;
	t_list *cur_client = client->cl_list->next;

	// Name
	mx_authorization(client->db, cur_client, &leave_fl);
// TODO : mb make normal concted/disconnected clients monitor

	while (!leave_fl) {
		errno = 0;
		recv_rslt = recv(cur_client->sock_fd, buf, BUF_SZ, 0);

		if (recv_rslt > 0) {
			if (strlen(buf) > 0) {
				mx_parse_and_proceed_req(buf, cur_client, client);

				// if you need server to send back clients request, uncomment next line. it should send buf to all connected users
				// mx_send_msg(buf, cur_client, client);
			}
		}
		else if (!recv_rslt || (recv_rslt < 0 && errno == ECONNRESET) || !strcmp(buf, "exit")) {
			printf("%s has left\n", cur_client->name);
			sprintf(buf, "%s has left\n", cur_client->name);
			mx_send_msg(buf, cur_client, client);
			leave_fl = 1;
		}
		else {
			write(STDERR_FILENO, "  !!  Recv err !!\n", strlen("  !!  Recv err !!\n"));
			perror("mult client:");
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
