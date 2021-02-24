#include "server.h"

void mx_set_receivers(t_list *cur_client, int peer_uid) {
    cur_client->receivers = (int*)calloc(3 ,sizeof(int));

    cur_client->receivers[0] = cur_client->uid;
    cur_client->receivers[1] = peer_uid;
    cur_client->receivers[2] = -1;
}
