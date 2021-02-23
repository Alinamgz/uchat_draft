#include "server.h"


static int count_found_users(sqlite3 *db, t_list *cur_client, const char *search_str);
static int count_callback(void *data, int argc, char **argv, char **azColName);
static int search_users_callback(void *data, int argc, char **argv, char **azColName);
// TODO: instead of reusinf auth_req_res make new struct for search req or rename this struct less specific

void mx_do_search_user(sqlite3 *db, t_list *cur_client, char *search_str) {
    char sql[128] = "";
    char *err = NULL;
    int rows = 0;

    rows = count_found_users(db, cur_client, search_str);

    if (rows > 0) {

        cur_client->found_users = (t_chat_req_res**)calloc(1 + rows, sizeof(t_chat_req_res*));

        memset(sql, 0, 128);
        sprintf(sql,
                "SELECT uid, user_name, first_name, last_name FROM users WHERE uid <> '0' AND user_name LIKE '%%%s%%'",
                search_str);

        if (sqlite3_exec(db, sql, search_users_callback, cur_client, &err)) {
            cur_client->res_code = INTERNAL_SRVR_ERR;
            fprintf(stderr, "select found users from db: %s\n", sqlite3_errmsg(db));
            free(err);
        }
        else if (cur_client->found_users) {
            cur_client->res_code = OK;
        }
    }
    else {
        cur_client->res_code = NOT_FOUND;
    }

}

static int search_users_callback(void *data, int argc, char **argv, char **azColName) {
    t_list *cur_client = (t_list *)data;

    cur_client->found_users[cur_client->cur_row] =(t_chat_req_res*)malloc(sizeof(t_chat_req_res));
    memset(cur_client->found_users[cur_client->cur_row], 0, sizeof(cur_client->found_users));

    for (int i = 0; i <= argc; i++) {
        if (!strcmp(azColName[i], "uid")) {
            cur_client->found_users[cur_client->cur_row]->chat_id = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "user_name")) {
            cur_client->found_users[cur_client->cur_row]->name_or_msg = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "first_name")) {
            // NB! here from uid is for first name.
            // TODO: change to less specific name
            cur_client->found_users[cur_client->cur_row]->from_uid = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "last_name")) {
            // NB! here to uid is for last name.
            // TODO: change to less specific name
            cur_client->found_users[cur_client->cur_row]->to_uid = strdup(argv[i]);
        }
    }

    cur_client->found_users[++cur_client->cur_row] = NULL;

    return 0;
}

static int count_found_users(sqlite3 *db, t_list *cur_client, const char *search_str) {
    char sql[128] = "";
    char *err = NULL;
    cur_client->rows_cnt = 0;
    cur_client->cur_row = 0;

    memset(sql, 0, 128);
    sprintf(sql, "SELECT COUNT(*) from users WHERE uid <> '0' AND user_name LIKE '%%%s%%'", search_str);

    if (sqlite3_exec(db, sql, count_callback, &cur_client->rows_cnt, &err)) {
        cur_client->res_code = INTERNAL_SRVR_ERR;
        fprintf(stderr, "count users from db: %s\n", sqlite3_errmsg(db));
        free(err);
    }

    return cur_client->rows_cnt;
}

static int count_callback(void *data, int argc, char **argv, char **azColName) {
    if (argc && azColName) {
        int *rows_cnt = (int *)data;
        *rows_cnt = atoi(argv[0]);
    }

    return 0;
}
