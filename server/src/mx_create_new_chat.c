#include "server.h"

static int count_callback(void *data, int argc, char **argv, char **azColName) {
    if (argc && azColName) {
        int *rows_cnt = (int *)data;
        *rows_cnt = atoi(argv[0]);
    }

    return 0;
}

void mx_create_new_chat(sqlite3 *db, t_list *cur_client, int peer_uid, char *peer_name) {
    char sql[256];
    char *err = NULL;
    memset(sql, 0, 256);
    int rows_cnt = 0;

// check if chat between users already exists
    sprintf(sql,
            "SELECT COUNT(*) FROM chats WHERE (from_uid = '%d' AND to_uid = "
            "'%d') OR (from_uid = '%d' AND to_uid = '%d');",
            cur_client->uid,
            peer_uid,
            peer_uid,
            cur_client->uid);

    if (sqlite3_exec(db, sql, count_callback, &rows_cnt, &err)) {
        fprintf(stderr, "Check for existent chat err: %s\n",
                sqlite3_errmsg(db));
        free(err);
        cur_client->res_code = INTERNAL_SRVR_ERR;
    }

// if no chat yet, create new chat
    if (!rows_cnt) {
        memset(sql, 0, 256);

        sprintf(sql,
                "INSERT INTO chats (chat_name, from_uid, to_uid)"
                "VALUES ('%s <-> %s', '%d', '%d');",
                cur_client->name, peer_name, cur_client->uid, peer_uid);

        if (sqlite3_exec(db, sql, NULL, NULL, &err)) {
            fprintf(stderr, "Creating new chat err: %s\n", sqlite3_errmsg(db));
            free(err);
            cur_client->res_code = INTERNAL_SRVR_ERR;
        }
    }
}
