#include "client.h"

void mx_parse_search_response(t_client *client, char *resp_str) {
    pthread_mutex_lock(&client->resp_mut);

    cJSON *res = cJSON_Parse(resp_str);
    cJSON *code = cJSON_GetObjectItemCaseSensitive(res, "resp_code");
    cJSON *msg = cJSON_GetObjectItemCaseSensitive(res, "msg");
    cJSON *users_amt = cJSON_GetObjectItemCaseSensitive(res, "users_amt");
    cJSON *users_arr = NULL;
    cJSON *user_obj = NULL;
    cJSON *uid = NULL;
    cJSON *username = NULL;
    cJSON *first_name = NULL;
    cJSON *last_name = NULL;
    int i = 0;

    if (code->valueint == OK && users_amt->valueint > 0) {
        users_arr = cJSON_GetObjectItemCaseSensitive(res, "users_arr");
        client->found_users = (t_self**)calloc(users_amt->valueint + 1, sizeof(t_self*));

        cJSON_ArrayForEach(user_obj, users_arr) {
            uid = cJSON_GetObjectItemCaseSensitive(user_obj, "uid");
            username = cJSON_GetObjectItemCaseSensitive(user_obj, "username");
            first_name = cJSON_GetObjectItemCaseSensitive(user_obj, "first_name");
            last_name = cJSON_GetObjectItemCaseSensitive(user_obj, "last_name");

            client->found_users[i] = (t_self*)malloc(sizeof(t_self));

            client->found_users[i]->uid = atoi(uid->valuestring);
            client->found_users[i]->username = strdup(username->valuestring);
            client->found_users[i]->first_name = strdup(first_name->valuestring);
            client->found_users[i]->last_name = strdup(last_name->valuestring);

            i++;
        }
        client->found_users[i] = NULL;
    }
    else {
         gtk_label_set_text(GTK_LABEL(client->ui->search_status), msg->valuestring);
    }

    cJSON_Delete(res);

    pthread_mutex_unlock(&client->resp_mut);
}
