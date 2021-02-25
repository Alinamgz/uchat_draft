#include "server.h"

static void search_req_res_memfree(t_list *cur_client);

void mx_proceed_search_req(char *buf, t_list *cur_client, t_cl_data *client) {
    cJSON *req = cJSON_Parse(buf);
    cJSON *str = cJSON_GetObjectItemCaseSensitive(req, "search_str");
    char *resp = NULL;

    mx_do_search_user(client->db, cur_client, str->valuestring);

    resp = mx_create_search_response(cur_client);

    mx_set_receivers(cur_client, cur_client->uid, -1);
    mx_send_response(resp, cur_client, client);

    search_req_res_memfree(cur_client);
    free(resp);
    cJSON_Delete(req);
}

static void search_req_res_memfree(t_list *cur_client) {
    t_chat_req_res **runner = cur_client->found_users;

    if (runner && *runner){
        for (int i = 0; runner[i]; i++) {
            if (runner[i]->chat_id) {
                free(runner[i]->chat_id);
                runner[i]->chat_id = NULL;
            }
            if (runner[i]->name_or_msg) {
                free(runner[i]->name_or_msg);
                runner[i]->name_or_msg = NULL;
            }
            if (runner[i]->from_uid) {
                free(runner[i]->from_uid);
                runner[i]->from_uid = NULL;
            }
            if (runner[i]->to_uid) {
                free(runner[i]->to_uid);
                runner[i]->to_uid = NULL;
            }
            free(runner[i]);
            runner[i] = NULL;
        }
    }

    free(cur_client->found_users);
    cur_client->found_users = NULL;
}
