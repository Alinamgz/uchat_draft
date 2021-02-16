#include "client.h"

// static inline void str_overwrite_stdout(void) {
// 	printf(" >> ");
// 	fflush(stdout);
// }

void *mx_recv_msg_handler(void *arg) {
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
			// flag = 0;
			pthread_mutex_unlock(&client->mut);
			break;
		}
		else if (recv_rslt > 0) {
			printf("<:> %s\n", msg);
		}

		// str_overwrite_stdout();
	}

	return (void*)client;
}
