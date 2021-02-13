#include "client.h"

void *mx_do_auth_th(void *arg) {
    t_client *client = (t_client *)arg;

    pthread_mutex_lock(&client->connection_mut);

    char *send_buf = cJSON_PrintUnformatted(client->auth_req);
    size_t res_len = BUF_SZ;
    char res_buf[res_len];

    if (send(client->sock_fd, send_buf, strlen(send_buf), 0) < 0) {
        write(STDERR_FILENO, "Err: sending auth failed\n", strlen("Err: sending auth failed\n"));
    }
    else {
        printf("Sent to server: %s\n", send_buf);
    }

    free(send_buf);

// TODO: you stopped here!
    memset(res_buf, 0, BUF_SZ);
    if (recv(client->sock_fd, res_buf, res_len, 0) > 0) {
        printf("Received answer: %s\n", res_buf);
        fflush(stdout);
    }
    else {
        write(STDERR_FILENO, "Err: recv auth response failed\n", strlen("/!\\Err: recv auth response failed\n"));
    }
    cJSON_Delete(client->auth_req);
    pthread_mutex_unlock(&client->connection_mut);
    return NULL;
}
