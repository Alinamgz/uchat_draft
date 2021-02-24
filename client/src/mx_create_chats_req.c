#include "client.h"

void mx_create_chats_req(t_client *client, int my_uid, int peer_uid) {
    printf("have to create new chat for %d and %d\n", my_uid, peer_uid);

    cJSON *req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "type", NEW_CHAT);
    cJSON_AddNumberToObject(req, "from_uid", my_uid);
    cJSON_AddNumberToObject(req, "to_uid", peer_uid);

    pthread_mutex_lock(&client->req_sig_mut);

    client->req = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    pthread_mutex_unlock(&client->req_sig_mut);
    pthread_cond_signal(&client->req_cond);
}
