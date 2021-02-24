#include "client.h"

void mx_parse_chats_response(t_client *client, char *resp_str) {
    // pthread_mutex_lock(&client->resp_mut);

    cJSON *res = cJSON_Parse(resp_str);
    cJSON *code = cJSON_GetObjectItemCaseSensitive(res,"resp_code");
    cJSON *chats_amt = cJSON_GetObjectItemCaseSensitive(res,"chats_amt");
    cJSON *msg = NULL;
    cJSON *chats_arr = NULL;
    cJSON *chat = NULL;
    cJSON *chat_id = NULL;
    cJSON *chat_name = NULL;
    cJSON *from_uid = NULL;
    cJSON *to_uid = NULL;
    int i = 0;

    if (code->valueint == OK || code->valueint == OK_CHATS) {
        if (chats_amt->valueint > 0) {
            chats_arr = cJSON_GetObjectItemCaseSensitive(res, "chats_arr");
            client->chats = (t_chats**)calloc(chats_amt->valueint + 1, sizeof(t_chats*));

            cJSON_ArrayForEach(chat, chats_arr) {
                chat_id = cJSON_GetObjectItemCaseSensitive(chat, "chat_id");
                chat_name = cJSON_GetObjectItemCaseSensitive(chat, "name_or_msg");
                from_uid = cJSON_GetObjectItemCaseSensitive(chat, "from_uid");
                to_uid = cJSON_GetObjectItemCaseSensitive(chat, "to_uid");

                client->chats[i] = (t_chats*)malloc(sizeof(t_chats));

                client->chats[i]->chat_id = atoi(chat_id->valuestring);
                client->chats[i]->chat_name = strdup(chat_name->valuestring);
                client->chats[i]->from_uid = atoi(from_uid->valuestring);
                client->chats[i]->to_uid = atoi(to_uid->valuestring);

                i++;
            }
            client->chats[i] = NULL;
        }
        else {
            // no chatas found
            printf("No chats found\n");
        }
    }
    else {
        // chats recv err
        printf("chats recv err\n");
    }

    // mx_show_chats(client);

    cJSON_Delete(res);

    // pthread_mutex_unlock(&client->resp_mut);
}
