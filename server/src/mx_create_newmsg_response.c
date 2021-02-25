#include "server.h"

char *mx_create_newmsg_response(t_list *cur_client, t_chat_req_res *new_msg) {
    char *rslt = NULL;
    cJSON *res = cJSON_CreateObject();

    cJSON_AddNumberToObject(res, "type", NEW_MSG);
    cJSON_AddNumberToObject(res, "res_code", cur_client->res_code);

    switch(cur_client->res_code) {
        case OK:
            cJSON_AddNumberToObject(res, "chat_id", atoi(new_msg->chat_id));
            cJSON_AddNumberToObject(res, "from_uid", atoi(new_msg->from_uid));
            cJSON_AddNumberToObject(res, "to_uid", atoi(new_msg->to_uid));
            cJSON_AddStringToObject(res, "msg", new_msg->name_or_msg);

            break;
        case INTERNAL_SRVR_ERR:
            cJSON_AddStringToObject(res, "msg", INTERNAL_SRVR_ERR_MSG);
            break;
        default:
            cJSON_AddStringToObject(res, "msg", "Unknown status code\n");
            break;
    }

    rslt  = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);

    return rslt;
}
