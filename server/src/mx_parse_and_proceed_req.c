#include "server.h"

void mx_parse_and_proceed_req(char *buf, t_list *cur_client, t_cl_data *client) {

    pthread_mutex_lock(&client->req_mut);

    char *req_str = strdup(buf);

    pthread_mutex_unlock(&client->req_mut);

    cJSON *req = cJSON_Parse(req_str);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(req, "type");

    switch(type->valueint) {
        case USER_SEARCH:
            mx_proceed_search_req(req_str, cur_client, client);
            break;
        case NEW_CHAT:
            mx_proceed_newchat_req(req_str, cur_client, client);
            break;
        case NEW_MSG:
            mx_proceed_newmsg_req(req_str, cur_client, client);
            break;
        case HISTORY:
            printf("SERVER HISTORY\n");
            printf("Request = %s", buf);
            break;
        default:
            printf("\n!!! --- Err: unknown request code ----- !!!\n");
            break;
    }

    cJSON_Delete(req);
    free(req_str);
    req_str = NULL;
}
