#include "server.h"

char *mx_create_history_response(t_list *cur_client) {
    char *rslt = NULL;
    cJSON *res = cJSON_CreateObject();
    cJSON *history_arr = NULL;
    cJSON *history_obj = NULL;

    cJSON_AddNumberToObject(res, "type", HISTORY);
    cJSON_AddNumberToObject(res, "resp_code", cur_client->res_code);
    cJSON_AddNumberToObject(res, "history_amt", cur_client->rows_cnt);

    switch(cur_client->res_code) {
        case OK:
            cJSON_AddStringToObject(res, "msg", OK_MSG);

            history_arr = cJSON_AddArrayToObject(res, "history_arr");

            for (int i = 0; i < cur_client->rows_cnt; i++) {
                history_obj = cJSON_CreateObject();

                cJSON_AddStringToObject(history_obj,
                                        "chat_id",
                                        cur_client->history_req_res[i]->chat_id);
                cJSON_AddStringToObject(history_obj,
                                        "from_uid",
                                        cur_client->history_req_res[i]->from_uid);
                cJSON_AddStringToObject(history_obj,
                                        "to_uid",
                                        cur_client->history_req_res[i]->to_uid);
                cJSON_AddStringToObject(history_obj,
                                        "name_or_msg",
                                        cur_client->history_req_res[i]->name_or_msg);
                cJSON_AddStringToObject(history_obj,
                                        "timestamp",
                                        cur_client->history_req_res[i]->timestamp);
                cJSON_AddItemToArray(history_arr, history_obj);
            }
            break;
        case INTERNAL_SRVR_ERR:
            cJSON_AddStringToObject(res, "msg", INTERNAL_SRVR_ERR_MSG);
            break;
        case NOT_FOUND:
            cJSON_AddStringToObject(res, "msg", "No HISTORY!\n");
            break;
        default:
            cJSON_AddStringToObject(res, "msg", "Unknown status code\n");
            break;
    }

    rslt = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);

    return rslt;
}
