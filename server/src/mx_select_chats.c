#include "server.h"

static int count_callback(void *data, int argc, char **argv, char **azColName);
static int select_chats_callback(void *data, int argc, char **argv, char **azColName);

void mx_select_chats(sqlite3 *db, t_list *cur_client) {
    char sql[128] = "";
    char *err = NULL;
    int rows = 0;

    // counting rows in resp
    memset(sql, 0, 128);

    sprintf(sql,
            "SELECT COUNT(*) FROM chats WHERE from_uid = '%d' OR  to_uid = '%d';",
            cur_client->uid,
            cur_client->uid);

    if (sqlite3_exec(db, sql, count_callback, &cur_client->rows_cnt, &err)) {
        cur_client->auth_req_res->res_code = INTERNAL_SRVR_ERR;
        fprintf(stderr, "count chats from db: %s\n", sqlite3_errmsg(db));
        free(err);
    }

    rows = cur_client->rows_cnt;
printf("------- ch 1 -----------\n");
    // selecting chats
    // cur_client->chat_req_res = (t_chat_req_res **)malloc(sizeof(t_chat_req_res *));
    cur_client->chat_req_res = (t_chat_req_res**)calloc(rows+1, sizeof(t_chat_req_res*));
    cur_client->cur_row = 0;

printf("------- ch 2 -----------\n");
    memset(sql, 0, 128);
    sprintf(sql,
                "SELECT chat_id, chat_name, from_uid, to_uid FROM chats WHERE "
                "from_uid = '%d' OR  to_uid = '%d';",
            cur_client->uid,
            cur_client->uid);

printf("------- ch 3 -----------\n");
     if (sqlite3_exec(db, sql, select_chats_callback, cur_client, &err)) {
        cur_client->auth_req_res->res_code = INTERNAL_SRVR_ERR;
        fprintf(stderr, "select chats from db: %s\n", sqlite3_errmsg(db));
        free(err);
    }

}

static int count_callback(void *data, int argc, char **argv, char **azColName) {
    if (argc && azColName) {
        int *rows_cnt = (int *)data;
        *rows_cnt = atoi(argv[0]);
    }
printf("------- ch 4 -----------\n");

    return 0;
}

static int select_chats_callback(void *data, int argc, char **argv, char **azColName) {

    t_list *cur_client = (t_list *)data;

    cur_client->chat_req_res[cur_client->cur_row] =(t_chat_req_res*)malloc(sizeof(t_chat_req_res));
printf("------- ch 5 -----------\n");
    memset(cur_client->chat_req_res[cur_client->cur_row], 0, sizeof(cur_client->chat_req_res));
printf("------- ch 6 -----------\n");

        printf("\n------------\nrow %d\n", cur_client->cur_row);
    for (int i = 0; i <= argc; i++) {
        if (!strcmp(azColName[i], "chat_id")) {
            cur_client->chat_req_res[cur_client->cur_row]->chat_id = strdup(argv[i]);
            // printf("chat_id: %s\n", cur_client->chat_req_res[cur_client->cur_row]->chat_id);

        }
        else if (!strcmp(azColName[i], "chat_name")) {
            cur_client->chat_req_res[cur_client->cur_row]->name_or_msg = strdup(argv[i]);
            // printf("name or string: %s\n", cur_client->chat_req_res[cur_client->cur_row]->name_or_msg);
        }
        else if (!strcmp(azColName[i], "from_uid")) {
            cur_client->chat_req_res[cur_client->cur_row]->from_uid = strdup(argv[i]);
            // printf("from_uid: %s\n", cur_client->chat_req_res[cur_client->cur_row]->from_uid);
        }
        else if (!strcmp(azColName[i], "to_uid")) {
            cur_client->chat_req_res[cur_client->cur_row]->to_uid = strdup(argv[i]);
            // printf("to_uid: %s\n", cur_client->chat_req_res[cur_client->cur_row]->to_uid);
        }
    }

    cur_client->chat_req_res[++cur_client->cur_row] = NULL;
printf("------- ch 7 -----------\n");

    return 0;
}
