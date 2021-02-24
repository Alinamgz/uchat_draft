#include "client.h"

void *mx_send_req_handler(void *arg) {
    t_client *client = arg;
    // char *req = NULL;

    pthread_mutex_lock(&client->req_sig_mut);

    while (client->th_ret) {
        pthread_cond_wait(&client->req_cond, &client->req_sig_mut);
        // req = client->search_req ? client->search_req : client->msg_req;
        // if (client->search_req)
        //     req = client->search_req;
        // else if (client->msg_req)
        //     req = client->msg_req;
        // else
        //     req = NULL;

		// TODO: delete it
        printf("\n\t-----\n\tsender: fd: %d,\nreq: %s\nreq_len: %lu\n", client->sock_fd, client->req, strlen(client->req));
        fflush(stdout);

// TODO: find max msg len and limit it
        // if (send(client->sock_fd, client->msg_req, strlen(client->msg_req), 0) < 0) {
        if (send(client->sock_fd, client->req, strlen(client->req), 0) < 0) {
            perror("Msg sending err");
        }
		else {
            printf("\n------------\n\tSent: %s\n", client->req);
        }

        free(client->req);
        client->req = NULL;

        pthread_mutex_unlock(&client->req_sig_mut);
    }
    printf("\n\t--- CH ---,  th_ret: %d\n", client->th_ret);
    return (void *)client;
}
