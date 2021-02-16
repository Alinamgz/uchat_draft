#include "client.h"

// static inline void str_overwrite_stdout(void) {
// 	printf(" >> ");
// 	fflush(stdout);
// }

void *mx_send_msg_handler(void *arg) {
	t_client *client = arg;

	// size_t msg_len = BUF_SZ;
	// size_t buf_len = BUF_SZ + NAME_LEN;
	// char *msg = (char*)malloc(msg_len);
	// char *buf = (char*)malloc(buf_len);

	// memset(msg, 0, msg_len);
	// memset(buf, 0, buf_len);


	while (client->th_ret) {
	// while (flag) {
		// str_overwrite_stdout();

// 		if (getline(&msg, &msg_len, stdin) < 0)
// 			perror("Msg reading err");

// 		mx_strtrim(&msg);

// 		if (!strcmp(msg, "exit")) {
// 			// pthread_mutex_lock(&client->mut);
// 			// client->th_ret = 0;
// 			// // flag = 0;
// 			// pthread_mutex_unlock(&client->mut);

// // TODO: figure out if we can stop it less violently
// 			system("leaks -q uchat");
// 			exit(0);
// 			// break;
// 		}
// 		else {
			// sprintf(buf, "%s: %s", client->name, msg);
			if (send(client->sock_fd, client->msg_req, strlen(client->msg_req), 0) < 0) {
				perror("Msg sending err");
			}
			else {
				printf("Sent: %s\n", client->msg_req);
			}
		}
		// memset(buf, 0, strlen(buf));
		// memset(msg, 0, strlen(msg));
	// }

	// free(msg);
	// free(buf);

printf("\n\t--- CH ---,  th_ret: %d\n", client->th_ret);
	return (void*)client;
}
