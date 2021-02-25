#include "server.h"

static int get_timestamp_callback(void *data, int argc, char **argv, char **azColName) {
    cJSON *res = (cJSON*)data;

    if (argc && argv && azColName)
        cJSON_AddStringToObject(res, "timestamp", argv[0]);

    return 0;
}

char *mx_create_newmsg_response(sqlite3 *db,
                                t_list *cur_client,
                                t_chat_req_res *new_msg,
                                int newmsg_id) {
    char *rslt = NULL;
    cJSON *res = cJSON_CreateObject();
    char sql[128] = "";
    char *err;

    cJSON_AddNumberToObject(res, "type", NEW_MSG);
    cJSON_AddNumberToObject(res, "res_code", cur_client->res_code);

    switch(cur_client->res_code) {
        case OK:
            cJSON_AddNumberToObject(res, "msg_id", newmsg_id);
            cJSON_AddNumberToObject(res, "chat_id", atoi(new_msg->chat_id));
            cJSON_AddNumberToObject(res, "from_uid", atoi(new_msg->from_uid));
            cJSON_AddNumberToObject(res, "to_uid", atoi(new_msg->to_uid));
            cJSON_AddStringToObject(res, "msg", new_msg->name_or_msg);

            memset(sql, 0, 128);
            sprintf(sql, "SELECT timestamp FROM messages WHERE msg_id='%d';", newmsg_id);

            if (sqlite3_exec(db, sql, get_timestamp_callback, (void*)res, &err)) {
                fprintf(stderr, "Adding new msg err: %s\n", sqlite3_errmsg(db));
                free(err);
                cur_client->res_code = INTERNAL_SRVR_ERR;
            }
            break;
        case INTERNAL_SRVR_ERR:
            cJSON_AddStringToObject(res, "err_msg", INTERNAL_SRVR_ERR_MSG);
            cJSON_AddStringToObject(res, "msg", new_msg->name_or_msg);
            break;
        default:
            cJSON_AddStringToObject(res, "msg", "Unknown status code\n");
            break;
    }

    rslt  = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);

    return rslt;
}
