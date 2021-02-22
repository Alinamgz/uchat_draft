#include "server.h"

static int reqistration_callback(void *data, int argc, char **argv, char **azColName) {
    if (argc && argv && azColName) {
        *(int*)data = atoi(argv[0]);
    }

    return 0;
}

void mx_do_registration(sqlite3 *db, t_list *cur_client) {
    char sql[256] = "";
    char *err = NULL;

    memset(sql, 0, 256);
    sprintf(sql,
            "INSERT INTO users (user_name, password, first_name, last_name)"
            "VALUES ('%s', '%s', '%s', '%s');"
            "SELECT uid FROM users WHERE user_name = '%s';",
            cur_client->auth_req_res->username,
            cur_client->auth_req_res->password,
            cur_client->auth_req_res->first_name,
            cur_client->auth_req_res->last_name,
            cur_client->auth_req_res->username);

    if (sqlite3_exec(db, sql, reqistration_callback, &cur_client->auth_req_res->uid, &err)) {
        cur_client->auth_req_res->res_code =
            strcmp(DB_UNIQUE_USER_ERR, err) ? INTERNAL_SRVR_ERR : BAD_REQ;

        fprintf(stderr, "Creating new user err: %s\n", sqlite3_errmsg(db));
        free(err);
    }
    else if (cur_client->auth_req_res->uid > 0) {
        cur_client->uid = cur_client->auth_req_res->uid;
        cur_client->auth_req_res->res_code = CREATED;
    }
    else {
        cur_client->auth_req_res->res_code = INTERNAL_SRVR_ERR;
    }
}
