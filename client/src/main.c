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
		// TODO: pass srvr addr from argv -> init_client() -> set_addr() -> etc if needed
		mx_usg_err(argv[0]);

	t_client *client = (t_client*)malloc(sizeof(t_client));
	pthread_t send_msg_th;
	pthread_t recv_msg_th;

	signal(SIGINT, leaks_ch);

// test gtk run
// printf("------ Bef wind init\n");
// 	mx_init_gtk_app();
// printf("------ After wind init\n");

	mx_init_client(client, argv[1], argv[2]);
	mx_authorization(client);

	printf("\n\t --- Welcome! ---\n");

// TODO: write send msg handler
// TODO: wtf is with this argument
	if (pthread_create(&send_msg_th, NULL, mx_send_msg_handler, (void *)client) != 0) {
		write(STDERR_FILENO, SEND_TH_ERR, sizeof(SEND_TH_ERR));
		exit(1);
	}

// TODO: write recv msg handler
// TODO: wtf is with this argument
	if (pthread_create(&recv_msg_th, NULL, mx_recv_msg_handler, (void *)client) != 0) {
		write(STDERR_FILENO, RECV_TH_ERR, sizeof(RECV_TH_ERR));
		exit(1);
	}

	pthread_join(send_msg_th, (void*)&client->th_ret);
	pthread_join(recv_msg_th, (void*)&client->th_ret);

	// pthread_join(send_msg_th, NULL);
	// pthread_join(recv_msg_th, NULL);

	close(client->sock_fd);

	pthread_mutex_destroy(&client->mut);

	printf("\n---------------------------------------------\n");
	// system("leaks -q uchat");
	return 0;
}
