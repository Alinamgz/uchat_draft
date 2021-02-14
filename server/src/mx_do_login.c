#include "server.h"

static int login_callback(void *data, int argc, char **argv, char **azColName) {
    t_auth_req *req_parsed = (t_auth_req *)data;

    if (argc && argv && azColName)
        req_parsed->res_code = strcmp(argv[0], req_parsed->password) ? UNAUTHORIZED : OK;

    return 0;
}

void mx_do_login(t_cl_data *client, t_auth_req *req_parsed) {
    char sql[128] = "";
    char *err = NULL;

    memset(sql, 0, 128);
    sprintf(sql, "SELECT password FROM users WHERE user_name = '%s';", req_parsed->username);

    if (sqlite3_exec(client->db, sql, login_callback, req_parsed, &err)) {
        fprintf(stderr, "select login from db: %s\n", sqlite3_errmsg(client->db));

        free(err);
    }
}
