#include "server.h"

static int login_callback(void *data, int argc, char **argv, char **azColName) {
    t_auth_req *req_parsed = (t_auth_req *)data;

    if (argc == 4 && !strcmp(azColName[0], "password")) {
        req_parsed->res_code =
            strcmp(argv[0], req_parsed->password) ? UNAUTHORIZED : OK;
        // -----------------------------------------------------
        // NB! columns in azColName are in the same order as listed in SELECT
        // statement.
        //     if we SELECT *, coumns are in the order from CREATE TABLE
        //     statement
        // -----------------------------------------------------

        if (req_parsed->res_code == OK) {
            req_parsed->uid = atoi(argv[1]);
            req_parsed->first_name = strdup(argv[2]);
            req_parsed->last_name = strdup(argv[3]);
        }
    }

    return 0;
}

void mx_do_login(sqlite3 *db, t_list *cur_client) {
    char sql[128] = "";
    char *err = NULL;

    memset(sql, 0, 128);
    sprintf(sql,
            "SELECT password, uid, first_name, last_name FROM users WHERE user_name = '%s';",
            cur_client->auth_req_res->username);

    if (sqlite3_exec(db, sql, login_callback, cur_client->auth_req_res, &err)) {
        cur_client->auth_req_res->res_code = INTERNAL_SRVR_ERR;
        fprintf(stderr, "select login from db: %s\n", sqlite3_errmsg(db));
        free(err);
    }
    else if (cur_client->auth_req_res->res_code == OK) {
        cur_client->uid = cur_client->auth_req_res->uid;
        strcpy(cur_client->name, cur_client->auth_req_res->username);
    }
}
