#include "client.h"

void mx_parse_newmsg_response(t_client *client, char *resp_str) {
    cJSON *res = cJSON_Parse(resp_str);
    cJSON *res_code = cJSON_GetObjectItemCaseSensitive(res, "res_code");
    cJSON *err_msg = NULL;
    cJSON *msg_id = NULL;
    cJSON *chat_id = NULL;
    cJSON *from_uid = NULL;
    cJSON *to_uid = NULL;
    cJSON *msg =  cJSON_GetObjectItemCaseSensitive(res, "msg");
    cJSON *timestamp =  cJSON_GetObjectItemCaseSensitive(res, "timestamp");

    switch(res_code->valueint) {
        case OK:
            msg_id = cJSON_GetObjectItemCaseSensitive(res, "msg_id");
            chat_id = cJSON_GetObjectItemCaseSensitive(res, "chat_id");
            from_uid = cJSON_GetObjectItemCaseSensitive(res, "from_uid");
            to_uid = cJSON_GetObjectItemCaseSensitive(res, "to_uid");

            client->new_msg = (t_msgs*)malloc(sizeof(t_msgs));

            client->new_msg->msg_id = msg_id->valueint;
            client->new_msg->chat_id = chat_id->valueint;
            client->new_msg->from_uid = from_uid->valueint;
            client->new_msg->to_uid = to_uid->valueint;
            client->new_msg->msg = strdup(msg->valuestring);
            client->new_msg->timestamp = strdup(timestamp->valuestring);

            break;
        case INTERNAL_SRVR_ERR:
            err_msg = cJSON_GetObjectItemCaseSensitive(res, "err_msg");

            gtk_label_set_text(GTK_LABEL(client->ui->selected_chat_name), err_msg->valuestring);
            // show msg as unsent
            break;
        default:
            err_msg = cJSON_GetObjectItemCaseSensitive(res, "err_msg");
            gtk_label_set_text(GTK_LABEL(client->ui->selected_chat_name), err_msg->valuestring);
            // show msg as unsent
            break;
    }

    cJSON_Delete(res);
}
