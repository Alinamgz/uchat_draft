#include "server.h"

static int count_callback(void *data, int argc, char **argv, char **azColName);

void mx_count_chat_rows(sqlite3 *db, t_list *cur_client) {
    char sql[128] = "";
    char *err = NULL;

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
}

static int count_callback(void *data, int argc, char **argv, char **azColName) {
    if (argc && azColName) {
        int *rows_cnt = (int *)data;
        *rows_cnt = atoi(argv[0]);
    }

    return 0;
}
