#include "server.h"

void mx_send_response(char *msg, t_list *cur_client, t_cl_data *client) {
    pthread_mutex_lock(client->mut);

    t_list *tmp = client->cl_list;

    while (tmp) {
        if (tmp->uid && cur_client && cur_client->receivers) {
            for (int i = 0; cur_client->receivers[i] > 0; i++) {
                if ((int)tmp->uid == cur_client->receivers[i]) {
                    if (send(tmp->sock_fd, msg, strlen(msg), 0) < 0) {
                        perror("Sending err");
                        printf("FAILED Send to: name %s, uid %d , fd %d||\n",
                               tmp->name,
                               tmp->uid,
                               tmp->sock_fd);
                        break;
                    }
                }
            }
        }
        tmp = tmp->next;
    }

    free(cur_client->receivers);

    pthread_mutex_unlock(client->mut);
}
