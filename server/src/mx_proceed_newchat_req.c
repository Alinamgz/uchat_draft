#include "server.h"

static void update_clients_chats(t_cl_data *client, t_list *cur_client, int cur_uid);

void mx_proceed_newchat_req(char *buf, t_list *cur_client, t_cl_data *client) {
    cJSON *req = cJSON_Parse(buf);
    cJSON *peer_uid = cJSON_GetObjectItemCaseSensitive(req, "peer_uid");
    cJSON *peer_username = cJSON_GetObjectItemCaseSensitive(req, "peer_username");

    mx_create_new_chat(client->db, cur_client, peer_uid->valueint, peer_username->valuestring);
    update_clients_chats(client, cur_client, cur_client->uid);
    update_clients_chats(client, cur_client, peer_uid->valueint);

    cJSON_Delete(req);
}

static void update_clients_chats(t_cl_data *client, t_list *cur_client, int cur_uid) {
    char *resp = NULL;

    mx_count_chat_rows(client->db, cur_client, cur_uid);
    mx_select_chats(client->db, cur_client, cur_uid);

    resp = mx_create_chats_response(cur_client);

    mx_set_receivers(cur_client, cur_uid, -1);
    mx_send_response(resp, cur_client, client);
    mx_chat_req_memfree(cur_client);

    free(resp);
}
