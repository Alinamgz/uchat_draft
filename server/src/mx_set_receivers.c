#include "server.h"

void mx_set_receivers(t_list *cur_client, int my_uid, int peer_uid) {
    cur_client->receivers = (int*)calloc(3 ,sizeof(int));

    cur_client->receivers[0] = my_uid;
    cur_client->receivers[1] = peer_uid;
    cur_client->receivers[2] = -1;
}
