#include "client.h"

void *mx_send_msg_handler(void *arg) {
    t_client *client = arg;

    pthread_mutex_lock(&client->msg_sig_mut);

    while (client->th_ret) {
        pthread_cond_wait(&client->msg_cond, &client->msg_sig_mut);
		// TODO: delete it
        printf("\n\t-----\n\tsender: fd: %d,\nreq: %s\nreq_len: %lu\n", client->sock_fd, client->msg_req, strlen(client->msg_req));
        fflush(stdout);

// TODO: find max msg len and limit it
        if (send(client->sock_fd, client->msg_req, strlen(client->msg_req), 0) < 0) {
            perror("Msg sending err");
        }
		else {
            printf("\n------------\n\tSent: %s\n", client->msg_req);
            free(client->msg_req);
            client->msg_req = NULL;
        }
        pthread_mutex_unlock(&client->msg_sig_mut);
    }
    printf("\n\t--- CH ---,  th_ret: %d\n", client->th_ret);
    return (void *)client;
}
