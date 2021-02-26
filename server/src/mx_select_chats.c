#include "server.h"


static int select_chats_callback(void *data, int argc, char **argv, char **azColName);

void mx_select_chats(sqlite3 *db, t_list *cur_client, int my_uid) {
    char sql[128] = "";
    char *err = NULL;
    int rows = 0;

    rows = cur_client->rows_cnt;
    cur_client->chat_req_res = (t_chat_req_res**)calloc(rows+1, sizeof(t_chat_req_res*));
    cur_client->cur_row = 0;

    memset(sql, 0, 128);
    sprintf(sql,
            "SELECT chat_id, chat_name, from_uid, to_uid FROM chats WHERE "
            "from_uid = '%d' OR  to_uid = '%d';",
            my_uid, my_uid);

     if (sqlite3_exec(db, sql, select_chats_callback, cur_client, &err)) {
        cur_client->res_code = INTERNAL_SRVR_ERR;
        fprintf(stderr, "select chats from db: %s\n", sqlite3_errmsg(db));
        free(err);
    }
    else {
        cur_client->res_code = OK;
    }

}

static int select_chats_callback(void *data, int argc, char **argv, char **azColName) {
    t_list *cur_client = (t_list *)data;

    cur_client->chat_req_res[cur_client->cur_row] =(t_chat_req_res*)malloc(sizeof(t_chat_req_res));
    memset(cur_client->chat_req_res[cur_client->cur_row], 0, sizeof(cur_client->chat_req_res));

    for (int i = 0; i <= argc; i++) {
        if (!strcmp(azColName[i], "chat_id")) {
            cur_client->chat_req_res[cur_client->cur_row]->chat_id = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "chat_name")) {
            cur_client->chat_req_res[cur_client->cur_row]->name_or_msg = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "from_uid")) {
            cur_client->chat_req_res[cur_client->cur_row]->from_uid = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "to_uid")) {
            cur_client->chat_req_res[cur_client->cur_row]->to_uid = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "timestamp")) {
            cur_client->chat_req_res[cur_client->cur_row]->timestamp = NULL;
        }
    }

    cur_client->chat_req_res[++cur_client->cur_row] = NULL;

    return 0;
}
