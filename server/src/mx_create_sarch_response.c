#include "server.h"

char *mx_create_search_response(t_list *cur_client) {
    char *rslt = NULL;
    cJSON *res = cJSON_CreateObject();
    cJSON *users_arr = NULL;
    cJSON *user_obj = NULL;

    cJSON_AddNumberToObject(res, "resp_code", cur_client->res_code);
    cJSON_AddNumberToObject(res, "users_amt", cur_client->rows_cnt);

    switch(cur_client->res_code) {
        case OK:
            cJSON_AddStringToObject(res, "msg", OK_MSG);
            cJSON_AddNumberToObject(res, "users_amt", cur_client->rows_cnt);

            users_arr = cJSON_AddArrayToObject(res, "users_arr");
            for (int i = 0; i < cur_client->rows_cnt; i++) {
                user_obj = cJSON_CreateObject();

                cJSON_AddStringToObject(user_obj,
                                        "uid",
                                        cur_client->found_users[i]->chat_id);
                cJSON_AddStringToObject(user_obj,
                                        "username",
                                        cur_client->found_users[i]->name_or_msg);
                cJSON_AddStringToObject(user_obj,
                                        "first_name",
                                        cur_client->found_users[i]->from_uid);
                cJSON_AddStringToObject(user_obj,
                                        "last_name",
                                        cur_client->found_users[i]->to_uid);
                cJSON_AddItemToArray(users_arr, user_obj);
            }
            break;
        case NOT_FOUND:
            cJSON_AddStringToObject(res, "msg", NOT_FOUND_MSG);
            break;
        case INTERNAL_SRVR_ERR:
            cJSON_AddStringToObject(res, "msg", INTERNAL_SRVR_ERR_MSG);
            break;
        default:
            cJSON_AddStringToObject(res, "msg", "Unknown status code\n");
            break;
    }

    rslt = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);

    return rslt;
}
