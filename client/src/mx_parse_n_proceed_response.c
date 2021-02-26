#include "client.h"

void mx_parse_n_proceed_response(t_client *client, char *resp_str) {
    pthread_mutex_lock(&client->resp_mut);

    cJSON *res = cJSON_Parse(resp_str);

    if (res) {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(res, "type");
        char *resp = strdup(resp_str);

        pthread_mutex_unlock(&client->resp_mut);

        switch(type->valueint) {
            case SEARCH:
                mx_proceed_search_response(client, resp);
                break;
            case NEW_CHAT:
                mx_proceed_chat_response(client, resp);
                break;
            case NEW_MSG:
                printf("got resp: %s\n", resp_str);
                mx_proceed_newmsg_response(client, resp);
                break;
            case HISTORY:
                printf("HISTORY\n");
                printf("res = %s\n", resp);
                mx_proceed_history_response(client, resp);
                break;
            default:
                printf("\n ----- ??? ---- Unknown resp type ------- ??? ------\n");
                printf("got resp: %s\n", resp_str);
                break;
        }

        cJSON_Delete(res);
        free(resp);
        resp = NULL;
    }
    else {
        printf("got UNPARSED resp: %s\n", resp_str);
    }
}
