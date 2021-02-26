#include "client.h"

void mx_parse_history_response(t_client *client, char *resp_str) {
    pthread_mutex_lock(&client->resp_mut);

    cJSON *res = cJSON_Parse(resp_str);
    cJSON *code = cJSON_GetObjectItemCaseSensitive(res, "resp_code");
    cJSON *err_msg = cJSON_GetObjectItemCaseSensitive(res, "msg");
    cJSON *history_arr = NULL;
    cJSON *history_obj = NULL;
    cJSON *history_amt = NULL;
    cJSON *chat_id = NULL;
    cJSON *from_uid = NULL;
    cJSON *to_uid = NULL;
    cJSON *name_or_msg = NULL;
    cJSON *timestamp = NULL;
    int i = 0;

    if (code->valueint == OK) {
        history_arr = cJSON_GetObjectItemCaseSensitive(res, "history_arr");
        history_amt = cJSON_GetObjectItemCaseSensitive(res, "history_amt");
        client->msg_arr = (t_msgs**)calloc(history_amt->valueint + 1, sizeof(t_msgs*));

        cJSON_ArrayForEach(history_obj, history_arr) {
            chat_id = cJSON_GetObjectItemCaseSensitive(history_obj, "chat_id");
            from_uid = cJSON_GetObjectItemCaseSensitive(history_obj, "from_uid");
            to_uid = cJSON_GetObjectItemCaseSensitive(history_obj, "to_uid");
            cJSON *name_or_msg =  cJSON_GetObjectItemCaseSensitive(history_obj, "name_or_msg");
            cJSON *timestamp =  cJSON_GetObjectItemCaseSensitive(history_obj, "timestamp");

            client->msg_arr[i] = (t_msgs*)malloc(sizeof(t_msgs));

            client->msg_arr[i]->msg_id = -1;
            client->msg_arr[i]->chat_id = atoi(chat_id->valuestring);
            client->msg_arr[i]->from_uid = atoi(from_uid->valuestring);
            client->msg_arr[i]->to_uid = atoi(to_uid->valuestring);
            client->msg_arr[i]->msg = strdup(name_or_msg->valuestring);
            client->msg_arr[i]->timestamp = strdup(timestamp->valuestring);

            i++;
        }
        client->msg_arr[i] = NULL;
    }
    cJSON_Delete(res);
    pthread_mutex_unlock(&client->resp_mut);
}
