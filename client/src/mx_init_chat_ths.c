#include "client.h"

void mx_init_chat_ths(t_client *client) {
    pthread_t send_msg_th;
	pthread_t recv_msg_th;

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

    // pthread_join(send_msg_th, (void*)&client->th_ret);
	// pthread_join(recv_msg_th, (void*)&client->th_ret);

}
