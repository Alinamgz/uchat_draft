#include "server.h"

void mx_parse_and_proceed_req(char *buf, t_list *cur_client, t_cl_data *client) {
    pthread_mutex_lock(&client->req_mut);

    cJSON *req = cJSON_Parse(buf);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(req, "type");
    cJSON *str = NULL;
    char *resp = NULL;

    switch(req->valueint) {
        case USER_SEARCH:
            str = cJSON_GetObjectItemCaseSensitive(req, "search_str");
            mx_do_search_user(client->db, cur_client, str->valuestring);

            cur_client->receivers = (int*)calloc(2,sizeof(int));
            cur_client->receivers[0] = cur_client->uid;
            cur_client->receivers[1] = -1;
            resp = mx_create_search_response(cur_client);

            mx_send_response(resp, cur_client, client);

            free(cur_client->receivers);
            free(resp);
            printf("\t------- Search done --------\n");
            break;
        default:
            printf("\n!!! --- Err: unknown request code ----- !!!\n");
            break;
    }

    pthread_mutex_unlock(&client->req_mut);
}
