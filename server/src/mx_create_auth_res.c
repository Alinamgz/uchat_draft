#include "server.h"

char *mx_create_auth_res(t_list *cur_client) {
    char *rslt = NULL;
    cJSON *res = cJSON_CreateObject();
    cJSON *chats_arr = NULL;
    cJSON *msg_obj = NULL;

    cJSON_AddNumberToObject(res, "code", cur_client->auth_req_res->res_code);

    switch(cur_client->auth_req_res->res_code) {
        case OK:
            cJSON_AddStringToObject(res, "msg", OK_MSG);
            cJSON_AddNumberToObject(res, "uid", cur_client->auth_req_res->uid);
            cJSON_AddStringToObject(res, "username", cur_client->auth_req_res->username);
            cJSON_AddStringToObject(res, "first_name", cur_client->auth_req_res->first_name);
            cJSON_AddStringToObject(res, "last_name", cur_client->auth_req_res->last_name);

            cJSON_AddNumberToObject(res, "chats_amt", cur_client->rows_cnt);
            chats_arr = cJSON_AddArrayToObject(res, "chats_arr");

            for (int i = 0; i < cur_client->rows_cnt; i++) {
                msg_obj = cJSON_CreateObject();

                cJSON_AddStringToObject(msg_obj,
                                        "chat_id",
                                        cur_client->chat_req_res[i]->chat_id);
                cJSON_AddStringToObject(msg_obj,
                                        "from_uid",
                                        cur_client->chat_req_res[i]->from_uid);
                cJSON_AddStringToObject(msg_obj,
                                        "to_uid",
                                        cur_client->chat_req_res[i]->to_uid);
                cJSON_AddStringToObject(msg_obj,
                                        "name_or_msg",
                                        cur_client->chat_req_res[i]->name_or_msg);
                cJSON_AddItemToArray(chats_arr, msg_obj);
            }

            break;
        case CREATED:
            cJSON_AddStringToObject(res, "msg", CREATED_MSG);
            break;
        case BAD_REQ:
            cJSON_AddStringToObject(res, "msg", BAD_REQ_MSG);
            break;
        case UNAUTHORIZED:
            cJSON_AddStringToObject(res, "msg", UNAUTHORIZED_MSG);
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
