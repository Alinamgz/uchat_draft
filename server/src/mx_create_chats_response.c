#include "server.h"

char *mx_create_chats_response(t_list *cur_client) {
    char *rslt = NULL;
    cJSON *res = cJSON_CreateObject();
    cJSON *chats_arr = NULL;
    cJSON *chat_obj = NULL;

    cJSON_AddNumberToObject(res, "type", NEW_CHAT);
    cJSON_AddNumberToObject(res, "resp_code", cur_client->res_code);
    cJSON_AddNumberToObject(res, "users_amt", cur_client->rows_cnt);

    switch(cur_client->res_code) {
        case OK:
            cJSON_AddStringToObject(res, "msg", OK_MSG);
            cJSON_AddNumberToObject(res, "chats_amt", cur_client->rows_cnt);

            chats_arr = cJSON_AddArrayToObject(res, "chats_arr");

            for (int i = 0; i < cur_client->rows_cnt; i++) {
                chat_obj = cJSON_CreateObject();

                cJSON_AddStringToObject(chat_obj,
                                        "chat_id",
                                        cur_client->chat_req_res[i]->chat_id);
                cJSON_AddStringToObject(chat_obj,
                                        "from_uid",
                                        cur_client->chat_req_res[i]->from_uid);
                cJSON_AddStringToObject(chat_obj,
                                        "to_uid",
                                        cur_client->chat_req_res[i]->to_uid);
                cJSON_AddStringToObject(chat_obj,
                                        "name_or_msg",
                                        cur_client->chat_req_res[i]->name_or_msg);
                cJSON_AddItemToArray(chats_arr, chat_obj);
            }
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
