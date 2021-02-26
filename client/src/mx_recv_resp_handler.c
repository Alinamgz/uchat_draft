#include "client.h"

void *mx_recv_resp_handler(void *arg) {
	t_client *client = arg;

	int recv_rslt = 0;
	int msg_len = BUF_SZ;
	char msg[msg_len];

	while (client->th_ret) {

		memset(msg, 0, BUF_SZ);
		recv_rslt = recv(client->sock_fd, msg, msg_len, 0);

		if (recv_rslt < 0) {
			pthread_mutex_lock(&client->mut);
			client->th_ret = 0;
			pthread_mutex_unlock(&client->mut);
			break;
		}
		else if (recv_rslt > 0) {
			printf("\n\t--<Received>-- %s\n", msg);
			mx_parse_n_proceed_response(client, msg);
		}
	}

	return (void*)client;
}
