#include "client.h"

void mx_create_chats_req(t_client *client, int peer_uid, char *peer_name) {
    printf("have to create new chat for %d and %d\n", client->self->uid, peer_uid);

    cJSON *req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "type", NEW_CHAT);
    cJSON_AddNumberToObject(req, "peer_uid", peer_uid);
    cJSON_AddStringToObject(req, "peer_username", peer_name);

    pthread_mutex_lock(&client->req_sig_mut);

    client->req = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    pthread_mutex_unlock(&client->req_sig_mut);
    pthread_cond_signal(&client->req_cond);
}
