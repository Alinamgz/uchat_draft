#include "client.h"

static void send_req(t_client *client);

void *mx_do_auth_th(void *arg) {
    t_client *client = (t_client *)arg;

    pthread_mutex_lock(&client->connection_mut);

    size_t res_len = BUF_SZ;
    char res_buf[res_len];
    memset(res_buf, 0, BUF_SZ);

    send_req(client);

    if (recv(client->sock_fd, res_buf, res_len, 0) > 0) {
    // TODO: Del it:
        printf("Received answer: %s\n", res_buf);
        fflush(stdout);
    // --------------------------------------
        mx_parse_n_proceed_auth_response(client, res_buf);
    }
    else {
        write(STDERR_FILENO, "Err: recv auth response failed\n", strlen("/!\\Err: recv auth response failed\n"));
    }
    cJSON_Delete(client->auth_req);
    pthread_mutex_unlock(&client->connection_mut);
    return NULL;
}

static void send_req(t_client *client) {
    char *send_buf = cJSON_PrintUnformatted(client->auth_req);

    if (send(client->sock_fd, send_buf, strlen(send_buf), 0) < 0) {
        write(STDERR_FILENO, "Err: sending auth failed\n", strlen("Err: sending auth failed\n"));
    }

    free(send_buf);
}
