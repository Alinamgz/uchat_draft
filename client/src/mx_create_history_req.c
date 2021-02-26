#include "client.h"

void mx_create_history_req(t_client *client, int chat_id) {

    cJSON *req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "type", HISTORY);
    cJSON_AddNumberToObject(req, "chat_id", chat_id);

    pthread_mutex_lock(&client->req_sig_mut);

    client->req = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    pthread_mutex_unlock(&client->req_sig_mut);
    pthread_cond_signal(&client->req_cond);
}
