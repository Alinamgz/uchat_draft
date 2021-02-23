#include "server.h"

static void search_req_res_memfree(t_chat_req_res **users_req_res);

void mx_parse_and_proceed_req(char *buf, t_list *cur_client, t_cl_data *client) {
    pthread_mutex_lock(&client->req_mut);
printf("||| got req: %s\n", buf);

    cJSON *req = cJSON_Parse(buf);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(req, "type");
    cJSON *str = NULL;
    char *resp = NULL;
    switch(type->valueint) {
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
            cJSON_Delete(req);

            search_req_res_memfree(cur_client->found_users);
            printf("\t------- Search done --------\n");
            system("leaks -q uchat_server");
            break;
        default:
            printf("\n!!! --- Err: unknown request code ----- !!!\n");
            break;
    }

    pthread_mutex_unlock(&client->req_mut);
}

static void search_req_res_memfree(t_chat_req_res **users_req_res) {

        t_chat_req_res **runner = users_req_res;
printf("ch_01\n");
    if (runner && *runner){
printf("ch_01_01\n");
        for (int i = 0; runner[i]; i++) {
printf("ch_01_02\n");
            if (runner[i]->chat_id) {
                free(runner[i]->chat_id);
                runner[i]->chat_id = NULL;
printf("ch_01_03\n");
            }
            if (runner[i]->name_or_msg) {
                free(runner[i]->name_or_msg);
                runner[i]->name_or_msg = NULL;
printf("ch_01_04\n");
            }
            if (runner[i]->from_uid) {
                free(runner[i]->from_uid);
                runner[i]->from_uid = NULL;
printf("ch_01_05\n");
            }
            if (runner[i]->to_uid) {
                free(runner[i]->to_uid);
                runner[i]->to_uid = NULL;
printf("ch_01_06\n");
            }
            free(runner[i]);
printf("ch_01_07\n");
            runner[i] = NULL;
        }
    }
    free(users_req_res);
printf("ch_01_08\n");
    users_req_res = NULL;
printf("ch_02\n");
}
