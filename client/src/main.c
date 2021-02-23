#include "client.h"

// volatile sig_atomic_t flag_ex = 1;

static inline void leaks_ch(int sig) {
	printf("\n--------------- Signal ------------\n");
	system("leaks -q uchat");

	// flag = 0;
	exit(sig);
}

int main(int argc, char **argv) {
	if (argc!= 3)
		mx_usg_err(argv[0]);

	// pthread_t send_msg_th;
	// pthread_t recv_msg_th;

	t_client *client = (t_client*)malloc(sizeof(t_client));

	client->ui = (t_ui*)malloc(sizeof(t_ui));
	client->argv = argv;
	client->prev_scene = -1;
	client->th_ret = 1;
	client->auth_req = NULL;
	client->search_req = NULL;
	client->chats = NULL;
	// client->scene = CONNECTION;
	// client->scene = CONNECTION;
	pthread_mutex_init(&client->mut, NULL);
	pthread_mutex_init(&client->connection_mut, NULL);
	pthread_cond_init(&client->msg_cond, NULL);
	pthread_mutex_init(&client->msg_sig_mut, NULL);
	pthread_mutex_init(&client->resp_mut, NULL);

	signal(SIGINT, leaks_ch);

	mx_init_gtk_app(client);
	// mx_authorization(client);

	printf("\n\t --- Welcome! ---\n");

// // TODO: write send msg handler
// // TODO: wtf is with this argument
// 	if (pthread_create(&send_msg_th, NULL, mx_send_msg_handler, (void *)client) != 0) {
// 		write(STDERR_FILENO, SEND_TH_ERR, sizeof(SEND_TH_ERR));
// 		exit(1);
// 	}

// // TODO: write recv msg handler
// // TODO: wtf is with this argument
// 	if (pthread_create(&recv_msg_th, NULL, mx_recv_msg_handler, (void *)client) != 0) {
// 		write(STDERR_FILENO, RECV_TH_ERR, sizeof(RECV_TH_ERR));
// 		exit(1);
// 	}

// 	pthread_join(send_msg_th, (void*)&client->th_ret);
// 	pthread_join(recv_msg_th, (void*)&client->th_ret);

	// pthread_join(send_msg_th, NULL);
	// pthread_join(recv_msg_th, NULL);

	close(client->sock_fd);

	pthread_mutex_destroy(&client->mut);

	printf("\n---------------------------------------------\n");
	// system("leaks -q uchat");
	return 0;
}
